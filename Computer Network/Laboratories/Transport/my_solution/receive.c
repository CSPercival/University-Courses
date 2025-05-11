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
#include "receive.h"

int wait_for_datagrams(int timeout){
    struct pollfd ps;
    ps.fd = sock_fd_list;
    ps.events = POLLIN;
    ps.revents = 0;
    
    int ready;
    ready = poll(&ps, 1, timeout);

    if(ready < 0)   ERROR("poll error");
    if(ps.revents != POLLIN) ready = 0;

    return ready;
}

int parse_datagram(struct sockaddr_in *sender_struct, u_int8_t *data, int *datagram_first_byte_index, int *datagram_data_length, char *datagram_data){
    // int sender = (*((int*)(&sender_struct->sin_addr)));
    u_int32_t sender_ip = (*((u_int32_t*)(&sender_struct->sin_addr)));
    u_int16_t sender_port = (*((u_int16_t*)(&sender_struct->sin_port)));

    if(sender_ip != (u_int32_t)GLOBAL_bin_dest_ip || (int)sender_port != GLOBAL_dest_port) return 0;

    int it = 5;
    (*datagram_first_byte_index) = 0;
    while('0' <= data[it] && data[it] <= '9'){
        (*datagram_first_byte_index) *= 10;
        (*datagram_first_byte_index) += data[it] - '0';
        it++;
    }
    it++;
    (*datagram_data_length) = 0;
    while('0' <= data[it] && data[it] <= '9'){
        (*datagram_data_length) *= 10;
        (*datagram_data_length) += data[it] - '0';
        it++;
    }
    it++;

    memcpy(datagram_data, data + it, (*datagram_data_length));
    return 1;
}

void interpret_datagram(int datagram_first_byte_index, int datagram_data_length, char *datagram_data){
    int window_min_byte = window.shift * CONST_data_size;
    int window_max_byte = window_min_byte + CONST_window_size * CONST_data_size;
    if(datagram_first_byte_index < window_min_byte || window_max_byte <= datagram_first_byte_index) return;

    int dp_idx = (datagram_first_byte_index / CONST_data_size) % CONST_window_size;
    if(window.dp[dp_idx]->first_byte != datagram_first_byte_index) ERROR("Window error");
    if(window.dp[dp_idx]->received) return;
    memcpy(window.dp[dp_idx]->buffer, datagram_data, datagram_data_length);
    window.dp[dp_idx]->received = 1;
}

void receive_datagrams(){
    ssize_t datagram_len;
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    u_int8_t buffer[IP_MAXPACKET];
    char datagram_data[CONST_data_size];
    int datagram_first_byte_index;
    int datagram_data_length;

    int datagram_for_me = 0;
    while((datagram_len = recvfrom(sock_fd_list, buffer, IP_MAXPACKET, MSG_DONTWAIT, (struct sockaddr*)&sender, &sender_len)) != -1){
        datagram_for_me = parse_datagram(&sender, buffer, &datagram_first_byte_index, &datagram_data_length, datagram_data);
        if(datagram_for_me)
            interpret_datagram(datagram_first_byte_index, datagram_data_length,  datagram_data);
    }
    if(errno != EWOULDBLOCK){
        ERROR("recvfrom error");
    }
}
