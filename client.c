#include <stdio.h>  /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdbool.h>
#include <stdlib.h> /* for atoi() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */

#define RCVBUFSIZE 32   /* Size of receive buffer */

// Trying this as a global...
// extern int sock;

void die_with_error(char *error_msg);     // Error handling

int main(int argc, char const *argv[])
{
    // printf("BUFSIZ: %d", BUFSIZ); // <-- 1024 on my LT; FYI me.

    /**
     * AF_[...] - Address Family
     * PF_[...] - Protocol Family
     */
    int sock;                      /* Socket descriptor */
    u_short server_port;           /* Echo server port */
    char *server_ip;               /* Server IP address (dotted quad) */
    // char *request;              /* String to send to HTTP server */

    ///// char response[BUFSIZ];   /* Buffer for HTTP response */
    char response[2048];           /* Temporary response buffer placeholder */

    ssize_t bytes_rcvd = 0;
    ssize_t total_bytes_rcvd = 0;

    server_ip = "127.0.0.1";
    server_port = 8080;

    // PF_INET - Internet version 4 protocols
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

        if (bytes_rcvd < 0)
            die_with_error("recv() failed");

        total_bytes_rcvd += bytes_rcvd;

        /**
         * @todo: Temporary stop-gap. Add proper malloc checks and allocation.
         *        Any page larger than the BUFSIZ will throw: `Abort trap: 6`.
         */
        if (total_bytes_rcvd >= BUFSIZ)
            die_with_error("Bytes received is greater than `BUFSIZ`.\n");

        printf("Bytes received: %zd\n", bytes_rcvd);
        printf("Total bytes received: %zd\n", total_bytes_rcvd);
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

    close(sock);
    rcvbuf[0] = '\0';

    printf("\n[SERVER RESPONSE MESSAGE]\n\n");
    printf("%s\n", response);

    puts("\n[END]");

    printf("Length of response: %lu\n", strlen(response));

    /// u_int scany;
    uint8_t scany;
    /// int sscan = sscanf("Content-Length: 399\r\nContent-Type: text/html\r\n\r\n", "Content-Length: %du", &scany);
    int sscan = sscanf("Content-Length: 399\r\nContent-Type: text/html\r\n\r\n", "Content-Length: %cu", &scany);

    printf("SSCAN: %d, SCANY: %d\n", sscan, scany);

    exit(0);
}

void die_with_error(char *error_msg)
{
    perror(error_msg);
    exit(1);
}
