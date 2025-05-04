#include "my_string.h"


// multi printf
#if 1

    #include <stdio.h>
    #include <stdarg.h>
    #include <stdlib.h>
    #include <string.h>

    #define mpa_MAX_STR_LEN 1024  // 结果字符串最大长度


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
        static char result[mpa_MAX_STR_LEN];  // 静态缓冲区存储结果
        result[0] = '\0';  // 初始化为空字符串

        va_list args;
        va_start(args, format);

        char buffer[32];  // 临时存储格式化后的数据
        int first = 1;    // 用于避免首个元素前的分隔符

        for (int i = 0; i < num_arrays; i++) {
            
            if (strcmp(format, "%f") == 0) {
                float* array = va_arg(args, float*);  // 处理浮点数数组
                int size = va_arg(args, int);

                for (int j = 0; j < size; j++) {
                    if (!first) {
                        strcat(result, delimiter);
                    }
                    first = 0;
                    snprintf(buffer, sizeof(buffer), format, array[j]);  // 处理浮点数
                    strcat(result, buffer);
                }
            } else {
                int* array = va_arg(args, int*);  // 处理整数数组
                int size = va_arg(args, int);

                for (int j = 0; j < size; j++) {
                    if (!first) {
                        strcat(result, delimiter);
                    }
                    first = 0;
                    snprintf(buffer, sizeof(buffer), format, array[j]);  // 处理整数
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
        static char result[mpa_MAX_STR_LEN];  // 静态缓冲区存储结果
        result[0] = '\0';  // 初始化为空字符串

        va_list args;
        va_start(args, format);

        char buffer[32];  // 临时存储格式化后的数据
        int first = 1;    // 用于避免首个元素前的分隔符

        for (int i = 0; i < num_arrays; i++) {
            
            if (strcmp(format, "%f") == 0) {
                float* array = va_arg(args, float*);  // 处理浮点数数组
                int size = va_arg(args, int);

                for (int j = 0; j < size; j++) {
                    if (!first) {
                        strcat(result, delimiter);
                    }
                    first = 0;
                    snprintf(buffer, sizeof(buffer), format, array[j]);  // 处理浮点数
                    strcat(result, buffer);
                }
            } else {
                int* array = va_arg(args, int*);  // 处理整数数组
                int size = va_arg(args, int);

                for (int j = 0; j < size; j++) {
                    if (!first) {
                        strcat(result, delimiter);
                    }
                    first = 0;
                    snprintf(buffer, sizeof(buffer), format, array[j]);  // 处理整数
                    strcat(result, buffer);
                }
            }
        }

        va_end(args);
        return (char **) result;
    }

    int muti_printf_self_test(void) {
        int arr1[] = {1, 2, 3};
        int arr2[] = {4, 5};
        int arr3[] = {16, 17, 18, 19};
        float arr4[] = {1.2f, 2.3f};

        // 示例：使用逗号作为分隔符，数字以 %d 格式输出
        char* result = multi_printf_arrays(3, ",", "%d", arr1, 3, arr2, 2, arr3, 4);
        printf("拼接结果: %s\r\n", result);

        // 示例：使用空格作为分隔符，数字以十六进制格式输出
        printf("拼接结果 (十六进制): %s\r\n", multi_printf_arrays_direct(3, " ", "0x%02X", arr1, 3, arr2, 2, arr3, 4));

        // 示例：使用逗号作为分隔符，数字以 %.3f 格式输出
        /* 有bug */
            // result = multi_printf_arrays(1, ",", "%.3f", arr4, 2);
        /* ---- */
        result = multi_printf_arrays(1, ",", "%f", arr4, 2);
        printf("拼接结果 (float数组):%s\r\n", result);

        for(int i = 0; i < 1000*100; i++);
        return 0;
    }

#endif
// multi printf



// my fat 里用到的字符串和数字的函数
#if 1

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <ctype.h>

    // 判断字符串是否为有效的数字
    int is_valid_number(const char *str) {
        if (str == NULL || *str == '\0') {
            return 0;
        }
        for (int i = 0; str[i] != '\0'; i++) {
            if (!isdigit(str[i])) {
                return 0;
            }
        }
        return 1;
    }

    // 判断字符串表示的数字是否大于等于 target
    int is_number_greater_than_or_equal_to_target(const char *str, uint32_t target) {
        if (!is_valid_number(str)) {
            return 0;
        }
        long num = strtol(str, NULL, 10);
        // printf("num: %d, target: %d\r\n", num, target);
        return num >= target;
    }

    int my_string_to_int(char *str){
        for(int i = 0; str[i] != '\0'; i++){
            
        }
    }

    // 示例调用
    void test_string_number_check(void) {
        char test_str1[] = "1001";
        char test_str2[] = "1000";
        char test_str3[] = "abc";

        printf("'%s' 是否大于等于 1001: %s\n", test_str1, is_number_greater_than_or_equal_to_target(test_str1, 1001) ? "是" : "否");
        printf("'%s' 是否大于等于 1001: %s\n", test_str2, is_number_greater_than_or_equal_to_target(test_str2, 1001) ? "是" : "否");
        printf("'%s' 是否大于等于 1001: %s\n", test_str3, is_number_greater_than_or_equal_to_target(test_str3, 1001) ? "是" : "否");
    }

    void int_to_string_with_snprintf(int inumber, char* istring) {
        // 使用 snprintf 将整数转换为字符串，最多写入 sizeof(str) - 1 个字符
        snprintf(istring, sizeof(istring), "%d", inumber);
    }

#endif
// my fat 里用到的字符串和数字的函数


