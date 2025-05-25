//cyryl szatan 340510
#ifndef AUX_FILE
#define AUX_FILE

#include <unistd.h>
#include <sys/types.h>
#include <math.h>
void ERROR(const char* str);
void print_as_bytes (unsigned char* buff, ssize_t length);
long long current_time_ms();
int string_to_int(char* ptr);
int cpy_int_to_string(char *dest, int src);
int check_ip_address_correctness(char *ip_address);
int check_port_correctness(int port);
int check_path_correctness(char *path);

struct RequestInformations{
    int raw_len;
    char raw_message[65536];
    struct sockaddr_in sender;
    socklen_t sender_len;
    int correct_data;
    char page[256];
    char host[256];
    int port;
    // char connection[256];
    int connection; // -1 -> close, 1 -> keep-alive
};

extern const int CONST_max_path_len;
extern const int CONST_max_waiting_time;
extern const int CONST_max_packet_len;

#endif