#include "my_number.h"


void reset_array_INT(int len, int *pointer){
    // int length = sizeof(pointer) / pointer[0];
    int length = len;
    for (size_t i = 0; i < length; i++)
    {
        pointer[i] = 0;
    }
    
}

void reset_array_FLOAT(int len, float *pointer){
    // int length = sizeof(pointer) / pointer[0];
    int length = len;
    for (size_t i = 0; i < length; i++)
    {
        pointer[i] = 0;
    }
    
}


