#include "convert_string.h"

// �ж��ַ����Ƿ�Ϊ��Ч������
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

// �ж��ַ�����ʾ�������Ƿ���ڵ��� target
int is_number_greater_than_or_equal_to_target(const char *str, uint32_t target) {
    if (!is_valid_number(str)) {
        return 0;
    }
    long num = strtol(str, NULL, 10);
    #include "atk_usmart.h"
    printf("num: %d, target: %d\r\n", num, target);
    return num >= target;
}

int my_string_to_int(char *str){
    for(int i = 0; str[i] != '\0'; i++){
        
    }
}

// ʾ������
void test_string_number_check(void) {
    char test_str1[] = "1001";
    char test_str2[] = "1000";
    char test_str3[] = "abc";

    printf("'%s' �Ƿ���ڵ��� 1001: %s\n", test_str1, is_number_greater_than_or_equal_to_target(test_str1, 1001) ? "��" : "��");
    printf("'%s' �Ƿ���ڵ��� 1001: %s\n", test_str2, is_number_greater_than_or_equal_to_target(test_str2, 1001) ? "��" : "��");
    printf("'%s' �Ƿ���ڵ��� 1001: %s\n", test_str3, is_number_greater_than_or_equal_to_target(test_str3, 1001) ? "��" : "��");
}

void int_to_string_with_snprintf(int inumber, char* istring) {
    // ʹ�� snprintf ������ת��Ϊ�ַ��������д�� sizeof(str) - 1 ���ַ�
    snprintf(istring, sizeof(istring), "%d", inumber);
}
