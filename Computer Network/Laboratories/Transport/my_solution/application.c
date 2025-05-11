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

void write_to_file(char *data, int data_length){
    if(data_length <= 0) ERROR("Cannot write 0 or less bytes into file");
    int correct_bytes = fwrite(data, sizeof(char), data_length, GLOBAL_out_file);
    if(correct_bytes != data_length) ERROR("Write Error");
}

void update_window(int idx){
    window.shift++;
    window.dp[idx]->data_length = 0;
    window.dp[idx]->first_byte += CONST_window_size * CONST_data_size;
    window.dp[idx]->received = 0;
    if(window.dp[idx]->first_byte >= GLOBAL_data_length){
        window.read_index++;
        if(window.read_index == CONST_window_size) window.read_index = 0;
        return;
    }
    timer_set(&window.dp[idx]->timer, CONST_timeout);
    timer_start(&window.dp[idx]->timer);
    // window.dp[idx]->timer.time_left = 0;

    struct QueueItem qi;
    qi.first_byte = window.dp[idx]->first_byte;
    qi.idx = idx;
    queue_push_front(&timeout_queue, qi);
    window.read_index++;
    if(window.read_index == CONST_window_size) window.read_index = 0;
}

int pass_data_to_app(int all_analyzed_bytes){
    int idx = window.read_index;
    int bytes_analyzed = 0;
    while(window.dp[idx]->received){
        // printf("PASS bytes nr %d\n", window.dp[idx]->first_byte);
        write_to_file(window.dp[idx]->buffer, window.dp[idx]->data_length);
        bytes_analyzed += window.dp[idx]->data_length;
        printf("Progres: %.3f\n", (double)(all_analyzed_bytes + bytes_analyzed) * 100.0 / (double)GLOBAL_data_length);
        // printf("p ok2\n");
        update_window(idx);
        // printf("p ok3\n");
        idx = window.read_index;
    }
    return bytes_analyzed;
}