//cyryl szatan 340510
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>

#include "aux.h"
#include "sending.h"
#include "maintenance.h"

void create_message(char *message, uint8_t *net_addr, uint8_t *mask, uint8_t *distance){
    for(int i = 0; i < 4; i++){
        (*message) = (*net_addr);
        message++;
        net_addr++;
    }
    (*message) = (*mask);
    message++;
    for(int i = 0; i < 4; i++){
        (*message) = (*distance);
        message++;
        distance++;
    }
    (*message) = 0;
}

void spread_routing_table(int sock_fd){
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(CONST_listening_port);
    
    char message[10];
    u_int32_t distance = 0;
    for(int i = 0; i < neighbours.n; i++){
        inet_pton(AF_INET, neighbours.table[i]->brd_addr, &server_address.sin_addr);
        for(int j = 0; j < routing.n; j++){
            if(neighbours.table[i]->bnet_addr != routing.table[j]->bnet_addr
                && routing.table[j]->last_seen == CONST_last_seen_lim) continue;
            distance = neighbours.table[i]->dist + routing.table[j]->distance;
            create_message(message, (uint8_t *)(&routing.table[j]->bnet_addr), &routing.table[j]->mask, (uint8_t *)(&distance));
            
            ssize_t message_len = 10;
            if (sendto(sock_fd, message, message_len, 0, (struct sockaddr*) &server_address, sizeof(server_address)) != message_len){
                if(errno == ENETUNREACH){
                    interface_down(neighbours.table[i]->bnet_addr);   // if interface direct -> set to infiinity
                } else {
                    ERROR("sendto error");
                }
            }
        }
    }
}