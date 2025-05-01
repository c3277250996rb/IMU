#include "atk_usmart.h"
#include "atk_usmart_str.h"
////////////////////////////用户配置区///////////////////////////////////////////////
//这下面要包含所用到的函数所申明的头文件(用户自己添加) 
#include "atk_delay.h"	 	
#include "atk_sys.h"
// #include "lcd.h"

#include "my_gpio.h"
#include "my_fatfs.h"
#include "test.h"
								 
extern void led_set(u8 sta);
extern void test_fun(void(*ledset)(u8),u8 sta);			


// quick
#if 1
void i_file_read(char* path, uint8_t is_echo){
	FATFS_Read(path, is_echo);
}
#endif
// quick


//函数名列表初始化(用户自己添加)
//用户直接在这里输入要执行的函数名及其查找串
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//如果使能了读写操作
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",
#endif
	(void*)delay_ms,"void delay_ms(u16 nms)",
 	(void*)delay_us,"void delay_us(u32 nus)",

	(void*)my_test,"int my_test(void)",
	(void*)my_gpio_self_test,"void my_gpio_self_test(void)",

	(void*)i_file_read,"void i_file_read(char* path, uint8_t is_echo)",
	(void*)FATFS_Write,"void FATFS_Write(char* path, char* write_buffer, int write_position)",
	(void*)FATFS_Delete,"void FATFS_Delete(char* path)",
	(void*)FATFS_Scan,"void FATFS_Scan(void)",
	(void*)FATFS_Index_Check,"FATFS_Result_Struct FATFS_Index_Check(void)",
	(void*)FATFS_Deinit,"void FATFS_Deinit(void)",
	(void*)FATFS_Init,"void FATFS_Init(void)",
};
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//函数控制管理器初始化
//得到各个受控函数的名字
//得到函数总数量
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//函数数量
	0,	  	//参数数量
	0,	 	//函数ID
	1,		//参数显示类型,0,10进制;1,16进制
	0,		//参数类型.bitx:,0,数字;1,字符串	    
	0,	  	//每个参数的长度暂存表,需要MAX_PARM个0初始化
	0,		//函数的参数,需要PARM_LEN个0初始化
};   



















