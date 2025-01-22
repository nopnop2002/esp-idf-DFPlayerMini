# PlayWithHTTP

Play music using the HTTP.   
This example requires WiFi.   

![config-top](https://user-images.githubusercontent.com/6020549/213999270-4d38f93e-7130-4d35-939f-1f2b93746c28.jpg)
![config-app-1](https://user-images.githubusercontent.com/6020549/214173796-e36c5c53-13f0-4cac-87a9-579f6657a7f7.jpg)

You can connect using the mDNS hostname instead of the IP address.   
![config-app-2](https://user-images.githubusercontent.com/6020549/214173801-54c53ce0-15bd-4dba-8840-e39e9f645717.jpg)

# Using curl
- Play   
```curl "http://esp32-player.local:8080/play"``` or ```curl "http://IP_OF_ESP32:8080/play"```   
- Stop   
```curl "http://esp32-player.local:8080/stop"``` or ```curl "http://IP_OF_ESP32:8080/stop"```   
- Next   
```curl "http://esp32-player.local:8080/next"``` or ```curl "http://IP_OF_ESP32:8080/next"```   
- Information inquiry   
```curl "http://esp32-player.local:8080/query"``` or ```curl "http://IP_OF_ESP32:8080/query"```   


# Using Web Browser
Double-click the links below or open them in a new tab.   
This will launch your browser.   
- Play   
 http://esp32-player.local:8080/play
- Stop   
 http://esp32-player.local:8080/stop
- Next   
 http://esp32-player.local:8080/next
- Query current status   
 http://esp32-player.local:8080/query

# Using Android
With this [app](https://play.google.com/store/apps/details?id=com.app.restclient) you can start playing using your Android.   
![Image](https://github.com/user-attachments/assets/8db268bd-82b2-4939-94b9-83bfe254c427)

Once you register a URL in Collections, you can select the URL from Collections.   
![Image](https://github.com/user-attachments/assets/c0a22237-509e-404c-80c8-b546fe88d43f)

![Image](https://github.com/user-attachments/assets/d26d8352-d15c-43ed-aa31-5522b7780a65)
