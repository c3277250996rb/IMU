#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// �ж��ַ����Ƿ�Ϊ��Ч������
int is_valid_number(const char *str);

// �ж��ַ�����ʾ�������Ƿ���ڵ��� target
int is_number_greater_than_or_equal_to_target(const char *str, uint32_t target);

// ʾ������
void test_string_number_check(void);

void int_to_string_with_snprintf(int inumber, char* istring);
