ssize_t recv_header(int sockfd, char dest[])
{
    ssize_t bytes_rcvd       = 0;
    ssize_t total_bytes_rcvd = 0;
    bool    cr_flag          = false;
    bool    crlf_flag        = false;
    char    cbuf             = '\0';

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
            dest[total_bytes_rcvd] = '\0';
            crlf_flag = true;
        }

    } while (!crlf_flag);

    return total_bytes_rcvd;
}
