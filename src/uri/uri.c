#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h> // intptr_t, uint8_t
#include <stdlib.h>

// `uri_` function headers
#include "get_scheme.h"

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

// ==========================================

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
