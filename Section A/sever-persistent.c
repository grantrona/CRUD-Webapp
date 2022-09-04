#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include <netinet/in.h>
#include<ctype.h>
#include <time.h>

#define DEFAULT_STRLEN 30000
#define SERVER_PORT  8080
#define DEFAULT_NUM_HEADERS 10
#define MAX_DIGITS 10

const char *products[] = { "My Product 1", "My Product 2", "My Product 3", "My Product 4"};

struct response_builder {
    char *http_ver;
    char *status;
    char *headers[DEFAULT_NUM_HEADERS]; // array of headers
    char *body;
};

/* Define functions */
void db_handle_client(int client_fd);
void send_welcome(int client_fd, char* out_msg);
void handle_get(int client_fd, char *query, char* out_msg);
char* construct_response(struct response_builder *resp, int hasBody);
void send_response(int client_fd, char* response, char* buffer);
void send_bad_request(int client_fd, char* out_msg);
char* get_current_time(char* buffer);


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
    printf("Listening for requests (persistent)...\n\n");

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

/**
 * Handles requests from clients and sent responses accordingly
 * @param client_fd
 */
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
    memset(out_msg, 0, DEFAULT_STRLEN);
    recv(client_fd, in_msg, DEFAULT_STRLEN, 0);
    printf("%s", in_msg);

    // Respond to GET request
    if (strstr(in_msg, "GET /") != NULL) {
        char* query = strchr(in_msg, '/') + 1;
        if (query[0] != ' ') {
            handle_get(client_fd, query, out_msg);
        }
        else {
            send_welcome(client_fd, out_msg);
        }
    } else {
        // sends bad request (400) to any other request message types
        send_bad_request(client_fd, out_msg);
    }

    shutdown(client_fd,SHUT_RDWR); //TODO CURRENTLY NON PERSISTENT!
}

void handle_get(int client_fd, char *query, char* out_msg)
{
    // Initialise memory for new response message to 0
    struct response_builder* resp = calloc(1, sizeof(struct response_builder));
    if (resp == NULL) {
        error("Memory allocation error");
    }
    resp->http_ver = "HTTP/1.1";
    //generate HTTP headers
    char *access_control = "Access-Control-Allow-Origin: *";
    char *conn = "Connection: keep-alive";
    char *content_type = "Content-Type: text/plain; charset=UTF-8";
    char time_buffer[128] = {"Date: "};
    char *date = get_current_time(time_buffer);
    resp->headers[0] = access_control;
    resp->headers[1] = conn;
    resp->headers[2] = content_type;
    resp->headers[3] = date;

    // determine body of the message
    char to_convert[MAX_DIGITS] = {'\0'};
    char* end_of_input = strchr(query, ' ');
    int i = 0;
    for (char* ptr = query; ptr != end_of_input; ptr++) {
        // determine if input from GET is valid
        if (i == MAX_DIGITS - 1 || !isdigit(ptr[0])) {
            resp->status = "400 bad request";
            send_response(client_fd, construct_response(resp, 0), out_msg);
            free(resp);
            return;
        }
        to_convert[i] = ptr[0];
        i++;
    }

    int index = atoi(to_convert);
    if ((sizeof(products)/sizeof(products[0])) < index || index <= 0) {
        resp->status = "404 not found";
        send_response(client_fd, construct_response(resp, 0), out_msg);
    }
    else {
        resp->status = "200 OK";
        char tmp[strlen(products[index -1])];
        strcpy(tmp, products[index - 1]);
        resp->body = tmp;
        send_response(client_fd, construct_response(resp, 1), out_msg);
    }
    free(resp);
}

/**
 * Sends a welcome message (Acts as the website homepage)
 * @param client_fd
 * @param out_msg
 */
void send_welcome(int client_fd, char* out_msg)
{
    // Initialise memory for new response message to 0
    struct response_builder* resp = calloc(1, sizeof(struct response_builder));
    if (resp == NULL) {
        error("Memory allocation error");
    }
    resp->http_ver = "HTTP/1.1";
    resp->status = "200 OK";
    //generate HTTP headers
    char *access_control = "Access-Control-Allow-Origin: *";
    char *conn = "Connection: keep-alive";
    char *content_type = "Content-Type: text/plain; charset=UTF-8";
    char time_buffer[128] = {"Date: "};
    char *date = get_current_time(time_buffer);
    resp->headers[0] = access_control;
    resp->headers[1] = conn;
    resp->headers[2] = content_type;
    resp->headers[3] = date;
    resp->body = "Hello from Server";

    send_response(client_fd, construct_response(resp, 1), out_msg);
    free(resp);
}

/**
 * Composes all response message components as a single string
 * @param resp
 * @return
 */
char* construct_response(struct response_builder *resp, int hasBody)
{
    char *message;
    message = malloc(strlen(resp->http_ver) + 2);
    strcpy(message, resp->http_ver);
    strcat(message, " ");
    strcat(message, "\0");

    char *concat_status = realloc(message, strlen(resp->status) + strlen(message) + 2);
    strcat(concat_status, resp->status);
    strcat(concat_status, "\n");
    strcat(concat_status, "\0");

    char* all_headers = calloc(1, 0);
    int num_headers = sizeof(resp->headers)/sizeof(resp->headers[0]);
    for (int i = 0; i < num_headers; i++) {
        if (resp->headers[i] == NULL) { break; }
        char *header = resp->headers[i];
        all_headers = realloc(all_headers, strlen(header) + strlen(all_headers) + 2);
        strcat(all_headers, header);
        strcat(all_headers, "\n");
        strcat(all_headers, "\0");
    }

    char *concat_headers = realloc(concat_status, strlen(all_headers) + strlen(concat_status) + 2);
    strcat(concat_headers, all_headers);
    strcat(concat_headers, "\n");
    strcat(concat_headers, "\0");
    free(all_headers);

    if (hasBody) {
        char *concat_body = realloc(concat_headers, strlen(resp->body) + strlen(concat_headers) + 2);
        strcat(concat_body, resp->body);
        strcat(concat_body, "\n");
        strcat(concat_body, "\0");

        return concat_body;
    }

    return concat_headers;
}

void send_bad_request(int client_fd, char* out_msg) {
    // Initialise memory for new response message to 0
    struct response_builder* resp = calloc(1, sizeof(struct response_builder));
    if (resp == NULL) {
        error("Memory allocation error");
    }
    resp->http_ver = "HTTP/1.1";
    resp->status = "400 bad request";
    //generate HTTP headers
    char *access_control = "Access-Control-Allow-Origin: *";
    char *conn = "Connection: keep-alive";
    char *content_type = "Content-Type: text/plain; charset=UTF-8";
    char time_buffer[128] = {"Date: "};
    char *date = get_current_time(time_buffer);
    resp->headers[0] = access_control;
    resp->headers[1] = conn;
    resp->headers[2] = content_type;
    resp->headers[3] = date;

    send_response(client_fd, construct_response(resp, 0), out_msg);
    free(resp);
}

void send_response(int client_fd, char* response, char* buffer)
{
    u_long output_len = strlen(response);
    strcpy(buffer, response);

    // Send response to request
    send(client_fd, buffer, output_len, 0);

    // free dynamically allocated memory used to generate response
    free(response);
}

char* get_current_time(char* buffer) {
    time_t raw_time;
    time(&raw_time);
    struct tm *t = localtime(&raw_time);

    strftime(buffer + strlen("Date: "), 126, "%a, %d %m %Y %H:%M:%S GMT", t);


    return buffer;
}