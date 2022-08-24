#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include <netinet/in.h>
#include<unistd.h>

#define DEFAULT_STRLEN 30000
#define SERVER_PORT  8080

const char *products[] = { "My Product 1", "My Product 2", "My Product 3", "My Product 4"};

/*
 * Defining structs and
 */
struct urlBuilder;
void db_handle_client(int client_fd);
char* construct_url(struct urlBuilder url);


void error(const char *msg){
    printf("Error: %s\n",msg);
    exit(1);
}

struct urlBuilder{
    char *schema;
    char *host;
    int port;
    char *path;
    char *parameter;
} ;


int main(void)
{
    /* Create Socket*/
    int socket_number = 8080;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0){
        error("Socket creation failed");
    }
    printf("Opening socket %d:\n", socket_number);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(socket_number);

    /* Bind socket with IP address and Port*/
    int br = bind(fd, (const struct sockaddr *) &server_address, sizeof(server_address));
    if (br < 0) {
        error("Binding failed!");
    }

    /* Listen for incoming connections */
    listen(fd, SOMAXCONN);
    printf("Listening for requests...\n\n");

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
    char out_msg[DEFAULT_STRLEN];
    char in_msg[DEFAULT_STRLEN];

    if(client_fd < 0) {
        error("Failed to accept client connection\n");
    }
    printf("Connection established:\n");

    /* Read request message from client and print the message*/
    memset(in_msg, 0, DEFAULT_STRLEN);
    ssize_t request = recv(client_fd, in_msg, DEFAULT_STRLEN, 0);
//    printf("%s", in_msg);

    /* Send response to client */
    if (strstr(in_msg, "GET") != NULL) {
        printf("DETECTED GET\n");
    }
    if (strstr(in_msg, "POST") != NULL) {
        printf("DETECTED POST\n");
    }
    if (strstr(in_msg, "DELETE") != NULL) {
        printf("DETECTED DELETE\n");
    }


    shutdown(client_fd,SHUT_RDWR);
}


char* construct_url(struct urlBuilder url) {
    return NULL;
}
