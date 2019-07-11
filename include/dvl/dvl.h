/** @file dvl.h
 *  @brief Interface function definitions for DVL.
 *
 *  @author Timothy Kanarsky
 *  @author David Zhang
 */
#ifndef DVL_H
#define DVL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/** @brief Configures proper DVL data components.
 */
void dvl_set_data_format();

/** @brief Tells DVL to begin pinging.
 */
void dvl_begin_pinging();

/** @brief Discards the datagram that is being parsed.
 */
void reset_parser();

/** @brief Replaces old data on the DVL with new data.
 *
 *  @return True on success.
 */
bool dvl_data_update();

/** @brief Finds velocity over bottom towards starboard.
 *
 *  @return Velocity as 32-bit integer in 1um/s.  
 */
int32_t dvl_get_starboard_vel();

/** @brief Finds velocity over bottom towards bow.
 *
 *  @return Velocity as 32-bit integer in 1um/s.  
 */
int32_t dvl_get_forward_vel();

/** @brief Finds velocity over bottom towards surface.
 *
 *  @return Velocity as 32-bit integer in 1um/s.  
 */
int32_t dvl_get_upward_vel();

/** @brief Finds distance to bottom.
 *
 *  Should not be negative. (:P)
 *
 *  @return Distance as 32-bit integer in 0.1mm.
 */
int32_t dvl_get_range_to_bottom();

/** @brief Parses and processes received data.
 *
 *  @return True when a complete datagram has been parsed.
 */
bool dvl_receive_handler();

/** @brief Sends command for DVL to process.
 *
 *  @param cmd The command as a character.
 *  @param wait If true, waits until DVL is finished processing before sending
 *  the command.
 *  @return 1 on success, negative number on error.
 */
int send_command(char* cmd, bool wait);

#ifdef __cplusplus
}
#endif

#endif
