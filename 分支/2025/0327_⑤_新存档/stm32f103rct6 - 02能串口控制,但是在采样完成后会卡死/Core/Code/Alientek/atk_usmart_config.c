#include "atk_usmart.h"
#include "atk_usmart_str.h"
////////////////////////////�û�������///////////////////////////////////////////////
//������Ҫ�������õ��ĺ�����������ͷ�ļ�(�û��Լ����) 
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



















