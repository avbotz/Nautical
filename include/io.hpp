/** @file io.hpp
 *  @brief IO and kill switch function definitions.
 *
 *  @author David Zhang
 */
#ifndef IO_HPP
#define IO_HPP

#include "motor.hpp"
#include "m5/m5.h"
#include "m5/io_m5.h"

#define DEPTH_PIN A0
#define KILL_PIN 30

/** @brief Starts IO initialization for all major hardware (AHRS, DVL, M5, depth
 *         sensor, kill switch).
 */
void io();

/** @brief Reads kill state from the kill switch.
 *  
 *  @return True on alive.
 */
bool alive();

#endif
