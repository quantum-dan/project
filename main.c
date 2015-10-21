#include <server.h>

void main()
{
    char *routes[] = {"/404", "/", "/hello", "/hi"};
    int routes_len = 4;
    char *replies[] = {"not found", "hello1", "hello2", "iwin"};
    printf("Server is up and running...\n");
    printf("%d\n", server_html(routes, routes_len, replies));
}
