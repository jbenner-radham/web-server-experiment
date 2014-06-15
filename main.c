#include <stdio.h>       // printf(), fprintf()
#include <sys/socket.h>  // socket(), bind(), connect(), recv()
#include <arpa/inet.h>   // socketaddr_in, inet_ntoa()
#include <stdlib.h>      // atoi()
#include <string.h>      // memset()
#include <unistd.h>      // close()

#define MAXPENDING 5     // Max outstanding connection requests
#define RCVBUFSIZE 32    // Size of receive buffer

/**
 * REFERENCES
 *
 * @see https://silviocesare.wordpress.com/2007/10/22/setting-sin_zero-to-0-in-struct-sockaddr_in/
 * @see http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html
 * @see https://en.wikipedia.org/wiki/Berkeley_sockets
 * @see https://en.wikipedia.org/wiki/Getaddrinfo
 * @see https://en.wikibooks.org/wiki/C_Programming/Networking_in_UNIX
 * @see http://uw714doc.sco.com/en/SDK_netapi/sockC.PortIPv4appIPv6.html
 */

void die_with_error(char *error_msg);     // Error handling
void handle_tcp_client(int client_sock);  // TCP client handling


int main(int argc, char *argv[])
{
    int server_sock;                      // Socket descriptor for the server
    int client_sock;                      // Socket descruotir for the client
    struct sockaddr_in echo_client_addr;  // Client address
    unsigned short echo_server_port;      // Server port
    unsigned int client_len;              // Length of the client address struct

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <server port>\n", argv[0]);
        exit(1);
    }

    echo_server_port = atoi(argv[1]);     // Local server port from first argv

    // Create socket for incoming connections
    // Prototype: `int socket(int domain, int type, int protocol);`
    if ((server_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)  // PF_INET <- Protocol Family
    {
        die_with_error("socket() failed");
    }

    // Construct local address structure
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,                 // AF_INET <- Address Family
        .sin_addr.s_addr = htonl(INADDR_ANY),  // Any incoming interface
        .sin_port = htons(echo_server_port)    // Local server port
    };

    memset(&server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

    // Bind to the local address
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        die_with_error("bind() failed");
    }

    // Mark the socket so it will listen for incoming connections
    if (listen(server_sock, MAXPENDING) < 0)
    {
        die_with_error("listen() failed");
    }

    for (;;)
    {
        // Set the size of the in-out param
        client_len = sizeof(echo_client_addr);

        // Wait for a client to connect yo
        if ((client_sock = accept(server_sock, (struct sockaddr*)&echo_client_addr, &client_len)) < 0)
        {
            die_with_error("accept() failed");
        }

        // At this point `client_sock` is connected to a client
        printf("Handling client: %s\n", inet_ntoa(echo_client_addr.sin_addr));

        handle_tcp_client(client_sock);
    }

    // NOT REACHED!
}

void die_with_error(char *error_msg)
{
    perror(error_msg);
    exit(1);
}

void handle_tcp_client(int client_sock)
{
    char echo_buffer[RCVBUFSIZE];  // Buffer for echo string
    int recv_msg_size;             // Size of received message

    // Receive message from client
    if ((recv_msg_size = recv(client_sock, echo_buffer, RCVBUFSIZE, 0)) < 0)
    {
        die_with_error("recv() failed");
    }

    // Send received string and receive again until end of transmission
    while (recv_msg_size > 0)  // <- Zero indicates end of transmission
    {
        // Echo message back to client
        if (send(client_sock, echo_buffer, recv_msg_size, 0) != recv_msg_size)
        {
            die_with_error("send() failed");
        }

        // See if there is more data to receive
        if ((recv_msg_size = recv(client_sock, echo_buffer, RCVBUFSIZE, 0)) < 0)
        {
            die_with_error("recv() failed");
        }
    }

    close(client_sock);  // Close client socket
}
