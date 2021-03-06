#include <server.h>

#define BACKLOG 10
#define PORT 8080

/* typedef struct gsreturn
{
    // Return value for gen_socket()
    int s;
    struct sockaddr *sa;
} gsreturn; */

gsreturn gen_socket()
{
    // Create a socket, bind to PORT, listen and return file descriptor
    register int s;
    struct sockaddr_in sa;
    s = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&sa, sizeof sa);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(PORT);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, (struct sockaddr *)&sa, sizeof sa) < 0)
    {
        struct gsreturn gs;
        gs.s = -1;
        return gs;
    }
    listen(s, BACKLOG);
    struct gsreturn gs;
    gs.s = s;
    gs.sa = (struct sockaddr*)&sa;
    return gs;
}

int http_index_listen(gsreturn gs, char *reply)
{
    // Listen for requests and return an HTTP response.  Only capable of one response (index page).
    register int s, c;
    struct sockaddr *sa;
    FILE *client;
    int b;
    s = gs.s;
    sa = gs.sa;
    for (;;)
    {
        b = sizeof sa;
        if ((c = accept(s, sa, &b)) < 0 ) return c;
        if (http_html_response(c, reply) != 0) return -1;
    }
    return 0;
}

int http_response(int caddr, char *base, char *body)
{
    // Generic HTTP response function
    FILE *client;
    if ((client = fdopen(caddr, "w")) == NULL) return -1;
    fprintf(client, base, strlen(body), body);
    fclose(client);
    return 0;
}

int http_html_response(int caddr, char *body)
{
    // Basic HTML response
    char *base = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\nContent-length: %d\r\n\r\n%s\r\n";
    int result = http_response(caddr, base, body);
    return result;
}

int server_html_index(char *filename)
{
    char buff; // Character for reading file
    FILE *f;
    struct stat st;
    stat(filename, &st);
    int size = st.st_size + 1; // Size of file
    char *output = malloc(size);
    int counter_output = 0; // To keep track of position when reading the file
    f = fopen(filename, "r"); // File to read
    while ((buff = fgetc(f)) != EOF)
    {
        output[counter_output] = buff;
        ++counter_output;
    }
    output[counter_output] = 0; // Just to make sure it terminates
    fclose(f);
    struct gsreturn gs;
    gs = gen_socket();
    int result = http_index_listen(gs, output);
    free(output);
    return result;
}

int server_html(binding routes[], int routes_len)
{
    struct gsreturn gs;
    gs = gen_socket();
    return http_router(gs, routes, routes_len);
}

int http_router(gsreturn gs, binding routes[], int routes_len)
{
    register int s, c;
    struct sockaddr *sa;
    s = gs.s;
    sa = gs.sa;
    int b;
    int r;
    for (;;)
    {
        b = sizeof sa;
        if ((c = accept(s, sa, &b)) < 0) return c;
        if ((r = parse_route(c, routes, routes_len)) != 0) http500(c);
    }
    return 0;
}

int http500(int target)
{
    http_html_response(target, "500 internal server error");
    printf("ERROR processing request\n");
}

int http404(int target)
{
    return http_html_response(target, "404 URL not found");
}

int parse_route(int target, binding routes[], int routes_len)
{
    // Parses the content of target HTTP request, then matches in routes and returns index
    char *buff = malloc(5001);
    int v;
    for (v = 0; v < 5001; ++v) buff[v] = 0;
    read(target, buff, 5000);
    // printf("%s\n", buff);
    const char *second = strdup(buff);
    int found = 0;
    int type = 0;
    char *split;
    do
    {
        split = strtok(buff, "\r\n");
        if (strlen(split) > 4)
        {
            if (split[0] == 'G' && split[1] == 'E' && split[2] == 'T' && split[3] == ' ')
            {
                found = 1;
                break;
            }
        }
        if (strlen(split) > 5)
        {
            if (split[0] == 'P' && split[1] == 'O' && split[2] == 'S' && split[3] == 'T' && split[4] == ' ')
            {
                found = 1;
                type = 1;
                break;
            }
        }
    }
    while (!found && split != NULL);
    printf("%s\n", buff);
    char *path = malloc(100);
    strtok(split, " ");
    path = strtok(NULL, " ");
    int index;
    binding route;
    for (index = 0; index < routes_len; ++index)
    {
        if ((route = routes[index]).indicator == type && strncmp(route.path, path, sizeof route.path) == 0)
        {
            if (route.indicator)
            {
                parse_post(target, second, route.post);
            }
            else return route.get(target);
            break;
        }
    }
    return http404(target);
}

int parse_post(int fd, char *httpstring, int (*target) (int, mapitem[]))
{
    char *split;
    char *item;
    split = malloc(5001);
    int I;
    for (I = 0; I < 5001; ++I)
    {
        split[I] = 0;
    }
    item = malloc(5000);
    printf("RECEIVED:\n%s\nEND RECEIVED\n", httpstring);
    strtok(httpstring, "\r\n\r\n");
    int i;
    for (i = 0; i < 12; ++i) strtok(NULL, "\r\n\r\n"); // Yep, it's a terrible solution, but strtok is being weird.
    split = strtok(NULL, "\r\n\r\n");
    split[5000] = 0;
    int n = char_count(split, '&') + 1;
    mapitem values[n];
    char *items[n];
    items[0] = strtok(split, "&");
    i = 1;
    while ((item = strtok(NULL, "&")) && i < n)
    {
        items[i] = item;
        ++i;
    }
    for (i = 0; i < n; ++i)
    {
        mapitem result;
        result.key = strtok(items[i], "=");
        result.value = strtok(NULL, "=");
        values[i] = result;
    }
    target(fd, values);
}

int is_in(char *str, char seek)
{
    int found;
    found = 0;
    int i;
    for (i = 0; i < strlen(str); ++i)
    {
        if (str[i] == seek)
        {
            found = 1;
            break;
        }
    }
    return found;
}

int char_count(char *str, char seek)
{
    int result = 0;
    int i;
    for (i = 0; i < strlen(str); ++i)
    {
        if (str[i] == seek) ++result;
    }
    return result;
}

int http_html_response_f(int caddr, char *filename)
{
    // http_html_response from a file
    FILE *file;
    int counter;
    char *body;
    char c;
    struct stat st;
    stat(filename, &st);
    body = malloc(st.st_size);
    counter = 0;
    file = fopen(filename, "r");
    while ((c = fgetc(file)) != EOF)
    {
        body[counter] = c;
        ++counter;
    }
    return http_html_response(caddr, body);
}

binding get_binding(char *path, int (*target) (int))
{
    binding result;
    result.path = path;
    result.get = target;
    result.indicator = 0;
    return result;
}

binding post_binding(char *path, int (*target) (int, mapitem[]))
{
    binding result;
    result.path = path;
    result.post = target;
    result.indicator = 1;
    return result;
}
