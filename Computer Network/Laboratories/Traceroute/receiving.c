//cyryl szatan 340510
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif				// _DEFAULT_SOURCE

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

#include "aux_func.h"
#include "aux_structs.h"
#include "receiving.h"

void update_replies(struct sockaddr_in *sender, long long receiving_time, int sending_idx, int final_reply, struct icmp_reply_status *replies){
    int idx = replies->reply_ctr;
    inet_ntop(AF_INET, &(sender->sin_addr), replies->ipaddr[idx], sizeof(replies->ipaddr[idx]));
    replies->time[idx] = receiving_time - replies->sending_time[sending_idx];
    replies->reply_ctr++;
    replies->final_reply = final_reply;
}

void extract_icmp_header(u_int8_t *data, struct icmp **icmp_header){
    struct ip* ip_header = (struct ip*) data;
    ssize_t	ip_header_len = 4 * (ssize_t)(ip_header->ip_hl);
    *icmp_header = (struct icmp*) (data + ip_header_len);
}

void interpret_packet(struct sockaddr_in *sender, u_int8_t *data, int ttl, struct icmp_reply_status *replies){
    struct icmp *icmp_header;
    extract_icmp_header(data, &icmp_header);
    if(icmp_header->icmp_type == ICMP_ECHOREPLY){
        if(icmp_header->icmp_id == htons(getpid() & 0xFFFF)){
            update_replies(sender, current_time_ms(), (icmp_header->icmp_seq) % 3, 1, replies);
            return;
        }
    }
    if(icmp_header->icmp_type == ICMP_TIME_EXCEEDED){
        extract_icmp_header(icmp_header->icmp_data, &icmp_header);
        if(icmp_header->icmp_id == htons(getpid() & 0xFFFF) && __builtin_bswap32(((int)icmp_header->icmp_seq) << 16)/3 == (unsigned int)ttl){
            update_replies(sender, current_time_ms(), (icmp_header->icmp_seq) % 3, 0, replies);
            return;
        }
    }
}


void receive_icmp_reply(int sock_fd, int ttl, struct icmp_reply_status *replies){
    struct pollfd ps;
    ps.fd = sock_fd;
    ps.events = POLLIN;
    ps.revents = 0;
    
    int ready;
    int timeout = 1000;
    long long start_time = 0, end_time = 0, elapsed_time = 0;
    
    ssize_t packet_len;
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    u_int8_t buffer[IP_MAXPACKET];
    while(replies->reply_ctr < 3 && timeout > 0){
        start_time = current_time_ms();
        ready = poll(&ps, 1, timeout);
        end_time = current_time_ms();
        elapsed_time = end_time - start_time;
        timeout -= elapsed_time;
        
        if(ready < 0)   ERROR("poll error");
        if(ready == 0)  break;
        if(ps.revents != POLLIN) continue;
        
        while((packet_len = recvfrom(sock_fd, buffer, IP_MAXPACKET, MSG_DONTWAIT, (struct sockaddr*)&sender, &sender_len)) != -1){
            interpret_packet(&sender, buffer, ttl, replies);
        }
        if(errno != EWOULDBLOCK){
            ERROR("recvfrom error");
        }
    }
}
