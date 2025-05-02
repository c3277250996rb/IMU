#ifndef __DATA_MANAGEMENT_H
#define __DATA_MANAGEMENT_H

#define Max_Q_Row 120
#define Max_Q_Column 5

#include "code.h"
#include "ff.h"
#include "quaternion.h"

#define First_Cache_Than_Write
// extern int Max_Q_Row;


typedef struct __DM_Result_Struct
{
    uint8_t Status;
}DM_Result_Struct;

extern DM_Result_Struct DM_Result_default;

// app
void DM_Write(float *input_Array, uint8_t previous_index);
void set_DM_row(int input_row);
// driver


//
int get_row(float arr[][5]);
int get_column(float arr[][5]);


#endif  // __DATA_MANAGEMENT_H
