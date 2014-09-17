#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h> // intptr_t
#include <stdlib.h>

/**
 * --------------------------
 * IETF Notational References
 * --------------------------
 *
 * Augmented Backus-Naur Form (ABNF)
 * @see https://tools.ietf.org/html/rfc5234
 *
 * Routing Backus-Naur Form (RBNF)
 * @see http://tools.ietf.org/html/rfc5511
 */

/**
 * From: IETF - RFC3986
 * --------------------
 * <scheme name> : <hierarchical part> [ ? <query> ] [ # <fragment> ]
 */

typedef struct {
    // char *raw <-- Have this be the raw string version?
    char *scheme;
    char *hostname;  // Change to ptr?
    char *query;     // Change to ptr?
} URI;

/**
 * Use this to store a substring's coordinates... in theory we shall see.
 */
typedef struct {
    int a;
    int b;
} LINE_SEGMENT;

typedef enum {
    HTTP,
    HTTPS
//    FTP,
//    SFTP,
//    GIT,
//    DAV
} SCHEME;

char *get_line_segment(LINE_SEGMENT segment, const char* parent_line)
{
    // Do stuff here...

    return NULL;
}

char *uri_get_helper(const char *haystack, char needle)
{
    char *dest = strchr(haystack, needle);

    if (dest) {
        dest++;
    }

    return dest;
}

char *uri_get_query(const char *uri_str)
{
    return uri_get_helper(uri_str, '?');
}

char *uri_get_fragment(const char *uri_str)
{
    return uri_get_helper(uri_str, '#');
}

/**
 * Augmented Backus–Naur Form (RFC5234) Definition | RFC3986 - Section 3.1
 * -----------------------------------------------------------------------
 * scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
 *
 * From: RFC7230 - Section 5.5 "Effective Request URI"
 * ---------------------------------------------------
 * > If the server's configuration (or outbound gateway) provides a
 * > fixed URI scheme, that scheme is used for the effective request
 * > URI.  Otherwise, if the request is received over a TLS-secured TCP
 * > connection, the effective request URI's scheme is "https"; if not,
 * > the scheme is "http".
 *
 * > The components of the effective request URI, once determined as
 * > above, can be combined into absolute-URI form by concatenating the
 * > scheme, "://", authority, and combined path and query component.
 */
char *uri_get_scheme(const char *uri_str)
{
    // char buf[55];
    enum found_status { NONE, COLON, SLASH, DOUBLE_SLASH };
    enum found_status found_index = NONE;
    const char needle[] = "://";
    unsigned int needle_len = 3;

    for (size_t i = 0, len = strlen(uri_str); i < len; ++i)
    {
        if (uri_str[i] == needle[found_index]) {
            // printf("Found char: %c @ index: %zu\n", uri_str[i], i);
            // printf("found_index: %d, DOUBLE_SLASH: %d\n", found_index, DOUBLE_SLASH);

            if ((found_index + 1) == DOUBLE_SLASH) {
                // puts("Found 'em!!!!");
                puts("-------------");
                // printf("The URI scheme is: ");

                /**
                 * Struct for string (char*) coordinates?
                 * e.g. str.x = 0, str.y = 5
                 */
                for (size_t x = 0; x <= (i - needle_len); ++x)
                {
                    printf("%c", uri_str[x]);
                }

                puts("");
                printf("x: %d, y: %lu\n", 0, (i - needle_len));
                break;
            }

            ++found_index;
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    const char *uri;

    if (argv[1]) {
        uri = argv[1];
    } else {
        uri = "http://www.radioactivehamster.com";
    }

    printf("URI: %s\n", uri);

    const char *query_str = uri_get_query(uri);

    if (query_str != NULL) {
        printf("Huzzah! -> %s\n", query_str);
    } else {
        puts("No query string yoz!");
    }

    const char *fragment_str = uri_get_fragment(uri);

    if (fragment_str != NULL) {
        printf("Fragment time! -> %s\n", fragment_str);
    } else {
        puts("No fragment time kaplow!?!");
    }

    uri_get_scheme(uri);
}
