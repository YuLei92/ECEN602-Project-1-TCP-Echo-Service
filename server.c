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
#define MAX_LENGTH 10 
#define MAX_PEDING 10

// n is MAX_LENGTH + 2
int readline(int socket_id, char* buf_read, int n){
    char *read_buf;
//    memset(read_buf, 0, n);
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

void subprocess(int socket){
//    printf("Try to get...");
    int len, lenw;
    char buf[MAX_LENGTH + 1];
    memset(buf, 0, MAX_LENGTH + 2);
    while(len = readline(socket, buf, MAX_LENGTH + 1)){
        if(len < 0){
            printf("Error when receiving from the client!");
            exit(1);
        }else if(len == 0){
          printf("A connection has been shut off.");
            exit(1);
        }else{
            printf("Message from one client is: ");
            fputs(buf, stdout);
        }
        lenw = writen(socket, buf, len + 1);
        if(lenw < 0){
            printf("Unable to send message to client...");
            exit(1);
        }
    }
}

//run : ./Server <port no.>
int main(int argc, char* argv[]){
    struct sockaddr_in sin;
    int len;
    char* port_no;
    int socket_id, new_socket_id; //This is the socket
    pid_t ppid, pid;
//we don't need buf in the main.
    
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
    
    
    if((bind(socket_id, (struct sockaddr *)&sin, sizeof(sin))) < 0){
        perror("simplex -talk: bind\n");
        exit(0);
    }else{
        printf("Socket binded.\n");
    }
    
    printf("Listening to the client...\n");
    
    if(listen(socket_id, MAX_PEDING)< 0){
        perror("Cannot find the client!\n");
    }
    
    int write_len = 0;
    while(1){
        if((new_socket_id = accept(socket_id,(struct sockaddr *)&sin, &len)) <0){
            perror("simplex - talk: accept\n");
            exit(0);
        }
        
        ppid = fork();
        
        if(ppid < 0){
            perror("Failure to fork 1");
            exit(0);
        }
        if (ppid == 0) { // The child process is generated.
            pid = fork();
            if(pid < 0){
                perror("Failure to fork 1");
            }
            if(pid == 0){
                subprocess(new_socket_id); // Fork successfully and try to handle the messages
                exit(0);
                }
        }
        else{
             close(new_socket_id);
        }
        
        /*
        
        int pid = fork();
        
        if(pid < 0){
            perror("Failure to fork");
        }
        if (pid == 0) { // The child process is generated.
            subprocess(new_socket_id); // Fork successfully and try to handle the messages
            exit(0);
        }
        else {
             close(new_socket_id);
        }
        */
        
    }
}
