#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include<talk.h>
#include <poll.h> 
#include <pwd.h>

/*for client */
#define MAX_LEN 1000
#define LOCAL 0
#define REMOTE (LOCAL + 1)

/*for server */
#define LISTENER 0
#define MESSAGE_LEN 10 /*defined for acceptance message */

void serv_recv(int acceptedfd, struct sockaddr_in peeraddr, int verbose);
void serv_accept(int acceptedfd, int verbose, int N);
void client_talk(int sockfd);

int setup_connection_client(const char *hostname, int port, int verbose,int N){
    struct hostent *host;
    int sockfd;
    struct sockaddr_in servaddr;

    /*get host info*/
    if(!(host = gethostbyname(hostname) )){
        perror("gethostbyname error");
        exit(EXIT_FAILURE);
    }

    /*create a socket */
    /*int socket(int domain, int type, int protocol); 
        *AF_PACKET - low level packages sent 
        *SOCK_STREAM - reliable two way connection 
    */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        fprintf(stderr, "socket creation failed ...\n");
        exit(EXIT_FAILURE);
    }else if(verbose){
        printf("SOCKET() created successfully\n");
    }
    
    bzero(&servaddr, sizeof(servaddr));
    /*assign IP, Port to server address*/
    servaddr.sin_family = AF_INET; /*unspec for IP6 or IP4*/
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = *(uint32_t *)(host->h_addr_list[0]);
    if(verbose){
        printf("server address is %d and port is %d\n", 
            servaddr.sin_addr.s_addr, port);
    }


    /*Bind - assign an address and port number to the socket 
    if( (bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr))) ){
        
    } ONLY FOR SERVERS*/

    printf("Waiting for a response from %s\n", hostname);
    if(connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) != 0){
        printf("%s connection failed.\n", hostname);
        close(sockfd);
        exit(EXIT_FAILURE);
    }else if(verbose){
        printf("client connected successfully\n");
    }

    
    /*Once a connection is established, client will send its name */
    char username[NI_MAXHOST] = {'\0'}; 
    uid_t user_id = getuid();
    struct passwd *user = getpwuid(user_id);
    
    /*send its name to server */
    strcat(username, user->pw_name);
    if (( send(sockfd, username, NI_MAXHOST, 0) == -1))
        perror("send host message\n");
   
    /*recieve the servers answer */
    /*int mlen; */
    char message[MESSAGE_LEN] = {'\0'};
    if( (recv(sockfd, message, MESSAGE_LEN, 0)) == -1){
        perror("recv error");
    }else if(verbose){
        printf("server answered %s\n", message);
    }

    if( (strcmp(message, "y") == 0) || (strcmp(message, "yes") == 0) ){     
        /*if connection is accepted, both server and client switch          
        to graphical mode */
        if(verbose) printf("client entering graphical mode\n");
        
        if(!N){ /*if N is defined, don't start windowing */
            start_windowing();
            client_talk(sockfd);
        }
    }                                                           
    else{ /*if not accepted, close and */                                    
        printf("%s declined connection.\n", hostname);                        
        exit(0);                                  
    }    
    close(sockfd);
    return sockfd;
        

}

int setup_connection_host(int port, int verbose, int a, int N){
    struct sockaddr_in servaddr, peer_addr;
    int sockfd, acceptedfd, mlen;
    socklen_t len;
    /*char localaddr[INET_ADDRSTRLEN], peeraddr[INET_ADDRSTRLEN]; */
    
    /*create a socket and get a file descriptor*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        printf("error in server creating\n");
        exit(EXIT_FAILURE);
    }
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) < 0){
        perror("socket bind failed...");
        exit(EXIT_FAILURE);
    }else if(verbose){
        printf("SOCKET() successfully binded\n");
    }
    
    /*start listening for clients...here the process will go in sleep mode 
    and will wait for the incoming connection */
    if((listen(sockfd, 1)) != 0){
        perror("Listen failed...");
        exit(EXIT_FAILURE);
    }else if(verbose){
        printf("Server listening\n");
    }
    
   
    /*accept connection initially, and send y/n for client to print message*/
    len = sizeof(peer_addr);
    acceptedfd = accept(sockfd, (struct sockaddr *) &peer_addr, &len);
    if(acceptedfd < 0){
        perror("accept");
        exit(EXIT_FAILURE);
    }else if(verbose){
        printf("server accepted client\n");
    }


    
    /*recieves host name and sends acceptance */
    if(!a){
        serv_recv(acceptedfd, peer_addr, verbose);
        serv_accept(acceptedfd, verbose, N);
    }else if(a){ /*server accepts connection without asking */
        char *message = "yes";
        serv_recv(acceptedfd, peer_addr, verbose); /*server recv hostname */
        /*send acceptance to client */
        if (( send(acceptedfd, message, 4, 0) == -1))
            perror("send acceptance message\n");
        start_windowing();
        client_talk(acceptedfd);
    }

    close(acceptedfd);
    close(sockfd);

    return acceptedfd;
}


/*function which will handle server send/recv
    *accepts filedescriptor for send/recv
*/

/*RECIEVES HOSTNAME FROM CLIENT */
void serv_recv(int acceptedfd, struct sockaddr_in peeraddr, int verbose){
    int mlen;
    char hostname[NI_MAXHOST];
    char hbuf[NI_MAXHOST];

    if( (mlen = recv(acceptedfd, hostname, sizeof(hostname), 0)) == -1){                       
        perror("recv error");                                               
    }


    if( getnameinfo((struct sockaddr *) &peeraddr, sizeof(peeraddr), hbuf, 
                                sizeof(hbuf), NULL, 0, NI_NAMEREQD) != 0){
        printf("could not resolve hostname...will use user@hostname\n");
        strcpy(hbuf, "@hostname");
    }else if(verbose){
        printf("host is %s\n", hbuf);
    }
    
    strcat(hostname, "@");
    strcat(hostname, hbuf); 


    printf("Mytalk request from %s. Accept (y/n)? ", hostname);
}



/*DECIDES WHETHER TP ACCEPT, AND ENTERS GRAPHICAL MODE IF IT DOES ACCEPT*/
void serv_accept(int acceptedfd, int verbose, int N){
    
    /*DISPLAY ACCEPTANCE MESSAGE AND ACCEPT/DECLINE*/
    /* how do i get hostname??*/
    char message[MESSAGE_LEN] = {'\0'};
    int ch;
    for(int i = 0; ch != '\n' && i < MESSAGE_LEN; i++){
        ch = fgetc(stdin);
        message[i] = ch;
        if (ch == '\n' || i == 9)
            message[i] = '\0';
    }

    for(size_t i = 0; i < strlen(message); i++){
        message[i] = tolower(message[i]);
    }

    if (( send(acceptedfd, message, MESSAGE_LEN, 0) == -1))
        perror("send acceptance message\n");
    
    /*ENTER GRAPHICAL MODE IF ACCEPTED */
    if( (strcmp(message, "y") == 0) || (strcmp(message, "yes") == 0) ){     
        /*if connection is accepted, both server and client switch          
        to graphical mode */
        if(verbose) printf("server entering graphical mode\n");
        if(!N){
            start_windowing();
            client_talk(acceptedfd);
        }
    }                                                           
}


void server_talk(int acceptedfd){
    /*LOCAL IS SERVER IN THIS CASE && REMOTE IS CLIENT */
    struct pollfd fds[REMOTE + 1]; /*initialize poll structures 
    int len, mlen;
    fds[LOCAL].fd = STDIN_FILENO; /*locally we're taking data from STDIN TO
    SEND*/
    fds[LOCAL].events = POLLIN; /*locally we're waiting for data to come in*/
    fds[LOCAL].revents = 0;    /*initially 0 events have happened */
    fds[REMOTE] = fds[LOCAL]; 
    fds[REMOTE].fd = acceptedfd; /*remote has a different file descriptor */

}

void client_talk(int sockfd){
    char buff[MAX_LEN + 1]; 
    char outbuff[MAX_LEN +1];
    int infinite_poll = -1;
    struct pollfd fds[REMOTE + 1]; /*initialize poll structures 
                                    (2 fd are talking) */

    int len, mlen, slen;
    fds[LOCAL].fd = STDIN_FILENO; /*locally we're taking data from STDIN TO
    SEND*/
    fds[LOCAL].events = POLLIN; /*locally we're waiting for data to come in*/
    fds[LOCAL].revents = 0;    /*initially 0 events have happened */
    fds[REMOTE] = fds[LOCAL]; 
    fds[REMOTE].fd = sockfd; /*remote has a different file descriptor */
    
    int done = 0;
    do{
        /*start polling to listen to other side */
        /*poll(fds struct, number fd's (2), poll infinitely) */
        if(poll(fds, sizeof(fds)/sizeof(struct pollfd) ,infinite_poll) == -1)
            perror("poll in client_talk failed\n");
        
        if(fds[LOCAL].revents & POLLIN){
            /*len = fgets(STDIN_FILENO, buff, MAXLINE); */
            update_input_buffer();
            /*check to see if it has a whole line and send if so */
            if(has_whole_line()){
                memset(&buff, 0, sizeof(buff));
                if( (len = read_from_input(buff, MAX_LEN)) ){
                    if((slen = send(sockfd, buff, len, 0)) == -1){
                        close(sockfd);
                        done = 1;
                        perror("send in client_talk failed:");
                    }
                    if(slen == 0){
                        done = 1;
                    }
                }
            }
            if(has_hit_eof()){
                    close(sockfd);
                    done = 1;
            }
        }
        if(fds[REMOTE].revents & POLLIN){
            memset(&outbuff, 0, sizeof(outbuff));
            if((mlen = recv(sockfd, outbuff, sizeof(buff), 0)) == -1)
                perror("recv in client_talk failed:");
            if(mlen == 0){
                done = 1;
            }

            /*write to console */
            if(write_to_output(outbuff, mlen)){
                fprintf(stderr, "write_to_output error\n");
            }
            if(has_hit_eof()){
                close(sockfd);
                done = 1;
            }
        }
    }while(!done);
    

    fprint_to_output("Conncection closed. ^C to terminate.\n");
    done = 0;
    do{
        /*After done...perform no other I/O*/
        /*WAIT UNTIL ^C TO TERMINATE */
    }while(!done);

}
