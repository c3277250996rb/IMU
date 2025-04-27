#include "windows_filter.h"


int is_WF_trigger = enum_RESET;
float WF_array[WINDOWS_SIZE];
int WF_a_current_index = 0;
int WF_a_add_up = 0;

float WF_a_average = 0;
int is_WF_init = 0;
int dynamic_index = 0;

void WF_Judge_From_Static_To_Dynamic(float input_float){
    /* info */ printf("1: input_float: %f\r\n", input_float);

    /* 1.��ʼ�� & ����� */

        /* 1.1 ���ڳ�ʼ�� */
            if(WINDOWS_SIZE > is_WF_init){
                /* info */ printf("[%d]��ʼ��ʼ��\r\n", is_WF_init);

        /* 1.2 ��ɳ�ʼ�� */
            }else if(WINDOWS_SIZE == is_WF_init){
                /* info */ printf("���\r\n");

        /* 1.3 ����� (|��ǰֵ - ���ھ�ֵ| > ��ֵ��? ��������Ǵ���) */
            }else if(WINDOWS_SIZE < is_WF_init){
                int temp_i = (int)input_float - (int)WF_a_average;
                /* info */ printf("temp_i: %d\r\n", temp_i);
                if(abs(temp_i) > TRIGGER_THRESHOLD){
                    /* ��Ǵ��� */
                    is_WF_trigger = enum_TRIGGER;
                    /* ��ʼ������ */
                    WF_a_current_index = 0;
                    return;
                }
            }
        
        /* 1.4 ���³�ʼ�������� */
            is_WF_init++;

    /* ----------------- */


    /* 2.�������� & ��ֵ */
        WF_update_average_index_and_array(input_float);
    /* ----------------- */

}

void WF_Judge_From_Dynamic_To_Static(float input_float){
    int temp_i = (int)input_float - (int)WF_a_average;
    /* info */ printf("temp_i: %d\r\n", temp_i);
    /* info */ printf("2: input_float: %f\r\n", input_float);


    /* 1.�ȴ��ﵽ��Сʱ�� */            /* [0, 2W] */
        if(WINDOWS_SIZE * 2 > dynamic_index){
            /* �˶������� ���� */
            dynamic_index++;
            return;
        }
    /* ----------------- */


    /* 2.���ô��� */                    /* {3W} */
        if(WINDOWS_SIZE * 3 == dynamic_index){
            /* info */ printf("\r\n���ô���.\r\n");
            /* �������� */
            memset(WF_array, 0, sizeof(WF_array));
            /* ���þ�ֵ */
            WF_a_average = 0;
        }
    /* --------- */


    /* 3.���´���, ��ֵ������ */        /* [3W, +��] */
        if(WINDOWS_SIZE * 3 < dynamic_index){
            WF_update_average_index_and_array(input_float);
        }
    /* --------------------- */


    /* 4.�ȴ���ֹ (����) */             /* [4W, +��] */
        if(WINDOWS_SIZE * 4 < dynamic_index){
            /* info */ printf("\r\n���Ծ�ֹ��.\r\n");

            /* 4.1 �����ۼ� -> (|��ǰֵ - ���ھ�ֵ| < ��ֵ) -> ����С */
                if(abs(temp_i) < TRIGGER_THRESHOLD){
                    /* info */ printf("[%d]�����ۼ�\r\n", WF_a_add_up);
                    /* ����ۼ� */
                    is_WF_trigger = enum_ADDUP;
                    /* �ۼƼ����� ���� */
                    WF_a_add_up++;
                }
        
            /* 4.2 ȡ���ۼ� -> (|��ǰֵ - ���ھ�ֵ| > ��ֵ) -> ������ */
                if(abs(temp_i) > TRIGGER_THRESHOLD){
                    /* info */ printf("\r\n����!ȡ���ۼ�.\r\n");
                    /* ����˶� */
                    is_WF_trigger = enum_TRIGGER;
                    /* �ۼƼ����� ���� */
                    WF_a_add_up = 0;
                }
        }
    /* ---------------- */


    /* 5.����ۼ���� */
        if(enum_ADDUP == is_WF_trigger && WINDOWS_SIZE <= WF_a_add_up){
            /* ������ */
            is_WF_trigger = enum_FINISH;
            /* ���� �˶������� */
            dynamic_index = 0;
            /* ���� �ۼƼ����� */
            WF_a_add_up = 0;
            return;
        }
    /* ------------- */


    /* 6.�˶������� ���� */
        dynamic_index++;
    /* ---------------- */

}

void WF_update_average_index_and_array(float input_float){
    /* ����ƽ��ֵ */
    WF_a_average += (input_float - WF_array[WF_a_current_index]) / WINDOWS_SIZE;

    /* �������� */
    WF_array[WF_a_current_index] = input_float;

    /* �������� */
    WF_a_current_index = (WF_a_current_index + 1) % WINDOWS_SIZE;
}


