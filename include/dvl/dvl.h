#ifndef DVL_H
#define DVL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

// Configures proper DVL data components (high-res velocity and range to bottom)
void dvl_set_data_format();
// Tells DVL to begin pinging.
void dvl_begin_pinging();
// Discards the datagram that is currently being parsed. 
void reset_parser();

int32_t dvl_data_update();

// Returns signed 32-bit integer, representing velocity over bottom towards starboard in 0.01mm/s.
int32_t dvl_get_starboard_vel();

// Returns signed 32-bit integer, representing velocity over bottom towards bow in 0.01mm/s.
int32_t dvl_get_forward_vel();

// Returns signed 32-bit integer, representing velocity over bottom towards surface in 0.01mm/s.
int32_t dvl_get_upward_vel();

// Returns signed 32-bit integer, representing range from bottom in 0.1mm.
// I'd be worried if this value is ever negative, though.
int32_t dvl_get_range_to_bottom();

// A function that is called whenever the receive interrupt fires. Calls parsing function.
// Returns true when a complete datagram has just been parsed.
bool dvl_receive_handler();

int send_command(char* cmd, bool wait);


#ifdef __cplusplus
}
#endif

#endif

