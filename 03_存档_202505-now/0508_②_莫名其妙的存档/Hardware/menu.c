#include "stm32f10x.h"
#include "menu.h"
#include "motor.h"
#include "OLED.h"
#include "key.h"
#include <stdint.h>
#include <stddef.h>


extern uint8_t speed;

MenuState menu_state = main_menu;       //初始化菜单状态为主菜单

uint8_t cloose_item = 0;                // 当前选中的菜单项


//菜单结构体定义 - 名字 - 动作函数
MenuItem menu[] = {
    { "蓝牙模式", show_bluetooth },
    { "循迹模式", show_xunji },
    { "避障模式", show_bizhang },
};

//蓝牙模式二级菜单显示及动作实现
void show_bluetooth(void)
{
    OLED_ShowImage(0, 8, 100, 44, Diode);
    OLED_ShowString(0, 0, "蓝牙模式", OLED_8X16);
    OLED_ShowString(85, 0, "speed:", OLED_8X16);
    OLED_ShowNum(95, 20, speed, 3, OLED_8X16);
}
//循迹模式二级菜单显示及动作实现
void show_xunji(void)
{
    OLED_ShowImage(0, 8, 100, 44, Diode);
    OLED_ShowString(0, 0, "循迹模式", OLED_8X16);
    OLED_ShowString(85, 0, "speed:", OLED_8X16);
    OLED_ShowNum(95, 20, speed, 3, OLED_8X16);
    tracking();
    
}
//避障模式二级菜单显示及动作实现
void show_bizhang(void)
{
    OLED_ShowImage(0, 8, 100, 44, Diode);
    OLED_ShowString(0, 0, "避障模式", OLED_8X16);
    OLED_ShowString(85, 0, "speed:", OLED_8X16);
    OLED_ShowNum(95, 20, speed, 3, OLED_8X16);
    bizhang();
}

//主菜单处理函数
void main_menu_handle(void)
{
    car_stop();                                                 //车辆保持停止

    if (key_flag[0] == key_short_down)                          //key1 - 短按 - 上下切换模式
    {
        cloose_item = (cloose_item + 1) % MENU_ITEM_COUNT;      //通过取余来循环菜单
        key_res();                                 //松开按键案件标志位归0，为下一次按键做准备
    }

    if (key_flag[1] == key_short_down)                          // key2 - 短按 - 进入二级菜单
    {
        menu_state = secondary_menu;                            //进入二级菜单
        key_res();                              //松开按键案件标志位归0，为下一次按键做准备
    }

    OLED_Clear();
    for (uint8_t i = 0; i < MENU_ITEM_COUNT; i++) {             //在菜单项范围内便利循环
        OLED_ShowString(0, i * OLED_LINE_HEIGHT, menu[i].name, OLED_8X16);
    }
    OLED_ReverseArea(0, cloose_item * OLED_LINE_HEIGHT, OLED_WIDTH, OLED_LINE_HEIGHT); // 反色高亮选中项
    OLED_Update();
}

//二级菜单处理函数
void secondary_menu_handle(void)
{
    if (key_flag[2] == key_short_down) // key3 - 短按 - 返回一级菜单
    {
        menu_state = main_menu;
        key_res();
    }

    OLED_Clear();
    if (menu[cloose_item].action != NULL) {
        menu[cloose_item].action(); // 执行对应的功能
    }
    OLED_Update();
}

//总菜单调度器（状态机）
void menu_task(void)
{
    switch (menu_state)
    {
        case main_menu:
            main_menu_handle();
            break;
        case secondary_menu:
            secondary_menu_handle();
            break;
        default:
            menu_state = main_menu;
            break;
    }
}

