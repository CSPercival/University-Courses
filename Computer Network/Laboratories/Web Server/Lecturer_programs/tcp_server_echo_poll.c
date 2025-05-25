#include <arpa/inet.h>
#include <errno.h>
#include <poll.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>


void ERROR(const char* str)
{
    fprintf(stderr, "%s: %s\n", str, strerror(errno));  // NOLINT(*-err33-c)
    exit(EXIT_FAILURE);
}


// Czeka na dane do odczytu w gnieździe fd co najwyżej max_waiting_time milisekud. 
// Zwraca 1 jeśli dane są, 0 jeśli nastąpił timeout. 
// Ustawia zmienną max_waiting_time na liczbę pozostałych milisekund.

int poll_wrapper (int fd, int* max_waiting_time)
{
    struct timeval clock_before, clock_after, time_elapsed;
    struct pollfd ps;

    gettimeofday(&clock_before, NULL);
    ps.fd = fd;
    ps.events = POLLIN;
    ps.revents = 0;
    int ready = poll(&ps, 1, *max_waiting_time);
    if (ready < 0 || (ready > 0 && ps.revents != POLLIN))
        ERROR("poll error");
    if (ready == 0) {
        *max_waiting_time = 0;
        return 0;
    }

    gettimeofday(&clock_after, NULL);
    timersub(&clock_after, &clock_before, &time_elapsed);
    *max_waiting_time -= (int)(time_elapsed.tv_sec * 1000 + time_elapsed.tv_usec / 1000);
    if (*max_waiting_time < 0)
        *max_waiting_time = 0;

    return 1;
}


// Czyta z gniazda, czekajac co najwyzej timeout milisekund na wystapienie znaku zadanego separatora.
//
// Zwraca 0 jeśli klientowi nie udało się w zadanym czasie wysłać znaku separatora (wystąpił timeout lub klient zamknął
// połączenie lub przeczytano buffer_size znaków). W p.p. zwraca długość ciągu bajtów zawierających separator.
//
// Uwaga: może przeczytac wiecej niż do znaku separatora: te dane zostana zwrócone i usuniete z bufora odbiorczego.

size_t recv_till_separator (int fd, u_int8_t *buffer, size_t buffer_size, int max_waiting_time, char separator)
{
    size_t total_bytes_read = 0;
    while (total_bytes_read < buffer_size) {
        printf("DEBUG: Remaining time in ms = %d\n", max_waiting_time);

        int poll_wrapper_ret = poll_wrapper(fd, &max_waiting_time);
        if (poll_wrapper_ret == 0) {
            printf("DEBUG: Timeout\n");
            return 0;
        }

        ssize_t bytes_read = recv(fd, buffer + total_bytes_read, buffer_size - total_bytes_read, 0);
        if (bytes_read < 0)
            ERROR("recv error");

        if (bytes_read == 0) {
            printf("DEBUG: Connection closed by client\n");
            return 0;
        }

        printf("DEBUG: %ld bytes read\n", bytes_read);
        bool separator_found = memchr(buffer + total_bytes_read, separator, (size_t)bytes_read) != NULL;
        total_bytes_read += (size_t)bytes_read;
        if (separator_found)
            return total_bytes_read;
    }
    return 0;
}


int main()
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
        ERROR("socket error");

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
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

        int connected_sock_fd = accept(sock_fd, NULL, NULL);
        if (connected_sock_fd < 0)
            ERROR("accept error");

        // Czekamy co najwyżej 10 tys. milisekund na ciąg bajtów zawierający znak 'X'.
        size_t bytes_read = recv_till_separator(connected_sock_fd, recv_buffer, BUFFER_SIZE, 10000, 'X');
        if (bytes_read > 0) {
            recv_buffer[bytes_read] = '\0';

            enum { reply_length = 20 };
            u_int8_t reply_buffer[reply_length + 1];
            strncpy((char*)reply_buffer, (char*)recv_buffer, reply_length);
            reply_buffer[reply_length] = '\0';
            printf("%ld bytes read, first %d bytes: '%s'\n", bytes_read, reply_length, (const char*)reply_buffer);

            send(connected_sock_fd, reply_buffer, strlen((char*)reply_buffer), 0);
        }

        if (close (connected_sock_fd) < 0)
            ERROR("close error");
    }
}
