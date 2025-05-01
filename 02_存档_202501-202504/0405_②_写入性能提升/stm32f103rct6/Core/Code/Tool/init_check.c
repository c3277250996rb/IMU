#include "init_check.h"

static int hasRun = 0;  // 标志变量，初始值为 0

void checkFunction() {
    if (!hasRun) {
        printf("myFunction was NOT executed!\n");
    }
}

static void myFunction() {
    hasRun = 1;  // 标记函数已执行
    printf("myFunction was executed!\n");
}

#if 0
int main() {
    atexit(checkFunction);  // 在程序结束时调用 checkFunction
    // myFunction();  // 取消注释就不会打印 "NOT executed"
    return 0;
}
#endif
