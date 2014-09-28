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
 * >
 * > The components of the effective request URI, once determined as
 * > above, can be combined into absolute-URI form by concatenating the
 * > scheme, "://", authority, and combined path and query component.
 *
 * From: RFC3986 - Section 3.1 "Scheme"
 * ------------------------------------
 * > Each URI begins with a scheme name that refers to a specification for
 * > assigning identifiers within that scheme.  As such, the URI syntax is
 * > a federated and extensible naming system wherein each scheme's
 * > specification may further restrict the syntax and semantics of
 * > identifiers using that scheme.
 * >
 * > Scheme names consist of a sequence of characters beginning with a
 * > letter and followed by any combination of letters, digits, plus
 * > ("+"), period ("."), or hyphen ("-").  Although schemes are case-
 * > insensitive, the canonical form is lowercase and documents that
 * > specify schemes must do so with lowercase letters.  An implementation
 * > should accept uppercase letters as equivalent to lowercase in scheme
 * > names (e.g., allow "HTTP" as well as "http") for the sake of
 * > robustness but should only produce lowercase scheme names for
 * > consistency.
 * >
 * >     scheme      = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
 *
 * ---------------------------------------------------------------------
 *
 * > When presented with a URI that violates one or more scheme-specific
 * > restrictions, the scheme-specific resolution process should flag the
 * > reference as an error rather than ignore the unused parts; doing so
 * > reduces the number of equivalent URIs and helps detect abuses of the
 * > generic syntax, which might indicate that the URI has been
 * > constructed to mislead the user (Section 7.6).
 */

#define SCHEME_TERMINATOR_LENGTH 3

char *uri_get_scheme(const char *uri_str)
{
    enum found_status { NONE, COLON, SLASH, DOUBLE_SLASH };
    enum found_status found_index = NONE;
    const char scheme_terminator[] = "://";

    for (size_t i = 0, len = strlen(uri_str); i < len; ++i)
    {
        if (uri_str[i] == scheme_terminator<:found_index:>) {

            if (found_index + 1 == DOUBLE_SLASH) {
                puts("-------------");
                /// printf("The URI scheme is: ");

                /**
                 * Struct for string (char*) point coordinates?
                 * e.g. str.x = 0, str.y = 5
                 */
                for (size_t x = 0; x <= i - SCHEME_TERMINATOR_LENGTH; ++x)
                {
                    printf("%c", uri_str[x]);
                }

                puts("");
                printf("x: %d, y: %lu\n", 0, i - SCHEME_TERMINATOR_LENGTH);
                break;
            }

            ++found_index;
        }
    }

    return NULL;
}
