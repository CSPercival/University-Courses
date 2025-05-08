//cyryl szatan 340510
#ifndef MAINTENANCE
#define MAINTENANCE

void update_routing_table(u_int32_t sender, u_int32_t bnet_addr, int mask, u_int32_t distance);
void print_routing_table();
void inspect_last_seen();
void interface_down(u_int32_t bnet_addr);

#endif