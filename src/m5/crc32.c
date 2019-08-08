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

/**
 * The CRC32 is reverse bit order (Most significant bit last), and data bytes
 * are in non-reverse bit order (or reverse relative to the crc).
 */

#include <stdint.h>

#define CRC32_POLY ((uint32_t)0xEDB88320) // reverse polynomial

/**
 * NOTE: An optimization would be to use a lookup-table implementation instead
 * (with the table in flash on the avr).
 */
uint32_t crc32_update(uint32_t crc, uint8_t data)
{
	crc ^= data; // xor in the new data

	for (uint_fast8_t i = 8; i--;)
	{
		if (crc & 1U) // is high bit 1? (reverse bit order)
		{
			crc = (crc >> 1) ^ CRC32_POLY; // note right rather than left shift
		}
		else
		{
			crc >>= 1;
		}
	}

	return crc;
}

uint32_t crc32_final_mask(uint32_t crc)
{
	return 0xFFFFFFFFU ^ crc; // aka ~crc
}
