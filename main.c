#include <stdio.h>       // printf(), fprintf()
#include <sys/socket.h>  // socket(), bind(), connect(), recv()
#include <arpa/inet.h>   // socketaddr_in, inet_ntoa()
#include <stdlib.h>      // atoi()
#include <string.h>      // memset()
#include <unistd.h>      // close()

#include "src/rfc1123_date.c"

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

void send_msg(int sockfd, const char* msg);

int main(int argc, char *argv[])
{
    int server_sock;                      // Socket descriptor for the server
    int client_sock;                      // Socket descruotir for the client
    struct sockaddr_in echo_client_addr;  // Client address
    // struct addrinfo hints, *res;
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
    else
    {
        puts("Socket created.");
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
    else
    {
        puts("Socket bound.");
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

    /*
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
    */

    const char* msg_body = "<h1>HELLO Z-RAD-WORLD!!!!!!</h1>";

    char *content_len_p = (char*)malloc(150);
    sprintf(content_len_p, "Content-Length: %zd\r\n", strlen(msg_body) * sizeof(char));

    char datetime[RFC1123_LENGTH];
    char date_header[RFC1123_LENGTH + 9];
    rfc1123_date(datetime);
    sprintf(date_header, "Date: %s\r\n", datetime);

    send_msg(client_sock, "HTTP/1.1 200 OK\r\n");
    send_msg(client_sock, date_header);
    send_msg(client_sock, "Content-Type: text/html\r\n");
    send_msg(client_sock, content_len_p);
    send_msg(client_sock, "\r\n");
    send_msg(client_sock, msg_body);

    /**
     * char *response_start_line = "HTTP/1.1 YO-JOE\r\n";
     * size_t len = strlen(response_start_line) * sizeof(char);
     * ssize_t bytes_sent = send(client_sock, response_start_line, len, 0);
     * // ^ FYI `ssize_t` is *Signed* Size Type
     */

/*
    // It appears you can only send 8 bytes of your message at a time.
    // ...or I'm just an idiot an was doing a `sizeof` on the pointer
    // so 8 bytes on a 64-bit system *womp womp*
    printf("So we sent %zd bytes to the client sockfd out of the total %zd bytes.\n", bytes_sent, len);

    while (bytes_sent < len) {
        printf("So that make the remainder of our message: %s\n", response_start_line + bytes_sent);
        bytes_sent = send(client_sock, response_start_line + bytes_sent, len, 0);
        printf("So we've now sent %zd bytes to the client sockfd.\n", bytes_sent);
        break;
    }

    const char* msg_body = "<h1>HELLO Z-WORLD!!!!!!</h1>\r";
    size_t body_size = strlen(msg_body) * sizeof(char);

    //printf("Content-Length: %zd\r\n", body_size);
    //close(client_sock);
    //exit;

    //char moar_data[BUFSIZ] = "Date: Wed, 18 Jun 2014 04:42:06 GMT\r\n"
    //                         "Content-Type: text/plain\r\n";

    char *content_len_p = (char*)malloc(150);
    content_len_p = "Content-Length: %zd\r\n";
    sprintf(content_len_p, "Content-Length: %zd\r\n", body_size);
    moar_data[strlen(moar_data)] = *content_len_p;

    puts(moar_data);
*/
/*
    size_t data_length = strlen(moar_data) * sizeof(char);

    bytes_sent = 0;

    bytes_sent = send(client_sock, moar_data, data_length, 0);
    printf("So we sent %zd bytes to the client sockfd out of the total %zd bytes.\n", bytes_sent, data_length);
    puts(moar_data);

    send(client_sock, msg_body, body_size, 0);
*/
    puts("Yippee? *hopefully*");
    close(client_sock);  // Close client socket
    free(content_len_p); // Free the allocated memory
}

void send_msg(int sockfd, const char* msg)
{
    size_t bytes = strlen(msg) * sizeof(char);
    ssize_t bytes_sent = send(sockfd, msg, bytes, 0);
}
