
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 1024  // ����ַ�����󳤶�

// ƴ�Ӷ�������Ԫ��
// char* concat_arrays(int num_arrays, const char* delimiter, const char* format, ...) {
char* concat_arrays(int num_arrays, const char* delimiter, const char* format, ...) {
    static char result[MAX_STR_LEN];  // ��̬�������洢���
    result[0] = '\0';  // ��ʼ��Ϊ���ַ���

    va_list args;
    va_start(args, format);

    char buffer[32];  // ��ʱ�洢��ʽ���������
    int first = 1;    // ���ڱ����׸�Ԫ��ǰ�ķָ���

    for (int i = 0; i < num_arrays; i++) {
        int* array = va_arg(args, int*);  // ��ȡ����ָ��
        int size = va_arg(args, int);     // ��ȡ�����С

        for (int j = 0; j < size; j++) {
            if (!first) {
                strcat(result, delimiter);  // ��ӷָ���
            }
            first = 0;
            snprintf(buffer, sizeof(buffer), format, array[j]);  // ���ݴ���ĸ�ʽ�ַ�����ʽ������
            strcat(result, buffer);  // ׷�ӵ�����ַ���
        }
    }

    strcat(result, "\r\n");  // ׷�ӵ�����ַ���
    va_end(args);
    return result;
}

int my_test() {
    int arr1[] = {1, 2, 3};
    int arr2[] = {4, 5};
    int arr3[] = {6, 7, 8, 9};

    // ʾ����ʹ�ö�����Ϊ�ָ����������� %d ��ʽ���
    char* result = concat_arrays(3, ",", "%d", arr1, 3, arr2, 2, arr3, 4);
    printf("ƴ�ӽ��: %s\n", result);

    // ʾ����ʹ�ÿո���Ϊ�ָ�����������ʮ�����Ƹ�ʽ���
    result = concat_arrays(3, " ", "%X", arr1, 3, arr2, 2, arr3, 4);
    printf("ƴ�ӽ�� (ʮ������): %s\n", result);

    return 0;
}


