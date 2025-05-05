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
#include "code.h"
 

// USMART (G)I-DLE
#if 1
char *my_func_list[] = {
	"minnie",
	"yuqi",
	"shuhua", 
	"reset",

	"multi",
	"led"
};
int my_func_len = sizeof(my_func_list) / sizeof(my_func_list[0]);


int usmart_minnie(void){
	int result = 1;
	/* list all */
	printf("my_func_list[%d]: {", my_func_len);
	for(int i = 0; i < my_func_len ;i++){
		printf("\"%s\"", my_func_list[i]);
		if(i != my_func_len - 1) printf(", ");
	}
	printf("}\r\n");
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
	int match;
	for(match = 0; match < my_func_len + 1 ;match++){
		if(match == my_func_len) return result;
		if(0 == strcmp(idle, my_func_list[match])) break;
	}

	printf("it is %s!\r\n", my_func_list[match]);
	switch(match){
		case 0:
			result = usmart_minnie();
			break;
		case 1:
			result = usmart_yuqi();
			break;
		case 2:
			result = usmart_shuhua();
			break;
		case 3:
			printf("��������...\r\n");
			HAL_NVIC_SystemReset();
			break;

		case 4:
			muti_printf_self_test();
			break;
		case 5:
			my_led_self_test();
			break;
		default:
			printf("%s[%d]: ��?��...\r\n", __FILE__, __LINE__);
			break;
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

