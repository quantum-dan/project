#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

typedef struct gsreturn {int s; struct sockaddr *sa;} gsreturn; // Used to return both socket identifier and sockaddr struct
gsreturn gen_socket(void);
int http_index_listen(gsreturn gs, char *reply);
int server_html_index(char *filename);
int http_response(int caddr, char *base, char *body);
int http_html_response(int caddr, char *body);
int parse_route(int target, char **routes, int routes_len);
int http_listen(gsreturn gs, char **routes, int routes_len, char **replies);
int server_html(char **routes, int routes_len, char **replies);
