#include "my_tool.h"

    #ifndef __Multi__Printf__Reference__
    #define __Multi__Printf__Reference__
    
        // head
        #include <stdio.h>
        #include <stdarg.h>
        #include <stdlib.h>
        #include <string.h>
        #define mpa_MAX_STR_LEN 1024  // ����ַ�����󳤶�
        uint8_t mp_BUFFER[512];  // ��ʱ�洢�ַ����Ļ�����
        // head

        /**
        * @brief 
        * 
        * @param format 
        * @param ... 
        */
        void my_print(const char *format, ...){
            va_list args;
            memset(mp_BUFFER, 0, sizeof(mp_BUFFER));  // ��� buffer��ȷ��û�в�������
            
            va_start(args, format);
            vsnprintf(mp_BUFFER, sizeof(mp_BUFFER), format, args);  // ����ʽ������д�� mp_BUFFER
            va_end(args);
            
            UART1_SendString(mp_BUFFER, sizeof(mp_BUFFER));
        }

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
        char* muti_printf_arrays(int num_arrays, const char* delimiter, const char* format, ...){
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

        void multi_printf_self_test(){
            int arr1[] = {1, 2, 3};
            int arr2[] = {4, 5};
            int arr3[] = {6, 7, 8, 9};

            // ʾ����ʹ�ö�����Ϊ�ָ����������� %d ��ʽ���
            char* result = muti_printf_arrays(3, ",", "%d", arr1, 3, arr2, 2, arr3, 4);
            my_print("ƴ�ӽ��: %s\r\n", result);

            DelayMs(1000);
        }

    #endif




