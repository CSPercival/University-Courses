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
#include "send.h"

int create_message(char *message, int first_byte, int data_len){
    int message_len = 0;
    memcpy(message, "GET ", 4);
    message_len += 4;
    message_len += cpy_int_to_string(message + message_len, first_byte);
    message_len += cpy_int_to_string(message + message_len, data_len);
    (*(message + message_len)) = '\n';
    message_len++;
    return message_len;
}

void send_message(int first_byte_index){
    int data_length = CONST_data_size;
    if(GLOBAL_data_length - first_byte_index < data_length) data_length = GLOBAL_data_length - first_byte_index;
    
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(GLOBAL_dest_port);
    
    char message[19];
    inet_pton(AF_INET, GLOBAL_dest_ip, &server_address.sin_addr);
    ssize_t message_len = 19;
    message_len = create_message(message, first_byte_index, data_length);
    if (sendto(sock_fd_send, message, message_len, 0, (struct sockaddr*) &server_address, sizeof(server_address)) != message_len){
        ERROR("sendto error");
    }
}

void check_timers(){
    struct QueueItem qi;
    while(!queue_empty(&timeout_queue)){
        qi = queue_front(&timeout_queue);
        if(!verify_queue_item(qi)){
            queue_pop_front(&timeout_queue);
            continue;
        }
        timer_stop(&window.dp[qi.idx]->timer);
        if(timer_get(&window.dp[qi.idx]->timer) <= 0){
            queue_pop_front(&timeout_queue);
            send_message(qi.first_byte);
            timer_reset(&window.dp[qi.idx]->timer);
            timer_start(&window.dp[qi.idx]->timer);
            queue_push_back(&timeout_queue, qi);
        } else {
            timer_start(&window.dp[qi.idx]->timer);
            break;
        }
    }
}
