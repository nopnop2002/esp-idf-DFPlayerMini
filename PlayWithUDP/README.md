# PlayWithUDP

Play music using the UDP.   
This example requires WiFi.   

![config-top](https://user-images.githubusercontent.com/6020549/215076675-9ff62410-2996-4259-9262-307726c466ea.jpg)
![config-app](https://user-images.githubusercontent.com/6020549/215078308-82bb0c81-80ac-41f6-b11d-63db1dfa2f01.jpg)

- Play   
```echo "play" | socat - UDP-DATAGRAM:255.255.255.255:9876,broadcast```   
- Stop   
```echo "stop" | socat - UDP-DATAGRAM:255.255.255.255:9876,broadcast```   
- Next   
```echo "next" | socat - UDP-DATAGRAM:255.255.255.255:9876,broadcast```   
- Query current status   
```echo "query" | socat - UDP-DATAGRAM:255.255.255.255:9876,broadcast```   


