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
#define MAX_LENGTH 10

/*
    Writen will write n bytes to a socket.
    Return: the number of bytes written or -1 on error.
    One error is "EINTR"
    I don't know that the return number should be just of the number of character we send or plus one additional.
    In this function, I choose n + 1.
*/
int readline(int socket_id, char* buf_read, int n){
    char *read_buf;
    int buf_shift = 0;
    int read_state = 0;
    char read_char;
    if (n <= 0 || read_buf == NULL) {
        errno = EINVAL;
        return -1;
    }
    
    int read_no = 0;
    read_buf = buf_read;
    
    while(read_no < n - 1){
        read_state = read(socket_id, &read_char, 1);
//        putchar(read_char);
        
        if(read_state == -1){// check the error
            if(errno == EINTR){
                continue;
            }else{
                return -1;
            }
        }
        
        if(read_char == '\n'){ // if receive '\n'
            break;
        }
    
        if(read_state == 0){ //Check EOF
            if(read_no == 0){
                return 0;
            }else{
                break;
            }
        }

        *read_buf = read_char;
        read_buf++;
        read_no++;
    }
//    printf("The len of received bytes is  %d\n",read_no);
    *read_buf = '\n';
    read_buf++;
    *read_buf = '\0';
    return read_no;
}

int writen(int socket_id, char* buf_write, int n){
    char* write_buf = buf_write;
    if (n <= 0 || write_buf == NULL) {
        errno = EINVAL;
        return -1;
    }
    
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
    int socket_id;
    char* port_no;
    char* server_address;
    int len;
    char buf_write[MAX_LENGTH + 2];
    char buf_recv[MAX_LENGTH + 2];
    
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
    int read_len;
    
    while(fgets(buf_write, sizeof(buf_write), stdin)|| feof(stdin)){
        if(feof(stdin)){
            fputs("EOF inputed, the socket will be closed and exited.\n", stdout);
            break;
        }
        len = strlen(buf_write);
        
        if((strlen(buf_write) == (MAX_LENGTH + 1))&& buf_write[MAX_LENGTH] != '\n'){
            fputs("The input is to long, please try again！\n", stdout);
            __fpurge(stdin);
            continue;
        }
//        buf_write[MAX_LENGTH] = '\0';
        write_len = writen(socket_id, buf_write, len);
        if(read_len = readline(socket_id, buf_recv, len)){
           printf("Received echo from the server : ");
           fputs(buf_recv, stdout);
           break;
        }
    }
    close(socket_id);
    printf("This client is terminated.\n");
}