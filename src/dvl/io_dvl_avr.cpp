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

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __STDC_NO_ATOMICS__
#error "stdatomic.h unsupported. If necessary, use of stdatomic can be removed and it can be hacked together with volatile instead."
#endif

//#include <cstdatomic>

#include "dvl/io_dvl.h"
#include "macrodef.h"
#include "dbg.h"

#define NUSART 1
#define BAUD 9600U


static bool (*io_dvl_recv_handler)();

static int uart_dvl_putchar(char c, FILE *stream)
{
    (void)stream;
    while (!(CC_XXX(UCSR, NUSART, A) & (1U << CC_XXX(UDRE, NUSART, )))) {}
    CC_XXX(UDR, NUSART, ) = c;
    return 0;
}

static int uart_dvl_getchar(FILE *stream)
{
    (void)stream;
    while (!(CC_XXX(UCSR, NUSART, A) & (1U << CC_XXX(RXC, NUSART, )))) {}
    unsigned char const status = CC_XXX(UCSR, NUSART, A);
    unsigned char const data = CC_XXX(UDR, NUSART, );

    assert(!(BIT_VALUE(status, CC_XXX(UPE, NUSART, ))));
    if (BIT_VALUE(status, CC_XXX(FE, NUSART, )))
    {
        DEBUG("Frame Error on usart " STRINGIFY_X(NUSART) ". Out-of-sync or break condition may have occured.");
        return _FDEV_EOF;
    }
    if (BIT_VALUE(status, CC_XXX(DOR, NUSART, )))
    {
        DEBUG("Receive buffer overrun on usart " STRINGIFY_X(NUSART) ". At least one uart frame lost.");
    }
    return data;
}

FILE *io_dvl = fdevopen(uart_dvl_putchar, uart_dvl_getchar);

void io_dvl_init(bool (*recv_handler)())
{
	sei(); // enable global interrupts (they may be already enabled aanyway)
    assert(io_dvl_recv_handler); 
    io_dvl_recv_handler = recv_handler;
#include <util/setbaud.h> // uses the BAUD macro to define UBRR_VALUE and USE_2X
    CC_XXX(UBRR, NUSART, ) = UBRR_VALUE;
    if (USE_2X)
    {
	    CC_XXX(UCSR, NUSART, A) |= ((uint8_t)USE_2X << CC_XXX(U2X, NUSART, ));
    }
	// register defaults are already 8 data, no parity, 1 stop bit
	CC_XXX(UCSR, NUSART, B) = (1U << CC_XXX(TXEN, NUSART, )) |
		(1U << CC_XXX(RXEN, NUSART, )); // enable transmitter and receiver
    return;
}

void io_dvl_clean()
{
    SET_BIT_LOW(CC_XXX(UCSR, NUSART, B), CC_XXX(TXEN, NUSART, )); // disable transmit
    SET_BIT_LOW(CC_XXX(UCSR, NUSART, B), CC_XXX(RXEN, NUSART, )); // and receive;
    return;
}

ISR(CC_XXX(USART, NUSART, _RX_vect))
{
    assert(io_dvl_recv_handler);
    io_dvl_recv_handler();
}

void io_dvl_recv_begin() 
{
	CC_XXX(UCSR, NUSART, B) |= (1U << CC_XXX(RXCIE, NUSART, ));
    return;
}

void io_dvl_recv_end() 
{
	CC_XXX(UCSR, NUSART, B) &= ~(1U << CC_XXX(RXCIE, NUSART, )); // disable Receive Complete Interrupt
    return;
}

static struct {
    uint8_t write;
    volatile uint8_t clean;
    uint8_t read;
    volatile uint8_t new_data;
} tripbuf = { 0, 1, 2, false};


// Used to be called tripbuf_update_crit() but I felt this was an unclear name
// If new data is present, updates read buffer index to point to new data.
static bool io_dvl_tripbuf_fetch() 
{
    assert(IN_RANGE(0, tripbuf.write, 2) && IN_RANGE(0, tripbuf.clean, 2) &&
    IN_RANGE(0, tripbuf.read, 2) && IN_RANGE(0, tripbuf.new_data, 1));
    if (tripbuf.new_data) {
        tripbuf.new_data = false;
        uint8_t tmp = tripbuf.read;
        tripbuf.read = tripbuf.clean;
        tripbuf.clean = tmp;
        return true;
    }
    return false;
}

bool io_dvl_tripbuf_update() 
{
	if (CC_XXX(UCSR, NUSART, B) & (1U << CC_XXX(RXCIE, NUSART, )))
    {
		CC_XXX(UCSR, NUSART, B) &= ~(1U << CC_XXX(RXCIE, NUSART, ));
        bool new_data_status = io_dvl_tripbuf_fetch();
		CC_XXX(UCSR, NUSART, B) |= (1U << CC_XXX(RXCIE, NUSART, ));
        return new_data_status;
    }
    return io_dvl_tripbuf_fetch();
}

void io_dvl_tripbuf_offer() 
{
    assert(IN_RANGE(0, tripbuf.write, 2) && IN_RANGE(0, tripbuf.clean, 2) &&
    IN_RANGE(0, tripbuf.read, 2) && IN_RANGE(0, tripbuf.new_data, 1));
    uint8_t tmp = tripbuf.write;
    tripbuf.write = tripbuf.clean;
    tripbuf.clean = tmp;
    tripbuf.new_data = true;
    return;
}

uint8_t io_dvl_tripbuf_get_write_idx() {
    return tripbuf.write;
}

uint8_t io_dvl_tripbuf_get_read_idx() {
    return tripbuf.read;
}
