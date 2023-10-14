import socket
import time 

s = socket.socket()         
 
s.bind(('0.0.0.0', 8090 ))
s.listen(0)                 
 

client, addr = s.accept()

while True:
    # time.sleep(1)
    tcp_msg = "hellow"
    client.send(tcp_msg.encode())
    print("Hellow")
   time.sleep(1)
    msf = client.recv(1204)
    print(msf)
    time.sleep(1)


