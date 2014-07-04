#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h> // intptr_t

/**
 * From: IETF - RFC3986
 * --------------------
 * <scheme name> : <hierarchical part> [ ? <query> ] [ # <fragment> ]
 */

typedef struct {
    // char *raw <-- Have this be the raw string version?
    char *scheme;
    char *hostname; // Change to ptr?
    char *query;     // Change to ptr?
} URI;

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
    /*
    char *query_str = strchr(uri_str, '?');

    // Advance the pointer so we don't return the "?"
    if (query_str)
        query_str++;

    return query_str;
    */
}

char *uri_get_fragment(const char *uri_str)
{
    return uri_get_helper(uri_str, '#');
}

int main(void)
{
    const char *uri = "http://www.radioactivehamster.com"; // ?a=alligator";

    const char *query_str = uri_get_query(uri);

    if (query_str != NULL) {
        printf("Huzzah! -> %s\n", query_str);
    } else {
        puts("No query string yoz!");
    }
}
