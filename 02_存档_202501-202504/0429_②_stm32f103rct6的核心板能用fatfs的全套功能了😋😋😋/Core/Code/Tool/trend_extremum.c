#include "trend_extremum.h"


void TE_Get_Extremum_And_Trend(int flag){
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

    // ��3�����屣������ε�����
    int trend[Max_Q_Row]; // ����100������
    int trd_idx = 0;    // trend����ĵ�ǰ����

    float extremum[Max_Q_Row];
    int ext_idx = 0;

    int i = 0;          // ��4����ǰ�����������±�

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
                break; // �������ƶ��ˣ�����
            }
        }

        // ��5.5����������������Ƶ�Ԫ������
        int count = (i - start + 1); // ע��+1����ΪҪ����startλ��

        // ��5.6������γ��ȣ��������ţ���ʾ���Ʒ���
        trend[trd_idx++] = dir * count;
        extremum[ext_idx++] = arr[start];
    }
    if(extremum[ext_idx-1] != arr[n-1]){
        extremum[ext_idx++] = arr[n-1];
    }

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
}








