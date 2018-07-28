#ifndef DVL_COMMANDS_H
#define DVL_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#define NUM_COMMANDS 10

char* break_command = "===";

char* setup_commands[NUM_COMMANDS] = {
    "CR1\r", // resets DVL to factory settings
    "BX00100\r", // sets max depth to 100dm (33ft)
    "#BJ000110000\r", // enables high-res velocity and range output
    "CF11110\r", // sets auto-ping, binary output
    //"EA????", //TODO: Determine the angle correction based on DVL mount angle
    "ED00010\r", // Depth of transducer - setting to 1 meter because it's pretty irrelevant
#ifdef SALTWATER
    "ES35\r",
#else
    "ES00\r",
#endif
    "EX01011\r",
    "EZ10000010\r",
    "#EU2\r",
    "CK\r"
};

char* ping_command = "cs\r";

#ifdef __cplusplus
}
#endif

#endif