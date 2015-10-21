#include <server.h>

void main()
{
    char *routes[5];
    int routes_len = 4;
    char *replies[5];
    routes[0] = "/404";
    routes[1] = "/";
    routes[2] = "/pg1";
    routes[3] = "/secret";
    replies[0] = "./html/404.html";
    replies[1] = "./html/index.html";
    replies[2] = "./html/pg1.html";
    replies[3] = "./html/secret.html";
    printf("Server is up and running...\n");
    printf("%d\n", server_html(routes, routes_len, replies));
}
