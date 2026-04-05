#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 全局变量   放在全局去（静态区）
int global_var = 100;
// 静态全局变量   也在全局区
static int static_global = 200;
// ========== 1. 演示内存分区 ==========
void demo_memory_regions(void)
{
    printf("========== 1. 内存分区演示 ==========\n");

    // 局部变量 —— 栈区
    int local_var = 10;
    static int static_local = 50; // 静态局部变量 —— 也在全局区

    // 字符串字面量 —— 常量区（只读）
    const char *str = "Hello"; // str在栈，指向常量区的"Hello"

    printf("代码区：存放程序指令，本函数代码在此区\n");
    printf("常量区：字符串字面量 \"Hello\" 的地址 = %p\n", (void *)str);
    printf("全局区：global_var 地址 = %p\n", (void *)&global_var);
    printf("全局区：static_global 地址 = %p\n", (void *)&static_global);
    printf("全局区：static_local 地址 = %p\n", (void *)&static_local);
    printf("栈区：local_var 地址 = %p\n", (void *)&local_var);
    printf("栈区：str 指针本身地址 = %p\n", (void *)&str);
    printf("堆区：稍后通过 malloc 分配，地址在堆中\n\n");
}
// ========== 2. 栈内存自动管理 & 栈溢出风险 ==========
void cause_stack_overflow(int depth)
{
    char buffer[1024];
    printf("");
    cause_stack_overflow(depth + 1); // 无限递归，很快溢出
}
void demo_stack(void)
{
    printf("========== 2. 栈内存特性 ==========\n");
    int x = 1, y = 2;
    printf("栈上变量 x 和 y 自动分配，函数返回时自动释放\n");
    printf("x 地址: %p, y 地址: %p\n", (void *)&x, (void *)&y);

    printf("\n【栈溢出风险】\n");
    printf("下面的递归调用会很快导致栈溢出，程序崩溃。\n");
    printf("是否运行？输入 1 运行（危险，会崩溃），其他键跳过: ");
    int choice;
    scanf("%d", &choice);
    if (choice == 1)
    {
        cause_stack_overflow(1); // 危险，不要轻易运行
    }
    else
    {
        printf("已跳过栈溢出演示。\n");
    }
    printf("\n");
}
// ========== 3. 堆内存手动管理 ==========
void demo_heap_allocation(void)
{
    printf("========== 3. 堆内存分配与释放 ==========\n");

    // 1. malloc：分配指定字节数，内容未初始化
    int *p1 = (int *)malloc(5 * sizeof(int));
    if (p1 == NULL)
    {
        printf("malloc 失败\n");
        return;
    }
    printf("malloc 分配 5 个 int，地址: %p\n", (void *)p1);

    // 2. calloc：分配并初始化为 0
    int *p2 = (int *)calloc(5, sizeof(int));
    if (p2 == NULL)
    {
        printf("calloc 失败\n");
        free(p1);
        return;
    }
    printf("calloc 分配 5 个 int，初始为 0: ");
    for (int i = 0; i < 5; i++)
        printf("%d ", p2[i]);
    printf("\n");

    // 3. realloc：调整已分配内存大小
    int *p3 = (int *)realloc(p1, 10 * sizeof(int));
    if (p3 == NULL)
    {
        printf("realloc 失败，原内存保留\n");
        free(p1);
    }
    else
    {
        p1 = p3; // 重新指向新内存
        printf("realloc 扩大到 10 个 int，新地址: %p\n", (void *)p1);
    }

    // 使用内存
    for (int i = 0; i < 10; i++)
        p1[i] = i;

    // 4. free：释放堆内存
    free(p1); // 释放 malloc/realloc 分配的内存
    free(p2);
    printf("堆内存已释放。\n\n");
}
// ========== 3. 堆内存手动管理 ==========
void demo_heap_allocation(void)
{
    printf("========== 3. 堆内存分配与释放 ==========\n");

    // 1. malloc：分配指定字节数，内容未初始化
    int *p1 = (int *)malloc(5 * sizeof(int));
    if (p1 == NULL)
    {
        printf("malloc 失败\n");
        return;
    }
    printf("malloc 分配 5 个 int，地址: %p\n", (void *)p1);

    // 2. calloc：分配并初始化为 0
    int *p2 = (int *)calloc(5, sizeof(int));
    if (p2 == NULL)
    {
        printf("calloc 失败\n");
        free(p1);
        return;
    }
    printf("calloc 分配 5 个 int，初始为 0: ");
    for (int i = 0; i < 5; i++)
        printf("%d ", p2[i]);
    printf("\n");

    // 3. realloc：调整已分配内存大小
    int *p3 = (int *)realloc(p1, 10 * sizeof(int));
    if (p3 == NULL)
    {
        printf("realloc 失败，原内存保留\n");
        free(p1);
    }
    else
    {
        p1 = p3; // 重新指向新内存
        printf("realloc 扩大到 10 个 int，新地址: %p\n", (void *)p1);
    }

    // 使用内存
    for (int i = 0; i < 10; i++)
        p1[i] = i;

    // 4. free：释放堆内存
    free(p1); // 释放 malloc/realloc 分配的内存
    free(p2);
    printf("堆内存已释放。\n\n");
}
// ========== 4. 内存泄漏与重复释放规避 ==========
void demo_memory_leak_and_double_free(void)
{
    printf("========== 4. 内存泄漏与重复释放规避 ==========\n");

    // 内存泄漏示例（演示后注释掉，避免实际泄漏）
    printf("【内存泄漏】\n");
    printf("如果分配了内存却不 free，程序结束前操作系统会回收，\n");
    printf("但长期运行的程序（如服务器）会导致内存耗尽。\n");
    int *leak = (int *)malloc(100 * sizeof(int));
    // 忘记 free(leak);   // 泄漏！
    printf("已分配但未释放的内存: %p（演示后程序结束，系统回收）\n", (void *)leak);
    // 实际开发中必须 free(leak);

    printf("\n【重复释放】\n");
    int *ptr = (int *)malloc(10 * sizeof(int));
    if (ptr == NULL)
        return;
    printf("分配 ptr: %p\n", (void *)ptr);
    free(ptr);
    // free(ptr);   // 重复释放会导致未定义行为（通常崩溃）
    printf("第一次释放成功。第二次释放（注释掉了）会崩溃。\n");

    printf("\n【规避方法】\n");
    printf("1. 分配后记得 free，成对出现。\n");
    printf("2. free 后立即将指针设为 NULL：free(ptr); ptr = NULL;\n");
    printf("3. 检查指针不为 NULL 再 free。\n");

    // 正确示范
    int *safe = (int *)malloc(20 * sizeof(int));
    if (safe != NULL)
    {
        // 使用 safe ...
        free(safe);
        safe = NULL; // 避免野指针
    }
    if (safe != NULL)
    {
        free(safe); // 不会执行，安全
    }
    printf("安全释放后置 NULL，再次 free 无害。\n");
}
int main(void)
{
    demo_memory_regions();
    demo_stack();
    demo_heap_allocation();
    demo_memory_leak_and_double_free();
    return 0;
}