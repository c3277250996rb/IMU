#include "my_senser.h"


/* define LSM6DS3 register address */
#if 1
    #define ADDRESS_CTRL1_XL_10h  0x10
    #define ADDRESS_CTRL2_G_11h   0x11

    #define DATA_CTRL1_XL_10h  0x61
    #define DATA_CTRL2_G_11h   0x68

    #define	GYRO_XOUT_L		0x22
    #define	GYRO_XOUT_H		0x23
    #define	GYRO_YOUT_L		0x24
    #define	GYRO_YOUT_H		0x25
    #define	GYRO_ZOUT_L		0x26
    #define	GYRO_ZOUT_H		0x27

    #define	ACCEL_XOUT_L	0x28
    #define	ACCEL_XOUT_H	0x29
    #define	ACCEL_YOUT_L	0x2A
    #define	ACCEL_YOUT_H	0x2B
    #define	ACCEL_ZOUT_L	0x2C
    #define	ACCEL_ZOUT_H	0x2D

    #define	GYRO_ADDRESS    0xD6    //Gyro and Accel device address
    #define ACCEL_ADDRESS   0xD6 
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


extern int16_t lsm6ds3_accel[3];
extern int16_t lsm6ds3_gyro[3];


void LSM6DS3_Init(void);

void LSM6DS3_Update_Data(void);

void LSM6DS3_Read_Accel(void);
void LSM6DS3_Read_Gyro(void);
void LSM6DS3_Read_Mag(void);




/* # LSM6DS3

## Mechanical characteristics */

/* ### Table 3. Mechanical characteristics 

| Symbol | Parameter                          | Test conditions | Convert    |       | Typ.(1) | Unit   |
| ------ | ---------------------------------- | --------------- | ---------- | ----- | ------- | ------ |
| LA_So  | Linear acceleration sensitivity(2) | FS = ¡À2 g       | 10^6 / 61  | LSB/g | 0.061   | mg/LSB |
|        |                                    | FS = ¡À4 g       | 10^6 / 122 |       | 0.122   |        |
|        |                                    | FS = ¡À8 g       | 10^6 / 244 |       | 0.244   |        |
|        |                                    | FS = ¡À16 g      | 10^6 / 488 |       | 0.488   |        |

<br>

| Symbol | Parameter                   | Test conditions | Convert      |         | Typ.(1) | Unit     |
| ------ | --------------------------- | --------------- | ------------ | ------- | ------- | -------- |
| G_So   | Angular rate sensitivity(2) | FS = ¡À125 ¡ã     | 10^6 / 4375  | LSB/dps | 4.375   | mdps/LSB |
|        |                             | FS = ¡À250 ¡ã     | 10^6 / 8750  |         | 8.75    |          |
|        |                             | FS = ¡À500 ¡ã     | 10^6 / 17500 |         | 17.50   |          |
|        |                             | FS = ¡À1000 ¡ã    | 10^6 / 35000 |         | 35      |          |
|        |                             | FS = ¡À2000 ¡ã    | 10^6 / 70000 |         | 70      |          |

<br> */



/* ## CTRL1_XL (10h) */

/* ### Table 51. CTRL1_XL register description

| CTRL1_XL (10h) |             |                                                              |
| -------------- | ----------- | ------------------------------------------------------------ |
| 7              | ODR_XL [3]  | Output data rate and power mode selection. Default value: 0000 (see Table 52). |
| 6              | ODR_XL [2]  |                                                              |
| 5              | ODR_XL [1]  |                                                              |
| 4              | ODR_XL [0]  |                                                              |
| 3              | FS_XL [1]   | Accelerometer full-scale selection. Default value: 00. (00: ¡À2 g; 01: ¡À16 g; 10: ¡À4 g; 11: ¡À8 g) |
| 2              | FS_XL [0]   |                                                              |
| 1              | LPF1_BW_SEL | Accelerometer digital LPF (LPF1) bandwidth selection. For bandwidth selection  refer to CTRL8_XL (17h). |
| 0              | BW0_XL      | Accelerometer analog chain bandwidth selection (only for accelerometer  ODR ¡Ý 1.67 kHz). (0: BW @ 1.5 kHz; 1: BW @ 400 Hz) |

<br> */



/* ### Table 52. Accelerometer ODR register setting 

| ODR_G [3:0] |      | ODR selection [Hz] (when  XL_HM_MODE = 1) | ODR selection [Hz] (when  XL_HM_MODE = 0) |
| ----------- | ---- | ----------------------------------------- | ----------------------------------------- |
| 0h          | 0000 | Power-down                                | Power-down                                |
| Bh          | 1011 | 1.6 Hz (low power only)                   | 12.5 Hz (high performance)                |
| 1h          | 0001 | 12.5 Hz (low power)                       | 12.5 Hz                                   |
| 2h          | 0010 | 26 Hz                                     | 26 Hz                                     |
| 3h          | 0011 | 52 Hz                                     | 52 Hz                                     |
| 4h          | 0100 | 104 Hz (normal mode)                      | 104 Hz                                    |
| 5h          | 0101 | 208 Hz                                    | 208 Hz                                    |
| 6h          | 0110 | 416 Hz (high performance)                 | 416 Hz                                    |
| 7h          | 0111 | 833 Hz                                    | 833 Hz                                    |
| 8h          | 1000 | 1.66 kHz                                  | 1.66 kHz                                  |
| 9h          | 1001 | 3.33 kHz                                  | 3.33 kHz                                  |
| Ah          | 1010 | 6.66 kHz                                  | 6.66 kHz                                  |
| Ch - Fh     | 11xx | Not allowed                               | Not allowed                               |

<br> */



/* ## CTRL2_G (11h) */

/* ### Table 54. CTRL2_G register description

| CTRL2_G (11h) |           |                                                              |
| ------------- | --------- | ------------------------------------------------------------ |
| 7             | ODR_G [3] | Gyroscope output data rate selection. Default value: 0000 (Refer to Table 55) |
| 6             | ODR_G [2] |                                                              |
| 5             | ODR_G [1] |                                                              |
| 4             | ODR_G [0] |                                                              |
| 3             | FS_G [1]  | Gyroscope full-scale selection. Default value: 00 (00: 245 dps; 01: 500 dps; 10: 1000 dps; 11: 2000 dps) |
| 2             | FS_G [0]  |                                                              |
| 1             | FS_125    | Gyroscope full-scale at 125 dps. Default value: 0 (0: disabled; 1: enabled) |
| 0             |           |                                                              |

<br> */



/* ### Table 55. Gyroscope ODR configuration setting

| ODR_XL [3:0] |      | ODR selection [Hz] (when  XL_HM_MODE = 1) | ODR selection [Hz] (when  XL_HM_MODE = 0) |
| ------------ | ---- | ----------------------------------------- | ----------------------------------------- |
| 0h           | 0000 | Power-down                                | Power-down                                |
| 1h           | 0001 | 12.5 Hz (low power)                       | 12.5 Hz                                   |
| 2h           | 0010 | 26 Hz                                     | 26 Hz                                     |
| 3h           | 0011 | 52 Hz                                     | 52 Hz                                     |
| 4h           | 0100 | 104 Hz (normal mode)                      | 104 Hz                                    |
| 5h           | 0101 | 208 Hz                                    | 208 Hz                                    |
| 6h           | 0110 | 416 Hz (high performance)                 | 416 Hz                                    |
| 7h           | 0111 | 833 Hz                                    | 833 Hz                                    |
| 8h           | 1000 | 1.66 kHz                                  | 1.66 kHz                                  |
| 9h           | 1001 | 3.33 kHz                                  | 3.33 kHz                                  |
| Ah           | 1010 | 6.66 kHz                                  | 6.66 kHz                                  |
| Bh           | 1011 | Not available                             | Not available                             |

<br> */

