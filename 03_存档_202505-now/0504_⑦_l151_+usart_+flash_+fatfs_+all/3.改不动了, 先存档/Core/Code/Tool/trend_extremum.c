#include "trend_extremum.h"

// ��3�����屣������ε�����
int trend[Max_Q_Row]; // ����100������
int trd_idx = 0;    // trend����ĵ�ǰ����

float extremum[Max_Q_Row];
int ext_idx = 0;


TE_Frame_Struct TE_Frame_default = {
    .Date = "2025/04/30 17:30:00",
    .Brief = "the first frame",
    // .File_Index = ,
    .Array = {
        .Trend = trend,
        .Extremum = extremum
    }
};


void TE_Get_Extremum_And_Trend(int flag){
    TE_ReInit();

    // ��1�����帡������
    // float arr[] = {3.3, -2.2, 1.1, 2.2, 3.3, 4.4, 2.2, 1.1, 2.2};
	// float arr[] = {5.0, 6.0, 7.5, 6.5, 5.5, 4.0, 4.5, 5.0, 3.0, 2.0};
	// float arr[] = {1.1, -2.2, 3.3, -4.4, 5.5, 6.6, 9.9, 7, 7, 8, 8, 11, 7};
    float *arr;
    switch (flag)
    {
        case F_TE_Roll:
            arr = SJ_Array_Roll;
            break;
        
        case F_TE_Pitch:
            arr = SJ_Array_Pitch;
            break;
        
        // case F_TE_Yaw:
        //     arr = SJ_Array_Roll;
        //     break;
        
        default:
            printf("[��ȡ���Ƽ�ֵ.c]: û�и�����!���˳���.");
            return;
            break;
    }

    // ��2����������Ԫ�ظ���
    // int n = sizeof(arr) / sizeof(arr[0]);
    int n = Max_Q_Row;

    int i = 0;          // ��4����ǰ�����������±�
    int trd_addup[Max_Q_Row];
    trd_addup[0] = 1;

    // ��5����ʼ��������
    while (i < n - 1) { // ��i��û�����鵹����2��Ԫ��ʱ
        int dir = 0;    // dir��¼��ǰ���Ʒ���1Ϊ������-1Ϊ�½�

        // ��5.1���жϵ�ǰ����һ��Ԫ�صĹ�ϵ��ȷ�����Ʒ���
        if (arr[i] < arr[i+1]) dir = 1;      // ����
        else if (arr[i] > arr[i+1]) dir = -1; // �½�
        else {
            // ��5.2�������ȣ�����������������
            i++;
            continue;
        }

        // ��5.3����¼��ǰ������ƿ�ʼ��λ��
        int start = i;
        i++; // ����ƶ�һ��λ�ã�׼�������������

        // ��5.4����������ͬһ��������
        while (i < n) {
            if ((dir == 1 && arr[i] < arr[i+1]) ||
                (dir == -1 && arr[i] > arr[i+1]) ||
                arr[i] == arr[i+1]) {
                if(n-1 == i){
                    break; // ���һ��������+1��
                }
                i++; // �������ͬһ�����ϣ�������
            } else {
                if(0 < trd_idx){
                    trd_addup[trd_idx] = dir * i;
                }
                break; // �������ƶ��ˣ�����
            }
        }

        // ��5.5����������������Ƶ�Ԫ������
        int count = (i - start + 1); // ע��+1����ΪҪ����startλ��

        // ��5.6������γ��ȣ��������ţ���ʾ���Ʒ���
        if(0 < trd_idx){
            // trd_addup[trd_idx] = dir * (count - 1 + abs(trd_addup[trd_idx - 1]));
        }
        trend[trd_idx++] = dir * count;
        extremum[ext_idx++] = arr[start];
    }
    if(extremum[ext_idx-1] != arr[n-1]){
        extremum[ext_idx++] = arr[n-1];
    }

    /* ����Ԥ�� */
        // printf("��������: [");
        // for (int j = 0; j < trd_idx; j++) {
        //     printf("%d", trd_addup[j]);
        //     if (j != trd_idx - 1) printf(", "); // ����������һ�����Ӷ���
        // }
        // printf("]\r\n");
        // printf("trd_idx: %d\r\n", trd_idx);
    /* ------- */

    // ��6�����������������
    printf("�����: [");
    for (int j = 0; j < trd_idx; j++) {
        printf("%d", trend[j]);
        if (j != trd_idx - 1) printf(", "); // ����������һ�����Ӷ���
    }
    printf("]\r\n");
    printf("trd_idx: %d\r\n", trd_idx);

    printf("��ֵ: [");
    for (int j = 0; j < ext_idx; j++) {
        printf("%.2f", extremum[j]);
        if (j != ext_idx - 1) printf(", "); // ����������һ�����Ӷ���
    }
    printf("]\r\n");
    printf("ext_idx: %d\r\n", ext_idx);

    // ��7���������
    TE_Frame_default.File_Index = FATFS_Index_Current;
}

void TE_ReInit(void){
    trd_idx = 0;
    ext_idx = 0;
    reset_array_INT(trend);
    reset_array_FLOAT(Max_Q_Row, extremum);
}


int ext_cpr_len = 0;

#define FLOAT_THRESHOLD 0.5F
void TE_Extremum_Compress(void){
    int length = ext_idx - 2;
    float start = extremum[length - 1];

    ext_cpr_len = 0;
	
    while(--length){
        float diff = start - extremum[length - 1];
        diff = diff > 0 ? diff : -diff;
        if(FLOAT_THRESHOLD > diff){
            continue;
        }else{
            length++;
            break;
        }
    }

    printf("��ֵѹ��: [");
    for(int i = 0; i < length; i++){
        printf("%.2f, ", extremum[i]);
    }
    printf("]\r\n");

    ext_cpr_len = length;
}

void TE_Generate_DataFrame(void){
    // 1.����
    // TE_Frame_Struct temp_TE_Frame_Struct = {
    //     .Date = ,
    //     .Brief = ,
    //     .File_Index = ,
    //     .Array = {
    //         .Trend = ,
    //         .Extremum = 
    //     }
    // }

    // 2.��ӡ
    printf("\r\n����Ϊ���͸�ƽ̨������:\r\n>>>\r\n");
    printf("{");

        printf("\"Date\":\"%s\",", TE_Frame_default.Date);
        printf("\"Brief\":\"%s\",", TE_Frame_default.Brief);
        printf("\"File_Index\":%d,", TE_Frame_default.File_Index);
        printf("\"Array\":{");
            printf("\"Trend\":[");
                int temp_addup[ext_cpr_len];
                int dir;
                temp_addup[0] = 1;
                for (int j = 0; j < ext_cpr_len - 1; j++) {
                    dir = trend[j] > 0 ? 1 : -1;
                    temp_addup[j+1] = dir * (abs(temp_addup[j]) + abs((dir == 1 ? trend[j]-1 : trend[j]+1)));
                }
                // for
                for (int j = 0; j < ext_cpr_len; j++) {
                    printf("%d", temp_addup[j]);
                    if (j != ext_cpr_len - 1) printf(","); // ����������һ�����Ӷ���
                }
            printf("],");
            printf("\"Extremum\":[");
                // for
                for (int j = 0; j < ext_cpr_len; j++) {
                    printf("%.2f", extremum[j]);
                    if (j != ext_cpr_len - 1) printf(","); // ����������һ�����Ӷ���
                }
            printf("]");
        printf("}");

    printf("}");
    printf("\r\n<<<\r\n");
}

