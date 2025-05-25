//cyryl szatan 340510
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "aux.h"
#include "setup.h"
#include "receive.h"
#include "send.h"

int main(int argc, char** argv)
{
    int sock_fd_main;
    int sock_fd_connected;
    int port_main;
    char path_to_wepages[CONST_max_path_len + 1];

    setup_all(argc, argv, &sock_fd_main, &port_main, path_to_wepages);

    struct RequestInformations request_info;

    while(1){
        wait_for_clients(sock_fd_main, &sock_fd_connected);
        printf("Found client\n");
        
        while(receive_packets(sock_fd_connected, &(request_info)) > 0){
            parse_packet(&request_info);
            analyze_and_reply(sock_fd_connected, &request_info, path_to_wepages);
            if(request_info.connection == -1) break;
        }
        printf("End for now\n");
        if(close(sock_fd_connected) < 0)
            ERROR("close error");
    }
}
