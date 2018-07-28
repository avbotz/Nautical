#ifndef IO_DVL_H
#define IO_DVL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>

void io_dvl_init(bool (*recv_handler)());
void io_dvl_clean();

void io_dvl_recv_begin();
void io_dvl_recv_end();

extern FILE *io_dvl;

bool io_dvl_tripbuf_update();

void io_dvl_tripbuf_offer();

unsigned char io_dvl_tripbuf_get_read_idx();

unsigned char io_dvl_tripbuf_get_write_idx();


#ifdef __cplusplus
}
#endif

#endif
