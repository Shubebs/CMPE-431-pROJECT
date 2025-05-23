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
    int client = *(int *)arg; 
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr); 
    int authenticated = 0; 

    send(client, "welcome to BOB file server \n", 30, 0); 

    if (getpeername(client, (struct sockaddr *)&client_addr, &addr_size) < 0) 
    {
        perror("getpeername() failed");
        return NULL; 
    }

    while (1)y
    {
        bytes_read = read(client, line, DEFAULT_BUFLEN); 

        if (bytes_read < 0) 
        {
            perror("ERROR reading from socket");
            break;
        }
        else if (bytes_read == 0)
        {
            printf("Client disconnected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            break;
        }

        line[bytes_read] = '\0';
        char *command = strtok(line, " \n");
        if (strcmp(command, "USER") == 0 || strcmp(command,"user")== 0)
        {
            char *username = strtok(NULL, " ");
            char *password = strtok(NULL, " ");

            if (username != NULL && password != NULL)
            {
                if (authentication(username, password))
                {
                    authenticated = 1;
                    send(client, "200 User granted to access\n", strlen("200 User granted to access\n"), 0);
                    printf("User logged in: %s\n", username);
                }
                else
                {
                    send(client, "User not found.\n", strlen("User not found.\n"), 0);
                }
            }}
            else if (!authenticated)
            {
            send(client, "Please log in first.\n", strlen("Please log in first.\n"), 0);
            }else if (strcmp(command, "LIST") == 0 || strcmp(command,"list") == 0)
            {
                printf("Received LIST_FILES command.\n");


                DIR *directory;
                struct dirent *entry;
                directory = opendir(".");
                if (directory == NULL)
                {
                    perror("Error opening directory");
                    break;
                }


                while ((entry = readdir(directory)) != NULL)
                {
                struct stat file_stat;
                if (stat(entry->d_name, &file_stat) < 0)
                {
                    perror("Error getting file status");
                    break;
                }
                char file_info[DEFAULT_BUFLEN];
                sprintf(file_info, "%s %ld\n", entry->d_name, file_stat.st_size);
                send(client, file_info, strlen(file_info), 0);
                }

                if (closedir(directory) == -1)
                {
                perror("Error closing directory.\n");
                break;
                }

                send(client, ".", 1, 0);
            }

            else if (strcmp(command ,"GET") == 0 || strcmp(command ,"get") == 0)
            {
            char *filename= strtok(NULL," \n ");
            
            if (filename != NULL)
            {
                FILE*myfile;
                myfile = fopen(filename,"r");

            if (myfile == NULL){

                char detection [DEFAULT_BUFLEN];
                sprintf(detection,"404 FILE %s can't be found \n",filename);
                send (client, detection , strlen(detection),0);

            }
            else{

           char show[DEFAULT_BUFLEN];
           while (fgets(show, DEFAULT_BUFLEN, myfile) != NULL)
             {
               send(client, show, strlen(show), 0);
             } 
             fclose(myfile);
             send(client, "\r\n.\r\n", strlen("\r\n.\r\n"), 0);
         }

            }
        }else if (strcmp(command ,"PUT") == 0 || strcmp(command ,"put") == 0)
{
    char *filename= strtok(NULL," \n ");
    if (filename != NULL)
    {
        FILE* myfile;
        myfile = fopen(filename,"w");
        if (myfile == NULL){
            char detection [DEFAULT_BUFLEN];
            sprintf(detection,"400 File %s cannot save on server side.\n",filename);
            send (client, detection , strlen(detection),0);
        } else{ 
            
            char show[DEFAULT_BUFLEN];
            int t_byte = 0;
            while (1) 
            {
                bytes_read = read(client, show, DEFAULT_BUFLEN);
                if (bytes_read <= 0) {
                    break;
                }
           
                if (bytes_read == 2 && show[0] == '.' && show[1] == '\n') {
                    break;
                }
                fwrite(show, sizeof(char), bytes_read, myfile);
                t_byte += bytes_read;
            } 
            fclose(myfile);
            char message[DEFAULT_BUFLEN];
            sprintf(message, "200 %d Byte %s file retrieved by server and was saved.\n", t_byte, filename);
            send(client, message, strlen(message), 0);
        }
        
    }
} else if (strcmp(command ,"DELETE")==0 || strcmp(command ,"delete")==0)
{
    char* myfilename = strtok(NULL,"\n");
    if(myfilename !=NULL){
    int detect =remove(myfilename);

    if(detect == 0){
        char message[DEFAULT_BUFLEN];
        sprintf(message,"200 file %s has been deleted.\n",myfilename);
        send(client,message,strlen(message),0);

    }else
    {
        char msg[DEFAULT_BUFLEN];
        sprintf(msg,"400 file %s is not on the server .\n",myfilename);
        send(client,msg,strlen(msg),0);
    }
    

    }

}  else if (strcmp(command,"QUIT")==0||strcmp(command,"quit")==0)
{
    send(client, "Goodbye!\n", strlen("Goodbye!\n"), 0);
         printf("Client disconnected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
         close(client);
         pthread_exit(NULL);
}
            else
            {
                send(client, "Invalid command.\n", strlen("Invalid command.\n"), 0);
            }
        
        
        }




    }
int main(int argc, char *argv[])
{
    int server_socket, optival, option;
    struct sockaddr_in addr;
    unsigned short port = 0;
    char dir[256] = ".";
    char pwd_file[256];

    while ((option = getopt(argc, argv, "d:p:u:")) != -1)
    {
        switch (option)
        {
        case 'd':
            strncpy(dir, optarg, sizeof(dir));
            break;
        case 'p':
            port = atoi(optarg);
            break;
        case 'u':
            strncpy(pwd_file, optarg, sizeof(pwd_file));
            break;
        default:
            fprintf(stderr, "Usage: %s -d <directory> -p <port> -u <password_file>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    printf("Directory: %s\n", dir);
    printf("Port: %d\n", port);
    printf("Password file: %s\n", pwd_file);

    if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        PANIC("Socket");

    addr.sin_family = AF_INET;

    if (port > 0)
        addr.sin_port = htons(port);
    else
        addr.sin_port = htons(PORT);

    addr.sin_addr.s_addr = INADDR_ANY;
    optival = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optival, sizeof optival);

    if (bind(server_socket, (struct sockaddr *)&addr, sizeof(addr)) != 0)
        PANIC("Bind");

    if (listen(server_socket, SOMAXCONN) != 0)
        PANIC("Listen");

    printf("System ready on port %d\n", ntohs(addr.sin_port));

    while (1)
    {
        int client;
        socklen_t addr_size = sizeof(addr);
        pthread_t child_thread;
        struct sockaddr_in client_addr;
        client = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
        if (client < 0)
        {
            perror("Accept failed");
            continue; 
        }
        if (pthread_create(&child_thread, NULL, child, &client) != 0)
            perror("Thread creation");

        pthread_detach(child_thread);
        printf("Connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    }

    return 0;
}