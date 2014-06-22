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

    Before you can send encrypted HTTP messages, the client and server need to do an SSL handshake, where they: Exchange protocol version numbers Select a cipher that each side knows Authenticate the identity of each side Generate temporary session keys to encrypt the channel Before any encrypted HTTP data flies across the network, SSL already has sent a bunch of handshake data to establish the communication.

    Gourley, David; Totty, Brian; Sayer, Marjorie; Aggarwal, Anshu; Reddy, Sailu (2002-09-27). HTTP: The Definitive Guide: The Definitive Guide (Definitive Guides) (Kindle Locations 7256-7260). O'Reilly Media. Kindle Edition.

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


## Content-Type

http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.17

    The Content-Type entity-header field indicates the media type of the entity-body sent to the recipient or, in the case of the HEAD method, the media type that would have been sent had the request been a GET.
    
         Content-Type   = "Content-Type" ":" media-type
    
    Media types are defined in section 3.7. An example of the field is
    
         Content-Type: text/html; charset=ISO-8859-4

## Date

    http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.18

     The Date general-header field represents the date and time at which the message was originated, having the same semantics as orig-date in RFC 822. The field value is an HTTP-date, as described in section 3.3.1; it MUST be sent in RFC 1123 [8]-date format.

           Date  = "Date" ":" HTTP-date

    An example is

           Date: Tue, 15 Nov 1994 08:12:31 GMT
