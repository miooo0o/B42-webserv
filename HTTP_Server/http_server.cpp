#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 12345
#define BUFFER_SIZE 1024  // Increased buffer size for HTTP parsing
#define MAX_CLIENTS 100

void send_http_response(int client_fd, int status_code, const char *content_type, const char *body) {
    char response[BUFFER_SIZE];
    int content_length = strlen(body);
    
    // Construct HTTP response header
    snprintf(response, sizeof(response),
             "HTTP/1.1 %d OK\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %d\r\n"
             "Connection: close\r\n"
             "\r\n%s",
             status_code, content_type, content_length, body);
    
    // Send the response
    send(client_fd, response, strlen(response), 0);
}

void handle_http_request(int client_fd) {
    char buffer[BUFFER_SIZE];
    int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0) {
        perror("recv() failed");
        return;
    }
    buffer[bytes_received] = '\0';  // Null-terminate the received data

    // Check if the request is a GET request
    if (strncmp(buffer, "GET ", 4) == 0) {
        char file_path[256] = "./";
        char *start = buffer + 4;
        char *end = strchr(start, ' ');
        if (end) {
            *end = '\0'; // Terminate string to isolate file path
        }

        // If only "/" is requested, serve "index.html"
        if (strcmp(start, "/") == 0) {
            strcat(file_path, "index.html");
        } else {
            strcat(file_path, start + 1); // Remove leading '/'
        }

        // Try opening the requested file
        FILE *file = fopen(file_path, "r");
        if (file) {
            char file_content[BUFFER_SIZE];
            fread(file_content, 1, sizeof(file_content), file);
            fclose(file);
            send_http_response(client_fd, 200, "text/html", file_content);
        } else {
            send_http_response(client_fd, 404, "text/plain", "404 Not Found");
        }
    } else {
        // Unsupported method response
        send_http_response(client_fd, 400, "text/plain", "400 Bad Request");
    }
}

int main() {
    int listen_sd, new_sd;
    struct sockaddr_in server_addr;
    struct pollfd fds[MAX_CLIENTS];
    int nfds = 1;

    // Create a TCP socket
    listen_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sd < 0) {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }

    // Set socket to reuse address
    int on = 1;
    setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    // Set non-blocking mode
    ioctl(listen_sd, FIONBIO, &on);

    // Bind to port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(listen_sd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind() failed");
        close(listen_sd);
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(listen_sd, 32) < 0) {
        perror("listen() failed");
        close(listen_sd);
        exit(EXIT_FAILURE);
    }

    // Initialize polling structure
    memset(fds, 0, sizeof(fds));
    fds[0].fd = listen_sd;
    fds[0].events = POLLIN;

    printf("HTTP Server listening on port %d...\n", SERVER_PORT);

    while (1) {
        int poll_count = poll(fds, nfds, -1);
        if (poll_count < 0) {
            perror("poll() failed");
            break;
        }

        for (int i = 0; i < nfds; i++) {
            if (fds[i].revents == 0)
                continue;

            if (fds[i].fd == listen_sd) {
                // Accept new connection
                new_sd = accept(listen_sd, NULL, NULL);
                if (new_sd < 0) {
                    if (errno != EWOULDBLOCK) {
                        perror("accept() failed");
                    }
                    break;
                }
                printf("New connection - FD %d\n", new_sd);
                fds[nfds].fd = new_sd;
                fds[nfds].events = POLLIN;
                nfds++;
            } else {
                // Handle HTTP request
                handle_http_request(fds[i].fd);
                close(fds[i].fd);
                fds[i].fd = -1;
            }
        }
    }

    // Cleanup
    for (int i = 0; i < nfds; i++) {
        if (fds[i].fd >= 0) {
            close(fds[i].fd);
        }
    }

    return 0;
}
