#include "code.h"
#include "ff.h"

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
