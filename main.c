#include <server.h>

int home(int target);
int pg1(int target);
int post_home(int target, mapitem input[]);
int login(int target, mapitem input[]);
int get_db(int target);

int main()
{
    binding routes[] = {
        get_binding("/", home),
        get_binding("/1", pg1),
        post_binding("/", post_home),
        post_binding("/l", login),
        get_binding("/db", get_db),
    };
    dbInsert("./db/main.db", "1", "hello, world!");
    printf("Server is launching...\n");
    printf("%d\n", server_html(routes, 5));
}

int home(int target)
{
    return http_html_response_f(target, "./html/index.html");
}

int get_db(int target)
{
    char *output;
    output = malloc(500);
    sprintf(output, "<html><body><p>%s</p></body></html>", dbRead("./db/main.db", "1"));
    return http_html_response(target, output);
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
        if (strcmp(item.key, "username") == 0 && strcmp(item.value, "daniel") == 0) username = 1;
        if (strcmp(item.key, "password") == 0 && strcmp(item.value, "hello") == 0) password = 1;
        printf("%s: %s\n", item.key, item.value);
        if (username && password) break;
    }
    if (username && password) output = "<html><body>You have successfully logged in.</body></html>";
    else output = "<html><body>You have failed to log in.</body></html>";
    return http_html_response(target, output);
}
