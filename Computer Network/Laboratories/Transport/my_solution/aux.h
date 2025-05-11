//cyryl szatan 340510
#ifndef AUX_FILE
#define AUX_FILE

#include <unistd.h>
#include <sys/types.h>
#include <math.h>

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
    int data_length;
    int received;
    struct Timer timer;
    // char buffer[CONST_data_size];
    char *buffer;
};

struct CyclicBuffer{
    int shift;
    int read_index;
    int write_index;
    struct DataPacket **dp;
};

struct QueueItem{
    int idx;
    int first_byte;
};

struct CyclicQueue{
    int front;
    int rear;
    int size;
    // struct QueueItem items[CONST_max_queue_size];
    struct QueueItem *items;
};

extern char* GLOBAL_dest_ip;
extern int GLOBAL_bin_dest_ip;
extern int GLOBAL_dest_port;
extern char* GLOBAL_out_file_name;
extern FILE *GLOBAL_out_file;
extern int GLOBAL_data_length;

extern int sock_fd;

extern struct CyclicBuffer window;
extern struct CyclicQueue timeout_queue;

extern const int CONST_window_size;
extern const int CONST_data_size;
extern const int CONST_datagram_size;
extern const int CONST_timeout;
extern const int CONST_max_queue_size;


void ERROR(const char* str);
void print_as_bytes (unsigned char* buff, ssize_t length);
long long current_time_ms();
int string_to_int(char* ptr);
int cpy_int_to_string(char *dest, int src);
int check_ip_address_correctness(char *ip_address);
int check_port_correctness(int port);
void timer_set(struct Timer *t, int timeout);
void timer_start(struct Timer *t);
void timer_stop(struct Timer *t);
void timer_reset(struct Timer *t);
int timer_get(struct Timer *t);
void queue_push_front(struct CyclicQueue *q, struct QueueItem item);
void queue_push_back(struct CyclicQueue *q, struct QueueItem item);
void queue_pop_front(struct CyclicQueue *q);
struct QueueItem queue_front(struct CyclicQueue *q);
int queue_empty(struct CyclicQueue *q);
int verify_queue_item(struct QueueItem qi);

#endif