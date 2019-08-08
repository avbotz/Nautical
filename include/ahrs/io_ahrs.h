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

/** @file io_ahrs.h
 *  @brief Low-level communication function definitions for AHRS.
 *  
 *  @author Seth Girvan (Lord)
 */
#ifndef IO_AHRS_H
#define IO_AHRS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>

/** @brief Prepare AHRS to receive data.
 * 
 *  Assumes uart NUSART will be used and connected with the ahrs IEA-232 
 *  interface via a ttl<->IEA-232 converter.
 *
 *  Per the TRAX PNI user manual:
 *  Start Bits: 1
 *  Number of Data Bits: 8
 *  Stop Bits: 1
 *  Parity: none
 *  Baud: BAUD
 *
 *  @param path Path to AHRS from main computer, unused at the moment.
 */
void io_ahrs_init(char const *path);

/** @brief Disable AHRS transmit and receive.
 *
 *  Unimplemented.
 */
void io_ahrs_clean();

/** @brief Tells AHRS to start receiving attitude data.
 *
 *  @param handler Function that handles data received from AHRS.
 *  @return 0 on success
 */
int io_ahrs_recv_start(int (*handler)());

/** @brief Tells AHRS to start receiving attitude data.
 *
 *  @return 0 on success
 */
void io_ahrs_recv_stop();

/** @brief Handles IO to AHRS using file streams.
 *
 *  IO with this is blocking, so one might use normal stdio functions directly
 *  on it when they are willing to wait, eg sending initial configuration data,
 *  but to be able to handle asynchronous io, one should use io_ahrs_..._start,
 *  which allows us to process data on the fly and avoid polling.
 */
extern FILE *io_ahrs;

/** @brief Updates tripbuf with new data.
 *
 *  Causes io_ahrs_tripbuf_read to return index that was most recently
 *  'submitted' by io_ahrs_tripbuf_offer.
 *
 *  This should only be run between complete 'uses' of the data, to avoid using
 *  disparate data.
 *
 *  Unfortunately, since there is no way to have a lock-free/interruptable
 *  triple buffer implementation on the avr, this has to be platform-specific.
 *  A lock-free ring buffer would not handle cases when the producer is faster
 *  than the consumer well.
 *
 *  May be interrupted by io_ahrs_tripbuf_offer, but cannot interrupt it (ie
 *  this can be interrupted by handler_ahrs_recv, but one probably does not want
 *  to call it from handler_ahrs_recv).
 *
 *  @return Whether there has been new data since last call.
 */
bool io_ahrs_tripbuf_update();

/** @brief Allows tripbuf to be updated again. (Might need Seth's confirmation)
 *
 *  Makes the current write index available to io_ahrs_tripbuf_update, and
 *  changes the value returned by io_ahrs_tripbuf_write.
 *
 *  May interrupt io_ahrs_tripbuf_update, but may not be interrupted by it.
 */
void io_ahrs_tripbuf_offer();

/** @brief Finds tripbuf index to read from.
 *
 *  @return The index of the buffer the data consumer should read from. Only
 *          changes if io_ahrs_tripbuf_update is called and returns true.
 */
unsigned char io_ahrs_tripbuf_read();

/** @brief Finds tripbuf index to write to.
 *
 *  @return The index of the buffer the data producer should write to. Only
 *          changes when io_ahrs_tripbuf_offer is called.
 */
unsigned char io_ahrs_tripbuf_write();

#ifdef __cplusplus
}
#endif

#endif
