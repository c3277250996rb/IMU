#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 判断字符串是否为有效的数字
int is_valid_number(const char *str);

// 判断字符串表示的数字是否大于等于 target
int is_number_greater_than_or_equal_to_target(const char *str, uint32_t target);

// 示例调用
void test_string_number_check(void);

void int_to_string_with_snprintf(int inumber, char* istring);
