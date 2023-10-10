import socket
import time 

s = socket.socket()         
 
s.bind(('0.0.0.0', 8090 ))
s.listen(0)                 
 
while True:
 
    client, addr = s.accept()
 
    while True:
        content = client.recv(32)
 
        if len(content) ==0:
           break
 
        else:
            print(content)
        
        time.sleep(1)

        write = "hellow from script"

        client.send(write.encode())

        time.sleep(1)
 
        print("Closing connection")
        client.close()
        exit()