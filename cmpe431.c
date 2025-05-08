#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#define DEFAULT_BUFLEN 1024
#define PORT 1225
void HANDLE_ERROR(char *msg);
#define PANIC(msg) { perror(msg); exit(-1); }

int authentication(char *username, char *password)
{
    FILE *file = fopen("password.cfg", "r");
    if (!file)
    {
        perror("Error opening password file");
        return 0;
    }

    char myline[256];
    while (fgets(myline, sizeof(myline), file))
    {
        char *file_userNm = strtok(myline, ":");
        char *file_pwd = strtok(NULL, ":");

        if (file_userNm != NULL && file_pwd != NULL)
        {
            if (strcmp(username, file_userNm) == 0 && strcmp(password, file_pwd) == 0)
            {
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);
    return 0;
}