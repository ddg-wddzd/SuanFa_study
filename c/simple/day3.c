/**
 * branch_review.c
 *
 * 知识点：
 * 1. if-else 分支、多分支嵌套、悬空 else 问题及规避
 * 2. switch-case 语句、break/default、case 穿透的场景与陷阱
 * 3. 分支语句的代码规范与可读性优化
 *
 * 编译：gcc -Wall -std=c99 -o branch_review branch_review.c
 * 运行：./branch_review
 */

#include <stdio.h>

/* ==================== 1. if-else 与悬空 else ==================== */
void demo_if_else(void)
{
    printf("\n========== 1. if-else 分支与悬空 else ==========\n");

    // 基本 if-else 链
    int score = 85;
    if (score >= 90)
        printf("等级 A\n");
    else if (score >= 80)
        printf("等级 B\n");
    else if (score >= 70)
        printf("等级 C\n");
    else
        printf("等级 D 或以下\n");

    // 多分支嵌套示例：年龄与收入决定优惠
    int age = 25;
    double income = 35000;
    printf("\n年龄: %d, 收入: %.0f\n", age, income);
    if (age < 18)
    {
        printf("免费入场\n");
    }
    else
    {
        if (income < 30000)
            printf("半价优惠\n");
        else if (income < 50000)
            printf("无优惠\n");
        else
            printf("全价票\n");
    }

    // 悬空 else 问题：else 与最近的 if 匹配（但缩进可能误导）
    int x = 0, y = 1;
    printf("\n悬空 else 演示 (x=%d, y=%d):\n", x, y);
   
    // 正确规避：始终使用花括号明确作用域
    if (x > 0)
    {
        if (y > 0)
        {
            printf("x>0 and y>0\n");
        }
    }
    else
    {
        printf("x <= 0 (正确匹配外层 if)\n");
    }
}

/* ==================== 2. switch-case 语句 ==================== */
void demo_switch(void)
{
    printf("\n========== 2. switch-case 语句 ==========\n");

    // 基本 switch + break
    int day = 3;
    printf("day = %d -> ", day);
    switch (day)
    {
    case 1:
        printf("Monday\n");
        break;
    case 2:
        printf("Tuesday\n");
        break;
    case 3:
        printf("Wednesday\n");
        break;
    case 4:
        printf("Thursday\n");
        break;
    case 5:
        printf("Friday\n");
        break;
    case 6:
        printf("Saturday\n");
        break;
    case 7:
        printf("Sunday\n");
        break;
    default:
        printf("无效日期\n");
    }

    // case 穿透（fall-through）的合理使用：合并多个 case
    char grade = 'B';
    printf("\n成绩等级 %c -> ", grade);
    switch (grade)
    {
    case 'A':
    case 'B':
    case 'C':
        printf("通过\n");
        break;
    case 'D':
        printf("补考\n");
        break;
    case 'F':
        printf("不通过\n");
        break;
    default:
        printf("无效等级\n");
    }

    // 故意穿透实现业务逻辑（必须加注释说明）
    int month = 4;
    printf("\n月份 %d 属于: ", month);
    switch (month)
    {
    case 12:
    case 1:
    case 2:
        printf("冬季");
        break;
    case 3:
    case 4:
    case 5:
        printf("春季");
        break;
    case 6:
    case 7:
    case 8:
        printf("夏季");
        break;
    case 9:
    case 10:
    case 11:
        printf("秋季");
        break;
    default:
        printf("无效月份");
    }
    printf("\n");

    // 穿透的陷阱：忘记 break 导致意外执行
    int code = 1;
    printf("\n穿透陷阱 (code=%d):\n", code);
    switch (code)
    {
    case 1:
        printf("执行 case 1\n");
        // 这里忘记 break —— 会继续执行 case 2
    case 2:
        printf("执行 case 2（不该执行的！）\n");
        break;
    default:
        printf("default\n");
    }
    // 正确做法：每个 case 后都加 break，除非故意穿透并注释 /* fall through */
}

/* ==================== 3. 代码规范与可读性优化 ==================== */
void demo_best_practices(void)
{
    printf("\n========== 3. 分支语句的代码规范与可读性优化 ==========\n");

    // 规范1：即使单条语句也使用花括号，避免悬空 else 和日后修改出错
    int a = 5, b = 10;
    if (a > b)
    {
        printf("a > b\n");
    }
    else
    {
        printf("a <= b\n");
    }

    // 规范2：switch 每个 case 都要有 break 或显式注释表示穿透
    int opt = 2;
    switch (opt)
    {
    case 1:
        printf("处理选项1\n");
        break;
    case 2:
        printf("处理选项2，并故意穿透到 case 3\n");
        /* fall through */
    case 3:
        printf("共享代码块\n");
        break;
    default:
        printf("无效选项\n");
        break;
    }

    // 规范3：复杂条件提取为有意义的变量，提高可读性
    int age = 20;
    int has_license = 1;
    int is_adult = (age >= 18);
    int can_drive = is_adult && has_license;
    if (can_drive)
    {
        printf("允许驾驶\n");
    }
    else
    {
        printf("不允许驾驶\n");
    }

    // 规范4：使用枚举代替魔数，增强 switch 可读性
    enum Color
    {
        RED,
        GREEN,
        BLUE
    };
    enum Color color = GREEN;
    printf("\n枚举示例: ");
    switch (color)
    {
    case RED:
        printf("红色\n");
        break;
    case GREEN:
        printf("绿色\n");
        break;
    case BLUE:
        printf("蓝色\n");
        break;
    }

    // 规范5：避免过深嵌套 —— 提前返回（guard clause）
    // 以下为示意（在函数中常用）
    printf("\n提前返回模式: 无效条件尽早退出，减少嵌套\n");
    int value = -5;
    if (value < 0)
    {
        printf("输入无效，无法处理\n");
        return; // 若在 main 中则直接返回
    }
    // 后续处理有效 value
    printf("处理 value = %d\n", value);
}

int main(void)
{
    printf("====== C语言分支语句复习 ======");
    demo_if_else();
    demo_switch();
    demo_best_practices();
    printf("\n====== 复习结束 ======\n");
    return 0;
}