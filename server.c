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

int server_html(char **routes, int routes_len, char **replies)
{
    struct gsreturn gs;
    gs = gen_socket();
    return http_listen(gs, routes, routes_len, replies);
}

int http_listen(gsreturn gs, char **routes, int routes_len, char **replies)
{
    // Test for routing server
    register int s, c;
    struct sockaddr *sa;
    s = gs.s;
    sa = gs.sa;
    int b;
    FILE *client;
    int route_val;
    int errval;
    for (;;)
    {
        b = sizeof sa;
        if ((c = accept(s, sa, &b)) < 0) return c;
        route_val = parse_route(c, routes, routes_len);
        if ((errval = http_html_response_f(c, replies[route_val])) != 0) return errval;
    }
    return 0;
}

int parse_route(int target, char **routes, int routes_len)
{
    // Parses the content of target HTTP request, then matches in routes and returns index
    char *buff = malloc(5000);
    read(target, buff, 5000);
    int found = 0;
    char *split;
    printf("%s\n", buff);
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
    } while (!found);
    char *path = malloc(10);
    strtok(split, " ");
    path = strtok(NULL, " ");
    int index;
    for (index = 0; index < routes_len; ++index)
    {
        if (strncmp(routes[index], path, sizeof routes[index]) == 0)
        {
            return index;
            break;
        }
    }
    return 0;
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
