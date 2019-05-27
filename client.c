#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>

int main(int argc, char *argv[])
{
    if(argc < 3){
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(1);
    }

    int sockfd, portno, n;
    char buffer[1024];
    struct sockaddr_in server_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("Error opening socket");
        exit(1);
    }
    
    portno = atol(argv[2]);

    server = gethostbyname(argv[1]);
    if(server == NULL){
        fprintf(stderr,"Error No such host\n");
    }
    bzero((char *)&server_addr, sizeof(server_addr)); //clearing server_addr
    server_addr.sin_family = AF_INET;
    bcopy((char *)server -> h_addr_list[0], (char *)&server_addr.sin_addr.s_addr, server -> h_length);
    server_addr.sin_port = htons(portno); // host to network short

    if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <0 ){
        perror("Connection failed");
    }
   // int bsize = sizeof(buffer);
    while(1){
        bzero(buffer, 1024);
        fgets(buffer, 1024, stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if(n <  0){
            perror("Error Writing");
        }
        bzero(buffer, 1024);
        n = read(sockfd, buffer, 1024);
        if(n <  0){
            perror("Error reading");
        }
        printf("Server: %s", buffer);

        int i = strncmp("Bye\n", buffer, 3);
        if (i == 0)
            break; 
    }
    close(sockfd);
    return 0;
}