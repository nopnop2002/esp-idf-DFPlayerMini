# PlayWithMQTT

Play music using the MQTT.   
This example requires WiFi and MQTT Broker.   

![config-top](https://user-images.githubusercontent.com/6020549/213997705-0dcb798f-7abb-40d3-a365-575713222be0.jpg)
![config-app](https://user-images.githubusercontent.com/6020549/213997717-5dcac080-2385-4ef2-9712-400bf9c1daee.jpg)
![config-wifi](https://user-images.githubusercontent.com/6020549/213997727-4e6535c5-1c6d-4e2e-b642-f84cc35d84ee.jpg)
![config-broker](https://user-images.githubusercontent.com/6020549/213997731-366e50c0-94e6-4f90-a44c-77c307fa3105.jpg)

- Play   
```mosquitto_pub -h broker.emqx.io -p 1883 -t "/topic/dfplay/play" -m ""```   
- Stop   
```mosquitto_pub -h broker.emqx.io -p 1883 -t "/topic/dfplay/stop" -m ""```   
- Next   
```mosquitto_pub -h broker.emqx.io -p 1883 -t "/topic/dfplay/next" -m ""```   
- Query current status   
```mosquitto_pub -h broker.emqx.io -p 1883 -t "/topic/dfplay/query" -m ""```   

