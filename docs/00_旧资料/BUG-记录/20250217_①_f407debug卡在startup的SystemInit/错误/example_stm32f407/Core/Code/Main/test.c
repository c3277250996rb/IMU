
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 1024  // 结果字符串最大长度

// 拼接多个数组的元素
// char* concat_arrays(int num_arrays, const char* delimiter, const char* format, ...) {
char* concat_arrays(int num_arrays, const char* delimiter, const char* format, ...) {
    static char result[MAX_STR_LEN];  // 静态缓冲区存储结果
    result[0] = '\0';  // 初始化为空字符串

    va_list args;
    va_start(args, format);

    char buffer[32];  // 临时存储格式化后的数据
    int first = 1;    // 用于避免首个元素前的分隔符

    for (int i = 0; i < num_arrays; i++) {
        int* array = va_arg(args, int*);  // 获取数组指针
        int size = va_arg(args, int);     // 获取数组大小

        for (int j = 0; j < size; j++) {
            if (!first) {
                strcat(result, delimiter);  // 添加分隔符
            }
            first = 0;
            snprintf(buffer, sizeof(buffer), format, array[j]);  // 根据传入的格式字符串格式化整数
            strcat(result, buffer);  // 追加到结果字符串
        }
    }

    strcat(result, "\r\n");  // 追加到结果字符串
    va_end(args);
    return result;
}

int my_test() {
    int arr1[] = {1, 2, 3};
    int arr2[] = {4, 5};
    int arr3[] = {6, 7, 8, 9};

    // 示例：使用逗号作为分隔符，数字以 %d 格式输出
    char* result = concat_arrays(3, ",", "%d", arr1, 3, arr2, 2, arr3, 4);
    printf("拼接结果: %s\n", result);

    // 示例：使用空格作为分隔符，数字以十六进制格式输出
    result = concat_arrays(3, " ", "%X", arr1, 3, arr2, 2, arr3, 4);
    printf("拼接结果 (十六进制): %s\n", result);

    return 0;
}


