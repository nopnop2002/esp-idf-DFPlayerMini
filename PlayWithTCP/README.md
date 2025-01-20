# PlayWithTCP

Play music using the TCP.   
This example requires WiFi.   

![Image](https://github.com/user-attachments/assets/ea42bf90-4550-43e1-9476-f9481ee1b839)
![Image](https://github.com/user-attachments/assets/4667d9f0-571b-43c9-b949-9965ece69136)

- Play   
```echo -n "play" | socat -t 30 tcp:esp32-server.local:9876 -```   
- Stop   
```echo -n "stop" | socat -t 30 tcp:esp32-server.local:9876 -```   
- Next   
```echo -n "next" | socat -t 30 tcp:esp32-server.local:9876 -```   
- Query current status   
```echo -n "query" | socat -t 30 tcp:esp32-server.local:9876 -```   


