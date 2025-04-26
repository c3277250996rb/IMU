#include "test.h"

#if !32703

int my_test(){
    FATFS_Index_Check(1);
    // is_Q_stop = enum_Ready;
    is_Q_stop = eunm_Continuous;
    while(is_Q_stop != enum_Stop){
        // 2.一组采?次
        quaternion_self_test();
    }
    delay_ms(100);
}

#endif



#if 427

int my_test(){
    FATFS_Index_Check(1);
    is_Q_stop = eunm_Continuous;
    while(1){
        quaternion_self_test();
    }
    delay_ms(100);
}

#endif


