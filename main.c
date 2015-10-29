#include <server.h>

int home(int target);
int pg1(int target);
int post_home(int target, mapitem input[]);
int login(int target, mapitem input[]);

int main()
{
    binding routes[] = {
        get_binding("/", home),
        get_binding("/1", pg1),
        post_binding("/", post_home),
        post_binding("/l", login),
    };
    printf("Server is launching...\n");
    printf("%d\n", server_html(routes, 4));
}

int home(int target)
{
    return http_html_response_f(target, "./html/index.html");
}

int pg1(int target)
{
    return http_html_response_f(target, "./html/pg1.html");
}

int post_home(int target, mapitem input[])
{
    char *output = malloc(5000);
    sprintf(output, "<html><body>%s %s</body></html>", input[0].value, input[1].value);
    return http_html_response(target, output);
}

int login(int target, mapitem input[])
{
    char *output = malloc(500);
    int username = 0;
    int password = 0;
    mapitem item;
    int i;
    for (i = 0; i < 2; ++i)
    {
        item = input[i];
        if (item.key == "username" && item.value == "daniel") username = 1;
        if (item.key == "password" && item.value == "hello") password = 1;
        printf("%s: %s\n", item.key, item.value);
        if (username && password) break;
    }
    if (username && password) output = "<html><body>You have successfully logged in.</body></html>";
    else output = "<html><body>You have failed to log in.</body></html>";
    return http_html_response(target, output);
}
