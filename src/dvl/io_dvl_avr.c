#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <assert.h>

#ifdef __STDC_NO_ATOMICS__
#error "stdatomic.h unsupported. If necessary, use of stdatomic can be removed and it can be hacked together with volatile instead."
#endif

#include <stdatomic.h>

#include "dvl/io_dvl.h"
#include "macrodef.h"
#include "dbg.h"

#define NUSART 1
#define BAUD 9600U

static int (*io_dvl_recv_handler)();

static int uart_dvl_putchar(char c, FILE *stream)
{
    (void)stream;
    while (!(BIT_VALUE(CC_XXX(UCSR, NUSART, A), CC_XXX(UDRE, NUSART, ))))
    {
    }
    CC_XXX(UDR, NUSART, ) = c;
    return 0;
}

static int uart_dvl_getchar(FILE *stream)
{
    (void)stream;
    while (!(BIT_VALUE(CC_XXX(UCSR, NUSART, A), CC_XXX(RXC, NUSART, ))))
    {
    }
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

FILE *io_dvl = &(FILE)FDEV_SETUP_STREAM(uart_dvl_putchar, uart_dvl_getchar,
                                        _FDEV_SETUP_RW);

void io_dvl_init(bool (*recv_handler)())
{
    assert(io_dvl_recv_handler);
    io_dvl_recv_handler = recv_handler;
#include <util/setbaud.h> // uses the BAUD macro to define UBRR_VALUE and USE_2X
    CC_XXX(UBRR, NUSART, ) = UBRR_VALUE;
    if (USE_2X)
    {
        SET_BIT_HIGH(CC_XXX(UCSR, NUSART, A), CC_XXX(U2X, NUSART, ));
    }
    SET_BIT_HIGH(CC_XXX(UCSR, NUSART, B), CC_XXX(TXEN, NUSART, )); // enable transmit
    SET_BIT_HIGH(CC_XXX(UCSR, NUSART, B), CC_XXX(RXEN, NUSART, )); // and receive;
    sei();                                                         // enable global interrupts;
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

void io_dvl_recv_begin() {
    atomic_signal_fence(memory_order_acq_rel);
    SET_BIT_HIGH(CC_XXX(UCSR, NUSART, B), CC_XXX(RXCIE, NUSART, ));
    return;
}

void io_dvl_recv_end() {
    SET_BIT_LOW(CC_XXX(UCSR, NUSART, B), CC_XXX(RXCIE, NUSART, ));
    return;
}

static struct {
    uint8_t write           : 2;
    volatile uint8_t clean  : 2;
    uint8_t read            : 2;
    volatile uint8_t new    : 1;
} tripbuf = {0, 1, 2, false};


// Used to be called tripbuf_update_crit() but I felt this was an unclear name
// If new data is present, updates read buffer index to point to new data.

static bool io_dvl_tripbuf_fetch() {
    assert(IN_RANGE(0, tripbuf.write, 2) && IN_RANGE(0, tripbuf.clean, 2) &&
    IN_RANGE(0, tripbuf.read, 2) && IN_RANGE(0, tripbuf.new, 1));
    if (tripbuf.new) {
        tripbuf.new = false;
        uint8_t tmp = tripbuf.read;
        tripbuf.read = tripbuf.clean;
        tripbuf.clean = tmp;
        atomic_signal_fence(memory_order_acquire);
        return true;
    }
    return false;
}

bool io_dvl_tripbuf_update() {
    if (BIT_VALUE(CC_XXX(UCSR, NUSART, B), CC_XXX(RXCIE, NUSART, ))) {
        SET_BIT_LOW(CC_XXX(UCSR, NUSART, B), CC_XXX(RXCIE, NUSART, ));
        bool new_data_status = io_dvl_tripbuf_fetch();
        SET_BIT_HIGH(CC_XXX(UCSR, NUSART, B), CC_XXX(RXCIE, NUSART, ));
        return new_data_status;
    }
    return io_dvl_tripbuf_fetch();
}

void io_dvl_tripbuf_offer() {
    assert(IN_RANGE(0, tripbuf.write, 2) && IN_RANGE(0, tripbuf.clean, 2) &&
    IN_RANGE(0, tripbuf.read, 2) && IN_RANGE(0, tripbuf.new, 1));
    atomic_signal_fence(memory_order_release);
    uint8_t tmp = tripbuf.write;
    tripbuf.write = tripbuf.clean;
    tripbuf.clean = tmp;
    tripbuf.new = true;
    return;
}

uint8_t io_dvl_tripbuf_get_write_idx() {
    return tripbuf.write;
}

uint8_t io_dvl_tripbuf_get_read_idx() {
    return tripbuf.read;
}