#include "my_string.h"


// multi printf
#if 1
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define mpa_MAX_STR_LEN 1024  // ����ַ�����󳤶�


/**
 * @brief  Concatenate array(s).
 * @param  num_arrays: Number of array(s).
 * @param  delimiter: Separator.
 * @param  format: Item format.
 *         @arg "%d": Dec
 *         @arg "%f": Float
 *         @arg "%x": Hex
 *         @arg "%s": String
 *         @arg ...
 * @retval Char*
 */
char* multi_printf_arrays(int num_arrays, const char* delimiter, const char* format, ...) {
    static char result[mpa_MAX_STR_LEN];  // ��̬�������洢���
    result[0] = '\0';  // ��ʼ��Ϊ���ַ���

    va_list args;
    va_start(args, format);

    char buffer[32];  // ��ʱ�洢��ʽ���������
    int first = 1;    // ���ڱ����׸�Ԫ��ǰ�ķָ���

    for (int i = 0; i < num_arrays; i++) {
        
        if (strcmp(format, "%f") == 0) {
            float* array = va_arg(args, float*);  // ������������
            int size = va_arg(args, int);

            for (int j = 0; j < size; j++) {
                if (!first) {
                    strcat(result, delimiter);
                }
                first = 0;
                snprintf(buffer, sizeof(buffer), format, array[j]);  // ��������
                strcat(result, buffer);
            }
        } else {
            int* array = va_arg(args, int*);  // ������������
            int size = va_arg(args, int);

            for (int j = 0; j < size; j++) {
                if (!first) {
                    strcat(result, delimiter);
                }
                first = 0;
                snprintf(buffer, sizeof(buffer), format, array[j]);  // ��������
                strcat(result, buffer);
            }
        }
    }

    va_end(args);
    return result;
}


/**
 * @brief  Concatenate array(s) (direction).
 * @param  num_arrays: Number of array(s).
 * @param  delimiter: Separator.
 * @param  format: Item format.
 *         @arg "%d": Dec
 *         @arg "%f": Float
 *         @arg "%x": Hex
 *         @arg "%s": String
 *         @arg ...
 * @retval Char**
 */
char** multi_printf_arrays_direct(int num_arrays, const char* delimiter, const char* format, ...) {
    static char result[mpa_MAX_STR_LEN];  // ��̬�������洢���
    result[0] = '\0';  // ��ʼ��Ϊ���ַ���

    va_list args;
    va_start(args, format);

    char buffer[32];  // ��ʱ�洢��ʽ���������
    int first = 1;    // ���ڱ����׸�Ԫ��ǰ�ķָ���

    for (int i = 0; i < num_arrays; i++) {
        
        if (strcmp(format, "%f") == 0) {
            float* array = va_arg(args, float*);  // ������������
            int size = va_arg(args, int);

            for (int j = 0; j < size; j++) {
                if (!first) {
                    strcat(result, delimiter);
                }
                first = 0;
                snprintf(buffer, sizeof(buffer), format, array[j]);  // ��������
                strcat(result, buffer);
            }
        } else {
            int* array = va_arg(args, int*);  // ������������
            int size = va_arg(args, int);

            for (int j = 0; j < size; j++) {
                if (!first) {
                    strcat(result, delimiter);
                }
                first = 0;
                snprintf(buffer, sizeof(buffer), format, array[j]);  // ��������
                strcat(result, buffer);
            }
        }
    }

    va_end(args);
    return (char **) result;
}

int muti_printf_self_test() {
    int arr1[] = {1, 2, 3};
    int arr2[] = {4, 5};
    int arr3[] = {16, 17, 18, 19};
    float arr4[] = {1.2f, 2.3f};

    // ʾ����ʹ�ö�����Ϊ�ָ����������� %d ��ʽ���
    char* result = multi_printf_arrays(3, ",", "%d", arr1, 3, arr2, 2, arr3, 4);
    printf("ƴ�ӽ��: %s\r\n", result);

    // ʾ����ʹ�ÿո���Ϊ�ָ�����������ʮ�����Ƹ�ʽ���
    printf("ƴ�ӽ�� (ʮ������): %s\r\n", multi_printf_arrays_direct(3, " ", "0x%02X", arr1, 3, arr2, 2, arr3, 4));

    // ʾ����ʹ�ö�����Ϊ�ָ����������� %.3f ��ʽ���
    /* ��bug */
        // result = multi_printf_arrays(1, ",", "%.3f", arr4, 2);
    /* ---- */
    result = multi_printf_arrays(1, ",", "%f", arr4, 2);
    printf("ƴ�ӽ�� (float����):%s\r\n", result);

    for(int i = 0; i < 1000*100; i++);
    return 0;
}


#endif
// multi printf





