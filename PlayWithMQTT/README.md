# PlayWithMQTT

Play music using the MQTT.   
This example requires WiFi and MQTT Broker.   

![config-top](https://user-images.githubusercontent.com/6020549/213997705-0dcb798f-7abb-40d3-a365-575713222be0.jpg)
![config-app](https://user-images.githubusercontent.com/6020549/213997717-5dcac080-2385-4ef2-9712-400bf9c1daee.jpg)

Set the information of your access point.   
![config-wifi](https://user-images.githubusercontent.com/6020549/213997727-4e6535c5-1c6d-4e2e-b642-f84cc35d84ee.jpg)

Set the information of your MQTT broker.   
![Image](https://github.com/user-attachments/assets/0cbbae87-d658-4ea5-b8e9-529c97181598)

### Select Transport   
This project supports TCP,SSL/TLS,WebSocket and WebSocket Secure Port.   

- Using TCP Port.   
 TCP Port uses the MQTT protocol.   

- Using SSL/TLS Port.   
 SSL/TLS Port uses the MQTTS protocol instead of the MQTT protocol.   

- Using WebSocket Port.   
 WebSocket Port uses the WS protocol instead of the MQTT protocol.   

- Using WebSocket Secure Port.   
 WebSocket Secure Port uses the WSS protocol instead of the MQTT protocol.   

__Note for using secure port.__   
The default MQTT server is ```broker.emqx.io```.   
If you use a different server, you will need to modify ```getpem.sh``` to run.   
```
chmod 777 getpem.sh
./getpem.sh
```

WebSocket/WebSocket Secure Port may differ depending on the broker used.   
If you use a different MQTT server than the default, you will need to change the port number from the default.   

### Select MQTT Protocol   
This project supports MQTT Protocol V3.1.1/V5.   
![Image](https://github.com/user-attachments/assets/c51bfe01-1ff1-4418-b3e3-6381aec65ade)

### Enable Secure Option   
Specifies the username and password if the server requires a password when connecting.   
[Here's](https://www.digitalocean.com/community/tutorials/how-to-install-and-secure-the-mosquitto-mqtt-messaging-broker-on-debian-10) how to install and secure the Mosquitto MQTT messaging broker on Debian 10.   
![Image](https://github.com/user-attachments/assets/2158d1ac-d3a1-4f83-b887-6633db6b6934)


# Using mosquitto client

- Play   
```mosquitto_pub -h broker.emqx.io -p 1883 -t "/topic/dfplay/play" -m ""```   
- Stop   
```mosquitto_pub -h broker.emqx.io -p 1883 -t "/topic/dfplay/stop" -m ""```   
- Next   
```mosquitto_pub -h broker.emqx.io -p 1883 -t "/topic/dfplay/next" -m ""```   
- Information inquiry   
```mosquitto_pub -h broker.emqx.io -p 1883 -t "/topic/dfplay/query" -m ""```   

# Using Android
With this [app](https://play.google.com/store/apps/details?id=appinventor.ai_myhomethings_eu.MqttClientApp) you can start playing using your Android.   
![Image](https://github.com/user-attachments/assets/01d74a9a-fd23-4207-817c-7325db8c3d08)

With this [app](https://play.google.com/store/apps/details?id=net.routix.mqttdash) you can start playing using your Android.   
![Image](https://github.com/user-attachments/assets/6d19efe1-2362-4f24-958d-8d1194f8672f)
