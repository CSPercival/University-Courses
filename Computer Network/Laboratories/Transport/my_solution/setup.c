//cyryl szatan 340510
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/ip.h>
#include <unistd.h>

#include "setup.h"
#include "aux.h"

void check_arguments(int argc, char **argv){
    if(argc != 5){
        ERROR("invalid arguments");
    }
    GLOBAL_dest_ip = argv[1];
    GLOBAL_dest_port = string_to_int(argv[2]);
    GLOBAL_out_file_name = argv[3];
    GLOBAL_data_length = string_to_int(argv[4]);


    if(!check_ip_address_correctness(GLOBAL_dest_ip)) ERROR("invalid ip address");
    if(!check_port_correctness(GLOBAL_dest_port)) ERROR("invalid port");
    if (inet_pton(AF_INET, GLOBAL_dest_ip, &GLOBAL_bin_dest_ip) != 1) {
        ERROR("conversion error");
    }
    // printf("setup %s %d %s %d\n", GLOBAL_dest_ip, GLOBAL_dest_port, GLOBAL_out_file_name, GLOBAL_data_length);
}

void setup_sockets(){
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0)
        ERROR("socket error");

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family      = AF_INET;
    server_address.sin_port        = htons(0);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
        ERROR("bind error");

    // sock_fd_send = socket(AF_INET, SOCK_DGRAM, 0);
    // if (sock_fd_send < 0)
    //     ERROR("socket error");
}

void setup_file(){
    GLOBAL_out_file = fopen(GLOBAL_out_file_name, "w");
    if(GLOBAL_out_file == NULL) ERROR("Could not open output file");
}

void setup_buffer_queue(){
    window.dp = malloc(sizeof(struct DataPacket*) * CONST_window_size);
    window.shift = 0;
    window.write_index = 0;
    window.read_index = 0;
    
    timeout_queue.size = 0;
    timeout_queue.front = 0; 
    timeout_queue.rear = 0; 
    timeout_queue.items = malloc(sizeof(struct QueueItem) * CONST_max_queue_size);
    
    struct QueueItem qi;

    for(int i = 0; i < CONST_window_size; i++){
        window.dp[i] = malloc(sizeof(struct DataPacket));
        window.dp[i]->first_byte = i * CONST_data_size;
        window.dp[i]->received = 0;
        window.dp[i]->data_length = 0;
        window.dp[i]->buffer = malloc(sizeof(char) * CONST_data_size);
        timer_set(&window.dp[i]->timer, CONST_timeout);
        timer_start(&window.dp[i]->timer);

        if(i * CONST_data_size >= GLOBAL_data_length) break;
        
        qi.first_byte = window.dp[i]->first_byte;
        qi.idx = i;
        queue_push_back(&timeout_queue, qi);
    }
}

void setup(int argc, char **argv){
    check_arguments(argc, argv);    
    setup_sockets();
    setup_file();
    setup_buffer_queue();
}