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

/** @file config.h
 *  @brief Configuration options for Nautical.
 *
 *  @author David Zhang
 */
#ifndef CONFIG_H
#define CONFIG_H 

/*! @name Runtime configuration.
 */
///@{
/** Set to true when in a pool deep enough for the DVL to work (more than 5 ft).
 *
 *  You can set this to true even if Nautical is being compiled on land, but
 *  make sure the kill switch isn't set to alive until the sub is in the water.
 *  Otherwise, the entire program will hang and stop producing output. 
 */
static const bool DVL_ON = true;

/** Set to true when using the initial heading after the sub is unkilled. For
 *  example, if the diver unkills at 274.5 degrees, that will be treated as
 *  north for the sub during the run.
 */
static const bool USE_INITIAL_HEADING = true;

/** This isn't being used at the moment because the ahrs is not producing the
 *  same value after recompiling (not sure if this is supported, or if this is a
 *  bug in the code).
 *
 *  The idea was to have the sub use an absolute value for north rather than one
 *  relative to unkill. Since this would depend less on the diver, it would be
 *  more consistent. 
 */
static const bool FAR = true;

/** Use this when running this code on a personal computer rather than the sub.
 *  It will ignore most of the IO functions, unless you have access to a DVL and
 *  M5 motors (:P).
 */
static const bool SIM = false;
///@}

/*! @name Constants for degrees of freedom with North-East-Down coordinates. 
 *
 *  The degrees of freedom are X, Y, Z, Yaw, Pitch, Roll. The 7th degree of
 *  option is to replace Z (down) with depth from bottom instead.
 */
///@{
/** The number of degrees of freedom.
 */
static const int DOF = 6;

/** The number of linear axes (North, East, Down).
 */
static const int BODY_DOF = 3;

/** The number of angular axes (Yaw, Pitch, Roll) plus the number of linear axes.
 */
static const int GYRO_DOF = 6;

/** Forward, or north, state index.
 */
static const int F = 0;

/** Horizontal, or east, state index.
 */
static const int H = 1;

/** Vertical, or down, state index.
 */
static const int V = 2;

/** Yaw state index.
 */
static const int Y = 3;

/** Pitch state index.
 */
static const int P = 4;

/** Roll state index.
 */
static const int R = 5;

/** Depth from bottom index.
 */
static const int D = 6;
///@}

/*! @name Motor ID configuration.
 */
///@{
/** Thruster enum makes code more readable. Do NOT set an ID to 0, the motor 
 *  will brick.
 */
enum thruster
{
	VERT_FL = 1,
	VERT_FR = 2,
	VERT_BL = 3,
	VERT_BR = 4,
	SURGE_FL = 5,
	SURGE_FR = 6,
	SURGE_BL = 7,
	SURGE_BR = 8,
	NUM_THRUSTERS
};

/** Number of motors.
 */
static const int NUM_MOTORS = 8;
///@}

/*! @name Motor orientation configuration.
 */
/** Rows are motors while columns are directions. Positive numbers are for 
 *  clockwise rotation and negative numbers are for counterclockwise.
 *
 *  The multipliers are either -1, 1, or 0 because they are meant to be
 *  direction only. I do set 1.1 for some motors when the sub is moving foward
 *  because it tends to drift right. If this code ever runs on new sub, which is
 *  supposed to correct for that, then it would be a good idea to change the
 *  values back to 1.
 */
static const float ORIENTATION[8][7] = 
{
	{ -.15, 0.00, 1.00, 0.00, 1.00, -1.0, 1.00 },
	{ 0.15, 0.00, -1.0, 0.00, -1.0, -1.0, -1.0 },
	{ -.15, 0.00, -1.0, 0.00, 1.00, 1.00, -1.0 },
	{ 0.15, 0.00, 1.00, 0.00, -1.0, 1.00, 1.00 },
	{ 1.00, 1.00, 0.00, 1.00, 0.00, 0.00, 0.00 },
	{ -1.1, 1.00, 0.00, 1.00, 0.00, 0.00, 0.00 },
	{ -1.1, 1.00, 0.00, -1.0, 0.00, 0.00, 0.00 }, 
	{ 1.00, 1.00, 0.00, -1.0, 0.00, 0.00, 0.00 }
};

/*! @name PID gains configuration.
 */
/** Rows correspond to F, H, V, Y, P, and R while columns correspond to kp, ki,
 *  and kd. To be honest, these gains are not great. The integral component is
 *  being ignored at the moment because it doesn't work as well in practice. 
 */
static const float GAINS[7][3] = 
{
	{ 2.00, 0.00, 2.50 },
	{ 2.50, 0.00, 2.00 },
	{ 1.75, 0.00, 0.00 },
	{ 0.10, 0.00, 0.05 },
	{ 0.10, 0.00, 0.05 },
	{ 0.10, 0.00, 0.05 },
	{ 0.85, 0.00, 0.10 }
};

/*! @name Conversions.
 */
///@{
#define D2R 3.1415/180.
#define R2D 180./3.1415
///@}

#endif 
