#include <stdio.h>
#include <string.h>

int createDb(char *title);
int dbInsert(char *dbname, char *key, char *value);
char *dbRead(char *dbname, char *key);
