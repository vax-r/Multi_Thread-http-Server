#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>

#define THREADS_LIMITATION 2

pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;
int num_of_threads;

void *Request_handler(void *arg_sockfd){
    int sockfd=*((int *)arg_sockfd);

    char inputBuffer[256]={};
    char Legal_message[]={"Good connection.\n"};
    char Bad_message[]={"Bad connection.\n"};
    char Busy_message[]={"Stop sending your idiot request.\n"};
    //receive the request content
    int request=recv(sockfd,inputBuffer,sizeof(inputBuffer),0);

    //check the number of threads
    pthread_mutex_lock(&mu);
    num_of_threads++;
    // if(num_of_threads>THREADS_LIMITATION){
    //     printf("====================================================\n");
    //     printf("Busy Server.\n");
    //     printf("Num of threads %d\n",num_of_threads);
    //     printf("====================================================\n");

    //     send(sockfd,Busy_message,sizeof(Busy_message),0);
    //     num_of_threads--;
    //     pthread_mutex_unlock(&mu);
    //     free(arg_sockfd);
    //     shutdown(sockfd,SHUT_RDWR);
    //     close(sockfd);
    //     sockfd=-1;
    //     pthread_exit(NULL);
    // }
    pthread_mutex_unlock(&mu);

    if(request>0){
        // printf("Connection Succeed, num of thread is %d\n",num_of_threads);
        if(num_of_threads>1)
            printf("Connection Succeed, num of thread is %d\n",num_of_threads);
        send(sockfd,Legal_message,sizeof(Legal_message),0);
    }
    else if(request==0){
        printf("Client disconnected unexpectedly.\n");
    }
    else{
        printf("Fail to receive.\n");
    }

    sleep(1);

    free(arg_sockfd);
    shutdown(sockfd,SHUT_RDWR);
    close(sockfd);
    sockfd=-1;
    
    // sleep(1);

    pthread_mutex_lock(&mu);
    num_of_threads--;
    pthread_mutex_unlock(&mu);
    pthread_detach(pthread_self());
    pthread_exit(NULL);
}

int main(int argc,char *argv[]){

    char inputBuffer[256]={};
    char message[]={"Hello,I am the server.\n"};
    int sockfd=0,ClientSockfd=-1,*new_sockfd;//socket描述符

    //create socket for server
    sockfd=socket(AF_INET,SOCK_STREAM,0);

    if(sockfd==-1){printf("Fail to create a socket.\n");}

    //socket connection
    struct sockaddr_in serverInfo,clientInfo;
    int addrlen=sizeof(clientInfo);

    bzero(&serverInfo,sizeof(serverInfo));
    serverInfo.sin_family=PF_INET;
    serverInfo.sin_addr.s_addr=INADDR_ANY;
    serverInfo.sin_port=htons(8088);
    bind(sockfd,(struct sockaddr*)&serverInfo,sizeof(serverInfo));
    listen(sockfd,20);

    num_of_threads=0;

    const int SET=20;
    pthread_t server_thread[SET];
    int rounds=0;
    printf("Started to wait for client...\n");
    while((ClientSockfd=accept(sockfd,(struct sockaddr*)&clientInfo,&addrlen))){
        // printf("Connection accepted\n");

        while(num_of_threads>THREADS_LIMITATION){
            printf("=======================\n");
            printf("Busy Server, number of threads is %d\n",num_of_threads);
            printf("wait for 1 sec...\n");
            sleep(1);
        }
        new_sockfd=malloc(sizeof(int));//make room for an integer
        *new_sockfd=ClientSockfd;

        //create a thread for each request
        pthread_create(&server_thread[rounds%SET],NULL,Request_handler,(void *)new_sockfd);
    }


    pthread_mutex_destroy(&mu);
    pthread_exit(NULL);
    return 0;
}