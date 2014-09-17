//#define __STDC_WANT_LIB_EXT1__ 1 // C11 invocation
//#define _GNU_SOURCE // asprintf()

#include <sys/socket.h>  // socket(), bind(), connect(), recv()
#include <arpa/inet.h>   // socketaddr_in, inet_ntoa()


/*@-skipposixheaders@*/
#include <sys/types.h>
/*@=skipposixheaders@*/

// #include "splint_config.h" // Config header to get SPLINT to work...

#include <assert.h>      // assert(), , static_assert(), NDEBUG
#include <ctype.h>       // isalnum(), isalpha(), iscntrl(), isdigit(), ispunct(), isspace(), tolower()
// #include <errno.h>       // errno_t (? - C11)
// #include <iso646.h>      // and
#include <math.h>        // nan()
#include <stdio.h>       // printf(), fprintf(), perror()
#include <string.h>      // memset(), strerror()

#include <stdlib.h>      // atoi()

#include <ctype.h>       // ispunct(), isupper(), isspace(), iscntrl(), toascii(), tolower()

// Not availabe due to being C11?
// #include <stdnoreturn.h> // noreturn

#include <signal.h>
#include <unistd.h>      // close()
