# esp-idf-DFPlayerMini
DFPlayer - A Mini MP3 Player For ESP-IDF.   

ESP32 has I2S output function.   
By using ESP-ADF and I2S amplifier, MP3 files can be played.   
However, ESP-ADF is esoteric and also depends on a specific version of ESP-IDF.   
Also, for ESP32C2, I can't find any mention of I2S.   

I made this for the purpose of playing MP3 files more easily.   

I ported from [here](https://github.com/DFRobot/DFRobotDFPlayerMini) and used [this](https://github.com/nopnop2002/esp-idf-serial) component.

# Software requirements
ESP-IDF V4.4/V5.x.   
ESP-IDF V5.0 is required when using ESP32C2.   
ESP-IDF V5.1 is required when using ESP32C6 and ESP32H2.   

# Hardware requirements
DFPlayer - A Mini MP3 Player For Arduino.   
https://www.dfrobot.com/index.php?route=product/product&product_id=1121   

# Installation
Copy all the contents of mp3Data to the SD card.   
Insert the SD card into the player.   

```Shell
git clone https://github.com/nopnop2002/esp-idf-DFPlayerMini
# mount your SD card
cp esp-idf-DFPlayerMini/mp3Data/* your_SD_card
# unmount your SD card and insert the SD card into the player
cd esp-idf-DFPlayerMini/GetStarted
idf.py menuconfig
idf.py flash
```

# Configuration

![config-top](https://user-images.githubusercontent.com/6020549/213992858-400a5d6d-0165-4394-9d3d-a84ef93270f1.jpg)

![config-DFPlayerMini](https://user-images.githubusercontent.com/6020549/213992853-8a2c9466-00a2-4981-af66-aea1bfc5b76b.jpg)


# Wiring with built-in amplifier
|ESP32||DFPlayerMini||Speker|
|:-:|:-:|:-:|:-:|:-:|
|3.3V|--|VCC|||
|GPIO4(*)|--|RX|||
|GPIO5(*)|--|TX|||
|||SPK+|--|Speaker+|
|GND|--|GND|||
|||SPK-|--|Speaker-|

(*) You can change it using menuconfig.   

__The built-in amplifier of this module is very sensitive to noise.__   

Typical circuit   
![ESP32-DFRobot-DFPlayer-Mini-1](https://user-images.githubusercontent.com/6020549/213993262-ed8a3f7a-add9-48e2-b618-0d4ecce669bc.jpg)


# Wiring with an external amplifier
|ESP32||DFPlayerMini||AMP|
|:-:|:-:|:-:|:-:|:-:|
|3.3V|--|VCC|--|VCC|
|GPIO4(*)|--|RX|||
|GPIO5(*)|--|TX|||
|||DAC_R|--|Right Channl Input|
|||DAC_I|--|Left Channl Input|
|GND|--|GND|--|GND|

Typical circuit   
![ESP32-DFRobot-DFPlayer-Mini-2](https://user-images.githubusercontent.com/6020549/213993306-7e591d0a-bfb6-42ce-bd71-62b3feef8a9c.jpg)

# Micro SD Card
I've tried several different brands of SD cards and some don't work.   
I get this error.   
I fixed it by replacing the SD card with another brand.   
It's probably related to the power consumption of the SD card.   
```
received:7e ff 6 3b 0 0 2 fe be ef
Card Removed!
```

# About sample mp3 data
I borrowed from here.(Japanese site)   
This is a typical alarm used in elementary and junior high schools in Japan to announce the start and end of classes.   
https://otologic.jp/free/se/school_bell01.html#google_vignette

# API
```
//----Mp3 play----
DF_next();  //Play next mp3
DF_previous();  //Play previous mp3
DF_play(1);  //Play the first mp3
DF_loop(1);  //Loop the first mp3
DF_pause();  //pause the mp3
DF_start();  //start the mp3 from the pause
DF_playFolder(15, 4);  //play specific mp3 in SD:/15/004.mp3; Folder Name(1~99); File Name(1~255)
DF_enableLoopAll(); //loop all mp3 files.
DF_disableLoopAll(); //stop loop all mp3 files.
DF_playMp3Folder(4); //play specific mp3 in SD:/MP3/0004.mp3; File Name(0~65535)
DF_advertise(3); //advertise specific mp3 in SD:/ADVERT/0003.mp3; File Name(0~65535)
DF_stopAdvertise(); //stop advertise
DF_playLargeFolder(2, 999); //play specific mp3 in SD:/02/004.mp3; Folder Name(1~10); File Name(1~1000)
DF_loopFolder(5); //loop all mp3 files in folder SD:/05.
DF_randomAll(); //Random play all the mp3.
DF_enableLoop(); //enable loop.
DF_disableLoop(); //disable loop.

//----Read imformation----
printf("%d",DF_readState()); //read mp3 state
printf("%d",DF_readVolume()); //read current volume
printf("%d",DF_readEQ()); //read EQ setting
printf("%d",DF_readFileCounts()); //read all file counts in SD card
printf("%d",DF_readCurrentFileNumber()); //read current play file number
printf("%d",DF_readFileCountsInFolder(3)); //read fill counts in folder SD:/03
```

# Document
Document is [here](https://picaxe.com/docs/spe033.pdf).   

# MP3 Chip
There are many variations of the MP3 IC installed in DF Player Mini.   
I own three DF Players, two with JC AA18 and one with YX5200.   
JC AA18 works very stable.   
But YX5200 give me this error sometime:
```
sending:7e ff 6 6 0 0 1e fe d7 ef

sending:7e ff 6 3 0 0 1 fe f7 ef
received:7e ff 6 40 0 0 4 fe b7 ef
DFPlayerError:Check Sum Not Match
```

In addition to this, there are YM5200, YM5300, MH2024K, GD3200B, etc.   
There are also articles like this.   
https://github.com/arendst/Tasmota/discussions/11737   
https://forum.arduino.cc/t/df-player-warning/952842   
https://discourse.voss.earth/t/probleme-mit-dem-dfplayer-mini/12203   

![MP3_CHIP](https://github.com/nopnop2002/esp-idf-DFPlayerMini/assets/6020549/8809b4e9-6765-41c7-8c71-5b413f8111da)

# For more info
https://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299   
