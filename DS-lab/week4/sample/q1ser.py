#TCP connection server
import socket
host = '172.16.57.114/24'
port = 12341
s = socket.socket()
s.bind((host, port))
s.listen(5)
conn, addr = s.accept()
print('Got connection from', addr[0],'{', addr[1],'}')
print("thank you for the connection")
while True:
	data = conn.recv(1224)
	if not data: break
	conn.sendall(data)
conn.close()	
