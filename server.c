/* Server for my project.
Needs to be able to serve HTTP requests first.

Needed:
* Socket programming.  Listen on a BSD socket.

Goals:
* Just return a plain string to an HTTP request. */

/* Currently:
* Compiles correctly, but does not respond to requests.
* Is the accept timed right? */

#include <server.h>

void test()
{
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sa;
    sa.sin_port = 80;
    sa.sin_family = AF_INET;
    struct in_addr address;
    address.s_addr = htons((uint16_t)INADDR_LOOPBACK);
    sa.sin_addr = address;
    struct sockaddr *socka = (struct sockaddr *) &sa;
    bind(sock, socka, sizeof(struct sockaddr));
    listen(sock, 5);
    int incoming;
    incoming = accept(sock, 0, 0);
    char outgoing[] = "HTTP/1.1 200 OK\r\nContent-length: 5\r\nContent-type: text/html\r\nConnection: closed\r\n\r\nHello";
    send(incoming, outgoing, sizeof(outgoing), 0);
}
