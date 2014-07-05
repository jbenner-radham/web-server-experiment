#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h> // intptr_t
#include <stdlib.h>

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
}

char *uri_get_fragment(const char *uri_str)
{
    return uri_get_helper(uri_str, '#');
}

char *uri_get_scheme(const char *uri_str)
{
    // char *buf = malloc(50);
    // printf("Scheme scan: %d\n", sscanf(uri_str, "%s://", buf));
    // puts(buf);
    // free(buf);
    char buf[55];

    enum found_status { NONE, COLON, SLASH, DOUBLE_SLASH };
    enum found_status found_index = NONE;
    const char needle[] = "://";
    unsigned int needle_len = 3;

    for (size_t i = 0, len = strlen(uri_str); i < len; ++i)
    {
        printf("Char Loop! %c | %c\n", uri_str[i], needle[found_index]);

        if (uri_str[i] == needle[found_index]) {
            printf("Found char: %c @ index: %zu\n", uri_str[i], i);

            printf("found_index: %d, DOUBLE_SLASH: %d\n", found_index, DOUBLE_SLASH);

            if ((found_index + 1) == DOUBLE_SLASH) {
                puts("Found 'em!!!!");
                puts("-------------");
                printf("The URI scheme is: ");

                for (size_t x = 0; x <= (i - needle_len); ++x)
                {
                    printf("%c", uri_str[x]);
                }

                puts("");
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
