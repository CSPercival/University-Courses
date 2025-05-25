//cyryl szatan 340510
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <netinet/ip.h>

#include "aux.h"

void parse_packet(struct RequestInformations *req_info){
    char *data_ptr;
    char *raw_ptr;
    //Extract Page name
    raw_ptr = strstr(req_info->raw_message, "GET");
    if(!raw_ptr){
        req_info->correct_data = -1;
        return;
    }
    raw_ptr += 4;
    data_ptr = req_info->page;
    while((*raw_ptr) != ' '){
        (*data_ptr) = (*raw_ptr);
        data_ptr++;
        raw_ptr++;
    }
    (*data_ptr) = 0;

    //Extract host and port name
    raw_ptr = strstr(req_info->raw_message, "Host");
    if(!raw_ptr){
        req_info->correct_data = -1;
        return;
    }
    raw_ptr += 6;
    data_ptr = req_info->host;
    while((*raw_ptr) != ':'){
        (*data_ptr) = (*raw_ptr);
        data_ptr++;
        raw_ptr++;
    }
    (*data_ptr) = 0;

    raw_ptr++;
    int tmp_port = 0;
    while((*raw_ptr) != '\r'){
        tmp_port *= 10;
        tmp_port += (*raw_ptr) - '0';
        raw_ptr++;
    }
    req_info->port = tmp_port;

    //Extract Connection
    raw_ptr = strstr(req_info->raw_message, "keep-alive");
    if(!raw_ptr){
        req_info->connection = -1;
    } else {
        req_info->connection = 1;
    }

    printf("after parsing\nPage: %s\nHost: %s:%d\nconnection: %d\n",req_info->page, req_info->host, req_info->port, req_info->connection);
}

int receive_packets(int sock_fd, struct RequestInformations *req_info){
    struct pollfd ps;
    ps.fd = sock_fd;
    ps.events = POLLIN;
    ps.revents = 0;
    
    int ready;
    ready = poll(&ps, 1, CONST_max_waiting_time);

    if(ready == 0) return ready;
    if(ready < 0 || (ps.revents != POLLIN))   ERROR("poll error");

    req_info->sender_len = sizeof(req_info->sender);
    req_info->connection = 0;
    req_info->correct_data = 0;
    printf("Received packet");
    if((req_info->raw_len = recvfrom(sock_fd, req_info->raw_message, IP_MAXPACKET, MSG_DONTWAIT, (struct sockaddr*)&(req_info->sender), &(req_info->sender_len))) != -1){
        return 1;
    }
    if(errno != EWOULDBLOCK){
        ERROR("recvfrom error");
    }
    return 0;
}

//accept może uzupełnić structure sockaddr, nie wiem czy to potrzebne
void wait_for_clients(int sock_fd_main, int *sock_fd_connect){
    (*sock_fd_connect) = accept(sock_fd_main, NULL, NULL);
    if ((*sock_fd_connect) < 0)
        ERROR("accept error");
}