//cyryl szatan 340510
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/ip.h>
#include <unistd.h>

#include "aux.h"


void split_address(char *comb_addr, char *addr, int *mask){
    (*mask) = 0;
    memset(addr, 0, 20);
    char *cc = comb_addr;
    char *ca = addr;
    int part = 0;
    while((*cc)){
        if((*cc) == '/'){
            part = 1;
            (*ca) = 0;
            cc++;
            continue;
        }
        if(part == 0){
            (*ca) = *(cc);
            ca++;
            cc++;
        } else {
            (*mask) *= 10;
            (*mask) += ((*cc) - '0');
            cc++;
        }
    }
    printf("%s   =  %s + %d\n", comb_addr, addr, (*mask));
    if(part == 0){
        ERROR("splitiong address error");
    }
}

struct Interface *create_interface(char *addr, u_int32_t dist){
    struct Interface *infc = malloc(sizeof(struct Interface));
    
    split_address(addr, infc->my_addr, &(infc->submask));
    // infc->bmask = (1 << infc->submask) - 1;
    int tmp_rest = infc->submask % 8;
    int tmp_eq = infc->submask - tmp_rest;
    infc->bmask = (1 << tmp_eq) - 1;
    for(int i = 1; i <= tmp_rest; i++){
        infc->bmask |= (1 << (tmp_eq + 8 - i));
    }
    // printf("bmask: %u\n", infc->bmask);
    if(inet_pton(AF_INET, infc->my_addr, &(infc->bmy_addr)) != 1){
        ERROR("conversion error");
    }
    infc->bnet_addr = (infc->bmy_addr & infc->bmask);
    infc->bbrd_addr = (infc->bmy_addr | (~infc->bmask));

    printf("bmy: %u  bbrd: %u    bnet: %u\n",infc->bmy_addr, infc->bbrd_addr, infc->bnet_addr);

    if(inet_ntop(AF_INET, &(infc->bnet_addr), infc->net_addr, 20) == NULL){
        ERROR("conversion error");
    }
    if(inet_ntop(AF_INET, &(infc->bbrd_addr), infc->brd_addr, 20) == NULL){
        ERROR("conversion error");
    }

    infc->dist = dist;
    
    return infc;
}

void parse_input(){
    int number_of_interfaces;
    scanf("%d", &number_of_interfaces);

    struct Interface *interface;
    char interface_addr[20];
    int dist;
    for(int i = 0; i < number_of_interfaces; i++){
        scanf(" %s distance %d", interface_addr, &dist);
        interface = create_interface(interface_addr, (u_int32_t)dist);
        neighbours.table[neighbours.n] = interface;
        neighbours.n++;
    }
}

void setup_routing_table(){
    for(int i = 0; i < neighbours.n; i++){
        struct RoutingEntry *re = malloc(sizeof(struct RoutingEntry));
        memcpy(re->net_addr, neighbours.table[i]->net_addr, 20);
        re->bnet_addr = neighbours.table[i]->bnet_addr;
        re->mask = neighbours.table[i]->submask;
        re->direct = 1;
        re->distance = neighbours.table[i]->dist;
        re->last_seen = 0;
        memset(re->next_hop, 0, 20);

        routing.table[routing.n] = re;
        routing.n++;
    }
}

void setup_sockets(int *lis_sock_fd, int *sen_sock_fd){
    (*lis_sock_fd) = socket(AF_INET, SOCK_DGRAM, 0);
    if ((*lis_sock_fd) < 0)
        ERROR("socket error");

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family      = AF_INET;
    server_address.sin_port        = htons(CONST_listening_port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind((*lis_sock_fd), (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
        ERROR("bind error");

    (*sen_sock_fd) = socket(AF_INET, SOCK_DGRAM, 0);
    if ((*sen_sock_fd) < 0)
        ERROR("socket error");
    int brd_enable = 1;
    setsockopt((*sen_sock_fd), SOL_SOCKET, SO_BROADCAST, &brd_enable, sizeof(brd_enable));

}


void setup_router(int *lis_sock_fd, int *sen_sock_fd){
    parse_input();    

    setup_routing_table();

    for(int i = 0; i < neighbours.n; i++){
        printf("my %s   brd %s   net %s   sub %d\n",neighbours.table[i]->my_addr, neighbours.table[i]->brd_addr ,neighbours.table[i]->net_addr, neighbours.table[i]->submask);
        // printf("my %s   brd %s    %s\n",neighbours.table[i]->my_addr, neighbours.table[i]->brd_addr ,neighbours.table[i]->net_addr);
    }

    setup_sockets(lis_sock_fd, sen_sock_fd);
}