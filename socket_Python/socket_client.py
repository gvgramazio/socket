# Echo client program
import socket

HOST = '127.0.0.1'    # The remote host
PORT = 12345              # The same port as used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
data_to_send = ['Hello', 'world', 'from', 'the', 'python', 'client']
for i in range(6):
    s.sendall(data_to_send[i])
    data = s.recv(1024)
    print 'Received message:', repr(data)
s.close()
#print 'Received', repr(data)
