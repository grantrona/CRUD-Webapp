#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include <netinet/in.h>
#include<unistd.h>


#define DEFAULT_STRLEN 30000
#define SERVER_PORT  8080

const char *products[] = { "My Product 1", "My Product 2", "My Product 3", "My Product 4"};

void db_handle_client(int clientfd);

void error(const char *msg){

    printf("Error: %s\n",msg);
    exit(1);
}


int main(void)
{
    /* Create Socket*/

    /* Bind socket with IP address and Port*/

    /* Listen for incoming connections */


    /* Accept */
    while(1){

        /* Handle client */
        db_handle_client(clientfd);
    }
    return 0;
}




void db_handle_client(int clientfd)
{
    char outmsg[DEFAULT_STRLEN];
    char inmsg[DEFAULT_STRLEN];


    if(clientfd < 0) {
        printf("Error: Failed to accept client connection\n");
        return;
    }
    /* Read request message from client and print the message*/

    /* Send response to client */


    shutdown(clientfd,SHUT_RDWR);
}
