/**
 ******************************************************************************
 * @file    usmart_config.c
 * @brief   USMART调试.
 ******************************************************************************
 * 
 *-----------------------------------------------------------------------------
 * @brief   1.ug参数列表
 * @param   my_func_list
 *  param   预设: "minnie", "yuqi", "shuhua", "reset".
 *-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------
 * @brief   2.快捷执行函数
 * @param   ug
 *  param   idle: 匹配字符串, 会到my_func_list中查找匹配.
 *                (不支持改变参数)
 *-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------
 * @brief   3.函数列表
 * @param   usmart_nametab
 *  param   模板: (void*)func_name,"retval func_name(arg, ...)",
 *-----------------------------------------------------------------------------
 * 
 */


#include "usmart.h"
#include "usmart_str.h"
#include "delay.h"	 	
#include "sys.h"
#include "code.h"

#include "my_flash.h"
#include "my_string.h"

// USMART (G)I-DLE
#if 1
char *my_func_list[] = {
	"minnie",
	"yuqi",
	"shuhua", 
	"reset"
};

int usmart_minnie(void){
	int result = 1;

	return result;
}

int usmart_yuqi(void){
	int result = 2;

	return result;
}

int usmart_shuhua(void){
	int result = 3;
	
	return result;
}

int ug(char *idle){
	printf("\r\nit is GIDLE!\r\n");
	int result = 0;
	if(*idle == *my_func_list[0]){
		printf("it is minnie!\r\n");
		result = usmart_minnie();
	}
	if(*idle == *my_func_list[1]){
		printf("it is yuqi!\r\n");
		result = usmart_yuqi();
	}
	if(*idle == *my_func_list[2]){
		printf("it is shuhua!\r\n");
		result = usmart_shuhua();
	}
	if(*idle == *my_func_list[3]){
		printf("正在重启...\r\n");
		HAL_NVIC_SystemReset();
	}

	printf("end of it\r\n");
	return result;
}

#endif
// USMART (G)I-DLE


// 函数列表 & 管理器
#if 1
////////////////////////////用户配置区///////////////////////////////////////////////			 
// extern void led_set(u8 sta);
// extern void test_fun(void(*ledset)(u8),u8 sta);
extern int my_test(void);
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
	

	/* multi printf */
	(void*)multi_printf_self_test,"int multi_printf_self_test(void)",

	/* flash */
	(void*)my_flash_self_test,"int my_flash_self_test(void)",

	/* fat */
	(void*)my_fatfs_self_test,"void my_fatfs_self_test(void)",
	(void*)i_file_read,"void i_file_read(char* path, uint8_t is_echo)",
	(void*)FATFS_Write,"void FATFS_Write(char* path, char* write_buffer, int write_position, uint8_t is_echo)",
	(void*)FATFS_Delete,"void FATFS_Delete(char* path, uint8_t is_scan, uint8_t is_secure)",
	(void*)FATFS_Scan,"void FATFS_Scan(void)",
	(void*)FATFS_Index_Check,"void FATFS_Index_Check(uint8_t is_append)",
	(void*)FATFS_Deinit,"void FATFS_Deinit(uint8_t is_secure)",
	(void*)FATFS_Init,"void FATFS_Init(uint8_t is_secure)",
	(void*)FATFS_Muti_Test,"void FATFS_Muti_Test(void)",
	(void*)FATFS_Convert_Array,"void FATFS_Convert_Array(char *input_Path)",

	
	(void*)ug,"int ug(char *idle)",
	(void*)my_test,"int my_test(void)",

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

#endif
// 函数列表 & 管理器

