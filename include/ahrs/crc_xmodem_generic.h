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
