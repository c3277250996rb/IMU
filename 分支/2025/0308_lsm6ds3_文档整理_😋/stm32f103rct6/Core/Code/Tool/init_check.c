#include "init_check.h"

static int hasRun = 0;  // ��־��������ʼֵΪ 0

void checkFunction() {
    if (!hasRun) {
        printf("myFunction was NOT executed!\n");
    }
}

static void myFunction() {
    hasRun = 1;  // ��Ǻ�����ִ��
    printf("myFunction was executed!\n");
}

#if 0
int main() {
    atexit(checkFunction);  // �ڳ������ʱ���� checkFunction
    // myFunction();  // ȡ��ע�;Ͳ����ӡ "NOT executed"
    return 0;
}
#endif
