#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>

#define SERVER_PORT  12345
#define TRUE             1
#define FALSE            0

volatile sig_atomic_t end_server = 0; // Global flag for server shutdown

void handle_signal(int sig) {
    end_server = 1; // Set flag to end server on SIGINT (Ctrl+C)
}

int main (int argc, char *argv[]) { // Fixed missing return type (int main)
    int    len, rc, on = 1;
    int    listen_sd = -1, new_sd = -1;
    int    desc_ready, compress_array = FALSE;
    int    close_conn;
    char   buffer[1024]; // Increased buffer size for better handling
    struct sockaddr_in6   addr;
    int    timeout;
    struct pollfd fds[200];
    int    nfds = 1, current_size = 0, i, j;

    // Register signal handler for graceful shutdown
    signal(SIGINT, handle_signal);

    // Create an IPv6 stream socket
    listen_sd = socket(AF_INET6, SOCK_STREAM, 0);
    if (listen_sd < 0) {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }

    // Allow socket descriptor to be reusable
    rc = setsockopt(listen_sd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
    if (rc < 0) {
        perror("setsockopt() failed");
        close(listen_sd);
        exit(EXIT_FAILURE);
    }

    // Set socket to be nonblocking
    rc = ioctl(listen_sd, FIONBIO, (char *)&on);
    if (rc < 0) {
        perror("ioctl() failed");
        close(listen_sd);
        exit(EXIT_FAILURE);
    }

    // Bind the socket
    memset(&addr, 0, sizeof(addr));
    addr.sin6_family      = AF_INET6;
    memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
    addr.sin6_port        = htons(SERVER_PORT);
    rc = bind(listen_sd, (struct sockaddr *)&addr, sizeof(addr));
    if (rc < 0) {
        perror("bind() failed");
        close(listen_sd);
        exit(EXIT_FAILURE);
    }

    // Set the listen backlog
    rc = listen(listen_sd, 32);
    if (rc < 0) {
        perror("listen() failed");
        close(listen_sd);
        exit(EXIT_FAILURE);
    }

    // Initialize the pollfd structure
    memset(fds, 0 , sizeof(fds));
    fds[0].fd = listen_sd;
    fds[0].events = POLLIN;

    // Set a timeout of 3 minutes
    timeout = (3 * 60 * 1000);

    // Main server loop
    while (!end_server) { // Changed loop condition for clean shutdown
        printf("Waiting on poll()...\n");
        rc = poll(fds, nfds, timeout);

        if (rc < 0) {
            perror("poll() failed");
            break;
        }
        if (rc == 0) {
            printf("poll() timed out. Ending server.\n");
            break;
        }

        current_size = nfds;
        for (i = 0; i < current_size; i++) {
            if (fds[i].revents == 0)
                continue;

            if (fds[i].revents & (POLLHUP | POLLERR)) { // Handle errors/disconnections
                printf("Closing connection %d due to error/disconnect\n", fds[i].fd);
                close(fds[i].fd);
                fds[i].fd = -1;
                compress_array = TRUE;
                continue;
            }

            if (fds[i].fd == listen_sd) {
                printf("Listening socket is readable\n");
                do {
                    new_sd = accept(listen_sd, NULL, NULL);
                    if (new_sd < 0) {
                        if (errno != EWOULDBLOCK) {
                            perror("accept() failed");
                            end_server = TRUE;
                        }
                        break;
                    }
                    printf("New incoming connection - %d\n", new_sd);
                    fds[nfds].fd = new_sd;
                    fds[nfds].events = POLLIN;
                    nfds++;
                } while (new_sd != -1);
            } else {
                printf("Descriptor %d is readable\n", fds[i].fd);
                close_conn = FALSE;
                do {
                    rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                    if (rc < 0) {
                        if (errno != EWOULDBLOCK) {
                            perror("recv() failed");
                            close_conn = TRUE;
                        }
                        break;
                    }
                    if (rc == 0) {
                        printf("Connection closed\n");
                        close_conn = TRUE;
                        break;
                    }
                    printf("%d bytes received\n", rc);
                    rc = send(fds[i].fd, buffer, rc, 0);
                    if (rc < 0) {
                        perror("send() failed");
                        close_conn = TRUE;
                        break;
                    }
                } while(TRUE);

                if (close_conn) {
                    close(fds[i].fd);
                    fds[i].fd = -1;
                    compress_array = TRUE;
                }
            }
        }

        if (compress_array) { // Optimize pollfd array
            compress_array = FALSE;
            for (i = 0; i < nfds; i++) {
                if (fds[i].fd == -1) {
                    for (j = i; j < nfds - 1; j++) {
                        fds[j] = fds[j+1];
                    }
                    i--;
                    nfds--;
                }
            }
        }
    }

    // Clean up sockets
    for (i = 0; i < nfds; i++) {
        if(fds[i].fd >= 0)
            close(fds[i].fd);
    }
    printf("Server shutting down.\n");
    return 0;
}

