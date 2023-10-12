import socket
import time 

s = socket.socket()         
 
s.bind(('0.0.0.0', 8090 ))
s.listen(0)                 
 
while True:
    try:
        client, addr = s.accept()
    
        while True:
            content = client.recv(1024)
    
            if len(content) ==0:
                break
    
            else:
                print(content.decode())
            
            time.sleep(1)

            write = "hellow from script"

            client.send(write.encode())

            time.sleep(1)

    except KeyboardInterrupt:
        print("exiting")
        print("Closing connection")
        s.detach()
        s.close()
        client.close()
        client.detach()
        exit()
