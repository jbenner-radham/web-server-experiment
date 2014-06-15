# HTTP Notes For Reference

## Protocol Stack & OSI Layers

_fig. a_ __HTTP and the protocols it piggybacks on in the order they are invoked__

__IP (Network Layer)__ -> __TCP (Transport Layer)__ -> __HTTP (Application Layer)__

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
