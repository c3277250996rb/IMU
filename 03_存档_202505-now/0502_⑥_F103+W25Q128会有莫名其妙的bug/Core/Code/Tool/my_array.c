#include "my_array.h"



void reset_array_INT(int *pointer){
    int length = sizeof(pointer) / pointer[0];
    for (size_t i = 0; i < length; i++)
    {
        pointer[i] = 0;
    }
    
}

void reset_array_FLOAT(float *pointer){
    int length = sizeof(pointer) / pointer[0];
    for (size_t i = 0; i < length; i++)
    {
        pointer[i] = 0;
    }
    
}





