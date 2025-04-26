#include "test.h"

#if !32703

int my_test(){
    FATFS_Index_Check(1);
    is_Q_stop = enum_Ready;
    while(is_Q_stop != enum_Stop){
        // 2.一组采?次
        quaternion_self_test();
    }
    delay_ms(100);
}

#endif

#if 411

int my_test(){
    FATFS_Index_Check(1);
    is_Q_stop = enum_Ready;
    // while(is_Q_stop != enum_Stop){
    while(1){
        // 2.一组采?次
        quaternion_self_test();
    }
    delay_ms(100);
}

#endif



