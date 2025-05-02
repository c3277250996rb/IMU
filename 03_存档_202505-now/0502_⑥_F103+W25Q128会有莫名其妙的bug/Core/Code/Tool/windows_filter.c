#include "windows_filter.h"

int WF_Status = F_WF_Reset;

float WF_Window[WF_WINDOWS_SIZE];
int WF_W_Current_Index = 0;
float WF_W_Average = 0;

int WF_Counter_AddUp = 0;
int WF_Counter_Init = 0;
int WF_Counter_Dynamic = 0;

void WF_Judge_From_Static_To_Dynamic(float input_Float){
    /* info */ printf("1: input_Float: %f\r\n", input_Float);

    /* 1.��ʼ�� & ����� */

        /* 1.1 ���ڳ�ʼ�� */
            if(WF_WINDOWS_SIZE > WF_Counter_Init){
                /* info */ printf("[%d]��ʼ��ʼ��\r\n", WF_Counter_Init);

        /* 1.2 ��ɳ�ʼ�� */
            }else if(WF_WINDOWS_SIZE == WF_Counter_Init){
                /* info */ printf("���\r\n");

        /* 1.3 ����� (|��ǰֵ - ���ھ�ֵ| > ��ֵ��? ��������Ǵ���) */
            }else if(WF_WINDOWS_SIZE < WF_Counter_Init){
                int temp_different = (int)input_Float - (int)WF_W_Average;
                /* info */ printf("temp_different: %d\r\n", temp_different);
                if(abs(temp_different) > WF_TRIGGER_THRESHOLD){
                    /* ��Ǵ��� */
                    WF_Status = F_WF_Trigger;
                    /* ��ʼ������ */
                    WF_W_Current_Index = 0;
                    return;
                }
            }
        
        /* 1.4 ���³�ʼ�������� */
            WF_Counter_Init++;

    /* ----------------- */


    /* 2.�������� & ��ֵ */
        WF_update_average_index_and_array(input_Float);
    /* ----------------- */

}

void WF_Judge_From_Dynamic_To_Static(float input_Float){
    int temp_different = (int)input_Float - (int)WF_W_Average;
    /* info */ printf("temp_different: %d\r\n", temp_different);
    /* info */ printf("2: input_Float: %f\r\n", input_Float);


    /* 1.�ȴ��ﵽ��Сʱ�� */            /* [0, 2W] */
        if(WF_WINDOWS_SIZE * 2 > WF_Counter_Dynamic){
            /* �˶������� ���� */
            WF_Counter_Dynamic++;
            return;
        }
    /* ----------------- */


    /* 2.���ô��� */                    /* {3W} */
        if(WF_WINDOWS_SIZE * 3 == WF_Counter_Dynamic){
            /* info */ printf("\r\n���ô���.\r\n");
            /* �������� */
            memset(WF_Window, 0, sizeof(WF_Window));
            /* ���þ�ֵ */
            WF_W_Average = 0;
        }
    /* --------- */


    /* 3.���´���, ��ֵ������ */        /* [3W, +��] */
        if(WF_WINDOWS_SIZE * 3 < WF_Counter_Dynamic){
            WF_update_average_index_and_array(input_Float);
        }
    /* --------------------- */


    /* 4.�ȴ���ֹ (����) */             /* [4W, +��] */
        if(WF_WINDOWS_SIZE * 4 < WF_Counter_Dynamic){
            /* info */ printf("\r\n���Ծ�ֹ��.\r\n");

            /* 4.1 �����ۼ� -> (|��ǰֵ - ���ھ�ֵ| < ��ֵ) -> ����С */
                if(abs(temp_different) < WF_TRIGGER_THRESHOLD){
                    /* info */ printf("[%d]�����ۼ�\r\n", WF_Counter_AddUp);
                    /* ����ۼ� */
                    WF_Status = F_WF_AddUp;
                    /* �ۼƼ����� ���� */
                    WF_Counter_AddUp++;
                }
        
            /* 4.2 ȡ���ۼ� -> (|��ǰֵ - ���ھ�ֵ| > ��ֵ) -> ������ */
                if(abs(temp_different) > WF_TRIGGER_THRESHOLD){
                    /* info */ printf("\r\n����!ȡ���ۼ�.\r\n");
                    /* ����˶� */
                    WF_Status = F_WF_Trigger;
                    /* �ۼƼ����� ���� */
                    WF_Counter_AddUp = 0;
                }
        }
    /* ---------------- */


    /* 5.����ۼ���� */
        if(F_WF_AddUp == WF_Status && WF_WINDOWS_SIZE <= WF_Counter_AddUp){
            /* ������ */
            WF_Status = F_WF_Finish;
            /* ���� �˶������� */
            WF_Counter_Dynamic = 0;
            /* ���� �ۼƼ����� */
            WF_Counter_AddUp = 0;
            return;
        }
    /* ------------- */


    /* 6.�˶������� ���� */
        WF_Counter_Dynamic++;
    /* ---------------- */

}

void WF_update_average_index_and_array(float input_Float){
    /* ����ƽ��ֵ */
    WF_W_Average += (input_Float - WF_Window[WF_W_Current_Index]) / WF_WINDOWS_SIZE;

    /* �������� */
    WF_Window[WF_W_Current_Index] = input_Float;

    /* �������� */
    WF_W_Current_Index = (WF_W_Current_Index + 1) % WF_WINDOWS_SIZE;
}


