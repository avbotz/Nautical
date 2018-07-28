#include <assert.h>
#include <string.h>
#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>

#include "dvl/dvl.h"
#include "dvl/io_dvl.h"
#include "dvl/dvl_commands.h"
#include "dbg.h"
#include "macrodef.h"

#define BAUD 9600U;
#define DATAGRAM_ID 0x7f
#define HEADER_ID 0x7f
#define VELOCITY_FRAME_ID 0x0358
#define RANGE_FRAME_ID 0x0458

static bool velocity_good(int32_t const velocity)
{
    return (-500000 < velocity && velocity < 500000); //We don't expect our sub to go faster than
}

static struct dvl_data
{
    int32_t velocity_starboard;
    int32_t velocity_forward;
    int32_t velocity_upward;
    int32_t range_to_bottom;
} dvl_data[3];

int32_t dvl_get_forward_vel()
{
    return dvl_data[io_dvl_tripbuf_get_read_idx()].velocity_forward;
}

int32_t dvl_get_starboard_vel()
{
    return dvl_data[io_dvl_tripbuf_get_read_idx()].velocity_starboard;
}

int32_t dvl_get_upward_vel()
{
    return dvl_data[io_dvl_tripbuf_get_read_idx()].velocity_upward;
}

int32_t dvl_get_range_to_bottom()
{
    return dvl_data[io_dvl_tripbuf_get_read_idx()].range_to_bottom;
}

volatile int32_t updates = 0;

typedef enum
{
    COMMAND_READY,
    COMMAND_PROCESSING,
    PINGING,
} cmd_state_t;

typedef enum
{
    SYNC,
    DATA_ID,
    SIZE_BYTE_1,
    SIZE_BYTE_2,
    SPARE,
    FRAME_COUNT,
    FRAME_OFFSET_LOOP,
    WAIT,
    FRAME_ID_BYTE_2,
    VELOCITY_DATA_SB,
    VELOCITY_DATA_FW,
    VELOCITY_DATA_UP,
    RANGE_DATA,
} rx_state_t;

static volatile cmd_state_t cmd_state = PINGING;

static struct parser_var_t
{
    rx_state_t rx_state;
    uint16_t datagram_bytecount;
    uint8_t num_data_types;
    uint16_t offsets[4]; // Bad idea to hardcode number of frames in datagram but it'll have to do (short of std::vector)
    uint8_t offset_loop_idx;
    uint16_t rx_wait_bytecount;
    uint8_t offset_wait_idx;
    uint16_t frame_id;
    uint8_t velocity_sb_byte_idx;
    uint8_t velocity_fw_byte_idx;
    uint8_t velocity_up_byte_idx;
    uint8_t range_byte_idx;
} parser_vars; // Structure that houses parser state variables (I didn't want to declare static variables all over the place)

static void init_parser()
{
    parser_vars.rx_state = SYNC;
}

static const struct parser_var_t blank;

void reset_parser()
{
    parser_vars = blank;
    parser_vars.rx_state = SYNC;
}

int32_t dvl_data_update()
{
    io_dvl_tripbuf_update();
    return updates;
}

static bool parse_velocities(unsigned char const c)
{
    switch (parser_vars.rx_state)
    {
    case SYNC:; // Semicolon is necessary because if next line is a declaration, C goes REEEEE
        if (c == DATAGRAM_ID)
            parser_vars.rx_state = DATA_ID;
        return false;

    case DATA_ID:;
        if (c == HEADER_ID)
            parser_vars.rx_state = SIZE_BYTE_1;
        else
            parser_vars.rx_state = SYNC;
        return false;

    case SIZE_BYTE_1:;
        parser_vars.datagram_bytecount = (uint16_t)(c);
        parser_vars.rx_state = SIZE_BYTE_2;
        return false;

    case SIZE_BYTE_2:;
        parser_vars.datagram_bytecount |= (((uint16_t)(c)) << 8);
        parser_vars.rx_state = SPARE;
        return false;

    case SPARE:;
        parser_vars.rx_state = FRAME_COUNT;
        return false;

    case FRAME_COUNT:;
        parser_vars.num_data_types = c;
        parser_vars.rx_state = FRAME_OFFSET_LOOP;
        return false;

    case FRAME_OFFSET_LOOP:;
        if (!(parser_vars.offset_loop_idx % 2))
        {
            parser_vars.offsets[(parser_vars.offset_loop_idx / 2)] = (uint16_t)(c);
        }
        else
        {
            parser_vars.offsets[(parser_vars.offset_loop_idx / 2)] |= ((uint16_t)(c) << 8);
        }
        if (parser_vars.offset_loop_idx == (parser_vars.num_data_types * 2) - 1)
        {
            parser_vars.rx_state = WAIT;
            parser_vars.rx_wait_bytecount = 6 + (2 * parser_vars.num_data_types);
            return false;
        }
        parser_vars.offset_loop_idx++;
        return false;

    case WAIT:;
        if (parser_vars.offset_wait_idx >= parser_vars.num_data_types)
        {
            parser_vars.rx_state = SYNC;
            return false;
        }
        if (parser_vars.rx_wait_bytecount == parser_vars.offsets[parser_vars.offset_wait_idx])
        {
            parser_vars.frame_id = (((uint16_t)(c)) << 8);
            parser_vars.rx_state = FRAME_ID_BYTE_2;
            parser_vars.offset_wait_idx++;
        }
        parser_vars.rx_wait_bytecount++;
        return false;

    case FRAME_ID_BYTE_2:;
        parser_vars.frame_id |= (uint16_t)(c);
        parser_vars.rx_wait_bytecount++;
        if (parser_vars.frame_id == 0x0358)
        {
            parser_vars.rx_state = VELOCITY_DATA_SB;
            return false;
        }

        else if (parser_vars.frame_id == 0x0458)
        {
            parser_vars.rx_state = RANGE_DATA;
            return 1;
        }
        else
        {
            parser_vars.rx_state = WAIT;
            return 1;
        }

    case VELOCITY_DATA_SB:;
        switch (parser_vars.velocity_sb_byte_idx)
        {

        case 0:;
            dvl_data[io_dvl_tripbuf_get_write_idx()].velocity_starboard = (int32_t)c;
            break;
        case 1:;
            dvl_data[io_dvl_tripbuf_get_write_idx()].velocity_starboard |= ((int32_t)c << 8);
            break;
        case 2:;
            dvl_data[io_dvl_tripbuf_get_write_idx()].velocity_starboard |= ((int32_t)c << 16);
            break;
        case 3:;
            dvl_data[io_dvl_tripbuf_get_write_idx()].velocity_starboard |= ((int32_t)c << 24);
            parser_vars.rx_state = VELOCITY_DATA_FW;
            parser_vars.rx_wait_bytecount += 4;
            break;
        }
        parser_vars.velocity_sb_byte_idx++;
        return false;

    case VELOCITY_DATA_FW:;
        switch (parser_vars.velocity_fw_byte_idx)
        {
        case 0:;
            dvl_data[io_dvl_tripbuf_get_write_idx()].velocity_forward = (int32_t)c;
            break;
        case 1:;
            dvl_data[io_dvl_tripbuf_get_write_idx()].velocity_forward |= ((int32_t)c << 8);
            break;
        case 2:;
            dvl_data[io_dvl_tripbuf_get_write_idx()].velocity_forward |= ((int32_t)c << 16);
            break;
        case 3:;
            dvl_data[io_dvl_tripbuf_get_write_idx()].velocity_forward |= ((int32_t)c << 24);
            parser_vars.rx_state = VELOCITY_DATA_UP;
            parser_vars.rx_wait_bytecount += 4;
            break;
        }
        parser_vars.velocity_fw_byte_idx++;
        return false;

    case VELOCITY_DATA_UP:;
        switch (parser_vars.velocity_up_byte_idx)
        {
        case 0:;
            dvl_data[io_dvl_tripbuf_get_write_idx()].velocity_upward = (int32_t)c;
            break;
        case 1:;
            dvl_data[io_dvl_tripbuf_get_write_idx()].velocity_upward |= ((int32_t)c << 8);
            break;
        case 2:;
            dvl_data[io_dvl_tripbuf_get_write_idx()].velocity_upward |= ((int32_t)c << 16);
            break;
        case 3:;
            dvl_data[io_dvl_tripbuf_get_write_idx()].velocity_upward |= ((int32_t)c << 24);
            parser_vars.rx_state = WAIT;
            parser_vars.rx_wait_bytecount += 4;
            break;
        }
        parser_vars.velocity_up_byte_idx++;
        return false;

    case RANGE_DATA:;
        switch (parser_vars.range_byte_idx)
        {
        case 0:;
            dvl_data[io_dvl_tripbuf_get_write_idx()].range_to_bottom = (int32_t)c;
            break;
        case 1:;
            dvl_data[io_dvl_tripbuf_get_write_idx()].range_to_bottom |= ((int32_t)c << 8);
            break;
        case 2:;
            dvl_data[io_dvl_tripbuf_get_write_idx()].range_to_bottom |= ((int32_t)c << 16);
            break;
        case 3:;
            dvl_data[io_dvl_tripbuf_get_write_idx()].range_to_bottom |= ((int32_t)c << 24);
            io_dvl_tripbuf_offer();
            reset_parser();
            updates++;
            return true;
        }
        parser_vars.range_byte_idx++;
        return false;
    }
    assert(0); // Should never get here
}

bool dvl_receive_handler()
{
    assert(io_dvl);
    uint8_t c = getc(io_dvl);
    if (cmd_state == PINGING)
    { // If we know that the DVL is pinging right now, pass the character to the parser.
        return parse_velocities(c);
    }
    else if (cmd_state == COMMAND_PROCESSING)
    { // If we know that the DVL is in command mode right now, ignore character but listen to beginning
        Serial.print((char)c);
        if (c == '>')
        { // Once a command is received by the DVL, it prints a newline and the command line symbol. We can now send another command.
            cmd_state = COMMAND_READY;
        }
    }
    else if (cmd_state == COMMAND_READY)
    { // If we're sending a command, then the output of the DVL is irrelevant (it echoes characters back)
        Serial.print((char)c);
        return false;
    }
    else if (c == EOF)
    {
        return EOF;
    }
    assert(0); // Should never get here
}

int send_command(char *cmd, bool wait)
{
    int status = fprintf(io_dvl, cmd);
    fflush(io_dvl);
    cmd_state = COMMAND_PROCESSING;
    if (wait)
        while (cmd_state != COMMAND_READY)
        {
        } // Wait for the DVL to print out welcome message, etc until command prompt is shown.
    return status;
}

static void send_break()
{
    send_command("===", true);
}

void dvl_set_data_format()
{
    send_break();
    for (size_t i = 0; i < NUM_COMMANDS; i++)
    {
        send_command(setup_commands[i], true);
    }
}

void dvl_begin_pinging()
{
    reset_parser();
    send_command(ping_command, false);
    cmd_state = PINGING;
}
