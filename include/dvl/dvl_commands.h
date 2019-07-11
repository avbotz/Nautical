/** @file dvl_commands.h
 *  @brief Interface function definitions for DVL.
 *
 *  @author Timothy Kanarsky
 */
#ifndef DVL_COMMANDS_H
#define DVL_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#define NUM_COMMANDS 12

char* break_command = "===";

/** @brief Important setup commands.
 *
 *  CR1 - Resets DVL to factory settings
 *  BX00100 - Sets max depth to 100dm (33ft)
 *  #BJ000110000 - Enables high-res velocity and range output
 *  CF11110 - Sets auto-ping, binary output
 *  EA - Determine angle correction based on DVL mount angle.
 *  EA-04500 - Determine the angle correction based on DVL mount angle.
 *  ED00010 - Depth of transducer, set to 1m because it doesn't matter too much.
 */
char* setup_commands[NUM_COMMANDS] = {
    "CR1\r", 
    "BX00100\r", 
    "#BJ000110000\r", 
    "CF11110\r", 
    //"EA????, 
    "EA-04500\r", 
    "ED00010\r", 
#ifdef SALTWATER
    "ES35\r",
#else
    "ES00\r",
#endif
    "EX01011\r",
    "EZ10000010\r",
    "#EU2\r",
    "PA\r",
    "CK\r"
};

char* ping_command = "cs\r";

#ifdef __cplusplus
}
#endif

#endif
