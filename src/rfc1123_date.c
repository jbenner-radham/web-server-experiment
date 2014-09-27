/*@-skipposixheaders@*/
#include <sys/_types/_time_t.h>
/*@=skipposixheaders@*/

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * ----------------------------------------------------------------------------
 * from: https://gustedt.wordpress.com/2013/10/28/different-times-in-c-calendar-times/
 * ----------------------------------------------------------------------------
 * One additional warning is in order. localtime, gmtime, ctime, and asctime use
 * internal state and return pointers to static data. They are not thread safe,
 * not reentrant, and calling one of them erases the return value of any
 * previous call to one of them, be careful.
 *
 * The new optional annexe K of C11, has functions that check for validity of
 * their arguments and are reentrant: localtime_s, gmtime_s, ctime_s, and
 * asctime_s. Use them when you may. P99 now implements them in its C11
 * emulation layer, for those of you that don’t have a complying C library, yet.
 * Give it some testing, if you like.
 */

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


// https://en.wikipedia.org/wiki/C_date_and_time_functions

// HTTP-date - RFC-1123 which is a updated version of RFC-822 referenced
// via RFC-2616. The 822 variant differed in the display of the year e.g.
// "YY" as opposed to "YYYY".
// e.g. "Sat, 05 May 2005 15:55:55 GMT", "Sun, 06 Nov 1994 08:49:37 GMT"
//
// RFC 822, updated by RFC 1123

void die_with_error(char *error_msg);  // Error handling

// This maximum number of characters in an RFC1123 date is 29 + 1 for '\0'.
#define RFC1123_LENGTH 30

#define RFC1123_FORMAT "%a, %d %b %Y %T GMT"

char *rfc1123_date(char buf[])
{
    // Upon successful completion, time() returns the value of time. Otherwise,
    // (time_t)-1 is returned.
    time_t t = time(NULL);

    if (t == ((time_t) - 1))
        die_with_error("Failed to generate the current time.");

    if (strftime(buf, RFC1123_LENGTH, RFC1123_FORMAT, gmtime(&t)))
        return buf;

    puts("strftime failed.");

    return NULL;
}

// void die_with_error(char *error_msg)
// {
//     perror(error_msg);
//     exit(1);
// }
