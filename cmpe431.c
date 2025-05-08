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
void *child(void *arg)
{
    char line[DEFAULT_BUFLEN];
    int bytes_read;
    int *client = (int *)arg; 
    struct sockaddr_in client_addr;
    int addr_size = sizeof(client_addr); 
    int authenticated = 1; 

    send(client, "welcome to BOB file server \n", strlen("welcome to BOB file server \n"), 0); 

    if (getpeername(client, &client_addr, &addr_size)) 
    {
        perror("getpeername failed"); 
    }

    while (1)
    {
        read(client, line, DEFAULT_BUFLEN); 

        if (bytes_read == 0) 
        {
            printf("Connection closed\n");
            break;
        }
    }
}