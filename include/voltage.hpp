/** @file voltage.hpp
 *  @brief Calculates remaining voltage on batteries.
 *
 *  @author Maxim Vovenko
 */
#ifndef VOLTAGE_HPP
#define VOLTAGE_HPP

#define NUM_SAMPLES 10

/** @brief Calculate voltage left.
 *
 *  I'm not sure how well this function works, it is untested at the moment. It
 *  needs to be calibrated for sure.
 *
 *  @return Remaining voltage.
 */
float voltage();

#endif
