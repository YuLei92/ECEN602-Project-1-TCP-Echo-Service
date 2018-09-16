#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include <errno.h>
#include <strings.h>
#include <string.h>
#define MAX_LINE 10
#define MAX_PEDING 10

//run : ./Server <port no.>
int main(int argc, char* argv[]){
    struct sockaddr_in sin;
    char buf[MAX_LINE];
    int len;
    char* port_no;
    int socket_id, new_socket_id; //This is the socket
    
    if(argc != 2){
        fprintf(stderr, "usage: simplex - talk host\n");
        exit(0);
    }
    port_no = argv[1];
    printf("The port number from command line is %s\n",port_no);
    
//    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr("127.0.0.1"); // maysbe ip address;
    sin.sin_port = htons(atoi(port_no));
    printf("The server address is 127.0.0.1\n");
    
    /* setup passive open*/
    if((socket_id = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Simplex - talk : socket");
        exit(0);
    }
    printf("Socket successfully created!\n");
    
 //   printf("Listening to the client...");
    
    if((bind(socket_id, (struct sockaddr *)&sin, sizeof(sin))) < 0){
        perror("simplex -talk: bind\n");
        exit(0);
    }else{
        printf("Socket binded.\n");
    }
    
    printf("Listening to the client...\n");
    
    if(listen(socket_id, MAX_PEDING)< 0){
        perror("Unable to find client\n");
    }
    
    while(1){
        if((new_socket_id = accept(socket_id,(struct sockaddr *)&sin, &len)) <0){
            perror("simplex - talk: accept\n");
            exit(0);
        }
        while(len = recv(new_socket_id, buf, sizeof(buf), 0))
            fputs(buf, stdout);
        close(new_socket_id);
    }
}