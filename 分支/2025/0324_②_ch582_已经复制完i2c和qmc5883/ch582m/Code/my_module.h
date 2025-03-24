#ifndef __MODULE_H
#define __MODULE_H

    #include "CH59x_common.h"
    #include "my_interface.h"
    #include "my_tool.h"

    #ifndef __QMC5883__All__Definition__
    #define __QMC5883__All__Definition__

        /* define QMC5883 register address */
        #if 1
            #define MAG_XOUT_L		0x00
            #define MAG_XOUT_H		0x01
            #define MAG_YOUT_L		0x02
            #define MAG_YOUT_H		0x03
            #define MAG_ZOUT_L		0x04
            #define MAG_ZOUT_H		0x05

            #define MAG_ADDRESS     0x1A    //compass device address
        #endif
        /* ------------------------------- */


        #if 1
            #define three_axis_d "%d,%d,%d\r\n"
            #define three_axis_f "%f,%f,%f\r\n"

            #define six_axis_d "%d,%d,%d,%d,%d,%d\r\n"
            #define six_axis_f "%f,%f,%f,%f,%f,%f\r\n"

            #define nine_axis_d "%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n"
            #define nine_axis_f "%f,%f,%f,%f,%f,%f,%f,%f,%f\r\n"
        #endif


        extern int16_t qmc5883_mag[3];


        void QMC5883_Init(void);

        void QMC5883_Update_Data(void);

        void QMC5883_Read_Mag(void);

    #endif


#endif // !__MODULE_H    

