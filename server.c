#include <server.h>

#define BACKLOG 10
#define PORT 8080

int test(char *output)
{
    register int s, c; // s will be server socket, c will be client socket
    int b; // b will be sizeof sockaddr_in
    struct sockaddr_in sa; // server socket address
    FILE *client; // client to write to

    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) // create socket and check for errors
    {
        perror("socket");
        return 1;
    }

    bzero(&sa, sizeof sa); // make sa empty

    sa.sin_family = AF_INET; // IPv4
    sa.sin_port = htons(PORT); // PORT macro
    sa.sin_addr.s_addr = htonl(INADDR_ANY); // any IP address (0.0.0.0)

    if (bind(s, (struct sockaddr *)&sa, sizeof sa) < 0) // bind s to sa and check for errors
    {
        perror("bind");
        return 2;
    }

    listen(s, BACKLOG); // listen on s

    for (;;)
    {
        b = sizeof sa;
        if ((c = accept(s, (struct sockaddr *)&sa, &b)) < 0) // accept connection and check for errors
        {
            perror("accept");
            return 4;
        }

        if ((client = fdopen(c, "w")) == NULL) // open connection as file and check for errors
        {
            perror("fdopen");
            return 5;
        }

        char base[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\nContent-length: %d\r\n\r\n%s\r\n"; // base HTTP string
        fprintf(client, base, strlen(output), output); // write to client
        fclose(client);
    }
}

int server_html(char *filename)
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
    test(output); // Call the server function with the output
}
