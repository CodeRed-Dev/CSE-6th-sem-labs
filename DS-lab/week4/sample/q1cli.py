#TCP client
import socket
host = socket.gethostname()
port = 1222
s = socket.socket()
s.connect((host, port))
s.sendall(b'Welcome users!')
data = s.recv(1224)
s.close()
print(repr(data))