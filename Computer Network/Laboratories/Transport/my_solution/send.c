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
    // printf("create message start\n");
    int message_len = 0;
    memcpy(message, "GET ", 4);
    message_len += 4;
    message_len += cpy_int_to_string(message + message_len, first_byte);
    (*(message + message_len)) = ' ';
    message_len++;
    message_len += cpy_int_to_string(message + message_len, data_len);
    (*(message + message_len)) = '\n';
    message_len++;
    (*(message + message_len)) = 0;
    message_len++;
    // printf("create message end\n");
    return message_len;
}

void send_message(int first_byte_index){
    // printf("send message start\n");
    // printf("sm ok1\n");
    int data_length = CONST_data_size;
    if(GLOBAL_data_length - first_byte_index < data_length) data_length = GLOBAL_data_length - first_byte_index;
    
    // printf("sm ok2\n");

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(GLOBAL_dest_port);
    
    // printf("sm ok3\n");

    char message[19];
    inet_pton(AF_INET, GLOBAL_dest_ip, &server_address.sin_addr);
    ssize_t message_len = 19;
    // printf("sm ok4\n");

    message_len = create_message(message, first_byte_index, data_length);
    // printf("message: %s  #\n",message);
    // printf("sm ok5 message len: %d\n", (int)message_len);

    // ssize_t sent = sendto(sock_fd_send, message, message_len, 0, (struct sockaddr*) &server_address, sizeof(server_address));
    // printf("sendto returned: %zd\n", sent);

    // printf("SENDING: %s", message);

    if (sendto(sock_fd, message, message_len, 0, (struct sockaddr*) &server_address, sizeof(server_address)) != message_len){
        ERROR("sendto error");
    }
    // printf("send message end\n");
}

void check_timers(){
    struct QueueItem qi;
    // printf("check timers start\n");
    while(!queue_empty(&timeout_queue)){
        qi = queue_front(&timeout_queue);
        // printf("ok1\n");
        if(!verify_queue_item(qi) || window.dp[qi.idx]->received){
            // printf("ok2\n");
            queue_pop_front(&timeout_queue);
            // printf("ok3\n");
            continue;
        }
        // printf("ok4\n");
        timer_stop(&window.dp[qi.idx]->timer);
        // printf("ok5\n");
        if(timer_get(&window.dp[qi.idx]->timer) <= 0){
            // printf("ok6\n");
            queue_pop_front(&timeout_queue);
            // printf("ok7\n");
            send_message(qi.first_byte);
            // printf("ok8\n");
            timer_reset(&window.dp[qi.idx]->timer);
            // printf("ok9\n");
            timer_start(&window.dp[qi.idx]->timer);
            // printf("ok10\n");
            queue_push_back(&timeout_queue, qi);
            // printf("ok11\n");
        } else {
            // printf("ok12\n");
            timer_start(&window.dp[qi.idx]->timer);
            break;
        }
        // printf("ok13\n");

    }
}
