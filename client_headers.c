#include <stdio.h>  /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdbool.h>
#include <stdlib.h> /* for atoi() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */

#define RCVBUFSIZE 32   /* Size of receive buffer */

void die_with_error(char *error_msg);     // Error handling

ssize_t recv_header(int sockfd, char dest[]);

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

    // This outputs "0" when it works FYI.
    // printf("This is strcmp working: %d\n", strcmp("hi", "hi"));

    u_int i = 0;

    char header_charbuf;
    char header_strbuf[BUFSIZ];
    bool cr_flag = false;
    bool crlf_flag = false;

    ssize_t r;

    r = recv_header(sock, header_strbuf);
    printf("recv_header: %zd\n", r);
    puts(header_strbuf);

    r = recv_header(sock, header_strbuf);
    printf("recv_header: %zd\n", r);
    puts(header_strbuf);

    exit(1);

    bytes_rcvd = total_bytes_rcvd = 0;

    do {
        bytes_rcvd = recv(sock, &header_charbuf, 1, 0);

        if (bytes_rcvd < 0)
            die_with_error("recv() failed");

        total_bytes_rcvd += bytes_rcvd;

        printf("Total bytes received: %zd\n", total_bytes_rcvd);

        if (bytes_rcvd > 0)
            // Do a "-1" to account for the zero-indexed char array.
            header_strbuf[total_bytes_rcvd - 1] = header_charbuf;
        else
            break;

        if (header_charbuf != '\n')
            cr_flag = false;

            if (header_charbuf == '\r')
                cr_flag = true;

        if (cr_flag && header_charbuf == '\n') {
            puts("Found a CRLF!!!");
            header_strbuf[total_bytes_rcvd] = '\0';
            //puts(header_strbuf);
            crlf_flag = true;
        }

        ++i;
    } while (!crlf_flag);
    // } while ( !(cr_flag == true && header_charbuf == '\n') );

    puts("");
    //printf("i <- %d\n", i);
    puts(header_strbuf);

    u_int status_code;
    // From: http://www.w3.org/Protocols/rfc2616/rfc2616-sec3.html
    // -----------------------------------------------------------
    // The version of an HTTP message is indicated by an HTTP-Version field in
    // the first line of the message.
    //
    //  HTTP-Version   = "HTTP" "/" 1*DIGIT "." 1*DIGIT
    sscanf(header_strbuf, "HTTP/1.1 %d OK\r\n", &status_code);
    printf("SSCANF? %zd\n", status_code);




    //printf("\n%s\n", response);

    close(sock);
    rcvbuf[0] = '\0';
    exit(0);

    do {
        bytes_rcvd = recv(sock, rcvbuf, RCVBUFSIZE - 1, 0);

        if (bytes_rcvd < 0)
            die_with_error("recv() failed");

        total_bytes_rcvd += bytes_rcvd;
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

    u_int scany;
    int sscan = sscanf("Content-Length: 399\r\nContent-Type: text/html\r\n\r\n", "Content-Length: %du", &scany);

    printf("SSCAN: %d, SCANY: %d\n", sscan, scany);

    exit(0);
}

ssize_t recv_header(int sockfd, char dest[])
{
    ssize_t bytes_rcvd       = 0;
    ssize_t total_bytes_rcvd = 0;
    bool    cr_flag          = false;
    bool    crlf_flag        = false;
    char    cbuf;

    do {
        bytes_rcvd = recv(sockfd, &cbuf, 1, 0);

        if (bytes_rcvd < 0)
            die_with_error("recv() failed");

        total_bytes_rcvd += bytes_rcvd;

        if (bytes_rcvd > 0)
            // Do a "-1" to account for the zero-indexed char array.
            dest[total_bytes_rcvd - 1] = cbuf;
        else
            break;

        if (cbuf != '\n')
            cr_flag = false;

            if (cbuf == '\r')
                cr_flag = true;

        if (cr_flag && cbuf == '\n') {
            puts("Found a CRLF!!!");
            dest[total_bytes_rcvd] = '\0';
            crlf_flag = true;
        }

    } while (!crlf_flag);

    return total_bytes_rcvd;
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