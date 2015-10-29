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
typedef struct mapitem {char *key; char *value;} mapitem; // Used to pass key-value pairs
typedef struct binding {char* path; int indicator; int (*get) (int); int (*post) (int, mapitem[]);} binding;
// path = URL; indicator = 0 if get, 1 if post; get is get target; post is post target (leave either null if unused)
gsreturn gen_socket(void);
int http_index_listen(gsreturn gs, char *reply);
int server_html_index(char *filename);
int http_response(int caddr, char *base, char *body);
int http_html_response(int caddr, char *body);
int parse_route(int target, binding routes[], int routes_len);
int http_listen(gsreturn gs, char **routes, int routes_len, char **replies);
int http_html_response_f(int caddr, char *filename);
binding get_binding(char *path, int (*target) (int));
binding post_binding(char *path, int (*target) (int, mapitem[]));
int http_router(gsreturn gs, binding routes[], int routes_len);
int http500(int target);
int http404(int target);
int server_html(binding routes[], int routes_len);
int is_in(char *str, char seek);
int char_count(char *str, char seek);

