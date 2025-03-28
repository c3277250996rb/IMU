#include "test.h"


#if 328

int my_test(){
    FATFS_Index_Check();
    is_Q_stop = enum_Ready;
    while(is_Q_stop != enum_Stop){
        // 2.一组采?次
        quaternion_self_test();
    }
    delay_ms(100);
}

#endif

