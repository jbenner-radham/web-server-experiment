# HTTP Notes For Reference

## Documentation Vocabulary

http://www.ietf.org/rfc/rfc2119.txt

## Protocol Stack & OSI Layers

_fig. a_ __HTTP and the protocols it piggybacks on in the order they are invoked__

__IP (Network Layer)__ -> __TCP (Transport Layer)__ -> __HTTP (Application Layer)__

_fig. b_ __HTTPS and it's respective ordered protocol stack__

__IP (Network Layer)__ -> __TCP (Transport Layer)__ -> __SSL or TLS (Security Layer[?])__
                       -> __HTTP (Application Layer)__

The above is related via the URL and/or URI as such:

`[scheme]://[ip address](:[port])`
            __^ IP__    __^ TCP__

The port number is optional, if it is not provided than "80" is assumed unless a non-HTTP
scheme is supplied.

If a domain name is provided instead of an IP address, as is usually the case. Then (and this
is obvious but still good to have in writing) a DNS resolution would need to be performed,
thus invoking a protocol separate from HTTP. The URL would then be:

`[scheme]://[hostname](:[port])`
            __^ Name__ __^ TCP__

Please note that "hostname" and "ip address" fields can be referred to by the "host" parent designation.


## URI Scheme

Reference:
https://en.wikipedia.org/wiki/URI_scheme
https://tools.ietf.org/html/rfc3986
http://www.w3.org/TR/url/

```
    <scheme name> : <hierarchical part> [ ? <query> ] [ # <fragment> ]
```

```
    foo://username:password@example.com:8042/over/there/index.dtb?type=animal&name=narwhal#nose
    \_/   \_______________/ \_________/ \__/            \___/ \_/ \______________________/ \__/
     |           |               |       |                |    |            |                |
     |       userinfo         hostname  port              |    |          query          fragment
     |    \________________________________/\_____________|____|/ \__/        \__/
     |                    |                          |    |    |    |          |
     |                    |                          |    |    |    |          |
  scheme              authority                    path   |    |    interpretable as keys
   name   \_______________________________________________|____|/       \____/     \_____/
     |                         |                          |    |          |           |
     |                 hierarchical part                  |    |    interpretable as values
     |                                                    |    |
     |            path               interpretable as filename |
     |   ___________|____________                              |
    / \ /                        \                             |
    urn:example:animal:ferret:nose               interpretable as extension
                  path
           _________|________
   scheme /                  \
    name  userinfo  hostname       query
    _|__   ___|__   ____|____   _____|_____
   /    \ /      \ /         \ /           \
   mailto:username@example.com?subject=Topic
```


## SSL

* HTTPS certificates are X.509 certificaes with site information

### SSL Handshake

> Before you can send encrypted HTTP messages, the client and server need to do an SSL handshake, where they: Exchange protocol version numbers Select a cipher that each side knows Authenticate the identity of each side Generate temporary session keys to encrypt the channel Before any encrypted HTTP data flies across the network, SSL already has sent a bunch of handshake data to establish the communication.
>
> - Gourley, David; Totty, Brian; Sayer, Marjorie; Aggarwal, Anshu; Reddy, Sailu (2002-09-27). HTTP: The Definitive Guide: The Definitive Guide (Definitive Guides) (Kindle Locations 7256-7260). O'Reilly Media. Kindle Edition.

### OpenSSL

*Doesn't fully work (but "All-in-One")*

  ```shell
  openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout server.key -out server.csr
  ```

*Successfully used to run a self-signed HTTPS server*

Reference:
https://www.linuxquestions.org/questions/linux-networking-3/openssl-x509-expecting-certificate-request-290773/

  ```shell
  openssl genrsa -out server.key 1024
  openssl req -new -x509 -days 365 -key server.key -out server.csr
  openssl x509 -x509toreq -days 365 -in server.csr -signkey server.key -out server.req
  openssl x509 -req -days 365 -in server.req -signkey server.key -out server.crt
  ```

## HTTP Response Message

http://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html#sec6

After receiving and interpreting a request message, a server responds with an HTTP response message.

       Response      = Status-Line               ; Section 6.1
                       *(( general-header        ; Section 4.5
                        | response-header        ; Section 6.2
                        | entity-header ) CRLF)  ; Section 7.1
                       CRLF
                       [ message-body ]          ; Section 7.2

6.1 Status-Line

The first line of a Response message is the Status-Line, consisting of the protocol version followed by a numeric status code and its associated textual phrase, with each element separated by SP characters. No CR or LF is allowed except in the final CRLF sequence.

       Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF

> 6.1.1 Status Code and Reason Phrase
>
> The Status-Code element is a 3-digit integer result code of the attempt to understand and satisfy the request. These codes are fully defined in section 10. The Reason-Phrase is intended to give a short textual description of the Status-Code. The Status-Code is intended for use by automata and the Reason-Phrase is intended for the human user. The client is not required to examine or display the Reason- Phrase.
>
> The first digit of the Status-Code defines the class of response. The last two digits do not have any categorization role. There are 5 values for the first digit:
>
>     - 1xx: Informational - Request received, continuing process
>
>     - 2xx: Success - The action was successfully received,
>       understood, and accepted
>
>     - 3xx: Redirection - Further action must be taken in order to
>       complete the request
>
>     - 4xx: Client Error - The request contains bad syntax or cannot
>       be fulfilled
>
>     - 5xx: Server Error - The server failed to fulfill an apparently
>       valid request

## HTTP Request

5 Request

A request message from a client to a server includes, within the first line of that message, the method to be applied to the resource, the identifier of the resource, and the protocol version in use.

        Request       = Request-Line              ; Section 5.1
                        *(( general-header        ; Section 4.5
                         | request-header         ; Section 5.3
                         | entity-header ) CRLF)  ; Section 7.1
                        CRLF
                        [ message-body ]          ; Section 4.3

5.1 Request-Line

The Request-Line begins with a method token, followed by the Request-URI and the protocol version, and ending with CRLF. The elements are separated by SP characters. No CR or LF is allowed except in the final CRLF sequence.

        Request-Line   = Method SP Request-URI SP HTTP-Version CRLF

5.1.1 Method

The Method token indicates the method to be performed on the resource identified by the Request-URI. The method is case-sensitive.

       Method         = "OPTIONS"                ; Section 9.2
                      | "GET"                    ; Section 9.3
                      | "HEAD"                   ; Section 9.4
                      | "POST"                   ; Section 9.5
                      | "PUT"                    ; Section 9.6
                      | "DELETE"                 ; Section 9.7
                      | "TRACE"                  ; Section 9.8
                      | "CONNECT"                ; Section 9.9
                      | extension-method
       extension-method = token

The list of methods allowed by a resource can be specified in an Allow header field (section 14.7). The return code of the response always notifies the client whether a method is currently allowed on a resource, since the set of allowed methods can change dynamically. An origin server SHOULD return the status code 405 (Method Not Allowed) if the method is known by the origin server but not allowed for the requested resource, and 501 (Not Implemented) if the method is unrecognized or not implemented by the origin server. The methods GET and HEAD MUST be supported by all general-purpose servers. All other methods are OPTIONAL; however, if the above methods are implemented, they MUST be implemented with the same semantics as those specified in section 9.
5.1.2 Request-URI

The Request-URI is a Uniform Resource Identifier (section 3.2) and identifies the resource upon which to apply the request.

       Request-URI    = "*" | absoluteURI | abs_path | authority

The four options for Request-URI are dependent on the nature of the request. The asterisk "*" means that the request does not apply to a particular resource, but to the server itself, and is only allowed when the method used does not necessarily apply to a resource. One example would be

       OPTIONS * HTTP/1.1

The absoluteURI form is REQUIRED when the request is being made to a proxy. The proxy is requested to forward the request or service it from a valid cache, and return the response. Note that the proxy MAY forward the request on to another proxy or directly to the server

specified by the absoluteURI. In order to avoid request loops, a proxy MUST be able to recognize all of its server names, including any aliases, local variations, and the numeric IP address. An example Request-Line would be:

       GET http://www.w3.org/pub/WWW/TheProject.html HTTP/1.1

To allow for transition to absoluteURIs in all requests in future versions of HTTP, all HTTP/1.1 servers MUST accept the absoluteURI form in requests, even though HTTP/1.1 clients will only generate them in requests to proxies.


## Content-Type

http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.17

    The Content-Type entity-header field indicates the media type of the entity-body sent to the recipient or, in the case of the HEAD method, the media type that would have been sent had the request been a GET.

         Content-Type   = "Content-Type" ":" media-type

    Media types are defined in section 3.7. An example of the field is

         Content-Type: text/html; charset=ISO-8859-4

## Date

http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.18

> The Date general-header field represents the date and time at which the message was originated, having the same semantics as orig-date in RFC 822. The field value is an HTTP-date, as described in section 3.3.1; it MUST be sent in RFC 1123 [8]-date format.
>
>     Date  = "Date" ":" HTTP-date
>
> An example is
>
>     Date: Tue, 15 Nov 1994 08:12:31 GMT

## Transfer-Encoding

from: https://tools.ietf.org/html/rfc2616#section-19.4.6

> 19.4.6 Introduction of Transfer-Encoding
>
> HTTP/1.1 introduces the Transfer-Encoding header field (section
> 14.41). Proxies/gateways MUST remove any transfer-coding prior to
> forwarding a message via a MIME-compliant protocol.
>
> A process for decoding the "chunked" transfer-coding (section 3.6)
> can be represented in pseudo-code as:
>
>     length := 0
>     read chunk-size, chunk-extension (if any) and CRLF
>     while (chunk-size > 0) {
>         read chunk-data and CRLF
>         append chunk-data to entity-body
>         length := length + chunk-size
>         read chunk-size and CRLF
>     }
>     read entity-header
>     while (entity-header not empty) {
>         append entity-header to existing header fields
>         read entity-header
>     }
>     Content-Length := length
>     Remove "chunked" from Transfer-Encoding

## Host

from: http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.23

> The Host request-header field specifies the Internet host and port number of
> the resource being requested, as obtained from the original URI given by the
> user or referring resource (generally an HTTP URL, as described in section
> 3.2.2). The Host field value MUST represent the naming authority of the origin
> server or gateway given by the original URL. This allows the origin server or
> gateway to differentiate between internally-ambiguous URLs, such as the root
> "/" URL of a server for multiple host names on a single IP address.
>
>     Host = "Host" ":" host [ ":" port ] ; Section 3.2.2
>
> A "host" without any trailing port information implies the default port for
> the service requested (e.g., "80" for an HTTP URL). For example, a request on
> the origin server for <http://www.w3.org/pub/WWW/> would properly include:
>
>     GET /pub/WWW/ HTTP/1.1
>     Host: www.w3.org
>
> A client MUST include a Host header field in all HTTP/1.1 request messages.
> If the requested URI does not include an Internet host name for the service
> being requested, then the Host header field MUST be given with an empty value.
> An HTTP/1.1 proxy MUST ensure that any request message it forwards does
> contain an appropriate Host header field that identifies the service being
> requested by the proxy. All Internet-based HTTP/1.1 servers MUST respond with
> a 400 (Bad Request) status code to any HTTP/1.1 request message which lacks a
> Host header field.

### URI Template - RFC

http://tools.ietf.org/html/rfc6570

### HTML POST Form Data

http://amundsen.com/examples/put-delete-forms/#simple-example

http://www.cs.tut.fi/~jkorpela/forms/methods.html

https://en.wikipedia.org/wiki/POST_%28HTTP%29#Posting_data

### Size Limits

http://en.wikipedia.org/wiki/List_of_HTTP_header_fields#Size_limits

> There are no limits to the size of each header field name or value or to the
> number of headers in the standard itself. However, most servers, clients, and
> proxy software impose some limits for practical and security reasons. For
> example, the Apache 2.3 server by default limits each header size to 8190
> bytes, and there can be at most 100 headers in a single request

### Status-Line

http://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html

6.1 Status-Line

The first line of a Response message is the Status-Line, consisting of the protocol version followed by a numeric status code and its associated textual phrase, with each element separated by SP characters. No CR or LF is allowed except in the final CRLF sequence.

       Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF

6.1.1 Status Code and Reason Phrase

The Status-Code element is a 3-digit integer result code of the attempt to understand and satisfy the request. These codes are fully defined in section 10. The Reason-Phrase is intended to give a short textual description of the Status-Code. The Status-Code is intended for use by automata and the Reason-Phrase is intended for the human user. The client is not required to examine or display the Reason- Phrase.

The first digit of the Status-Code defines the class of response. The last two digits do not have any categorization role. There are 5 values for the first digit:

      - 1xx: Informational - Request received, continuing process

      - 2xx: Success - The action was successfully received,
        understood, and accepted

      - 3xx: Redirection - Further action must be taken in order to
        complete the request

      - 4xx: Client Error - The request contains bad syntax or cannot
        be fulfilled

      - 5xx: Server Error - The server failed to fulfill an apparently
        valid request

The individual values of the numeric status codes defined for HTTP/1.1, and an example set of corresponding Reason-Phrase's, are presented below. The reason phrases listed here are only recommendations -- they MAY be replaced by local equivalents without affecting the protocol.

      Status-Code    =
            "100"  ; Section 10.1.1: Continue
          | "101"  ; Section 10.1.2: Switching Protocols
          | "200"  ; Section 10.2.1: OK
          | "201"  ; Section 10.2.2: Created
          | "202"  ; Section 10.2.3: Accepted
          | "203"  ; Section 10.2.4: Non-Authoritative Information
          | "204"  ; Section 10.2.5: No Content
          | "205"  ; Section 10.2.6: Reset Content
          | "206"  ; Section 10.2.7: Partial Content
          | "300"  ; Section 10.3.1: Multiple Choices
          | "301"  ; Section 10.3.2: Moved Permanently
          | "302"  ; Section 10.3.3: Found
          | "303"  ; Section 10.3.4: See Other
          | "304"  ; Section 10.3.5: Not Modified
          | "305"  ; Section 10.3.6: Use Proxy
          | "307"  ; Section 10.3.8: Temporary Redirect
          | "400"  ; Section 10.4.1: Bad Request
          | "401"  ; Section 10.4.2: Unauthorized
          | "402"  ; Section 10.4.3: Payment Required
          | "403"  ; Section 10.4.4: Forbidden
          | "404"  ; Section 10.4.5: Not Found
          | "405"  ; Section 10.4.6: Method Not Allowed
          | "406"  ; Section 10.4.7: Not Acceptable

          | "407"  ; Section 10.4.8: Proxy Authentication Required
          | "408"  ; Section 10.4.9: Request Time-out
          | "409"  ; Section 10.4.10: Conflict
          | "410"  ; Section 10.4.11: Gone
          | "411"  ; Section 10.4.12: Length Required
          | "412"  ; Section 10.4.13: Precondition Failed
          | "413"  ; Section 10.4.14: Request Entity Too Large
          | "414"  ; Section 10.4.15: Request-URI Too Large
          | "415"  ; Section 10.4.16: Unsupported Media Type
          | "416"  ; Section 10.4.17: Requested range not satisfiable
          | "417"  ; Section 10.4.18: Expectation Failed
          | "500"  ; Section 10.5.1: Internal Server Error
          | "501"  ; Section 10.5.2: Not Implemented
          | "502"  ; Section 10.5.3: Bad Gateway
          | "503"  ; Section 10.5.4: Service Unavailable
          | "504"  ; Section 10.5.5: Gateway Time-out
          | "505"  ; Section 10.5.6: HTTP Version not supported
          | extension-code

      extension-code = 3DIGIT
      Reason-Phrase  = *<TEXT, excluding CR, LF>

HTTP status codes are extensible. HTTP applications are not required to understand the meaning of all registered status codes, though such understanding is obviously desirable. However, applications MUST understand the class of any status code, as indicated by the first digit, and treat any unrecognized response as being equivalent to the x00 status code of that class, with the exception that an unrecognized response MUST NOT be cached. For example, if an unrecognized status code of 431 is received by the client, it can safely assume that there was something wrong with its request and treat the response as if it had received a 400 status code. In such cases, user agents SHOULD present to the user the entity returned with the response, since that entity is likely to include human- readable information which will explain the unusual status.

### Misc.

http://stackoverflow.com/questions/630453/put-vs-post-in-rest

https://jcalcote.wordpress.com/2008/10/16/put-or-post-the-rest-of-the-story/

http://en.wikipedia.org/wiki/Category:Hypertext_Transfer_Protocol

http://skrb.org/ietf/http_errata.html

__HTTP/1.1 Semantics and Content__
http://tools.ietf.org/html/rfc7231

http://en.wikipedia.org/wiki/Chunked_transfer_encoding

http://en.wikipedia.org/wiki/ISO/IEC_8859-1

https://en.wikipedia.org/wiki/Transmission_Control_Protocol

http://www.w3.org/TR/REC-html40/interact/forms.html#form-content-type

http://www.instructables.com/id/How-to-Convert-Between-base-10-Hexadecimal-and-B/?ALLSTEPS

https://en.wikipedia.org/wiki/UTF-8#Examples

http://facweb.cs.depaul.edu/sjost/it212/documents/binhex.htm

http://www.w3.org/TR/html5/forms.html#form-submission-algorithm

https://tools.ietf.org/html/rfc2045

http://www.ietf.org/rfc/rfc2045.txt

https://tools.ietf.org/html/rfc822

__Introduction to network functions in C__
http://shoe.bocks.com/net/

__HTTP/1.1 - The big one protocol RFC!!!__
https://tools.ietf.org/html/rfc2616

__HTTP/1.1 - HTTP Message__
http://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html#sec4.1

__HTTP/1.1 - Method Definitions__
http://www.w3.org/Protocols/rfc2616/rfc2616-sec9.html


### Random neat comparisson chart
hyperpolyglot.org/scripting
