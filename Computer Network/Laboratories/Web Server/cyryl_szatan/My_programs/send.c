//cyryl szatan 340510
#define _GNU_SOURCE
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

int create_message(char *message, char *verdict, char *content_type, char *location, char *path){
    FILE *file = fopen(path, "r");
    if(file == NULL) ERROR("fopen error");

    fseek(file, 0, SEEK_END);
    int content_length = ftell(file);
    rewind(file);
    if(strlen(location) > 0){
        if(sprintf(message, "HTTP/1.1 %s\r\nContent-Type: %s\r\nLocation: %s\r\nContent-Length: %d\r\n\r\n",verdict, content_type, location, content_length) == -1)
            ERROR("sprintf error");
    } else {
        if(sprintf(message, "HTTP/1.1 %s\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n",verdict, content_type, content_length) == -1)
            ERROR("sprintf error");
    }

    int header_length = strlen(message);
    fread(message + header_length, 1, content_length, file);
    message[header_length + content_length] = 0;
    fclose(file);
    return header_length + content_length;
}

int create501(char *path_to_webpages, char *message){
    printf("create501\n");
    char path_to_file[CONST_max_path_len];
    strcpy(path_to_file, path_to_webpages);
    strcat(path_to_file, "errors/501.html");
    return create_message(message, "501 Not Implemented", "text/html; charset=UTF-8","", path_to_file);
}

int create403(char *path_to_webpages, char *message){
    printf("create403\n");
    char path_to_file[CONST_max_path_len];
    strcpy(path_to_file, path_to_webpages);
    strcat(path_to_file, "errors/403.html");
    return create_message(message, "403 Forbidden", "text/html; charset=UTF-8","", path_to_file);
}

int create404(char *path_to_webpages, char *message){
    printf("create404\n");
    char path_to_file[CONST_max_path_len];
    strcpy(path_to_file, path_to_webpages);
    strcat(path_to_file, "errors/404.html");
    return create_message(message, "404 Not Found", "text/html; charset=UTF-8","", path_to_file);
}

int create301(char *path_to_webpages, char *message, char *location){
    printf("create301\n");
    char path_to_file[CONST_max_path_len];
    strcpy(path_to_file, path_to_webpages);
    strcat(path_to_file, "errors/301.html");
    return create_message(message, "301 Moved Permanently", "text/html; charset=UTF-8", location, path_to_file);
}

int create200(char *path_to_file, char *message){
    printf("create200\n");
    char *content_type;
    char *last_dot = strrchr(path_to_file, '.');
    if(last_dot == NULL){ 
        content_type = "application/octet-stream";
    }
    else if(strcmp(last_dot,".txt") == 0){
        content_type = "text/plain; charset=utf-8";
    }   
    else if(strcmp(last_dot,".html") == 0){
        content_type = "text/html; charset=utf-8";
    }
    else if(strcmp(last_dot,".css") == 0){
        content_type = "text/css; charset=utf-8";
    }
    else if(strcmp(last_dot,".jpg") == 0 || strcmp(last_dot,".jpeg") == 0){
        content_type = "image/jpeg";
    }
    else if(strcmp(last_dot,".png") == 0){
        content_type = "image/png";
    }
    else if(strcmp(last_dot,".pdf") == 0){
        content_type = "application/pdf";
    }
    return create_message(message, "200 OK", content_type, "", path_to_file);
}


int analyze_and_prepare_message(char *message, struct RequestInformations *req_info, char *path_to_webpages){
    // int message_len = 0;
    printf("prepare to send\n");
    if(req_info->correct_data == -1){
        return create501(path_to_webpages, message);   
    }
    char path_to_file[CONST_max_path_len];
    char real_path_to_file[CONST_max_path_len];
    char path_to_host[CONST_max_path_len];
    char real_path_to_host[CONST_max_path_len];

    strcpy(path_to_file, path_to_webpages);
    strcat(path_to_file, req_info->host);
    strcat(path_to_file, req_info->page);
    
    strcpy(path_to_host, path_to_webpages);
    strcat(path_to_host, req_info->host);

    realpath(path_to_file, real_path_to_file);
    realpath(path_to_host, real_path_to_host);
    
    // printf("path to file: %s\n", path_to_file);
    printf("real path to file: %s\n", real_path_to_file);
    // printf("path to host: %s\n",path_to_host);
    printf("real path to host: %s\n",real_path_to_host);

    char *tmp_ptr = strstr(real_path_to_file, real_path_to_host);
    if(tmp_ptr == NULL){
        return create403(path_to_webpages, message);
    }

    switch(check_path_correctness(real_path_to_file)){
        case 0:
            return create404(path_to_webpages, message);
        case 1:
            return create301(path_to_webpages, message, strcat(req_info->page, "index.html"));
        case 2:
            return create200(real_path_to_file, message);
        default:
            ERROR("path correctness error");
    }
    return 0;
}

void analyze_and_reply(int sock_fd, struct RequestInformations *req_info, char *path_to_webpages){
    printf("before sending\n");
    char message[CONST_max_packet_len + 1];
    int message_len = 0;
    message_len = analyze_and_prepare_message(message, req_info, path_to_webpages);
    printf("message prepared\n");
    int bytes_sent = 0;
    if((bytes_sent = send(sock_fd, message, message_len, 0)) < message_len){
        ERROR("send error");
    }
    printf("message sent\n");
}