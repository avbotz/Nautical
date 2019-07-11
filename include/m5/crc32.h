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
