#include "code.h"


#if 427

int my_test(){
    FATFS_Index_Check(1);
    Q_Record_Flag = F_Q_Continuous;
    while(1){
        quaternion_self_test();
    }
    delay_ms(100);
}

#endif

#if !504

int my_test(){
    i_file_read("1:/my_index.csv",1);
    i_file_read("1:/10010.csv",1);
    delay_ms(100);
}

#endif

