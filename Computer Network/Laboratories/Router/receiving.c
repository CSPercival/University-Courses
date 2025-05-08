//cyryl szatan 340510
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>

#include <netinet/ip_icmp.h>
#include <assert.h>
#include <unistd.h>

#include "aux.h"
#include "maintenance.h"

int wait_for_datagrams(int sock_fd, int *timeout){
    struct pollfd ps;
    ps.fd = sock_fd;
    ps.events = POLLIN;
    ps.revents = 0;
    
    int ready;
    long long start_time = 0, end_time = 0, elapsed_time = 0;
    
    start_time = current_time_ms();
    ready = poll(&ps, 1, (*timeout));
    end_time = current_time_ms();
    elapsed_time = end_time - start_time;
    (*timeout) -= elapsed_time;

    if(ready < 0)   ERROR("poll error");
    if(ps.revents != POLLIN) ready = 0;

    return ready;
}

void interpret_datagram(struct sockaddr_in *sender_struct, u_int8_t *data){
    // int sender = (*((int*)(&sender_struct->sin_addr)));
    u_int32_t sender = (*((u_int32_t*)(&sender_struct->sin_addr)));
    u_int32_t bnet_addr = 0;
    for(int i = 3; i >= 0; i--){
        bnet_addr <<= 8;
        bnet_addr += data[i];
    }
    int mask = data[4];
    u_int32_t distance = 0;
    for(int i = 8; i >= 5; i--){
        distance <<= 8;
        distance += data[i];
    }
    update_routing_table(sender, bnet_addr, mask, distance);
}

void receive_datagrams(int sock_fd){
    
    ssize_t datagram_len;
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    u_int8_t buffer[IP_MAXPACKET];
    
    while((datagram_len = recvfrom(sock_fd, buffer, IP_MAXPACKET, MSG_DONTWAIT, (struct sockaddr*)&sender, &sender_len)) != -1){
        interpret_datagram(&sender, buffer);
    }
    if(errno != EWOULDBLOCK){
        ERROR("recvfrom error");
    }
}
