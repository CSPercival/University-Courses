#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>




void ERROR(const char* str)
{
    fprintf(stderr, "%s: %s\n", str, strerror(errno));  // NOLINT(*-err33-c)
    exit(EXIT_FAILURE);
}


int main()
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
        ERROR("socket error");

    struct sockaddr_in server_address = { 0 };
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(32345);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind (sock_fd, (struct sockaddr*) &server_address, sizeof(server_address)) < 0)
        ERROR("bind error");

    if (listen (sock_fd, 64) < 0)
        ERROR("listen error");

    enum { BUFFER_SIZE = 5000000 };
    u_int8_t recv_buffer[BUFFER_SIZE + 1];

    for (;;) {
        int connected_sock_fd = accept (sock_fd, NULL, NULL);
        if (connected_sock_fd < 0)
            ERROR("accept error");

        ssize_t bytes_read = recv(connected_sock_fd, recv_buffer, BUFFER_SIZE, 0);
        if (bytes_read < 0)
            ERROR("recv error");
        recv_buffer[bytes_read] = '\0';

        enum { reply_length = 500 };
        u_int8_t reply_buffer[reply_length + 1];
        strncpy((char*)reply_buffer, (char*)recv_buffer, reply_length);
        reply_buffer[reply_length] = '\0';
        printf("%ld bytes read, first %d bytes: '%s'\n", bytes_read, reply_length, (const char*)reply_buffer);

        ssize_t bytes_sent = send (connected_sock_fd, reply_buffer, strlen((char*)reply_buffer), 0);
        if (bytes_sent < 0)
            ERROR("send error");

        if (close (connected_sock_fd) < 0)
            ERROR("close error");
    }
}
