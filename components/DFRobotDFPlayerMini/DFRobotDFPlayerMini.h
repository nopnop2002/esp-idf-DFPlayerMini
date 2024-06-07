/*!
 * @file DFRobotDFPlayerMini.h
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


#ifndef DFRobotDFPlayerMini
#define DFRobotDFPlayerMini


#define DFPLAYER_EQ_NORMAL 0
#define DFPLAYER_EQ_POP 1
#define DFPLAYER_EQ_ROCK 2
#define DFPLAYER_EQ_JAZZ 3
#define DFPLAYER_EQ_CLASSIC 4
#define DFPLAYER_EQ_BASS 5

#define DFPLAYER_DEVICE_U_DISK 1
#define DFPLAYER_DEVICE_SD 2
#define DFPLAYER_DEVICE_AUX 3
#define DFPLAYER_DEVICE_SLEEP 4
#define DFPLAYER_DEVICE_FLASH 5

#define DFPLAYER_RECEIVED_LENGTH 10
#define DFPLAYER_SEND_LENGTH 10

//#define _DEBUG

#define TimeOut 0
#define WrongStack 1
#define DFPlayerCardInserted 2
#define DFPlayerCardRemoved 3
#define DFPlayerCardOnline 4
#define DFPlayerPlayFinished 5
#define DFPlayerError 6
#define DFPlayerUSBInserted 7
#define DFPlayerUSBRemoved 8
#define DFPlayerUSBOnline 9
#define DFPlayerCardUSBOnline 10
#define DFPlayerFeedBack 11

#define Busy 1
#define Sleeping 2
#define SerialWrongStack 3
#define CheckSumNotMatch 4
#define FileIndexOut 5
#define FileMismatch 6
#define Advertise 7

#define Stack_Header 0
#define Stack_Version 1
#define Stack_Length 2
#define Stack_Command 3
#define Stack_ACK 4
#define Stack_Parameter 5
#define Stack_CheckSum 7
#define Stack_End 9

// Arduino compatible macros
#define delayMicroseconds(microsec) esp_rom_delay_us(microsec)
#define delay(millsec) esp_rom_delay_us(millsec*1000)
#define millis() xTaskGetTickCount()*portTICK_PERIOD_MS

#if 0
  Stream* _serial;
  
  unsigned long _timeOutTimer;
  unsigned long _timeOutDuration = 500;
  
  uint8_t _received[DFPLAYER_RECEIVED_LENGTH];
  uint8_t _sending[DFPLAYER_SEND_LENGTH] = {0x7E, 0xFF, 06, 00, 01, 00, 00, 00, 00, 0xEF};
  
  uint8_t _receivedIndex=0;
#endif

  void DF_sendStack();
  void DF_sendStack1(uint8_t command);
  void DF_sendStack2(uint8_t command, uint16_t argument);
  void DF_sendStack3(uint8_t command, uint8_t argumentHigh, uint8_t argumentLow);

  void DF_enableACK();
  void DF_disableACK();
  
  void DF_uint16ToArray(uint16_t value,uint8_t *array);
  
  uint16_t DF_arrayToUint16(uint8_t *array);
  
  uint16_t DF_calculateCheckSum(uint8_t *buffer);
  


  void DF_parseStack();
  bool DF_validateStack();
  
  bool DF_handleMessage(uint8_t type, uint16_t parameter);
  bool DF_handleError(uint8_t type, uint16_t parameter);

  uint8_t DF_readCommand();
  
  bool DF_begin(int txd, int rxd, bool isACK, bool doReset, bool debug);
  
  bool DF_waitAvailable(unsigned long duration);
  
  bool DF_available();
  
  uint8_t DF_readType();
  
  uint16_t DF_read();
  
  void DF_setTimeOut(unsigned long timeOutDuration);
  
  void DF_next();
  
  void DF_previous();
  
  void DF_play(int fileNumber);
  
  void DF_volumeUp();
  
  void DF_volumeDown();
  
  void DF_volume(uint8_t volume);
  
  void DF_EQ(uint8_t eq);
  
  void DF_loop(int fileNumber);
  
  void DF_outputDevice(uint8_t device);
  
  void DF_sleep();
  
  void DF_reset();
  
  void DF_start();
  
  void DF_pause();
  
  void DF_playFolder(uint8_t folderNumber, uint8_t fileNumber);
  
  void DF_outputSetting(bool enable, uint8_t gain);
  
  void DF_enableLoopAll();
  
  void DF_disableLoopAll();
  
  void DF_playMp3Folder(int fileNumber);
  
  void DF_advertise(int fileNumber);
  
  void DF_playLargeFolder(uint8_t folderNumber, uint16_t fileNumber);
  
  void DF_stopAdvertise();
  
  void DF_stop();
  
  void DF_loopFolder(int folderNumber);
  
  void DF_randomAll();
  
  void DF_enableLoop();
  
  void DF_disableLoop();
  
  void DF_enableDAC();
  
  void DF_disableDAC();
  
  uint16_t DF_readState();
  
  int DF_readVolume();
  
  int DF_readEQ();
  
  int DF_readFileCounts(uint8_t device);
  
  int DF_readCurrentFileNumber(uint8_t device);
  
  int DF_readFileCountsInFolder(int folderNumber);
  
#if 0
  int DF_readFileCounts();
#endif

  int DF_readFolderCounts();
  
#if 0
  int DF_readCurrentFileNumber();
#endif
  
  bool DF_isFinished(int *value);

  void DF_printDetail(uint8_t type, int value);

#endif
