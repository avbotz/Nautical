/**
 * Description: Handles communication with the PNI TRAX above the level of
 * 'FILE *' streams.
 *
 * The PNI TRAX user manual was used as a reference for this file, and an
 * effort was made to use the terminology used in that document.
 */

#include <assert.h>
#include <string.h>

#ifndef IEEE754
#include <math.h> // to deserialize floats from trax
#endif

#include <stdint.h>
#include <stdio.h>

#include "ahrs/ahrs.h"
#include "ahrs/io_ahrs.h"
#include "ahrs/crc_xmodem.h"
#include "dbg.h"
#include "macrodef.h"


#define BAUD 38400UL

// 2 Byte Count + 1 Frame Id + 1 ID Count + 6 * (1 Component ID + 4 float32) +
// + (1 kHeadingStatus + 1 UInt8) 2 CRC
#define DATAGRAM_BYTECOUNT  38U
#define FRAME_ID 0x05U // kGetDataResp command
// 1 Heading + 1 Pitch + 1 Roll + 1 HeadingStatus (in any order in payload)
#define ID_COUNT  7U
#define CRC_POST_ID_COUNT 0xBB74U // crc of first 4 assumed byte values


float const ahrs_range[NUM_ATT_AXES][2] = {
	[PITCH] = {[COMPONENT_MIN] = -90.f, [COMPONENT_MAX] = 90.f},
	[YAW] = {[COMPONENT_MIN] = 0.f, [COMPONENT_MAX] = 360.f /* Should technically be the next lower float */},
	[ROLL] = {[COMPONENT_MIN] = -180.f, [COMPONENT_MAX] = 180.f}};

// triple buffer coordinated with io_ahrs_tripbuf... functions
static struct ahrs
{
	float att[NUM_ATT_AXES];
	float accel[NUM_ACCEL_AXES];
	uint_fast8_t headingstatus;
} ahrs[3];


float ahrs_att(enum att_axis const dir)
{
	return ahrs[io_ahrs_tripbuf_read()].att[dir];
}

float ahrs_accel(enum accel_axis const dir)
{
	return ahrs[io_ahrs_tripbuf_read()].accel[dir];
}

uint_fast8_t ahrs_headingstatus()
{
	return ahrs[io_ahrs_tripbuf_read()].headingstatus;
}

bool ahrs_att_update()
{
	return io_ahrs_tripbuf_update();
}

static enum
{
	INIT,
	SYNC,
	COMPONENT_ID,
#ifdef IEEE754
	ANGLE,
#else
	ANGLE_BYTE0,
	ANGLE_BYTE1,
	ANGLE_BYTE2,
	ANGLE_BYTE3,
#endif
	HEADINGSTATUS,
	CRC1,
	CRC2
} state = INIT;

/*
 * Stateful coroutine style parsing. I felt stack switching wasn't worth it for
 * this one case. Macros weren't used for the 'state = foo; return; case foo:'
 * blocks because I felt it didn't increase clarity.
 *
 * returns true when a valid attitude data set has just been completely parsed.
 */
static bool parse_att(unsigned char const c)
{
	switch (state)
	{
		case INIT:;
		{
			/* first four bytes of datagram:
			 *
			 * sync[(idx + 1) % 4] == Most significant bytecount byte
			 * sync[(idx + 2) % 4] == Least significant bytecount byte
			 * sync[(idx + 3) % 4] == Frame ID
			 * sync[idx] == Data Components ID Count
			 *
			 * These are fixed for every datagram we can parse. ie a
			 * 'kGetDataResp' packet containing only the three data components
			 * 'kHeading', 'kPitch', and 'kRoll', in any order. Therefore, they
			 * are used to try to synchronize with a datagram in the case the
			 * expected values are not recieved. Even if sychronization is
			 * handled elsewhere (eg based on timing), this seems like  a
			 * decently logical way to handle unexpected values.
			 */
			static unsigned char sync[4];
			memset(sync, 0xFF, 4); // placeholder val equal to no expected val
			static uint_fast8_t idx = 3;
			while ((sync[idx] = c) != ID_COUNT ||
					sync[(idx + 3) % 4] != FRAME_ID ||
					sync[(idx + 2) % 4] != (DATAGRAM_BYTECOUNT & 0x00FF) ||
					sync[(idx + 1) % 4] != DATAGRAM_BYTECOUNT >> 8)
			{
				// This will always loop at least thrice as sync is populated.

				// First four bytes not what expected. Resynchronize assuming
				// the byte just received was the Frame ID.
				idx = (idx + 1) % 4;

				state = SYNC;
				return false;
		case SYNC:;
			}
		}

		static uint16_t crc;
		// no need to compute the crc of the first four bytes at runtime, since
		// their values are already assumed
		crc = CRC_POST_ID_COUNT;

		// Just get the triple buffer write index once, since it can't
		// change until io_ahrs_tripbuf_offer is invoked.
		static unsigned char write_idx;
		write_idx = io_ahrs_tripbuf_write();

		enum component
		{
			KHEADING,
			KPITCH,
			KROLL,
			KACCELX,
			KACCELY,
			KACCELZ,
			KHEADINGSTATUS
		};

		static struct
		{
			unsigned char components : ID_COUNT;
		} comp_is_read;
		comp_is_read.components = 0;

		static uint_fast8_t i;
		for (i = ID_COUNT; i--; crc = crc_xmodem_update(crc, c))
		{
			state = COMPONENT_ID;
			return false;
		case COMPONENT_ID:;

			static float *addr;
			static enum component dir;

			// data components may arrive in arbitrary order
			if (c == 5) // kHeading component
			{
				addr = &ahrs[write_idx].att[YAW];
				dir = KHEADING;
			}
			else if (c == 24) // kPitch component
			{
				addr = &ahrs[write_idx].att[PITCH];
				dir = KPITCH;
			}
			else if (c == 25) // kRoll component
			{
				addr = &ahrs[write_idx].att[ROLL];
				dir = KROLL;
			}
			else if (c == 21) // kAccelX component
			{
				addr = &ahrs[write_idx].accel[SURGE];
				dir = KACCELX;
			}
			else if (c == 22) // kAccelY component
			{
				addr = &ahrs[write_idx].accel[SWAY];
				dir = KACCELY;
			}
			else if (c == 23) // kAccelZ component
			{
				addr = &ahrs[write_idx].accel[HEAVE];
				dir = KACCELZ;
			}
			else if (c == 79) // kHeadingStatus component
			{
				if (comp_is_read.components & (1U << KHEADINGSTATUS))
				{
					// repeat component, fail datagram
					DEBUG("Repeat component.");
					state = INIT;
					return false;
				}
				comp_is_read.components |= 1U << KHEADINGSTATUS;

				crc = crc_xmodem_update(crc, c);

				state = HEADINGSTATUS;
				return false;
		case HEADINGSTATUS:;

				ahrs[write_idx].headingstatus = c;
				continue;
			}
			else // unrecognized component type
			{
				// fail datagram
				DEBUG("Unrecognized component.");
				state = INIT;
				return false;
			}

			if (comp_is_read.components & (1U << dir))
			{
				// component is a repeat, fail datagram
				DEBUG("Repeat component.");
				state = INIT;
				return false;
			}

/* There doesn't seem to be any compiler-defined macros to check for IEEE754
 * format floats. GCC never defines __STD_IEC_559__, since it doesn't conform.
 * avr-gcc uses IEEE754 format little endian floats.
 */
#ifndef IEEE754
			/* The native 'float' must be stored in single precision IEEE754
			 * format.
			 */
#error "Platform must use little endian IEEE754 single precision floats. Define IEEE754 if this is the case."
#endif

#ifndef AVR // avr-libc doesn't seem to support static_assert
			static_assert(sizeof(float) == 4, "float must not be IEEE754. Try compiling without 'IEEE754' defined.");
#endif
			{
				static uint_fast8_t j;
				for (j = 4; j--;)
				{
					crc = crc_xmodem_update(crc, c);
					state = ANGLE;
					return false;
		case ANGLE:;
					// The ahrs transmits floats as big endian by default,
					// while native floats are assumed to be little endian.
					//
					// type-pun float in order to write raw bytes
					((unsigned char *)addr)[j] = c;
				}
			}

			comp_is_read.components |= 1U << dir; // valid value has been read for this dir
		}
		if (!~comp_is_read.components)
		{
			DEBUG("Not all data components received.");
			// fail datagram
			state = INIT;
			return false;
		}

		state = CRC1;
		return false;
		case CRC1:;

		// last two bytes are the crc appended to the data packet
		crc = crc_xmodem_update(crc, c);

		state = CRC2;
		return false;
		case CRC2:;

		// The crc of a value with its crc appended == 0, so the crc is valid
		// if the crc of the entire datagram == 0.
		if (crc_xmodem_update(crc, c) == 0x0000U)
		{
			io_ahrs_tripbuf_offer();
			// Datagram and all attitude data is considered valid
			state = INIT;
			return true;
		}
		else
		{
			// Invalid crc, attitude data will be discarded
			DEBUG("Invalid CRC: 0x%04X", crc_xmodem_update(crc, c));
			state = INIT;
			return false;
		}
	}
	assert(0 /* Should never be reached. */);
}

/*
 * Sets state to be equivalent to what it was the first time parse_att()
 * was run.
 *
 * parse_att() will consider the next byte passed to potentially
 * be the first byte of a datagram.
 */
void ahrs_parse_att_reset()
{
	state = INIT;
	return;
}

int ahrs_att_recv()
{
	int c;
	if ((c = getc(io_ahrs)) == EOF)
	{
		return EOF;
	}
	return parse_att(c);
}

/**
 * writes n bytes from data to io_ahrs, or until error or EOF
 *
 * returns number of bytes written, which equals n if all data was written
 */
static size_t ahrs_write_raw(void const * const datagram, size_t const n)
{
	assert(io_ahrs /* ahrs_io has not been successfully initialized */);

	size_t const nwrit = fwrite(datagram, 1, n, io_ahrs);
	fflush(io_ahrs); // this is probably a null operation on avr
	return nwrit;
}

int ahrs_set_datacomp()
{
	/*
	 * kSave can be issued after setting to make data components are set to
	 * make them persistent, otherwise they reset to last kSave'd values when
	 * ahrs is powered off.
	 *
	 * parse_att() allows them to be in any order
	 */
	static unsigned char const datagram_set_comp[] = {
			0x00, 0x0D, // bytecount
			0x03, // Frame ID: kSetDataComponents
			0x07, // ID Count
			// Component IDs:
			5, // kHeading
			24, // kPitch
			25, // kRoll
			21, // kAccelX
			22, // kAccelY
			23, // kAccelZ
			79, // kHeadingStatus
			0xD5, 0xCE}; // crc
	if (ahrs_write_raw(datagram_set_comp, sizeof(datagram_set_comp)) !=
			sizeof(datagram_set_comp))
	{
		DEBUG("Failed sending kSetDataComponents command.");
		return -1;
	}
	return 0;
}

int ahrs_cont_start()
{
	static unsigned char const datagram_start_cont[] = {
			0x00, 0x05, // bytecount
			0x15, // Frame ID: kStartContinuousMode
			0xBD, 0x61}; // crc

	if (ahrs_write_raw(datagram_start_cont, sizeof(datagram_start_cont)) !=
			sizeof(datagram_start_cont))
	{
		DEBUG("Failed sending kStartContinuousMode command.");
		return -1;
	}
	return 0;
}
