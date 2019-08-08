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

#include <limits.h>
#include <stddef.h>

#include "ahrs/crc_xmodem_generic.h"


#define CRC16CCIT_XMODEM_POLY 0x1021U


uint16_t generic_crc_xmodem_update(uint16_t crc, uint8_t data)
{
	crc ^= (uint16_t)data << 8; // xor in the new data
	
	for (uint_fast8_t i = 8; i--;)
	{
		if (crc & 0x8000U) // is high bit 1?
		{
			crc = (crc << 1) ^ CRC16CCIT_XMODEM_POLY;
		}
		else
		{
			crc <<= 1;
		}
	}

	return crc;
}
