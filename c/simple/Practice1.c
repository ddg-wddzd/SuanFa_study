#include <stdio.h>
#include <stdint.h>

// 1. 设置特定位为1
void set_bit(int *num, int pos)
{
    *num |= (1 << pos);
}

// 2. 清除特定位为0
void clear_bit(int *num, int pos)
{
    *num &= ~(1 << pos);
}

// 3. 切换特定位
void toggle_bit(int *num, int pos)
{
    *num ^= (1 << pos);
}

// 4. 检查特定位是否为1
int check_bit(int num, int pos)
{
    return (num & (1 << pos)) != 0;
}

// 5. 打印整数的二进制表示（兼容所有编译器）
void print_binary(int num, int bits)
{
    printf("二进制: ");
    for (int i = bits - 1; i >= 0; i--)
    {
        printf("%d", (num >> i) & 1);
        if (i % 8 == 0 && i != 0)
            printf(" ");
    }
    printf(" (十进制: %d)\n", num);
}

// 6. 打印特定位的详细信息
void print_bit_info(int num, int pos)
{
    printf("第%d位的状态: %d\n", pos, check_bit(num, pos));
    printf("  设置该位后的值: %d\n", num | (1 << pos));
    printf("  清除该位后的值: %d\n", num & ~(1 << pos));
    printf("  切换该位后的值: %d\n", num ^ (1 << pos));
}

int main()
{
    int num = 0b1010; // 二进制字面量，C99标准支持

    printf("初始状态:\n");
    print_binary(num, 8);

    printf("\n=== 测试各种位操作 ===\n");

    // 设置第3位
    set_bit(&num, 3);
    printf("设置第3位后:\n");
    print_binary(num, 8);

    // 设置第5位
    set_bit(&num, 5);
    printf("设置第5位后:\n");
    print_binary(num, 8);

    // 清除第3位
    clear_bit(&num, 3);
    printf("清除第3位后:\n");
    print_binary(num, 8);

    // 切换第0位
    toggle_bit(&num, 0);
    printf("切换第0位后:\n");
    print_binary(num, 8);

    // 再次切换第0位
    toggle_bit(&num, 0);
    printf("再次切换第0位后:\n");
    print_binary(num, 8);

    // 检查特定位
    printf("\n=== 检查位状态 ===\n");
    for (int i = 0; i < 8; i++)
    {
        printf("第%d位: %s\n", i, check_bit(num, i) ? "1" : "0");
    }

    // 演示位操作的数学原理
    printf("\n=== 位操作的数学原理解析 ===\n");
    int test_num = 0b01010101; // 85
    printf("测试数值: ");
    print_binary(test_num, 8);

    printf("\n对第2位的详细分析:\n");
    print_bit_info(test_num, 2);

    // 演示多位移操作
    printf("\n=== 多位移操作演示 ===\n");
    int flags = 0;

    // 设置第0、2、4位
    flags |= (1 << 0) | (1 << 2) | (1 << 4);
    printf("设置第0、2、4位后的标志:\n");
    print_binary(flags, 8);

    // 清除第0、4位
    flags &= ~((1 << 0) | (1 << 4));
    printf("清除第0、4位后的标志:\n");
    print_binary(flags, 8);

    // 切换第1、2、3位
    flags ^= (1 << 1) | (1 << 2) | (1 << 3);
    printf("切换第1、2、3位后的标志:\n");
    print_binary(flags, 8);

    return 0;
}