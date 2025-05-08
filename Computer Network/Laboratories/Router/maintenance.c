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


void update_routing_entry(struct RoutingEntry *re, u_int32_t distance, u_int32_t sender){
    if(distance > CONST_distance_lim) distance = CONST_unreachable;
    re->last_seen = 0;
    re->distance = distance;
    re->bnext_hop = sender;
    if(inet_ntop(AF_INET, &sender, re->next_hop, 20) == NULL){
        ERROR("conversion error");
    }
}

void add_routing_entry(u_int32_t bnet_addr, int mask, u_int32_t distance, int direct, u_int32_t bnext_hop){
    struct RoutingEntry *re = malloc(sizeof(struct RoutingEntry));
    re->bnet_addr = bnet_addr;
    if(inet_ntop(AF_INET, &bnet_addr, re->net_addr, 20) == NULL){
        ERROR("conversion error");
    }
    re->mask = mask;
    re->distance = distance;
    re->direct = direct;
    re->last_seen = 0;
    if(direct) return;
    re->bnext_hop = bnext_hop;
    if(inet_ntop(AF_INET, &bnext_hop, re->next_hop, 20) == NULL){
        ERROR("conversion error");
    }
    routing.table[routing.n] = re;
    routing.n++;
}

void delete_routing_entry(int idx){
    free(routing.table[idx]);
    routing.table[idx] = routing.table[routing.n - 1];
    routing.n--;
}

void interface_down(u_int32_t bnet_addr){
    for(int i = 0; i < routing.n; i++){
        if(routing.table[i]->bnet_addr != bnet_addr) continue;
        if(routing.table[i]->direct == 1){
            routing.table[i]->direct = 2;
            update_routing_entry(routing.table[i], CONST_unreachable, 0);
        }
        break;
    }
}

void interface_up(u_int32_t bnet_addr, u_int32_t distance){
    for(int i = 0; i < routing.n; i++){
        if(routing.table[i]->bnet_addr != bnet_addr) continue;
        routing.table[i]->last_seen = 0;
        if(routing.table[i]->direct == 2){
            routing.table[i]->direct = 1;
            update_routing_entry(routing.table[i], distance, 0);
        }
        break;
    }
}

void update_routing_table(u_int32_t sender, u_int32_t bnet_addr, int mask, u_int32_t distance){
    for(int i = 0; i < neighbours.n; i++){
        if(neighbours.table[i]->bmy_addr == sender){
            if(neighbours.table[i]->bnet_addr == bnet_addr){
                interface_up(bnet_addr, neighbours.table[i]->dist);
            }
            return;
        }
    }
    
    int new_entry = 1;
    for(int i = 0; i < routing.n; i++){
        if(routing.table[i]->bnet_addr != bnet_addr) continue;
        new_entry = 0;
        if((routing.table[i]->distance >= CONST_unreachable) && (distance >= CONST_unreachable)) break;
        if((routing.table[i]->bnext_hop == sender 
            && routing.table[i]->direct != 1) 
            || routing.table[i]->distance >= distance )
        {   
            update_routing_entry(routing.table[i], distance, sender);
        }
        break;
    }
    if(new_entry){
        add_routing_entry(bnet_addr, mask, distance, 0, sender);
    }
}

void inspect_last_seen(){
    for(int i = 0; i < routing.n; i++){
        routing.table[i]->last_seen++;
        if(routing.table[i]->last_seen > CONST_last_seen_lim){
            if(routing.table[i]->distance == CONST_unreachable){
                if(routing.table[i]->direct){
                    routing.table[i]->last_seen--;
                } else {
                    delete_routing_entry(i);
                }
            } else {
                update_routing_entry(routing.table[i], CONST_unreachable, 0);
            }
        }
    }
}

void print_routing_table(){
    for(int i = 0; i < routing.n; i++){
        printf("%s/%d ", routing.table[i]->net_addr, routing.table[i]->mask);
        if(routing.table[i]->distance == CONST_unreachable){
            printf("unreachable ");
            if(routing.table[i]->direct)
                printf("connected directly\n");
            else 
                printf("\n");
        } else {
            printf("distance %d ", routing.table[i]->distance);
            if(routing.table[i]->direct == 1){
                printf("connected directly\n");
            } else {
                printf("via %s\n", routing.table[i]->next_hop);
            }
        }
        
    }
}