#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
//run : ./Client <Server-IP-address> <port no.>
#define MAX_LINE 10
#define SERVER_PORT 5432
//using namespace std;
/*
    Writen will write n bytes to a socket.
    Return: the number of bytes written or -1 on error.
    One error is "EINTR"
    I don't know that the return number should be just of the number of character we send or plus one additional.
    In this function, I choose n + 1.
*/

int writen(int socket_id, char* buf_write, int n){
    char* write_buf = buf_write;
    int num_left = n;
    int write_num;
    while(num_left > 0){
        write_num = write(socket_id, write_buf, num_left);
        
        if(write_num < 0 && errno != EINTR){
            return -1;
        }
        
        if(errno == EINTR){
            write_num = 0;
        }
        num_left -= write_num;
        write_buf += write_num;
    }
    return n - num_left;
}


int main(int argc, char* argv[]){
    struct sockaddr_in sin;
    char *host;
    char buf_write[MAX_LINE];
    int socket_id;
    char* port_no;
    char* server_address;
    int len;
    
    if(argc != 3){
        fprintf(stderr, "usage: simplex - talk host\n");
        exit(1);
    }
    
    server_address = argv[1];
    port_no = argv[2];
    
    printf("the ip address from command line is %s\n",server_address);
    printf("the port number from command line is %s\n",port_no);
    
    /* build address data structure */
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(server_address);
    sin.sin_port = htons(atoi(port_no));
    
    
    if((socket_id = socket(AF_INET, SOCK_STREAM, 0))< 0){
        perror("simplex - talk : socket\n");
        exit(0);
    }else{
        printf("Socket successfully created!\n");
    }
    
    if(connect(socket_id, (struct sockaddr *)&sin, sizeof(sin)) < 0){
        perror("simplex - talk: connect\n");
        close(socket_id);
        exit(0);
    }
    
    int write_len;
    
    while(fgets(buf_write, sizeof(buf_write), stdin)){
        buf_write[MAX_LINE - 1] = '\0';
        len = strlen(buf_write);
        write_len = writen(socket_id, buf_write, len);
        printf("The len of sended bytes is  %d\n",write_len);
    }
}