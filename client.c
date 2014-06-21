#include <stdio.h>  /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h> /* for atoi() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */

#define RCVBUFSIZE 32   /* Size of receive buffer */

void die_with_error(char *error_msg);     // Error handling

int main(int argc, char const *argv[])
{
    int sock;                        /* Socket descriptor */
    u_short server_port;      /* Echo server port */
    char *server_ip;                 /* Server IP address (dotted quad) */
    // char *request;                   /* String to send to HTTP server */
    char response[BUFSIZ];           /* Buffer for HTTP response */
    ssize_t bytes_rcvd = 0;
    ssize_t total_bytes_rcvd = 0;

    server_ip = "127.0.0.1";
    server_port = 8080;

    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        die_with_error("socket() failed");
    }

    // Construct the servr address structure
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,                    // AF_INET <- Address Family
        .sin_addr.s_addr = inet_addr(server_ip),  // Server IP
        .sin_port = htons(server_port)            // Local server port
    };

    memset(&server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
        die_with_error("connect() failed");

    char *request = "GET / HTTP/1.1\r\n"
                    "Accept: */*\r\n"
                    "Host: 127.0.0.1:8080\r\n"
                    "\r\n";
                    // "HOST: localhost:8080\r\n"

    printf("\n[Request Message]\n\n");
    printf("%s\n", request);

    size_t req_len = strlen(request);

    size_t sent = send(sock, request, req_len, 0);


    printf("\n[Traffic (in bytes)]\n\n");

    printf("Sent: %lu\n", sent);
    printf("Bytes received: %zd\n", bytes_rcvd);
    printf("Total bytes received: %zd\n", total_bytes_rcvd);
    puts("----------------");

    /**
     * ==================
     * From `man strncat`
     * ==================
     * if (strlen(arbitrary_string) + 1 >
     *    sizeof(onstack) - strlen(onstack))
     *        err(1, "onstack would be truncated");
     * (void)strncat(onstack, arbitrary_string,
     *     sizeof(onstack) - strlen(onstack) - 1);
     */

    char rcvbuf[RCVBUFSIZE];

    do {
        bytes_rcvd = recv(sock, rcvbuf, RCVBUFSIZE - 1, 0);
        total_bytes_rcvd += bytes_rcvd;
        printf("Bytes received: %zd\n", bytes_rcvd);
        printf("Total bytes received: %zd\n", total_bytes_rcvd);
        //response[bytes_rcvd] = '\0';
        //*(response) += bytes_rcvd;
        //response[total_bytes_rcvd] = '\0';
        // response = '\0'
        strncat(response, rcvbuf, BUFSIZ - 1);
        puts("----------------");

        // Hacky way of telling that we've reached the end of the server message
        // so we can break out of the loop instead of waiting for the `recv` to
        // time out.
        //
        // Formally we should do something like receive the headers byte by byte
        // until we hit a '\r\n' so we can tell we've hit one header, and then
        // scan for actionable relevance e.g. Content-Length, Transfer-Encoding,
        // or the '\r\n\r\n' signaling the end of the HTTP headers, etc.
        if (bytes_rcvd < (RCVBUFSIZE - 1)) {
            break;
        }

    } while (bytes_rcvd > 0); // <- Zero indicates end of transmission

    rcvbuf[0] = '\0';
    printf("\n[SERVER RESPONSE MESSAGE]\n");
    printf("%s\n", response);

        // See if there is more data to receive
        //if ((bytes_rcvd = recv(sock, response, RCVBUFSIZE - 1, 0)) < 0)
        //{
        //    die_with_error("recv() failed");
        //}

        //total_bytes_rcvd += bytes_rcvd;

        //printf("Total bytes received: %d\n", total_bytes_rcvd);

    //    printf("Strlen: %lu\n", strlen(response));
    //}

    //bytes_rcvd = recv(sock, response, BUFSIZ - 1, 0);

    //printf("Bytes Received: %d\n", bytes_rcvd);

    //while ((bytes_rcvd = recv(sock, response, BUFSIZ - 1, 0)) <= 0) {
    //    total_bytes_rcvd += bytes_rcvd;
    //    response[bytes_rcvd] = '\0';
    //    printf("%s\n", response);
    //}

    puts("\n[END]");

    //printf("%s\n", response);

    close(sock);
    exit(0);
    // send_msg(request);

    //return 0;
}

void send_msg(int sockfd, const char* msg)
{
    size_t bytes = strlen(msg) * sizeof(char);
    ssize_t bytes_sent = send(sockfd, msg, bytes, 0);
}

void die_with_error(char *error_msg)
{
    perror(error_msg);
    exit(1);
}
