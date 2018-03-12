# -*- coding: utf-8 -*-
import socket

HOST = '127.0.0.1'  # Host
PORT = 12345        # Arbitrary port

# Create a new socket using AF_INET address family and SOCK_STREAM socket type.
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Running an example several times with too small delay between executions,
# could lead to this error:
#   socket.error: [Errno 98] Address already in use
# This is because the previous execution has left the socket in a TIME_WAIT
# state, and can't be immediately reused.
# The SO_REUSEADDR flag tells the kernel to reuse a local socket in TIME_WAIT
# state, without waiting for its natural timeout to expire.
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

# Bind the socket to the address indicated by the tuple HOST,PORT
s.bind((HOST, PORT))

# Listen for connection made to the socket. In this case the maximum number of
# queued connections is 1.
s.listen(1)

# Accept a connection. conn is a new socket object usable to send and receive
# data on the connection. addr is the address bound to the socket on the other
# end of the connection.
conn, addr = s.accept()
print 'Connected by', addr

data_to_send = ['Hello', 'world', 'from', 'the', 'python', 'server']

for i in range(6):
    # Receive data from socket. For best match with hw the value should be a
    # power of 2.
    data = conn.recv(1024)
    if not data: break
    # Print received message
    print 'Received message:', repr(data)
    # Send data to the socket.
    conn.sendall(data_to_send[i])
conn.close()
