# PlayWithCron

Play music using the MQTT.   
This example requires WiFi and NTP Server.   

![config-top](https://user-images.githubusercontent.com/6020549/214195753-18010318-4560-42fe-8aed-239fa8f8b7f6.jpg)
![config-app](https://user-images.githubusercontent.com/6020549/214195744-3fbc0c75-16f4-497c-af03-80935bbb69b7.jpg)
![config-wifi](https://user-images.githubusercontent.com/6020549/214195754-ae72367f-7013-4096-9822-df56108c9ef8.jpg)
![config-ntp](https://user-images.githubusercontent.com/6020549/214195749-639c3ce1-3f7f-4f90-9109-d002028a2c45.jpg)

This example use [this](https://github.com/staticlibs/ccronexpr) library.   
Defining dates and times in crontab is complicated, but this library handles it well.   
It's Great job.   

# crontab
crontab is in the crontab directory.
Supports cron expressions with seconds field.
```
      +---------------------------  second (0 - 59)
      |  +------------------------  minute (0 - 59)
      |  |  +---------------------  hour (0 - 23)
      |  |  |  +------------------  day of month (1 - 31)
      |  |  |  |  +---------------  month (1 - 12)
      |  |  |  |  |  +------------  day of week (0 - 6) (Sunday to Saturday;
      |  |  |  |  |  |                                   7 is also Sunday on some systems)
      |  |  |  |  |  |  +---------  track number in SD
      |  |  |  |  |  |  |
      *  *  *  *  *  *  task
```

In Linux you specify the command, but in this project you specify the track number is SD.   
__Note:__   
This sample does not judge even if the same time overlaps.   
If the same time overlaps, the later definition in the file takes effect.   

```
# Edit this file to introduce tasks to be run by cron.
#
# Each task to run has to be defined through a single line
# indicating with different fields when the task will be run
# and what command to run for the task
#
# To define the time you can provide concrete values for
# minute (m), hour (h), day of month (dom), month (mon),
# and day of week (dow) or use '*' in these fields (for 'any').
#
# Notice that tasks will be started based on the cron's system
# daemon's notion of time and timezones.
#
# Output of the crontab jobs (including errors) is sent through
# email to the user the crontab file belongs to (unless redirected).
#
# For example, you can run a backup of all your user accounts
# at 5 a.m every week with:
# 0 5 * * 1 tar -zcf /var/backups/home.tgz /home/
#
# For more information see the manual pages of crontab(5) and cron(8)
#
# s m h  dom mon dow   task
0 0 0-11 * * * 1            # 0 minutes every hour in the morning
0 15,30,45 0-11 * * * 2     # every 1/4 hour in the morning
0 0 12-23 * * * 3           # 0 minutes every hour in the afternoon
0 15,30,45 12-23 * * * 4    # every 1/4 hour in the afternoon
```

# Screen Shot
```
I (9453) PLAY: Waitting ....
I (777643) MAIN: current time=[2023/01/24 09:45:00]
I (777643) MAIN: NotifGive to 2 [0 15,30,45 0-11 * * *]
I (777643) PLAY: key=0x32 --> Play track number 2

I (778653) PLAY: Waitting ....
I (1677303) MAIN: current time=[2023/01/24 10:00:00]
I (1677303) MAIN: NotifGive to 1 [0 0 0-11 * * *]
I (1677303) PLAY: key=0x31 --> Play track number 1

I (1678323) PLAY: Waitting ....
I (2577233) MAIN: current time=[2023/01/24 10:15:00]
I (2577233) MAIN: NotifGive to 2 [0 15,30,45 0-11 * * *]
I (2577233) PLAY: key=0x32 --> Play track number 2

I (2578253) PLAY: Waitting ....
I (3477433) MAIN: current time=[2023/01/24 10:30:00]
I (3477433) MAIN: NotifGive to 2 [0 15,30,45 0-11 * * *]
I (3477433) PLAY: key=0x32 --> Play track number 2

I (3478453) PLAY: Waitting ....

