#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/ip.h>
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
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0)
        ERROR("socket error");

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family      = AF_INET;
    server_address.sin_port        = htons(32345);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
        ERROR("bind error");

    for (;;) {

        struct sockaddr_in 	sender;
        socklen_t sender_len = sizeof(sender);
        u_int8_t buffer[IP_MAXPACKET+1];

        ssize_t datagram_len = recvfrom(sock_fd, buffer, IP_MAXPACKET, 0, (struct sockaddr*)&sender, &sender_len);
        if (datagram_len < 0)
            ERROR("recvfrom error");

        char sender_ip_str[20];
        inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str));
        printf("Received UDP packet from IP address: %s, port: %d\n", sender_ip_str, ntohs(sender.sin_port));

        buffer[datagram_len] = 0;
        printf("%ld-byte message: +%s+\n", datagram_len, (char*)buffer);

        char* reply = "Thank you!";
        size_t reply_len = strlen(reply);
        if (sendto(sock_fd, reply, reply_len, 0, (struct sockaddr*)&sender, sender_len) != reply_len)
            ERROR("sendto error");

        assert(fflush(stdout) == 0);
    }
}
