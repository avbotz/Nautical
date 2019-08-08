/*
 * MIT License
 * 
 * Copyright (c) 2019 AVBotz
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * ========================================================================== */

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
 *              the command.
 *  @return 1 on success, negative number on error.
 */
int send_command(char* cmd, bool wait);

#ifdef __cplusplus
}
#endif

#endif
