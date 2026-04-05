/*
 *
 * 知识点：
 * 1. 数组名的本质：数组首元素的常量指针
 * 2. 用指针遍历一维 / 二维数组，指针算术运算（+/-/++/--）
 * 3. 指针与字符串的关系，用字符指针处理字符串
 * 4. 指针数组（存储指针的数组）的适用场景
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ========== 1. 传值 vs 传址 ==========
void swap_by_value(int a, int b)
{
    int temp = a;
    a = b;
    b = temp;
    // 交换只在函数内部有效，外部实参不受影响
}

void swap_by_pointer(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
    // 通过指针直接修改外部变量
}

void demo_pass_by_value_vs_address(void)
{
    printf("\n========== 1. 传值调用 vs 传址调用 ==========\n");
    int x = 5, y = 10;
    printf("原始值: x = %d, y = %d\n", x, y);

    swap_by_value(x, y);
    printf("传值调用后: x = %d, y = %d (未交换)\n", x, y);

    swap_by_pointer(&x, &y);
    printf("传址调用后: x = %d, y = %d (已交换)\n", x, y);

    printf("\n【必用传址的场景】\n");
    printf("- 需要修改函数外部的变量（如交换、自增等）\n");
    printf("- 需要返回多个结果（例如同时返回商和余数）\n");
    printf("- 传递大块数据（数组、结构体）避免拷贝开销\n");
}

// ========== 2. 指针作为参数：修改外部变量 + 传递数组 ==========
void modify_value(int *val)
{
    *val = 100; // 修改外部变量
}

int sum_array(int *arr, int len)
{
    int sum = 0;
    for (int i = 0; i < len; i++)
    {
        sum += arr[i]; // 等价于 *(arr + i)
    }
    return sum;
}

void demo_pointer_as_parameter(void)
{
    printf("\n========== 2. 指针作为函数参数 ==========\n");
    int num = 10;
    modify_value(&num);
    printf("修改后的值: %d\n", num);

    int arr[] = {1, 2, 3, 4, 5};
    int len = sizeof(arr) / sizeof(arr[0]);
    int total = sum_array(arr, len); // 数组名自动退化为指针
    printf("数组元素和: %d\n", total);
}

// ========== 3. 函数指针：回调函数 ==========
// 一个简单的加法函数
int add(int a, int b)
{
    return a + b;
}

// 一个简单的减法函数
int subtract(int a, int b)
{
    return a - b;
}

// 回调函数：接收一个函数指针作为参数
int compute(int x, int y, int (*op)(int, int))
{
    return op(x, y); // 调用传入的函数
}

void demo_function_pointer(void)
{
    printf("\n========== 3. 函数指针与回调函数 ==========\n");
    int a = 20, b = 8;

    // 定义函数指针并初始化
    int (*func_ptr)(int, int) = add;
    printf("add: %d + %d = %d\n", a, b, func_ptr(a, b));

    func_ptr = subtract;
    printf("subtract: %d - %d = %d\n", a, b, func_ptr(a, b));

    // 回调示例
    printf("使用回调计算加法: %d\n", compute(a, b, add));
    printf("使用回调计算减法: %d\n", compute(a, b, subtract));
}

// ========== 4. 函数指针数组：替代 switch-case ==========
// 定义四个动作函数
void forward(void) { printf("执行动作: 前进\n"); }
void backward(void) { printf("执行动作: 后退\n"); }
void turn_left(void) { printf("执行动作: 左转\n"); }
void turn_right(void) { printf("执行动作: 右转\n"); }
void stop(void) { printf("执行动作: 停止\n"); }

void demo_function_pointer_array(void)
{
    printf("\n========== 4. 函数指针数组替代 switch-case ==========\n");
    // 函数指针数组：每个元素是一个无参无返回值的函数指针
    void (*actions[])(void) = {forward, backward, turn_left, turn_right, stop};
    const char *cmd_names[] = {"前进", "后退", "左转", "右转", "停止"};
    int num_actions = sizeof(actions) / sizeof(actions[0]);

    // 模拟收到指令索引（例如从串口读取 0~4）
    int command_index = 2; // 假设收到“左转”
    if (command_index >= 0 && command_index < num_actions)
    {
        printf("收到指令: %s\n", cmd_names[command_index]);
        actions[command_index](); // 直接通过数组调用对应函数
    }
    else
    {
        printf("无效指令\n");
    }

    // 演示遍历所有动作
    printf("\n遍历所有动作:\n");
    for (int i = 0; i < num_actions; i++)
    {
        printf("%d -> ", i);
        actions[i]();
    }
}

// ========== 1. 二级指针的基本概念 ==========
void demo_basic(void)
{
    printf("========== 1. 二级指针的定义与初始化 ==========\n");

    int a = 10;
    int *p = &a;   // 一级指针，指向变量 a
    int **pp = &p; // 二级指针，指向指针 p

    printf("a 的值: %d\n", a);
    printf("a 的地址: %p\n", (void *)&a);
    printf("p 中存的地址: %p (即 &a)\n", (void *)p);
    printf("p 自己的地址: %p\n", (void *)&p);
    printf("pp 中存的地址: %p (即 &p)\n", (void *)pp);
    printf("通过 pp 两次解引用得到 a 的值: %d\n\n", **pp);
    // 本质：二级指针就是“存放一级指针地址”的变量
}

// ========== 2. 通过二级指针修改一级指针的指向 ==========
void demo_modify_pointer(void)
{
    printf("========== 2. 通过二级指针修改一级指针的指向 ==========\n");

    int x = 10, y = 20;
    int *ptr = &x; // ptr 指向 x
    printf("初始: ptr 指向 %d (值=%d)\n", *ptr, *ptr);

    int **pptr = &ptr; // pptr 指向 ptr

    // 通过 pptr 修改 ptr 的指向，让它指向 y
    *pptr = &y; // 等价于 ptr = &y
    printf("修改后: ptr 指向 %d (值=%d)\n", *ptr, *ptr);

    // 也可以通过 *pptr 修改原变量的值
    **pptr = 99; // 等价于 *ptr = 99，即修改 y = 99
    printf("通过 **pptr 修改 y 的值: y = %d\n", y);
    printf("\n");
}

// ========== 3. 场景1：函数内修改一级指针的指向 ==========
void allocate_memory(int **pp, int size)
{
    // 在函数内为一级指针分配内存，并让它指向新内存
    *pp = (int *)malloc(size * sizeof(int));
    if (*pp != NULL)
    {
        (*pp)[0] = 100;
    }
}

void demo_modify_pointer_in_function(void)
{
    printf("========== 3. 场景1：函数内修改一级指针的指向 ==========\n");
    int *arr = NULL; // 一级指针初始为空

    // 调用函数，传入二级指针，让函数修改 arr 的指向
    allocate_memory(&arr, 5);
    if (arr != NULL)
    {
        printf("arr 指向了动态内存，第一个元素: %d\n", arr[0]);
        free(arr); // 释放内存
    }
    printf("\n");
    // 说明：如果只传一级指针，函数内修改的只是形参的副本，外部指针不会改变
}

// ========== 4. 场景2：处理指针数组（字符串数组） ==========
void demo_string_array(void)
{
    printf("========== 4. 场景2：处理指针数组（字符串数组） ==========\n");

    // 指针数组：每个元素是一个字符指针（指向字符串）
    const char *strs[] = {"Hello", "World", "C", "Pointer"};
    int count = sizeof(strs) / sizeof(strs[0]);

    // 定义一个二级指针，指向指针数组的第一个元素
    const char **p = strs; // 等价于 &strs[0]

    printf("通过二级指针遍历字符串数组:\n");
    for (int i = 0; i < count; i++)
    {
        printf("strs[%d] = %s\n", i, *(p + i)); // 或者 p[i]
    }

    // 二级指针常用于函数参数，接收指针数组
    // 例如：void print_strings(const char **arr, int n);
    printf("\n");
}

// 辅助函数：接收二级指针作为参数，处理指针数组
void print_strings(const char **arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%s ", arr[i]);
    }
    printf("\n");
}

void demo_pointer_array_in_function(void)
{
    printf("二级指针作为函数参数，处理指针数组:\n");
    const char *fruits[] = {"Apple", "Banana", "Cherry"};
    int n = sizeof(fruits) / sizeof(fruits[0]);
    print_strings(fruits, n); // 数组名退化为指向首元素的指针（二级指针）
}
void task3_sort_strings(void)
{
    printf("========== 任务3：二级指针实现字符串排序 ==========\n");

    // 待排序的字符串数组
    char *fruits[] = {"Banana", "Apple", "Cherry", "Date", "Blueberry"};
    int n = sizeof(fruits) / sizeof(fruits[0]);

    printf("排序前：");
    for (int i = 0; i < n; i++)
    {
        printf("%s ", fruits[i]);
    }
    printf("\n");

    // ==================== 任务3：用二级指针实现字符串排序 ====================
    // 比较函数（用于 qsort）
    int compare_strings(const void *a, const void *b)
    {
        // a 和 b 是指向指针的指针（二级指针），因为数组元素是 char*
        const char *str1 = *(const char **)a;
        const char *str2 = *(const char **)b;
        return strcmp(str1, str2);
    }
    // 使用标准库 qsort，传入数组名（二级指针），元素个数，元素大小，比较函数
    qsort(fruits, n, sizeof(char *), compare_strings);

    printf("排序后：");
    for (int i = 0; i < n; i++)
    {
        printf("%s ", fruits[i]);
    }
    printf("\n");
    // 注意：这里只交换了指针，并没有移动字符串本身，效率高
}

int main(void)
{
    // ========== 1. 数组名是首元素的常量指针 ==========
    printf("========== 1. 数组名是常量指针 ==========\n");
    int arr[5] = {10, 20, 30, 40, 50};
    printf("arr 的值（首地址）: %p\n", (void *)arr);
    printf("&arr[0] 的值       : %p\n", (void *)&arr[0]);
    printf("两者相等，且 arr 不能赋值（arr = something; 是错的）\n\n");

    // ========== 2. 指针遍历一维数组 + 指针算术 ==========
    printf("========== 2. 指针遍历一维数组 ==========\n");
    int *p = arr; // p 指向 arr[0]
    for (int i = 0; i < 5; i++)
    {
        printf("*(p+%d) = %d  ", i, *(p + i)); // 指针加法：移动 i 个 int
    }
    printf("\n");
    // 演示 ++ 和 --
    printf("使用 p++ 遍历: ");
    p = arr; // 重置指针
    while (p < arr + 5)
    {
        printf("%d ", *p);
        p++; // 指向下一个元素
    }
    printf("\n\n");

    // ========== 3. 指针遍历二维数组 ==========
    printf("========== 3. 指针遍历二维数组 ==========\n");
    int matrix[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int *q = &matrix[0][0]; // 二维数组在内存中也是连续存放
    printf("将二维数组当作一维数组遍历: ");
    for (int i = 0; i < 6; i++)
    {
        printf("%d ", *(q + i));
    }
    printf("\n");
    // 用行指针遍历（了解即可）
    int (*row)[3] = matrix; // row 是指向含3个int的一维数组的指针
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("matrix[%d][%d] = %d  ", i, j, *(*(row + i) + j));
        }
        printf("\n");
    }
    printf("\n");

    // ========== 4. 指针与字符串 ==========
    printf("========== 4. 指针与字符串 ==========\n");
    char str[] = "Hello"; // 字符数组，可修改
    char *str_ptr = str;  // 指向字符串首字符
    printf("用指针遍历字符串: ");
    while (*str_ptr != '\0')
    { // 遇到结束符停止
        putchar(*str_ptr);
        str_ptr++;
    }
    printf("\n");
    // 字符串字面量（只读）
    const char *msg = "World"; // 指向只读字符串，不能修改 *msg
    printf("msg 指向: %s\n", msg);
    // msg[0] = 'w';            // 错误，只读
    printf("\n");

    // ========== 5. 指针数组（存储指针的数组） ==========
    printf("========== 5. 指针数组（存储指针的数组） ==========\n");
    // 场景：存储多个字符串
    const char *commands[] = {"FWD", "BWD", "LEFT", "RIGHT", "STOP"};
    int num_cmds = sizeof(commands) / sizeof(commands[0]);
    printf("指令列表: ");
    for (int i = 0; i < num_cmds; i++)
    {
        printf("%s ", commands[i]); // commands[i] 是一个 const char* 指针
    }
    printf("\n");
    // 指针数组的另一个场景：动态管理多个变量地址
    int a = 10, b = 20, c = 30;
    int *ptrs[] = {&a, &b, &c};
    for (int i = 0; i < 3; i++)
    {
        printf("*ptrs[%d] = %d\n", i, *ptrs[i]);
    }
    demo_pass_by_value_vs_address();
    demo_pointer_as_parameter();
    demo_function_pointer();
    demo_function_pointer_array();
    demo_basic();
    demo_modify_pointer();
    demo_modify_pointer_in_function();
    demo_string_array();
    demo_pointer_array_in_function();
    task3_sort_strings();
    return 0;
}

// 安全的写法：
// printf("ptr指向: %s\n", ptr ? ptr : "(null)");
// 相当于：
/*if (ptr != NULL)
{
    ptr
}
else
{
    "(null)"
}
*/