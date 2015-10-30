#include <db.h>

int createDb(char *title)
{
    // Creates a database file
    FILE *f;
    f = fopen(title, "w");
    return 0;
}

int dbInsert(char *dbname, char *key, char *value)
{
    FILE *f;
    f = fopen(dbname, "a");
    if (f == NULL) return -1;
    fprintf(f, "\n%s:::%s", key, value);
    fclose(f);
    return 0;
}

char *dbRead(char *dbname, char *key)
{
    FILE *f;
    int i;
    int length;
    f = fopen(dbname, "r");
    static char result[10000];
    for (i = 0; i < 10000; ++i) result[i] = 0;
    char line[10000];
    for (i = 0; i < 10000; ++i) line[i] = 0;
    do
    {
        fgets(line, 10000, f);
        if (strncmp(line, key, strlen(key)) == 0)
        {
            strtok(line, ":::");
            strcpy(result, strtok(NULL, ":::"));
            return result;
            break;
        }
    } while (line != NULL);
    return NULL;
}
