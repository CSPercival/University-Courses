//cyryl szatan 340510
#ifndef AUX_FILE
#define AUX_FILE

#include <unistd.h>

#include <sys/types.h>

struct Interface{
    char my_addr[20];
    char net_addr[20];
    char brd_addr[20];
    u_int32_t bmy_addr;
    u_int32_t bnet_addr;
    u_int32_t bbrd_addr;
    int submask;
    u_int32_t bmask;
    u_int32_t dist;
};

struct Interfaces{
    int n;
    struct Interface *table[10];
};

struct RoutingEntry{
    char net_addr[20];
    u_int32_t bnet_addr;
    u_int8_t mask;
    u_int32_t distance;
    int direct;         // 0 - not a neighboring network, 1 - neighboring network and direct connection is up, 2 - neighboring network but direct connection is down 
    int last_seen;
    char next_hop[20];  //optional
    u_int32_t bnext_hop;      //optional
};

struct RoutingTable{
    int n;
    struct RoutingEntry *table[10];
};


extern struct Interfaces neighbours;
extern struct RoutingTable routing;
extern const int CONST_stage;
extern const int CONST_listening_port;
extern const int CONST_last_seen_lim;
extern const u_int32_t CONST_unreachable;
extern const u_int32_t CONST_distance_lim;

void ERROR(const char* str);
void print_as_bytes (unsigned char* buff, ssize_t length);
long long current_time_ms();

#endif