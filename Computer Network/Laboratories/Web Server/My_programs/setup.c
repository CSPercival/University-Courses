//cyryl szatan 340510
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "aux.h"

void check_arguments(int argv, char **argc, int *port_main, char *path_to_webpages){
    if(argv != 3)
        ERROR("[SETUP] wrong number of arguments");
    
    (*port_main) = string_to_int(argc[1]);
    if(!check_port_correctness((*port_main))) ERROR("[SETUP] invalid port");
    
    // (*path_to_webpages) = argc[2];
    int path_len = strlen(argc[2]);
    if(path_len > CONST_max_path_len) ERROR("[SETUP] given path is too long");
    if(check_path_correctness(argc[2]) != 1) ERROR("[SETUP] invalid webpages path");
    memcpy(path_to_webpages, argc[2], path_len);
    path_to_webpages[path_len] = '/';
    path_to_webpages[path_len + 1] = 0;
}

void setup_sockets(int *sock_fd_main, int *port_main){
    (*sock_fd_main) = socket(AF_INET, SOCK_STREAM, 0);
    if ((*sock_fd_main) < 0)
        ERROR("[SETUP] socket error");

    struct sockaddr_in server_address = { 0 };
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons((*port_main));
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind ((*sock_fd_main), (struct sockaddr*) &server_address, sizeof(server_address)) < 0)
        ERROR("[SETUP] bind error");
    if (listen ((*sock_fd_main), 64) < 0)
        ERROR("[SETUP] listen error");
}

void setup_all(int argv, char **argc, int *sock_fd_main, int *port_main, char *path_to_webpages){
    check_arguments(argv, argc, port_main, path_to_webpages);
    setup_sockets(sock_fd_main, port_main);
}