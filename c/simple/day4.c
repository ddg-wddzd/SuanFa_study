/**
 * loop_demo.c
 *
 * 知识点：
 * 1. for 循环：语法、执行流程、循环变量控制、嵌套循环
 * 2. while / do-while 循环及三种循环的适用场景
 * 3. break / continue / goto 的用法与使用禁忌
 * 4. 循环边界条件控制，避免死循环
 *
 * 编译：gcc -Wall -std=c99 -o loop_demo loop_demo.c
 * 运行：./loop_demo
 */

#include <stdio.h>
#include <string.h>

/* ==================== 1. for 循环 ==================== */
void demo_for(void)
{
    printf("\n========== 1. for 循环 ==========\n");

    // 基本 for 循环：打印 0~4
    printf("基本 for: ");
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", i);
    }
    printf("\n");

    // 循环变量控制：递减、步长不为1
    printf("递减步长2: ");
    for (int i = 10; i >= 0; i -= 2)
    {
        printf("%d ", i);
    }
    printf("\n");

    // 嵌套循环：打印乘法表
    printf("乘法表（9x9）:\n");
    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            printf("%d*%d=%-2d ", i, j, i * j);
        }
        printf("\n");
    }

    // 多个循环变量控制（C99允许）
    printf("多个循环变量: ");
    for (int a = 0, b = 5; a < b; a++, b--)
    {
        printf("(%d,%d) ", a, b);
    }
    printf("\n");
}

/* ==================== 2. while / do-while 及场景 ==================== */
void demo_while_do(void)
{
    printf("\n========== 2. while / do-while 循环 ==========\n");

    // while 循环：先判断后执行
    int count = 0;
    while (count < 3)
    {
        printf("while 迭代 %d\n", count);
        count++;
    }

    // do-while：至少执行一次
    int x = 10;
    do
    {
        printf("do-while 执行了一次，x=%d\n", x);
    } while (x < 5); // 条件为假，但仍会执行一次循环体

    /* ---------- 三种循环的适用场景 ---------- */
    // 1. for：已知循环次数（遍历数组、固定范围）
    // 2. while：未知次数，条件控制（读文件直到EOF、等待用户输入）
    // 3. do-while：至少需要执行一次（菜单程序、密码重试）

    // 典型 while 场景：读取输入直到遇到特定值
    int num = 0;
    printf("while 场景：输入负数退出: ");
    while (num >= 0)
    {
        scanf("%d", &num);
    }
    // 注意：上面的循环会多读一次，实际使用时通常需要更精细的控制，这里仅为演示

    // 典型 do-while 场景：密码验证（至少输入一次）
    int pwd;
    do
    {
        printf("请输入密码（do-while）: ");
        scanf("%d", &pwd);
    } while (pwd != 123456);
    printf("密码正确！\n");
}

/* ==================== 3. break / continue / goto ==================== */
void demo_break_continue_goto(void)
{
    printf("\n========== 3. break / continue / goto ==========\n");

    // break：跳出当前循环（或 switch）
    printf("break 示例: ");
    for (int i = 0; i < 10; i++)
    {
        if (i == 5)
            break; // i=5 时跳出循环
        printf("%d ", i);
    }
    printf("（循环在 i=5 时终止）\n");

    // continue：跳过本次循环剩余部分，进入下一次迭代
    printf("continue 示例（跳过偶数）: ");
    for (int i = 0; i < 8; i++)
    {
        if (i % 2 == 0)
            continue;
        printf("%d ", i);
    }
    printf("（只输出奇数）\n");

    // goto：无条件跳转（谨慎使用，容易破坏结构）
    printf("goto 示例: ");
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (i == 2 && j == 2)
            {
                goto exit_loop; // 直接跳出多层循环
            }
            printf("(%d,%d) ", i, j);
        }
    }
exit_loop:
    printf("—— 通过 goto 跳出双层循环\n");

    /* ---------- 使用禁忌 ---------- */
    // - break 只能跳出最内层的 switch/循环，不能跳出多层循环（此时可用 goto 或标志变量）
    // - continue 在 while/do-while 中容易造成死循环（注意更新条件的位置）
    // - goto 不要滥用，仅在跳出深层嵌套或统一错误处理时使用，否则会降低可读性

    // 反例：while 中 continue 导致死循环（演示并注释）
    int k = 0;
    while (k < 5)
    {
        // k++;   // 若更新写在这里，则 continue 会导致无法递增 -> 死循环
        if (k == 2)
        {
            k++; // 必须确保条件变量更新
            continue;
        }
        printf("%d ", k);
        k++;
    }
    printf("（正确的 continue 使用）\n");
}

/* ==================== 4. 循环边界条件与死循环规避 ==================== */
void demo_boundary(void)
{
    printf("\n========== 4. 循环边界条件与死循环规避 ==========\n");

    // 边界错误示例：数组越界访问（逻辑错误，可能不崩溃但数据错误）
    int arr[5] = {10, 20, 30, 40, 50};
    printf("正确遍历: ");
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
    // 错误写法: for (int i = 0; i <= 5; i++)  // 越界访问 arr[5]

    // 死循环常见原因与避免
    // 1. 条件永远为真
    // while (1) { ... }   // 需要内部 break 或修改条件

    // 2. 循环变量不更新
    // for (int i = 0; i < 10; ) { ... }  // 忘记 i++

    // 3. 浮点数精度导致条件不满足
    // for (float f = 0.0f; f != 1.0f; f += 0.1f)  // 可能永远不等于1

    // 安全边界检查示例：使用 < 而不是 <=，使用 size_t 类型
    size_t len = 5;
    for (size_t i = 0; i < len; i++)
    {
        // 安全访问 arr[i]
    }

    // 避免死循环的技巧：
    // - 使用明确的终止条件
    // - 确保循环变量向终点变化
    // - 对于 while，在循环体内更新条件相关变量
    // - 考虑添加最大迭代次数保护（用于调试）
    int safety = 0;
    while (1)
    {
        safety++;
        if (safety > 1000)
        {
            printf("死循环保护触发，强制退出\n");
            break;
        }
        // 正常逻辑...
        break; // 演示用，实际会正常退出
    }
}
/**
 * array_string_demo.c
 *
 * 知识点：
 * 1. 一维/二维数组的定义、初始化、元素访问、内存布局
 * 2. 数组的遍历、查找、排序、最值计算
 * 3. C语言字符串：字符数组、'\0'的核心作用
 * 4. 字符串处理函数（strlen/strcpy/strcmp等）用法与坑
 *
 * 编译：gcc -Wall -std=c99 -o array_string_demo array_string_demo.c
 * 运行：./array_string_demo
 */

/* ==================== 1. 一维数组 ==================== */
void demo_1d_array(void)
{
    printf("\n========== 1. 一维数组 ==========\n");

    // 定义并初始化
    int arr1[5] = {1, 2, 3, 4, 5}; // 完全初始化
    int arr2[] = {10, 20, 30};     // 自动推断长度
    int arr3[5] = {1, 2};          // 未初始化的元素自动为0

    // 访问元素
    printf("arr1[0] = %d\n", arr1[0]);
    arr1[0] = 100; // 修改元素

    // 遍历
    printf("arr1: ");
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", arr1[i]);
    }
    printf("\n");

    // 内存布局：数组元素在内存中是连续存放的
    printf("arr1 的内存地址：\n");
    for (int i = 0; i < 5; i++)
    {
        printf("&arr1[%d] = %p\n", i, (void *)&arr1[i]);
    }
    // 相邻元素地址相差 sizeof(int) 字节（通常4字节）

    // 查找元素（线性查找）
    int target = 3;
    int found = -1;
    for (int i = 0; i < 5; i++)
    {
        if (arr1[i] == target)
        {
            found = i;
            break;
        }
    }
    if (found != -1)
        printf("找到 %d，下标为 %d\n", target, found);
    else
        printf("未找到 %d\n", target);

    // 计算最值
    int max_val = arr1[0], min_val = arr1[0];
    for (int i = 1; i < 5; i++)
    {
        if (arr1[i] > max_val)
            max_val = arr1[i];
        if (arr1[i] < min_val)
            min_val = arr1[i];
    }
    printf("最大值：%d，最小值：%d\n", max_val, min_val);

    // 冒泡排序（升序）
    int sort_arr[] = {5, 2, 8, 1, 9};
    int n = sizeof(sort_arr) / sizeof(sort_arr[0]); // 计算数组长度
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
        {
            if (sort_arr[j] > sort_arr[j + 1])
            {
                int temp = sort_arr[j];
                sort_arr[j] = sort_arr[j + 1];
                sort_arr[j + 1] = temp;
            }
        }
    }
    printf("排序后：");
    for (int i = 0; i < n; i++)
        printf("%d ", sort_arr[i]);
    printf("\n");
}

/* ==================== 2. 二维数组 ==================== */
void demo_2d_array(void)
{
    printf("\n========== 2. 二维数组 ==========\n");

    // 定义与初始化
    int matrix[3][4] = {// 3行4列
                        {1, 2, 3, 4},
                        {5, 6, 7, 8},
                        {9, 10, 11, 12}};
    // 也可省略内层括号：int matrix[3][4] = {1,2,3,4,5,6,7,8,9,10,11,12};
    // 部分初始化：int matrix[3][4] = {{1,2}, {5}}; 其余补0

    // 访问元素
    printf("matrix[1][2] = %d\n", matrix[1][2]); // 第2行第3列，值为7

    // 遍历（行优先）
    printf("二维数组遍历：\n");
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }

    // 内存布局：也是连续存放，按行优先（先存完第一行，再第二行...）
    printf("二维数组内存布局（连续地址）：\n");
    int *p = &matrix[0][0];
    for (int i = 0; i < 12; i++)
    {
        printf("%d ", p[i]); // 可以当作一维数组访问
    }
    printf("\n");

    // 计算二维数组的行数、列数
    int rows = sizeof(matrix) / sizeof(matrix[0]);       // 总字节数 / 一行字节数
    int cols = sizeof(matrix[0]) / sizeof(matrix[0][0]); // 一行字节数 / 一个元素字节数
    printf("行数 = %d, 列数 = %d\n", rows, cols);
}

/* ==================== 3. C语言字符串（字符数组） ==================== */
void demo_string_basics(void)
{
    printf("\n========== 3. C语言字符串 ==========\n");

    // 字符串的本质：以 '\0'（空字符）结尾的字符数组
    char str1[] = {'H', 'e', 'l', 'l', 'o', '\0'}; // 显式加'\0'
    char str2[] = "Hello";                         // 字符串字面量，自动加'\0'
    char str3[10] = "Hello";                       // 剩余位置自动补'\0'

    printf("str1 = %s\n", str1);
    printf("str2 = %s\n", str2);
    printf("str3 = %s\n", str3);

    // '\0' 的核心作用：标记字符串的结束
    char str4[] = {'A', 'B', '\0', 'C', 'D'}; // 中间有'\0'
    printf("str4 = %s\n", str4);              // 只输出 "AB"，因为遇到'\0'停止

    // 字符串长度与数组长度的区别
    char str5[] = "Hello";
    printf("数组长度 = %zu\n", sizeof(str5));   // 6（包含'\0'）
    printf("字符串长度 = %zu\n", strlen(str5)); // 5（不包含'\0'）

    // 常见陷阱：忘记留出'\0'的空间
    char str6[5] = "Hello"; // 错误：数组长度必须至少为6才能存储"Hello"（含'\0'）
    // 这会导致缓冲区溢出，未定义行为。正确写法：char str6[6] = "Hello";
}

/* ==================== 4. 字符串处理函数 ==================== */
void demo_string_functions(void)
{
    printf("\n========== 4. 字符串处理函数 ==========\n");

    char src[] = "Hello, World!";
    char dest[20]; // 目标数组必须足够大

    // strlen：获取字符串长度（不含'\0'）
    size_t len = strlen(src);
    printf("strlen(\"%s\") = %zu\n", src, len);

    // strcpy：复制字符串（包括'\0'）
    strcpy(dest, src); // 危险：若dest空间不足会溢出
    printf("strcpy: dest = %s\n", dest);

    // strncpy：安全复制（指定最多复制n个字符，不自动加'\0'的坑）
    char dest2[10];
    strncpy(dest2, src, 9); // 复制最多9个字符
    dest2[9] = '\0';        // 必须手动添加'\0'，否则dest2不是合法字符串
    printf("strncpy 后手动加'\\0': %s\n", dest2);

    // strcat：拼接字符串（目标需有足够空间）
    char dest3[30] = "Hello";
    strcat(dest3, ", World!");
    printf("strcat: %s\n", dest3);

    // strncat：安全拼接（最多添加n个字符，自动加'\0'）
    char dest4[15] = "Hello";
    strncat(dest4, ", World!", 5); // 只添加 ", Wor"
    printf("strncat 限制长度: %s\n", dest4);

    // strcmp：比较字符串（字典序）
    char a[] = "apple";
    char b[] = "banana";
    int cmp = strcmp(a, b);
    if (cmp < 0)
        printf("\"%s\" < \"%s\"\n", a, b);
    else if (cmp > 0)
        printf("\"%s\" > \"%s\"\n", a, b);
    else
        printf("\"%s\" == \"%s\"\n", a, b);

    // strncmp：比较前n个字符
    char c[] = "applet";
    if (strncmp(a, c, 3) == 0)
        printf("前3个字符相同\n");

    /* ---------- 常见坑点 ---------- */
    // 1. 忘记'\0'导致输出乱码或越界
    char bad[3] = {'a', 'b', 'c'}; // 无'\0'
    // printf("%s", bad);             // 未定义行为，会一直读到遇到'\0'为止

    // 2. 使用strcpy/strcat时目标空间不足
    char small[5];
    // strcpy(small, "too long");     // 缓冲区溢出，极其危险！

    // 3. strncpy不自动加'\0'（如上例所示）

    // 4. strlen返回size_t，与有符号数比较时注意隐式转换
    if (strlen(src) > 5)
    { // 正确
      // ...
    }
    // 错误：if (strlen(src) - 10 > 0)  由于size_t无符号，结果为很大的正数
}

int main(void)
{
    printf("====== C语言循环控制综合复习 ======");
    demo_for();
    demo_while_do();
    demo_break_continue_goto();
    demo_boundary();
    printf("\n====== 复习结束 ======\n");
    printf("====== C语言数组与字符串综合复习 ======");
    demo_1d_array();
    demo_2d_array();
    demo_string_basics();
    demo_string_functions();
    printf("\n====== 复习结束 ======\n");
    return 0;
}