#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//全局变量   放在全局去（静态区）
int global_var = 100;
//静态全局变量   也在全局区
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
void cause_stack_overflow(int depth){
    char buffer[1024];
    printf("");
    cause_stack_overflow(depth + 1);//无限递归，很快溢出
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