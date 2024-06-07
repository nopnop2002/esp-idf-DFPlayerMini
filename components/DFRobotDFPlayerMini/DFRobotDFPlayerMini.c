/*!
 * @file DFRobotDFPlayerMini.cpp
 * @brief DFPlayer - An Arduino Mini MP3 Player From DFRobot
 * @n Header file for DFRobot's DFPlayer
 *
 * @copyright	[DFRobot]( http://www.dfrobot.com ), 2016
 * @copyright	GNU Lesser General Public License
 *
 * @author [Angelo](Angelo.qiao@dfrobot.com)
 * @version  V1.0.6
 * @date  2016-12-07
 */

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "serial.h"

#include "DFRobotDFPlayerMini.h"

unsigned long _timeOutTimer;
unsigned long _timeOutDuration = 500;

uint8_t _received[DFPLAYER_RECEIVED_LENGTH];
uint8_t _sending[DFPLAYER_SEND_LENGTH] = {0x7E, 0xFF, 06, 00, 01, 00, 00, 00, 00, 0xEF};

uint8_t _receivedIndex=0;

uint8_t device = DFPLAYER_DEVICE_SD;


uint8_t _handleType;
uint8_t _handleCommand;
uint16_t _handleParameter;
bool _isAvailable = false;
bool _isSending = false;
bool _isInstall = false;
bool _isDebug = false;

void DF_setTimeOut(unsigned long timeOutDuration){
  _timeOutDuration = timeOutDuration;
}

void DF_uint16ToArray(uint16_t value, uint8_t *array){
  *array = (uint8_t)(value>>8);
  *(array+1) = (uint8_t)(value);
}

uint16_t DF_calculateCheckSum(uint8_t *buffer){
  uint16_t sum = 0;
  for (int i=Stack_Version; i<Stack_CheckSum; i++) {
	sum += buffer[i];
  }
  return -sum;
}

/*
  In the case of Query command, the response is returned first
  Then Module reports a feedback with this command

  sending:7E FF 6 6 1 0 1E FE D6 EF --> Request
  received:7E FF 6 41 0 0 0 FE BA EF  --> Feedback

  sending:7e ff 6 42 1 0 0 fe b8 ef --> Query
  received:7e ff 6 42 0 2 0 fe b7 ef --> Responce
  received:7E FF 6 41 0 0 0 FE BA EF  --> Feedback. Read befor next send
*/

void DF_sendStack(){
  ESP_LOGD(__FUNCTION__, "_sending[Stack_ACK]=%d _isSending=%d", _sending[Stack_ACK], _isSending);
  if (_sending[Stack_ACK]) {  //if the ack mode is on wait until the last transmition
	while (_isSending) {
	  delay(0);
	  ESP_LOGD(__FUNCTION__, "DF_available()");
	  DF_available();
	}
  }

if (_isDebug) {
  printf("\n");
  printf("sending:");
  for (int i=0; i<DFPLAYER_SEND_LENGTH; i++) {
	printf("%x", _sending[i]);
	printf(" ");
  }
  printf("\n");
}
  serial_write_buffer(_sending, DFPLAYER_SEND_LENGTH);
  _timeOutTimer = millis();
  _isSending = _sending[Stack_ACK];
  
  if (!_sending[Stack_ACK]) { //if the ack mode is off wait 10 ms after one transmition.
	delay(10);
  }
  ESP_LOGD(__FUNCTION__, "end");
}

void DF_sendStack1(uint8_t command){ // sendStack->sendStack1
  DF_sendStack2(command, 0);
}

void DF_sendStack2(uint8_t command, uint16_t argument){ // sendStack->sendStack2
  _sending[Stack_Command] = command;
  DF_uint16ToArray(argument, _sending+Stack_Parameter);
  DF_uint16ToArray(DF_calculateCheckSum(_sending), _sending+Stack_CheckSum);
  DF_sendStack();
}

void DF_sendStack3(uint8_t command, uint8_t argumentHigh, uint8_t argumentLow){  // sendStack->sendStack3
  uint16_t buffer = argumentHigh;
  buffer <<= 8;
  DF_sendStack2(command, buffer | argumentLow);
}

void DF_enableACK(){
  _sending[Stack_ACK] = 0x01;
}

void DF_disableACK(){
  _sending[Stack_ACK] = 0x00;
}

bool DF_waitAvailable(unsigned long duration){
  ESP_LOGD(__FUNCTION__, "start");
  unsigned long timer = millis();
  if (!duration) {
	duration = _timeOutDuration;
  }
  while (!DF_available()){
	ESP_LOGD(__FUNCTION__, "!DF_available()");
	if (millis() - timer > duration) {
	  return false;
	}
	//delay(0);
	delay(1);
  }
  ESP_LOGD(__FUNCTION__, "end");
  return true;
}

bool DF_begin(int txd, int rxd, bool isACK, bool doReset, bool debug){
  _isDebug = debug;
  if (_isInstall == false) {
	serial_begin(9600, txd, rxd);
	_isInstall = true;
  }
  
  if (isACK) {
	DF_enableACK();
  }
  else{
	DF_disableACK();
  }
  
  if (doReset) {
	DF_reset();
	DF_waitAvailable(2000);
	delay(200);
  }
  else {
	// assume same state as with reset(): online
	_handleType = DFPlayerCardOnline;
  }


  return (DF_readType() == DFPlayerCardOnline) || (DF_readType() == DFPlayerUSBOnline) || !isACK;
}

uint8_t DF_readType(){
  _isAvailable = false;
  return _handleType;
}

uint16_t DF_read(){
  _isAvailable = false;
  return _handleParameter;
}

bool DF_handleMessage(uint8_t type, uint16_t parameter){
  _receivedIndex = 0;
  _handleType = type;
  _handleParameter = parameter;
  _isAvailable = true;
  return _isAvailable;
}

bool DF_handleError(uint8_t type, uint16_t parameter){
  DF_handleMessage(type, parameter);
  _isSending = false;
  return false;
}


uint8_t DF_readCommand(){
  _isAvailable = false;
  return _handleCommand;
}

void DF_parseStack(){
  uint8_t handleCommand = *(_received + Stack_Command);
  if (handleCommand == 0x41) { //handle the 0x41 ack feedback as a spcecial case, in case the pollusion of _handleCommand, _handleParameter, and _handleType.
	_isSending = false;
	return;
  }
  
  _handleCommand = handleCommand;
  _handleParameter =  DF_arrayToUint16(_received + Stack_Parameter);
  ESP_LOGD(__FUNCTION__, "_handleCommand=0x%x _handleParameter=0x%x", _handleCommand, _handleParameter);

  switch (_handleCommand) {
	case 0x3D:
	  DF_handleMessage(DFPlayerPlayFinished, _handleParameter);
	  break;
	case 0x3F:
	  if (_handleParameter & 0x01) {
		DF_handleMessage(DFPlayerUSBOnline, _handleParameter);
	  }
	  else if (_handleParameter & 0x02) {
		DF_handleMessage(DFPlayerCardOnline, _handleParameter);
	  }
	  else if (_handleParameter & 0x03) {
		DF_handleMessage(DFPlayerCardUSBOnline, _handleParameter);
	  }
	  break;
	case 0x3A:
	  if (_handleParameter & 0x01) {
		DF_handleMessage(DFPlayerUSBInserted, _handleParameter);
	  }
	  else if (_handleParameter & 0x02) {
		DF_handleMessage(DFPlayerCardInserted, _handleParameter);
	  }
	  break;
	case 0x3B:
	  if (_handleParameter & 0x01) {
		DF_handleMessage(DFPlayerUSBRemoved, _handleParameter);
	  }
	  else if (_handleParameter & 0x02) {
		DF_handleMessage(DFPlayerCardRemoved, _handleParameter);
	  }
	  break;
	case 0x40:
	  DF_handleMessage(DFPlayerError, _handleParameter);
	  break;
	case 0x3C:
	case 0x3E:
	case 0x42:
	case 0x43:
	case 0x44:
	case 0x45:
	case 0x46:
	case 0x47:
	case 0x48:
	case 0x49:
	case 0x4B:
	case 0x4C:
	case 0x4D:
	case 0x4E:
	case 0x4F:
	  DF_handleMessage(DFPlayerFeedBack, _handleParameter);
	  break;
	default:
	  DF_handleError(WrongStack, 0);
	  break;
  }
}

uint16_t DF_arrayToUint16(uint8_t *array){
  uint16_t value = *array;
  value <<=8;
  value += *(array+1);
  return value;
}

bool DF_validateStack(){
  return DF_calculateCheckSum(_received) == DF_arrayToUint16(_received+Stack_CheckSum);
}

bool DF_available(){
  while (serial_available()) {
	delay(0);
	if (_receivedIndex == 0) {
	  _received[Stack_Header] = serial_read();
if (_isDebug) {
	  printf("received:");
	  printf("%x", _received[_receivedIndex]);
	  printf(" ");
}
	  if (_received[Stack_Header] == 0x7E) {
		_receivedIndex ++;
	  }
	}
	else{
	  _received[_receivedIndex] = serial_read();
if (_isDebug) {
	  printf("%x", _received[_receivedIndex]);
	  printf(" ");
}
	  switch (_receivedIndex) {
		case Stack_Version:
		  if (_received[_receivedIndex] != 0xFF) {
			return DF_handleError(WrongStack, 0);
		  }
		  break;
		case Stack_Length:
		  if (_received[_receivedIndex] != 0x06) {
			return DF_handleError(WrongStack, 0);
		  }
		  break;
		case Stack_End:
if (_isDebug) {
		  printf("\n");
}
		  if (_received[_receivedIndex] != 0xEF) {
			return DF_handleError(WrongStack, 0);
		  }
		  else{
			if (DF_validateStack()) {
			  _receivedIndex = 0;
			  DF_parseStack();
			  return _isAvailable;
			}
			else{
			  return DF_handleError(WrongStack, 0);
			}
		  }
		  break;
		default:
		  break;
	  }
	  _receivedIndex++;
	}
  }
  
  if (_isSending && (millis()-_timeOutTimer>=_timeOutDuration)) {
	return DF_handleError(TimeOut, 0);
  }
  
	
  return _isAvailable;
}

void DF_next(){
  DF_sendStack1(0x01);
}

void DF_previous(){
  DF_sendStack1(0x02);
}

void DF_play(int fileNumber){
  DF_sendStack2(0x03, fileNumber);
}

void DF_volumeUp(){
  DF_sendStack1(0x04);
}

void DF_volumeDown(){
  DF_sendStack1(0x05);
}

void DF_volume(uint8_t volume){
  DF_sendStack2(0x06, volume);
}

void DF_EQ(uint8_t eq) {
  DF_sendStack2(0x07, eq);
}

void DF_loop(int fileNumber) {
  DF_sendStack2(0x08, fileNumber);
}

void DF_outputDevice(uint8_t device) {
  DF_sendStack2(0x09, device);
  delay(200);
}

void DF_sleep(){
  DF_sendStack1(0x0A);
}

void DF_reset(){
  DF_sendStack1(0x0C);
}

void DF_start(){
  DF_sendStack1(0x0D);
}

void DF_pause(){
  DF_sendStack1(0x0E);
}

void DF_playFolder(uint8_t folderNumber, uint8_t fileNumber){
  DF_sendStack3(0x0F, folderNumber, fileNumber);
}

void DF_outputSetting(bool enable, uint8_t gain){
  DF_sendStack3(0x10, enable, gain);
}

void DF_enableLoopAll(){
  DF_sendStack2(0x11, 0x01);
}

void DF_disableLoopAll(){
  DF_sendStack2(0x11, 0x00);
}

void DF_playMp3Folder(int fileNumber){
  DF_sendStack2(0x12, fileNumber);
}

void DF_advertise(int fileNumber){
  DF_sendStack2(0x13, fileNumber);
}

void DF_playLargeFolder(uint8_t folderNumber, uint16_t fileNumber){
  DF_sendStack2(0x14, (((uint16_t)folderNumber) << 12) | fileNumber);
}

void DF_stopAdvertise(){
  DF_sendStack1(0x15);
}

void DF_stop(){
  DF_sendStack1(0x16);
}

void DF_loopFolder(int folderNumber){
  DF_sendStack2(0x17, folderNumber);
}

void DF_randomAll(){
  DF_sendStack1(0x18);
}

void DF_enableLoop(){
  DF_sendStack2(0x19, 0x00);
}

void DF_disableLoop(){
  DF_sendStack2(0x19, 0x01);
}

void DF_enableDAC(){
  DF_sendStack2(0x1A, 0x00);
}

void DF_disableDAC(){
  DF_sendStack2(0x1A, 0x01);
}

uint16_t DF_readState(){
  DF_sendStack1(0x42);
  if (DF_waitAvailable(0)) {
	if (DF_readType() == DFPlayerFeedBack) {
	  return DF_read();
	}
	else{
	  return -1;
	}
  }
  else{
	return -1;
  }
}

int DF_readVolume(){
  DF_sendStack1(0x43);
  if (DF_waitAvailable(0)) {
	return DF_read();
  }
  else{
	return -1;
  }
}

int DF_readEQ(){
  DF_sendStack1(0x44);
  if (DF_waitAvailable(0)) {
	if (DF_readType() == DFPlayerFeedBack) {
	  return DF_read();
	}
	else{
	  return -1;
	}
  }
  else{
	return -1;
  }
}

int DF_readFileCounts(uint8_t device){
  switch (device) {
	case DFPLAYER_DEVICE_U_DISK:
	  DF_sendStack1(0x47);
	  break;
	case DFPLAYER_DEVICE_SD:
	  DF_sendStack1(0x48);
	  break;
	case DFPLAYER_DEVICE_FLASH:
	  DF_sendStack1(0x49);
	  break;
	default:
	  break;
  }
  
  if (DF_waitAvailable(0)) {
	if (DF_readType() == DFPlayerFeedBack) {
	  return DF_read();
	}
	else{
	  return -1;
	}
  }
  else{
	return -1;
  }
}

int DF_readCurrentFileNumber(uint8_t device){
  switch (device) {
	case DFPLAYER_DEVICE_U_DISK:
	  DF_sendStack1(0x4B);
	  break;
	case DFPLAYER_DEVICE_SD:
	  DF_sendStack1(0x4C);
	  break;
	case DFPLAYER_DEVICE_FLASH:
	  DF_sendStack1(0x4D);
	  break;
	default:
	  break;
  }
  if (DF_waitAvailable(0)) {
	if (DF_readType() == DFPlayerFeedBack) {
	  return DF_read();
	}
	else{
	  return -1;
	}
  }
  else{
	return -1;
  }
}

int DF_readFileCountsInFolder(int folderNumber){
  DF_sendStack2(0x4E, folderNumber);
  if (DF_waitAvailable(0)) {
	if (DF_readType() == DFPlayerFeedBack) {
	  return DF_read();
	}
	else{
	  return -1;
	}
  }
  else{
	return -1;
  }
}

int DF_readFolderCounts(){
  DF_sendStack1(0x4F);
  if (DF_waitAvailable(0)) {
	if (DF_readType() == DFPlayerFeedBack) {
	  return DF_read();
	}
	else{
	  return -1;
	}
  }
  else{
	return -1;
  }
}

#if 0
int DF_readFileCounts(){
  return DF_readFileCounts(DFPLAYER_DEVICE_SD);
}

int DF_readCurrentFileNumber(){
  return DF_readCurrentFileNumber(DFPLAYER_DEVICE_SD);
}
#endif

/*
Wait until play finish.
For some reason, when the play finished, Play Finished event is notified twice.
I don't know why.

received:7E FF 6 3D 0 0 1 FE BD EF
Number:1 Play Finished!
received:7E FF 6 3D 0 0 1 FE BD EF
Number:1 Play Finished!
*/
bool DF_isFinished(int *value) {
  static int finishd = 0;
  if (DF_available()) {
    uint8_t type = DF_readType();
	*value = DF_read();
if (_isDebug) {
    printf("type=%d value=%d finishd=%d\n", type, *value, finishd);
}
    if (type == DFPlayerPlayFinished) {
      finishd++;
    }
    if (finishd == 2) {
      finishd = 0;
      return true;
    }
  }
  return false;
}


void DF_printDetail(uint8_t type, int value){
  switch (type) {
	case TimeOut:
	  printf("Time Out!\n");
	  break;
	case WrongStack:
	  printf("Stack Wrong!\n");
	  break;
	case DFPlayerCardInserted:
	  printf("Card Inserted!\n");
	  break;
	case DFPlayerCardRemoved:
	  printf("Card Removed!\n");
	  break;
	case DFPlayerCardOnline:
	  printf("Card Online!\n");
	  break;
	case DFPlayerPlayFinished:
	  printf("Number:%d Play Finished!\n", value);
	  break;
	case DFPlayerError:
	  printf("DFPlayerError:");
	  switch (value) {
		case Busy:
		  printf("Card not found\n");
		  break;
		case Sleeping:
		  printf("Sleeping\n");
		  break;
		case SerialWrongStack:
		  printf("Get Wrong Stack\n");
		  break;
		case CheckSumNotMatch:
		  printf("Check Sum Not Match\n");
		  break;
		case FileIndexOut:
		  printf("File Index Out of Bound\n");
		  break;
		case FileMismatch:
		  printf("Cannot Find File\n");
		  break;
		case Advertise:
		  printf("In Advertise\n");
		  break;
		default:
		  break;
	  }
	  break;
	default:
	  break;
  }
}
