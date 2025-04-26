#include "windows_filter.h"


int is_WF_trigger = enum_RESET;
float WF_array[WINDOWS_SIZE];
int WF_a_current_index = 0;
int WF_a_add_up = 0;

float WF_a_average = 0;
int is_WF_init = 0;

void WF_Judge_From_Static_To_Dynamic(float input_float){
    // 1.��ʼ��ʼ��
    if(is_WF_init < WINDOWS_SIZE){
        printf("[%d]��ʼ��ʼ��\r\n", is_WF_init);
        is_WF_init++;
    }else if(is_WF_init == WINDOWS_SIZE){
        printf("���\r\n");
        is_WF_init++;
    }else if(is_WF_init > WINDOWS_SIZE){
    // 2.��ǰֵ - ���ھ�ֵ > ��ֵ��? ��������Ǵ���
        int temp_i = (int)input_float - (int)WF_a_average;
        printf("temp_i: %d\r\n", temp_i);
        if(abs(temp_i) > TRIGGER_THRESHOLD){
            is_WF_trigger = enum_TRIGGER;
            WF_a_current_index = 0;
            return;
        }
    }
    
    // 3.����ƽ��ֵ
    printf("1: input_float: %f\r\n", input_float);
    WF_update_average_index_and_array(input_float);

}

int dynamic_index = 0;
void WF_Judge_From_Dynamic_To_Static(float input_float){
    printf("2: input_float: %f\r\n", input_float);

    // 1.�ȴ��ﵽ��С��¼ʱ�� (WINDOWS_SIZE * 2)
    if(dynamic_index < WINDOWS_SIZE * 2){
        dynamic_index++;
        return;
    }

    int temp_i = (int)input_float - (int)WF_a_average;
    printf("temp_i: %d\r\n", temp_i);

    if(dynamic_index > WINDOWS_SIZE * 4){
        printf("\r\n���Ծ�ֹ��.\r\n");
        // 2.��ǰֵ - ���ھ�ֵ < ��ֵ��? ����������ۼ�
        if(abs(temp_i) < TRIGGER_THRESHOLD){
            is_WF_trigger = enum_ADDUP;
            printf("[%d]�����ۼ�\r\n", WF_a_add_up);
            WF_a_add_up++;
        }

        // 3.�м����������ȡ���ۼ�
        if(abs(temp_i) > TRIGGER_THRESHOLD){
            is_WF_trigger = enum_TRIGGER;
            WF_a_add_up = 0;
            printf("\r\n����!ȡ���ۼ�.\r\n");
        }
    }


    // 4.�ж��Ƿ��ۼƴ��
    if((is_WF_trigger == enum_ADDUP) && (WF_a_add_up >= WINDOWS_SIZE)){
        is_WF_trigger = enum_FINISH;
        dynamic_index = 0;
        WF_a_add_up = 0;
        return;
    }

    if(dynamic_index == WINDOWS_SIZE * 3){
        printf("\r\nif(dynamic_index == WINDOWS_SIZE * 3)\r\n");
        memset(WF_array, 0, sizeof(WF_array));
        WF_a_average = 0;
    }
    if(dynamic_index > WINDOWS_SIZE * 3){
        WF_update_average_index_and_array(input_float);
    }

    dynamic_index++;
}

void WF_update_average_index_and_array(float input_float){
    /* ����ƽ��ֵ */
    WF_a_average += (input_float - WF_array[WF_a_current_index]) / WINDOWS_SIZE;

    /* �������� */
    WF_array[WF_a_current_index] = input_float;

    /* �������� */
    WF_a_current_index = (WF_a_current_index + 1) % WINDOWS_SIZE;
}


