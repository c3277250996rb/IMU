#include "test.h"

#if !32703

int my_test(){
    FATFS_Index_Check(1);
    // Q_Record_Flag = F_Q_Ready;
    Q_Record_Flag = F_Q_Continuous;
    while(Q_Record_Flag != F_Q_Stop){
        // 2.一组采?次
        quaternion_self_test();
    }
    delay_ms(100);
}

#endif



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


#if !428

int my_test(){
    int i = 1;
    long l = 1L;
    float f = 1.0F;

    i = (int) l;
    f = (float) i;
    while (1)
    {
        printf("f: %f\r\n", (f+(float)i));
        i++;
        delay_ms(100);
    }

}

#endif
