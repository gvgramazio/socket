/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Called when a system call fails
void error(const char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]) {
  // File descriptors.
  int sockfd;     // Returned by the socket system call.
  int newsockfd;  // Returned by the accept system call.

  int portno;   // Port on which the server accepts connection

  socklen_t clilen; // Size of the address of the client

  char buffer[256];

  // A sockaddr_in is a structure containing an internet addres.
  struct sockaddr_in serv_addr;   // Contains the address of the server
  struct sockaddr_in cli_addr;    // Contains the address of the client

  int n; // Return value for the read() and write() calls.

  /* The user needs to pass in the port number on which the server will accept
   * connections as an argument. Display an error if the user fails to do this.
   */
  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }

  /* The socket() system call create a new socket. It takes three arguments.
   * 1. The address domain. AF_INET is the internet domain.
   * 2. Type of socket. With SOCK_STREAM the characters are read in a continuous
   *    stream as if from a file or pipe.
   * 3. The protocol. With 0 (default) the os will choose the most appropriate
   *    protocol (TCP for stream socket and UDP for datagram sockets).
   * The socket system call returns an entry into the file descriptor table. If
   * the socket fails, it return -1. */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) error("ERROR opening socket");

  // Sets all the values in a buffer to zero.
  bzero((char *) &serv_addr, sizeof(serv_addr));

  // The port number on which the server will listen for connections.
  portno = atoi(argv[1]);

  serv_addr.sin_family = AF_INET;           // Address family
  serv_addr.sin_addr.s_addr = INADDR_ANY;   // IP address of the host
  serv_addr.sin_port = htons(portno);       // Port number

  /* The bind() system call binds a socket to an address, in this case the
   * address of the current host and port number on which the server will run.
   * It takes three arguments.
   * 1. Socket file descriptor.
   * 2. Address to which is bound.
   * 3. The size of the address.
   * It can fail for a number of reasons, the most obvious being that this
   * socket is already in use on this machine. */
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");

  /* The listen system call allows the process to listen on the socket for
   * connections. The first argument is the socket file descriptor, and the
   * second is the size of the backlog queue (5 is the maximum size permitted
   * by most systems). */
  listen(sockfd,5);

  /* The accept() system call causes the process to block until a client
   * connects to the server. Thus it wakes up the process when a connection
    * from a client has been succesfully established. It returns a new file
    * descriptor, and all communication on this connection should be done using
    * the new file descriptor. The second argument is a reference pointer to the
    * address of the client on the other end of the connection, and the third
    * argument is the size of this structure. */
  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
  if (newsockfd < 0) error("ERROR on accept");

  /* Initialize the buffer and then reads from the socket. Note that the read
   * call uses the new file descriptor and that the read() will block until
   * there is something for it ro read in the socket.  */
  bzero(buffer,256);
  n = read(newsockfd,buffer,255);
  if (n < 0) error("ERROR reading from socket");
  printf("Here is the message: %s\n",buffer);

  /* This code simply writes a short message to the client. The last argument of
   * write is the size of the message. */
  n = write(newsockfd,"I got your message",18);
  if (n < 0) error("ERROR writing to socket");

  // Close the socket
  close(newsockfd);
  close(sockfd);


  return 0;
}
