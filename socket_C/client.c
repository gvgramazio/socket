#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// Called when a system call fails
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd;   // File descriptor returned from socket system call
    int portno;   // Port on which the server accepts connection
    int n;         // Return value for the read() and write() calls.

    // A sockaddr_in is a structure containing an internet addres.
    struct sockaddr_in serv_addr;   // Contains the address of the server

    /* hostent defines a host computer on the internet
     * h_name       Official name of the host.
     * h_aliases    A zero terminated array of alternate
     *              names for the host.
     * h_addrtype   The type of address being returned;
     *              currently always AF_INET.
     * h_length     The length, in bytes, of the address.
     * h_addr_list  A pointer to a list of network addresses
     *              for the named host. Host addresses are
     *              returned in network byte order. */
    struct hostent *server;

    char buffer[256];

    // Check if the user passed the hostname port.
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    // The port number on which the server will listen for connections.
    portno = atoi(argv[2]);

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

    /* Handle the host on the internet. The function gethostbyname() takes the
     * name as an argument and returns a pointer to a hostent containing
     * information about that host. */
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }


    bzero((char *) &serv_addr, sizeof(serv_addr));  // Clear serv_addr
    serv_addr.sin_family = AF_INET;                 // Address family
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);                         // IP address of the host
    serv_addr.sin_port = htons(portno);             // Port number

    /* The connect() function tries to establish a connection to the server. It
     * takes the socket file descriptor, the address of the host to which it
     * wants to connect (including the port number), and the size of this
     * address. */
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    /* Prompt the user to enter a message, use fgets to read the message from
     * stdin, write the message to the socket, read the reply from the socket,
     * display this reply on the screen. */
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");
    printf("%s\n",buffer);

    // Close the socket
    close(sockfd);
    
    return 0;
}
