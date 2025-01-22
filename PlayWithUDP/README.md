# PlayWithUDP

Play music using the UDP.   
This example requires WiFi.   

![config-top](https://user-images.githubusercontent.com/6020549/215076675-9ff62410-2996-4259-9262-307726c466ea.jpg)
![Image](https://github.com/user-attachments/assets/00a2c9fb-2776-4edf-8eda-4f5cf5cf5a18)

# Using socat
- Play   
```echo -n "play" | socat - UDP-DATAGRAM:esp32-server.local:9876```   
- Stop   
```echo -n "stop" | socat - UDP-DATAGRAM:esp32-server.local:9876```   
- Next   
```echo -n "next" | socat - UDP-DATAGRAM:esp32-server.local:9876```   
- Query current status   
```echo -n "query" | socat - UDP-DATAGRAM:esp32-server.local:9876```   

# Using netcat(nc)
- Play   
```echo -n "play" | netcat -u -w1 esp32-server.local 9876```   
- Stop   
```echo -n "stop" | netcat -u -w1 esp32-server.local 9876```   
- Next   
```echo -n "next" | netcat -u -w1 esp32-server.local 9876```   
- Query current status   
```echo -n "query" | netcat -u -w1 esp32-server.local 9876```   
