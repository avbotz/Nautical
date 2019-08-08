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

/** @file crc_xmodem_generic.h 
 *  @brief CRC update functions for testing.
 *  
 *  @author Seth Girvan (Lord)
 */
#ifndef CRC_XMODEM_GENERIC_H
#define CRC_XMODEM_GENERIC_H

#include <stdint.h>

/** @brief Update a CRC with 8 bits of additional data. 
 *
 *  This is unoptimized and meant to be used for testing on computer. On avr, 
 *  one should use the optimized _crc_xmodem_update() in util/crc16.h
 *
 *  @return New CRC.
 */
uint16_t generic_crc_xmodem_update(uint16_t crc, uint8_t data);

#endif
