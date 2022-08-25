#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include <netinet/in.h>
#include<unistd.h>

#define DEFAULT_STRLEN 30000
#define SERVER_PORT  8080

const char *products[] = { "My Product 1", "My Product 2", "My Product 3", "My Product 4"};

struct response_builder{
    char *http_ver;
    char *status;
    char *headers[10]; // array of headers
    char *body;
} ;

/* Define functions */
void db_handle_client(int client_fd);
char* construct_response(struct response_builder resp);


void error(const char *msg){
    printf("Error: %s\n",msg);
    exit(1);
}


int main(void)
{
    /* Create Socket*/
    int socket_number = SERVER_PORT;
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
    // length of the output message
    u_long output_len = 0;

    if(client_fd < 0) {
        error("Failed to accept client connection\n");
    }
    printf("Connection established:\n");

    /* Read request message from client and print the message*/
    memset(in_msg, 0, DEFAULT_STRLEN);
    recv(client_fd, in_msg, DEFAULT_STRLEN, 0);
    printf("%s", in_msg);

    /* Send response to client */
    if (strstr(in_msg, "GET /") != NULL) {
        // Generate response message
        struct response_builder resp;
        resp.http_ver = "HTTP/1.0";
        resp.status = "200 OK";
        char *content_type = "Content-Type: text/plain; charset=UTF-8";
        char *conn = "Connection: close";
        resp.headers[0] = content_type;
        resp.headers[1] = conn;
        resp.body = "Hello from Server";

        // Concatenate components into a single char array
        char *response = (char *) construct_response(resp);

        output_len = strlen(response);
        strcpy(out_msg, response);

        // free dynamically allocated memory used to generate response
        free(response);
    }
    if (strstr(in_msg, "POST /") != NULL) {
        printf("DETECTED POST\n");
    }
    if (strstr(in_msg, "DELETE /") != NULL) {
        printf("DETECTED DELETE\n");
    }

    // Send response to request
    send(client_fd, out_msg, output_len, 0);

    //TODO CURRENTLY NON PERSISTENT!
    shutdown(client_fd,SHUT_RDWR);
}

char* construct_response(struct response_builder resp) {
    char *message;
    message = malloc(strlen(resp.http_ver) + 2);
    strcpy(message, resp.http_ver);
    strcat(message, " ");
    strcat(message, "\0");

    char *concat_status = realloc(message, strlen(resp.status) + strlen(message) + 2);
    strcat(concat_status, resp.status);
    strcat(concat_status, "\n");
    strcat(concat_status, "\0");

    char* all_headers = malloc(0);
    int num_headers = sizeof(resp.headers)/sizeof(resp.headers[0]);
    for (int i = 0; i < num_headers; i++) {
        if (resp.headers[i] == NULL) { break; }
        char *header = resp.headers[i];
        all_headers = realloc(all_headers, strlen(header) + strlen(all_headers) + 2);
        strcat(all_headers, header);
        strcat(all_headers, "\n");
        strcat(all_headers, "\0");
    }

    char *concat_headers = realloc(concat_status, strlen(all_headers) + strlen(concat_status) + 2);
    strcat(concat_headers, all_headers);
    strcat(concat_headers, "\n");
    strcat(concat_headers, "\0");

    char *concat_body = realloc(concat_headers, strlen(resp.body) + strlen(concat_headers) + 2);
    strcat(concat_body, resp.body);
    strcat(concat_body, "\n");
    strcat(concat_body, "\0");

    return concat_body;
}
