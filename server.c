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

#define BACKLOG 10

int test()
{
    register int s, c;
    int b;
    struct sockaddr_in sa;
    FILE *client;

    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        return 1;
    }

    bzero(&sa, sizeof sa);

    sa.sin_family = AF_INET;
    sa.sin_port = htons(8080);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(s, (struct sockaddr *)&sa, sizeof sa) < 0)
    {
        perror("bind");
        return 2;
    }

    /* switch (fork()) // Forks the process, making it a daemon
    {
        case -1:
            perror("fork");
            return 3;
            break;
        default:
            close(s);
            return 0;
            break;
        case 0:
            break;
    } */

    listen(s, BACKLOG);

    for (;;)
    {
        b = sizeof sa;
        if ((c = accept(s, (struct sockaddr *)&sa, &b)) < 0)
        {
            perror("accept");
            return 4;
        }

        if ((client = fdopen(c, "w")) == NULL)
        {
            perror("fdopen");
            return 5;
        }

        char base[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\nContent-length: %d\r\n\r\n%s\r\n";
        char output[] = "<html><body><h1>Woohoo!</h1></body></html>";
        fprintf(client, base, sizeof(output)/sizeof(output[0]), output);
        fclose(client);
    }
}
