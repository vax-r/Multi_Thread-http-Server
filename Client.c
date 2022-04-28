#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc,char *argv[]){

    // int sockfd[5]={0};
    int sockfd;
    //create socket
    sockfd=socket(AF_INET,SOCK_STREAM,0);

    //set socket info
    struct sockaddr_in sockInfo;
    bzero(&sockInfo,sizeof(sockInfo));
    sockInfo.sin_family=PF_INET;

    sockInfo.sin_addr.s_addr=inet_addr("127.0.0.1");
    sockInfo.sin_port=htons(8088);

    //try to connect
    int err=connect(sockfd,(struct sockaddr*)&sockInfo,sizeof(sockInfo));
    while(err==-1){
        printf("Connection error.\n");
        printf("Trying again...\n");
        err=connect(sockfd,(struct sockaddr*)&sockInfo,sizeof(sockInfo));
    }

    char message[]={"Hi, I am a client."};
    char response[100]={};
    send(sockfd,message,sizeof(message),0);
    recv(sockfd,response,sizeof(response),0);

    printf("%s\n",response);
    printf("close socket.\n");
    close(sockfd);

    return 0;
}