/**
 ******************************************************************************
 * @file    usmart_config.c
 * @brief   USMART����.
 ******************************************************************************
 * 
 *-----------------------------------------------------------------------------
 * @brief   1.ug�����б�
 * @param   my_func_list
 *  param   Ԥ��: "minnie", "yuqi", "shuhua", "reset".
 *-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------
 * @brief   2.���ִ�к���
 * @param   ug
 *  param   idle: ƥ���ַ���, �ᵽmy_func_list�в���ƥ��.
 *                (��֧�ָı����)
 *-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------
 * @brief   3.�����б�
 * @param   usmart_nametab
 *  param   ģ��: (void*)func_name,"retval func_name(arg, ...)",
 *-----------------------------------------------------------------------------
 * 
 */


#include "usmart.h"
#include "usmart_str.h"
#include "delay.h"	 	
#include "sys.h"
// #include "lcd.h"
#include "code.h"
 

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
		printf("��������...\r\n");
		HAL_NVIC_SystemReset();
	}

	printf("end of it\r\n");
	return result;
}

#endif
// USMART (G)I-DLE


// �����б� & ������
#if 1
////////////////////////////�û�������///////////////////////////////////////////////			 
// extern void led_set(u8 sta);
// extern void test_fun(void(*ledset)(u8),u8 sta);
extern int my_test(void);
//�������б��ʼ��(�û��Լ����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif		   
	(void*)delay_ms,"void delay_ms(u16 nms)",
 	(void*)delay_us,"void delay_us(u32 nus)",	 
	// (void*)LCD_Clear,"void LCD_Clear(u16 Color)",
	// (void*)LCD_Fill,"void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)",
	// (void*)LCD_DrawLine,"void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)",
	// (void*)LCD_DrawRectangle,"void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)",
	// (void*)LCD_Draw_Circle,"void Draw_Circle(u16 x0,u16 y0,u8 r)",
	// (void*)LCD_ShowNum,"void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)",
	// (void*)LCD_ShowString,"void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)",
	// (void*)LCD_Fast_DrawPoint,"void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)",
	// (void*)LCD_ReadPoint,"u16 LCD_ReadPoint(u16 x,u16 y)",							 
	// (void*)LCD_Display_Dir,"void LCD_Display_Dir(u8 dir)",
	// (void*)LCD_ShowxNum,"void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)", 
		
	// (void*)led_set,"void led_set(u8 sta)",
	// (void*)test_fun,"void test_fun(void(*ledset)(u8),u8 sta)",	
	
	(void*)ug,"int ug(char *idle)",
	(void*)my_test,"int my_test(void)",

};						  
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

//�������ƹ�������ʼ��
//�õ������ܿغ���������
//�õ�����������
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//��������
	0,	  	//��������
	0,	 	//����ID
	1,		//������ʾ����,0,10����;1,16����
	0,		//��������.bitx:,0,����;1,�ַ���	    
	0,	  	//ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ��
	0,		//�����Ĳ���,��ҪPARM_LEN��0��ʼ��
};   

#endif
// �����б� & ������

