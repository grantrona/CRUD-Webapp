#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include <netinet/in.h>
#include<unistd.h>

#define DEFAULT_STRLEN 30000
#define SERVER_PORT  8080

const char *products[] = { "My Product 1", "My Product 2", "My Product 3", "My Product 4"};



void db_handle_client(int client_fd);



void error(const char *msg){
    printf("Error: %s\n",msg);
    exit(1);
}



int main(void)
{
    /* Create Socket*/
    int socket_number = 8080;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0){
        printf("Error in socket creation");
        return(-1);
    }
    printf("Opening socket %d:\n", socket_number);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(socket_number);
    printf("Server Initialised\n");

    /* Bind socket with IP address and Port*/
    int br = bind(fd, (const struct sockaddr *) &server_address, sizeof(server_address));
    if (br < 0) {
        printf("Error during binding!");
        return(-1);
    }
    printf("Socket bound to server address\n");

    /* Listen for incoming connections */
    listen(fd, SOMAXCONN);
    printf("Listening for requests...\n");

    /* Accept */
    while(1){
        struct sockaddr_in client_addr;
        int client_addr_len = sizeof(client_addr);
        int client_fd = accept(fd, (struct sockaddr *) &client_addr,
                               (socklen_t *) &client_addr_len);

        /* Handle client */
        db_handle_client(client_fd);
    }

    return 0;
}



void db_handle_client(int client_fd)
{
    char outmsg[DEFAULT_STRLEN];
    char inmsg[DEFAULT_STRLEN];

    if(client_fd < 0) {
        printf("Error: Failed to accept client connection\n");
        return;
    }
    printf("Connection established\n");

    /* Read request message from client and print the message*/

    /* Send response to client */


    shutdown(client_fd,SHUT_RDWR);
}
