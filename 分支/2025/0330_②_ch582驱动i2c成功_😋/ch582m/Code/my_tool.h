#ifndef __MY_TOOL_H
#define __MY_TOOL_H

    #include "CH59x_common.h"
    #include "my_interface.h"


    #ifndef __Multi__Printf__Definition__
    #define __Multi__Printf__Definition__

        extern uint8_t mp_BUFFER[512];

        void my_print(const char *format, ...);
        char* muti_printf_arrays(int num_arrays, const char* delimiter, const char* format, ...);

    #endif

#endif // !__MY_TOOL_H
