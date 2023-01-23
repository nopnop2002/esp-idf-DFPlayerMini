# esp-idf-DFPlayerMini
DFPlayer - A Mini MP3 Player For ESP-IDF.   

ESP32 has I2S output function.   
By using ESP-ADF and I2S amplifier, MP3 files can be played.   
However, ESP-ADF is esoteric and depends on a specific version of ESP-IDF.   
I made this for the purpose of playing MP3 files more easily.   

I ported from [here](https://github.com/DFRobot/DFRobotDFPlayerMini) and used [this](https://github.com/nopnop2002/esp-idf-serial) component.

# Software requirements
ESP-IDF V4.4/V5.0.   

# Hardware requirements
FN-M16P Embedded MP3 Audio Module.   
It's called DFPlayerMini.   

# Installation
Copy all the contents of mp3Data to the SD card.   
Insert the SD card into the player.   

```Shell
git clone https://github.com/nopnop2002/esp-idf-DFPlayerMini
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
|GPIO4|--|TX(*)|||
|GPIO5|--|RX(*)|||
|||SPK+|--|Speaker+|
|GND|--|GND|||
|||SPK-|--|Speaker-|

(*) You can change it using menuconfig.   

__This module is very vulnerable to noise.__   

Typical circuit   
![ESP32-DFRobot-DFPlayer-Mini-1](https://user-images.githubusercontent.com/6020549/213993262-ed8a3f7a-add9-48e2-b618-0d4ecce669bc.jpg)


# Wiring with an external amplifier
|ESP32||DFPlayerMini||AMP|
|:-:|:-:|:-:|:-:|:-:|
|3.3V|--|VCC|||
|GPIO4|--|TX(*)|||
|GPIO5|--|RX(*)|||
|||DAC_R|--|Right Channl Input|
|||DAC_I|--|Left Channl Input|
|GND|--|GND|--|GND|

Typical circuit   
![ESP32-DFRobot-DFPlayer-Mini-2](https://user-images.githubusercontent.com/6020549/213993306-7e591d0a-bfb6-42ce-bd71-62b3feef8a9c.jpg)

