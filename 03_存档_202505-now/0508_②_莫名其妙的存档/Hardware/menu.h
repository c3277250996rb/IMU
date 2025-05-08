#ifndef __MENU_H
#define __MENU_H



void show_bluetooth(void);
void show_xunji(void);
void show_bizhang(void);
void menu_task(void);

#define MENU_ITEM_COUNT 3
#define OLED_WIDTH  126         //oled的宽度
#define OLED_HEIGHT 64          //oled的高度
#define OLED_LINE_HEIGHT 20     //一行菜单的宽度

//菜单状态结构体
typedef enum {
    main_menu = 0,
    secondary_menu = 1,
} MenuState;


//菜单项结构体
typedef struct {
    char* name;              // 菜单名称
    void (*action)(void);    // 菜单对应动作（可以为空）
} MenuItem;




#endif
