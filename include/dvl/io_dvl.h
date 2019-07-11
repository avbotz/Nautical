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
