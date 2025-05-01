#ifndef __DATA_MANAGEMENT_H
#define __DATA_MANAGEMENT_H

#include "code.h"
#include "ff.h"

#define First_Cache_Than_Write
#define Cache_Max_Row 80
#define Info_Column 5


typedef struct __DM_Result_Struct
{
    uint8_t Status;
}DM_Result_Struct;

extern DM_Result_Struct DM_Result_default;

// app
DM_Result_Struct DM_Write(float *input_Array, uint8_t previous_index);

// driver


//
int get_row(float arr[][5]);
int get_column(float arr[][5]);


#endif  // __DATA_MANAGEMENT_H
