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

/** @file crc32.h
 *  @brief Helper functions for CRC.
 *
 *  @author Seth Girvan (Lord)
 */
#ifndef CRC32_H
#define CRC32_H

/** Implements the ANSI X3.66 32 bit cyclic reduncy check.
 */
#include <stdint.h>

/** Pass to crc32_update() as arg crc with the first byte of data.
 */
#define CRC32_INIT_SEED ((uint32_t)0xFFFFFFFF)

/** The CRC32 of any value with its little endian CRC32 appended.
 */
#define CRC32_LE_RESIDUE ((uint32_t)0x2144DF1C)

/** @brief Updates a CRC with a byte of data. 
 *
 *  Wikipedia on CRC:
 *  https://en.wikipedia.org/wiki/Cyclic_Redundancy_Check
 *
 *  @param crc CRC to update. 
 *  @param data Data to update CRC with.
 *  @return New CRC updated with data.
 */
uint32_t crc32_update(uint32_t crc, uint8_t data);

/** @brief Applies a final XOR mask to a CRC. (0xFFFFFFFF)
 *
 *  Should be done after crc32_update() has been called for all data.
 *
 *  @param crc CRC that receives the mask.
 *  @return New CRC after the mask.
 */
uint32_t crc32_final_mask(uint32_t crc);

#endif
