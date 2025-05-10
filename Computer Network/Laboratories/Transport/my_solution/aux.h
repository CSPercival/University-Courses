//cyryl szatan 340510
#ifndef AUX_FILE
#define AUX_FILE

#include <unistd.h>
#include <sys/types.h>

struct Timer{
    int timeout;
    int time_left;
    int start_time;
    int end_time;
    int elapsed_time;
    int running;
};

struct DataPacket{
    int first_byte;
    int received;
    struct Timer timer;
    char buffor[CONST_data_size];
};

struct CyclicBuffer{
    int shift;
    int read_index;
    int write_index;
    struct DataPacket *dp;
};

extern char* dest_ip;
extern int dest_port;
extern char* out_file_name;
extern FILE *out_file;
extern int data_length;

extern int sock_fd_send;
extern int sock_fd_list;

extern struct CyclicBuffer window;

extern const int CONST_window_size;
extern const int CONST_data_size;
extern const int CONST_datagram_size;
extern const int CONST_timeout;


void ERROR(const char* str);
void print_as_bytes (unsigned char* buff, ssize_t length);
long long current_time_ms();
int string_to_int(char* ptr);
int check_ip_address_correctness(char *ip_address);
int check_port_correctness(int port);
void timer_set(struct Timer *t, int timeout);
void timer_start(struct Timer *t);
void timer_stop(struct Timer *t);
void timer_reset(struct Timer *t);
int timer_get(struct Timer *t);



// extern const int CONST_stage;
// extern const int CONST_listening_port;
// extern const int CONST_last_seen_lim;
// extern const u_int32_t CONST_unreachable;
// extern const u_int32_t CONST_distance_lim;
#endif