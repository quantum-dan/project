#include <server.h>

int home(int target);
int pg1(int target);
int post_home(int target, char *input);

void main()
{
    binding routes[3];
    routes[0] = get_binding("/", home);
    routes[1] = get_binding("/1", pg1);
    routes[2] = post_binding("/", post_home);
    printf("Server is launching...\n");
    printf("%d\n", server_html(routes, 3));
}

int home(int target)
{
    return http_html_response_f(target, "./html/index.html");
}

int pg1(int target)
{
    return http_html_response_f(target, "./html/pg1.html");
}

int post_home(int target, char *input)
{
    return http_html_response_f(target, "./html/index.html");
}
