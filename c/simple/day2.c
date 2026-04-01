#include <stdio.h>
#include <stdbool.h>
/* ===================== 辅助函数：打印二进制（低8位） ===================== */
void print_binary(unsigned char n)
{
    for (int i = 7; i >= 0; i--)
        putchar((n >> i) & 1 ? '1' : '0');
}
/* ===================== 1. 位运算符 ===================== */
void demo_bitwise(void)
{
    unsigned char a = 0b1010; // 二进制1010，十进制10
    unsigned char b = 0b1100;
    // 按位与 &
    unsigned char and = a & b;
    printf("a&b=0b");
    print_binary(and);
    printf("(十进制%d)\n", and);
    // 按位或 |
    unsigned char or = a | b;
    printf("a|b=0b");
    printf_binary(or);
    printf("(十进制 %d)\n", or);
    // 按位异或 ^
    unsigned char xor = a ^ b;
    printf("a^b=0b");
    print_binary(xor);
    printf("(十进制 %d)\n", xor);
    // 按位非 ~(只显示低8位)
    unsigned char not_a = ~a;
    printf("~a  =0b");
    printf_binary(not_a);
    printf("(十进制 %d,只显示低8位)\n", not_a);
    // 右移 <<
    unsigned char right = b >> 1;
    printf("b >>1=0b");
    print_binary(right);
    printf("(十进制 %d)\n", right);
    // 左移
    unsigned char left = a << 1;
    printf("a<<1=0b");
    print_binary(left);
    printf("(十进制%d)\n", left);
    // 位运算常见应用：掩码操作
    unsigned char flags = 0b0000;
    unsigned char READ = 0b0001;
    unsigned char WRITE = 0b0010;
    flags |= READ;  // 设置读权限
    flags |= WRITE; // 设置写权限
    printf("设置权限后 flags=0b");
    print_binary(flags);
    printf("(读+写)\n");
    if (flags & READ)
        printf("具有读权限\n");
    if (flags & WRITE)
        printf("具有写权限\n");
    flags &= ~WRITE; // 清除写权限
    printf("清除写权限后flags=0b");
    print_binary(flags);
    printf("(仅读)\n");
}
/* ===================== 2. 逻辑运算符 ===================== */
void demo_logic(void)
{
    int a = 5, b = 0;
    printf("a=%d,b=%d\n", a, b);
    // 逻辑与&&
    printf("a&&b=%d(0表示假)", a && b);
    // 逻辑或||
    printf("a||b=%d(1表示真)", a || b);
    // 逻辑非 !
    printf("!a   =%d\n", !a);
    printf("!b   =%d\n", !b);
    // 布尔值实现：0为假，非0为真
    printf("\n布尔值实现：任何非0值都被视为真，逻辑运算符返回0或1\n");
    printf("(5>3)&&(2<4)=%d\n", (5 > 3) && (2 < 4));
    printf("(5>10)||(1)  =%d\n", (5 > 10) || 1);
    // 短路求值演示
    int x = 1, y = 0;
    printf("初始 x=%d,y=%d\n", x, y);
    // && 短路：左操作数为假，右侧不执行
    if (x == 0 && (y = 10))
    {
    } // x==0为假，右侧(y=10)不执行
    printf("x==0&&(y=10)后,y=%d(未改变)\n", y);
    // || 短路：左操作数未真，右侧不执行
    if (x == 1 || (y = 20))
    {
    } // x==1为真，右侧(y=20)不执行
    printf("x==1 || (y=20)后，y=%d(仍不变)\n", y);

    // 另一种写法：直接用赋值表达式观察
    int result;
    result = (x == 0) && (y = 30); // 短路，y不变
    printf("(x==0)&&(y=30)结果：result=%d,y=%d\n", result, y);
    result = (x == 1) || (y = 40); // 短路,y不变
    printf("(x==1)||(y=40)结果：result=%d,y=%d\n", result, y);
}
int main(void)
{
    demo_bitwise();
    demo_logic();
    return 0;
}