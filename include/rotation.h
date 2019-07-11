#ifndef ROTATION_H
#define ROTATION_H

#include <Arduino.h> 

/** @brief Converts values from body frame of reference to inertial frame of
 *         reference.
 *  
 *  @param input Pointer to where the values of the body frame are.
 *  @param angles Current euler angles.
 *  @param output Pointer to where the values in the inertial frame will be 
 *                stored.
 */
void body_to_inertial(float *input, float *angles, float *output);

#endif 
