/**
 * pointer_intro.c
 *
 * 专门讲解 C 语言中的指针和取地址运算符（&）
 *
 * 编译：gcc -Wall -std=c99 -o pointer_intro pointer_intro.c
 * 运行：./pointer_intro
 */

#include <stdio.h>

int main(void)
{
    /* ========== 1. 取地址运算符 & ========== */
    printf("========== 1. 取地址运算符 & ==========\n");

    int var = 42; // 普通变量
    printf("变量 var 的值: %d\n", var);
    printf("变量 var 的地址: %p\n", (void *)&var); // &var 得到 var 的地址

    // 注意：每次运行程序，地址值可能不同，这是正常的。

    /* ========== 2. 指针变量：存储地址的变量 ========== */
    printf("\n========== 2. 指针变量 ==========\n");

    int *ptr;   // 声明一个指向 int 的指针变量，ptr 本身也是一个变量
    ptr = &var; // 把 var 的地址赋值给 ptr
    printf("ptr 中存储的地址: %p\n", (void *)ptr);
    printf("ptr 自己的地址: %p\n", (void *)&ptr); // 指针变量也有自己的地址

    /* ========== 3. 解引用运算符 * ========== */
    printf("\n========== 3. 解引用运算符 * ==========\n");

    // 通过指针访问它指向的变量
    printf("通过 ptr 访问 var 的值: %d\n", *ptr); // *ptr 等同于 var
    *ptr = 100;                                   // 修改 ptr 指向的变量（即 var）
    printf("修改 *ptr 后，var 的值变为: %d\n", var);

    /* ========== 4. 指针的常见误区 ========== */
    printf("\n========== 4. 指针声明与赋值的注意点 ==========\n");

    // 误区1：声明多个指针时，每个变量前都需要 *
    int a, b, *p, *q; // a,b 是普通 int，p,q 是指向 int 的指针
    // 错误写法：int* p, q;  这里 p 是指针，q 是普通 int

    // 误区2：未初始化的指针（野指针）
    int *wild; // wild 指向未知内存
    // *wild = 10;      // 危险！野指针赋值会导致程序崩溃或未定义行为
    printf("永远不要使用未初始化的指针！\n");

    // 正确：让指针指向一个合法的变量或分配的内存
    int safe_var = 5;
    int *safe_ptr = &safe_var;
    printf("安全指针示例：*safe_ptr = %d\n", *safe_ptr);

    /* ========== 5. 指针与数组 ========== */
    printf("\n========== 5. 指针与数组 ==========\n");

    int arr[5] = {10, 20, 30, 40, 50};
    printf("数组名 arr 本身就是指向第一个元素的指针: %p\n", (void *)arr);
    printf("数组第一个元素的地址 &arr[0]: %p\n", (void *)&arr[0]);
    printf("两者相等: %s\n", (arr == &arr[0]) ? "是" : "否");

    // 通过指针访问数组元素
    int *arr_ptr = arr;                            // 等价于 int *arr_ptr = &arr[0];
    printf("arr_ptr[0] = %d\n", arr_ptr[0]);       // 10
    printf("*arr_ptr = %d\n", *arr_ptr);           // 10
    printf("*(arr_ptr+1) = %d\n", *(arr_ptr + 1)); // 20

    // 指针算术：指针加 n，实际地址增加 n * sizeof(类型)
    printf("\n指针算术示例：\n");
    printf("arr_ptr 地址: %p\n", (void *)arr_ptr);
    printf("arr_ptr+1 地址: %p (增加了 %zu 字节)\n", (void *)(arr_ptr + 1), sizeof(int));

    // 遍历数组的两种方式
    printf("方式1（下标）: ");
    for (int i = 0; i < 5; i++)
        printf("%d ", arr[i]);
    printf("\n");

    printf("方式2（指针）: ");
    for (int *p = arr; p < arr + 5; p++)
        printf("%d ", *p);
    printf("\n");

    /* ========== 6. 指针与字符串 ========== */
    printf("\n========== 6. 指针与字符串 ==========\n");

    char str[] = "Hello"; // 字符数组
    char *str_ptr = str;  // 指向字符串首字符
    printf("字符串: %s\n", str_ptr);
    while (*str_ptr != '\0')
    { // 遍历直到结束符
        printf("字符: %c, 地址: %p\n", *str_ptr, (void *)str_ptr);
        str_ptr++;
    }

    /* ========== 7. 指针的常见用途：修改函数外部变量 ========== */
    printf("\n========== 7. 指针作为函数参数（传递地址） ==========\n");

    // 定义一个交换函数（通过指针实现）
    void swap(int *x, int *y)
    {
        int temp = *x;
        *x = *y;
        *y = temp;
    }

    int u = 3, v = 7;
    printf("交换前: u=%d, v=%d\n", u, v);
    swap(&u, &v);
    printf("交换后: u=%d, v=%d\n", u, v);

    /* ========== 8. 空指针 NULL ========== */
    printf("\n========== 8. 空指针 NULL ==========\n");

    int *null_ptr = NULL; // 指向“空”的指针，不指向任何有效内存
    if (null_ptr == NULL)
    {
        printf("这是一个空指针，不能解引用！\n");
    }
    // printf("%d", *null_ptr);     // 错误：解引用空指针会导致崩溃

    /* ========== 9. 总结 ========== */
    printf("\n========== 总结 ==========\n");
    printf("1. & 取地址，得到变量的内存地址。\n");
    printf("2. * 在声明中表示“指针”，在表达式中表示“解引用”（取指针指向的值）。\n");
    printf("3. 数组名可以作为指向第一个元素的指针。\n");
    printf("4. 指针必须初始化后再使用，否则是野指针。\n");
    printf("5. 指针算术移动的字节数取决于它指向的类型。\n");

    return 0;
}