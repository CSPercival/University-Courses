//cyryl szatan 340510
#ifndef AUX_STRUCTS
#define AUX_STRUCTS

struct icmp_reply_status{
    int reply_ctr;
    char ipaddr[3][16];         // i-th number is related to the i-th packet in order of receiving
    int time[3];                // i-th number is related to the i-th packet in order of receiving
    long long sending_time[3];  // i-th number is related to the i-th packet in order of sending
    int final_reply;
};

#endif