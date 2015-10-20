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

typedef struct gsreturn {int s; struct sockaddr *sa;} gsreturn;
gsreturn gen_socket(void);
int http_index_listen(gsreturn gs, char *reply);
int server_html(char *filename);
int http_response(int caddr, char *base, char *body);
int http_html_response(int caddr, char *body);
