#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ---------------------------------------------------------------------
// from: http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.18
// ---------------------------------------------------------------------
// The Date general-header field represents the date and time at which the
// message was originated, having the same semantics as orig-date in RFC 822.
// The field value is an HTTP-date, as described in section 3.3.1; it MUST be
// sent in RFC 1123 [8]-date format.
//
//   Date  = "Date" ":" HTTP-date
//
// An example is
//
//   Date: Tue, 15 Nov 1994 08:12:31 GMT
//
// Origin servers MUST include a Date header field in all responses, except in
// these cases:
//
//   1. If the response status code is 100 (Continue) or 101 (Switching
//      Protocols), the response MAY include a Date header field, at
//      the server's option.
//
//   2. If the response status code conveys a server error, e.g. 500
//      (Internal Server Error) or 503 (Service Unavailable), and it is
//      inconvenient or impossible to generate a valid Date.
//
//   3. If the server does not have a clock that can provide a
//      reasonable approximation of the current time, its responses
//      MUST NOT include a Date header field. In this case, the rules
//      in section 14.18.1 MUST be followed.



// This maximum number of characters in an RFC1123 date is 29 + 1 for '\0'.
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
    // e.g. "Sat, 05 May 2005 15:55:55 GMT"
    //
    // Sun, 06 Nov 1994 08:49:37 GMT  ; RFC 822, updated by RFC 1123
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
