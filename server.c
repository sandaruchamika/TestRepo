#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>

int main(int argc, char *argv[])
{
    if(argc < 2){
        fprintf(stderr, "Port No not provided, Program terminated\n");
        exit(1);
    }

    int sockfd, newsockfd, portno, n;
    char buffer[1024];
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("Error opening socket");
    }

    bzero((char *)&server_addr, sizeof(server_addr)); //clearing server_addr
    portno = atol(argv[1]);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(portno); // host to network short

    if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("Binding failed");
    }

    listen(sockfd, 5);
    client_len = sizeof(client_addr);

    newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
    if(newsockfd < 0)
        perror("Accept failed");
    
    
    while(1){
        bzero(buffer, 1024);
        n = read(newsockfd, buffer, 1024);
        if(n < 0){
            perror("read failed");
        }
        printf("Client: %s", buffer);
        bzero(buffer, 1024);
        fgets(buffer, 1024, stdin);
        n = write(newsockfd, buffer, strlen(buffer));
        if(n < 0){
            perror("write failed");
        }

        int i = strncmp("Bye\n", buffer, 3);
        if (i == 0)
            break; 
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}