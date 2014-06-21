#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define RFC1123_LENGTH 30

char *rfc1123_date(char buf[])
{
    //if (current_time == ((time_t)-1))
    //{
    //    (void) fprintf(stderr, "Failure to compute the current time.");
    //    return EXIT_FAILURE;
    //}

    // https://en.wikipedia.org/wiki/C_date_and_time_functions

    // HTTP-date - RFC-1123 which is a updated version of RFC-822 referenced
    // via RFC-2616. The 822 variant differed in the display of the year e.g.
    // "YY" as opposed to "YYYY".
    const char *http_date_fmt = "%a, %d %b %Y %T GMT";
    time_t t = time(NULL);

    // The format is a fixed length of 29 chars and we add one for '\0'
    //char buf[30];

    if (strftime(buf, RFC1123_LENGTH, http_date_fmt, gmtime(&t))) {
        return buf;
    }

    puts("strftime failed.");

    return NULL;
}
