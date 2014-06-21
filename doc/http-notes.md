# HTTP Notes For Reference

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
