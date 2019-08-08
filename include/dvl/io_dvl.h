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

/** @file io_dvl.h
 *  @brief Low-level communication function definitions for DVL.
 *
 *  If attempting to understand DVL documentation, it is a good idea to look 
 *  under AHRS as well, as the two are similar.
 *
 *  @author Timothy Kanarsky
 */
#ifndef IO_DVL_H
#define IO_DVL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>

/** @brief Prepare DVL to receive data.
 *
 *  @param recv_handler Function that handles data received from DVL.
 */
void io_dvl_init(bool (*recv_handler)());

/** @brief Disable transmitting and receiving.
 */
void io_dvl_clean();

/** @brief Tell DVL to begin sending data for us to receive.
 */
void io_dvl_recv_begin();

/** @brief Tell DVL to stop sending data.
 */
void io_dvl_recv_end();

/** @brief Handles communication to DVL using file stream format.
 */
extern FILE *io_dvl;

/** @brief Updates tripbuf with new data if present.
 *
 *  @return Whether there has been new data since the last call.
 */
bool io_dvl_tripbuf_update();

/** @brief Allows tripbuf to be written to again.
 */
void io_dvl_tripbuf_offer();

/** @brief Finds tripbuf index to read from.
 *
 *  @return The index to read from.
 */
unsigned char io_dvl_tripbuf_get_read_idx();

/** @brief Finds tripbuf index to write to.
 *
 *  @return The index to write from.
 */
unsigned char io_dvl_tripbuf_get_write_idx();

#ifdef __cplusplus
}
#endif

#endif
