#include <stdio.h>
#include <stdlib.h>
#include <getopt.h> 
#include <string.h>
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
#include "connection.h"


/*DESCRIPTION
    1.) Client and server side...if hostname is present act as a client,
    if not present act as a server
    2.) Client and server use TCP SOCK_STREAM for communications
*/


int main(int argc, char *argv[]){
    int opt;
    int v = 0, a = 0, N = 0; /*flags for arguments */
    char *hostname = NULL;
    long int port = 0;
    int acceptfd = 0; /*servers file descriptor*/
    while(((opt = getopt(argc, argv, "vaN")) != -1)) {
        switch(opt){
            case 'v':
                v = 1;
                break;
            case 'a':
                a = 1;
                break;
            case 'N':
                N = 1;
                break;
            default:
                printf("Usage: mytalk [-v] [-a] [-N] [ hostname ] port\n");
        }

    }
    
    if(v){
        if(v) printf("v is true\n");
        if(a) printf("a is true\n");
        if(N) printf("N is true\n");
    }
    
    if(optind < argc && (optind + 1) == argc){ /*act as server */
        char **end = NULL;
        port = strtol(argv[optind], end, 10);
        if(v){
            printf("PORT is %ld\n", port);
        }
        if(end != NULL){
            perror("strtol");    
        }
        optind++;

        acceptfd = setup_connection_host(port, v, a, N);
        
    }
    else if(optind < argc && (optind + 1) < argc){/*if hostname given, client */
        /*evalutate host and port */
        hostname = argv[optind];
        optind++;
        if(v){
            printf("hostname is %s\n", hostname);
        }

        char **end = NULL;
        port = strtol(argv[optind], end, 10);
        if(end != NULL){
            perror("strtol");    
        }
        if(v){ /*verbose check */
            printf("port is %ld\n", port);
        }

        /*initialize connection*/
        int client_sockfd = setup_connection_client(hostname, port, v, N);

    }
    else{
        printf("Usage: mytalk [-v] [-a] [-N] [ hostname ] port\n");
    }

    return 0;
}
