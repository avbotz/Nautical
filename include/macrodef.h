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

/** @file macrodef.h
 *  @brief Helper macros for various parts of Nautical.
 *
 *  @author Seth Girvan (Lord)
 */
#ifndef MACRODEF_H
#define MACRODEF_H

// Cause divide by zero error on non-array types.
#define COUNTOF(a) (sizeof(a)/sizeof(0[a])/((void *)a == (void *)&a))

#define IN_RANGE(min, val, max) (min <= val && val <= max)

#define TRUNC(min, val, max) (val <= min ? min : (val >= max ? max : val))

#define STRINGIFY_N(a) #a

// Macro expands a, then stringifies it.
#define STRINGIFY_X(a) STRINGIFY_N(a)

#define CC_NNN(a, b, c) a ## b ## c

// Macro expands all arguments then concatenates them in argument order, eg:
//
// #define USART_NUM 2
// CC_XXX(UCSR, USART_NUM, A) <--- becomes 'USCR2A'
//
// Allowing us to make our registers parametric. Keep in mind that UCSR and A
// would have to not be macros themselves.
#define CC_XXX(a, b, c) CC_NNN(a, b, c)

// Concatenates the arguments in order, macro expanding only b. dum must be an
// empty argument. eg:
//
// #define USCR foo
// #define A bar
// #define USART_NUM 2
// CC_XXX(USCR, USART_NUM, A) <--- becomes 'foo2bar'
// CC_NXN(USCR, USART_NUM, A, ) <--- becomes 'USCR2A'
#define CC_NXN(a, b, c, dum) CC_NNN(a ## dum, b, c ##dum)

// Sets the `bit`-th bit of `reg` to 1. (beginning from LSB). eg:
// Pretend EXAMPLE_REG is a 1-byte register, initialized as 0000 0000.
// Then SET_BIT_HIGH(EXAMPLE_REG, 3) will set the 3rd bit from LSB to 1, i.e. 0000 0100.
// Same goes for the other macros below.
#define SET_BIT_HIGH(reg, bit) reg |= (1U << bit) 

// Sets the `bit`-th bit of `reg` to 0.
#define SET_BIT_LOW(reg, bit) reg &= ~(1U << bit)

// Gets the value of the `bit`-th bit of `reg`.
#define BIT_VALUE(reg, bit) reg & (1U << bit)

// Toggles `bit`-th bit of `reg`.
#define TOGGLE_BIT(reg, bit) reg ^= (1U << bit)

#endif
