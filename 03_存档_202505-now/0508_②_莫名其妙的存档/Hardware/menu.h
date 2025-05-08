#ifndef __MENU_H
#define __MENU_H



void show_bluetooth(void);
void show_xunji(void);
void show_bizhang(void);
void menu_task(void);

#define MENU_ITEM_COUNT 3
#define OLED_WIDTH  126         //oled�Ŀ��
#define OLED_HEIGHT 64          //oled�ĸ߶�
#define OLED_LINE_HEIGHT 20     //һ�в˵��Ŀ��

//�˵�״̬�ṹ��
typedef enum {
    main_menu = 0,
    secondary_menu = 1,
} MenuState;


//�˵���ṹ��
typedef struct {
    char* name;              // �˵�����
    void (*action)(void);    // �˵���Ӧ����������Ϊ�գ�
} MenuItem;




#endif
