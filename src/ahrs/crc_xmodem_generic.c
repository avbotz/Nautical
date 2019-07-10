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
