/**
 * pointer_advanced_demo.c
 *
 * 知识点：
 * 1. const 修饰指针的三种场景
 * 2. void* 通用指针的用法与类型转换
 * 3. 指向结构体的指针基础
 *
 * 编译：gcc -Wall -std=c99 -o pointer_advanced_demo pointer_advanced_demo.c
 * 运行：./pointer_advanced_demo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h> // 用于 sleep 模拟延时
// 定义一个结构体（用于第3部分）
typedef struct
{
    int x;
    int y;
} Point; // Point是类型别名

// 规则：const 在 * 左边 → 不能改内容
//       const 在 * 右边 → 不能改指针
//       const 在 * 两边 → 都不能改

// 枚举：机器人状态
typedef enum
{
    STOP = 0,
    FORWARD,
    BACKWARD
} RobotState;

// 结构体：位置（嵌套用）
typedef struct
{
    int x;
    int y;
} Position;

// 结构体：机器人
typedef struct
{
    char name[20];
    Position pos; // 嵌套
    int speed;
    RobotState state;
} Robot;

// 联合体：同一块内存存不同数据类型
typedef union
{
    int i;
    float f;
    char c[4];
} DataUnion;

// 结构体数组元素类型
typedef struct
{
    int id;
    float value;
} Sensor;

// 函数：传值（不会修改原结构体）
void changeByValue(Robot r)
{
    r.speed = 999;
}

// 函数：传址（修改原结构体）
void changeByPointer(Robot *r)
{
    r->speed = 999;
}

// 打印内存对齐信息
void printAlign(const char *name, size_t size)
{
    printf("%-12s 大小: %2zu 字节\n", name, size);
}

// ==================== 1. 位运算进阶 ====================
// 常用位操作宏（针对 uint32_t 类型，可推广）
#define SET_BIT(reg, bit) ((reg) |= (1UL << (bit)))    // 置1
#define CLEAR_BIT(reg, bit) ((reg) &= ~(1UL << (bit))) // 清0
#define TOGGLE_BIT(reg, bit) ((reg) ^= (1UL << (bit))) // 翻转
#define GET_BIT(reg, bit) (((reg) >> (bit)) & 1)       // 读取位状态

// 批量操作：对位段（多位）的读写
#define SET_MASK(reg, mask) ((reg) |= (mask))
#define CLEAR_MASK(reg, mask) ((reg) &= ~(mask))
#define GET_MASK(reg, mask) ((reg) & (mask))

void demo_bitwise_ops(void)
{
    printf("========== 1. 位运算进阶 ==========\n");
    uint32_t reg = 0x00000000;

    // 置位第3位（从0开始）
    SET_BIT(reg, 3);
    printf("置位第3位后: 0x%08X (二进制: ", reg);
    for (int i = 31; i >= 0; i--)
        putchar(GET_BIT(reg, i) ? '1' : '0');
    printf(")\n");

    // 清0第3位
    CLEAR_BIT(reg, 3);
    printf("清0第3位后: 0x%08X\n", reg);

    // 翻转第0位
    TOGGLE_BIT(reg, 0);
    printf("翻转第0位后: 0x%08X\n", reg);

    // 判断第0位是否为1
    printf("第0位状态: %d\n", GET_BIT(reg, 0));

    // 批量操作：低4位设置为 0b1010
    CLEAR_MASK(reg, 0x0F);
    SET_MASK(reg, 0x0A);
    printf("低4位设为1010后: 0x%08X (低4位=0x%X)\n\n", reg, GET_MASK(reg, 0x0F));
}

// ==================== 2. 位段（位域） ====================
// 定义一个包含位段的结构体，用于紧凑存储标志位
typedef struct
{
    uint8_t enable : 1;   // 位0
    uint8_t mode : 2;     // 位1-2
    uint8_t speed : 3;    // 位3-5
    uint8_t reserved : 2; // 位6-7
} MotorConfigBits;

// 演示位段内存布局
void demo_bitfield(void)
{
    printf("========== 2. 位段（位域） ==========\n");
    MotorConfigBits cfg;
    memset(&cfg, 0, sizeof(cfg));

    cfg.enable = 1;
    cfg.mode = 2;  // 二进制 10
    cfg.speed = 5; // 二进制 101

    // 通过联合体查看实际内存内容（字节视图）
    union
    {
        MotorConfigBits bits;
        uint8_t raw;
    } view;
    view.bits = cfg;
    printf("MotorConfigBits 结构体大小: %zu 字节\n", sizeof(MotorConfigBits));
    printf("赋值后: enable=%u, mode=%u, speed=%u\n", cfg.enable, cfg.mode, cfg.speed);
    printf("实际内存字节值: 0x%02X\n", view.raw);
    printf("位段可以节省内存，但跨平台移植性要注意（编译器分配顺序可能不同）。\n\n");
}

// ==================== 3. 大小端模式检测 ====================
void demo_endianness(void)
{
    printf("========== 3. 大小端模式 ==========\n");
    uint32_t test = 0x12345678;
    uint8_t *p = (uint8_t *)&test;

    printf("整数 0x12345678 在内存中的字节顺序（低地址→高地址）:\n");
    for (int i = 0; i < 4; i++)
    {
        printf("  地址 %p : 0x%02X\n", (void *)&p[i], p[i]);
    }
    if (p[0] == 0x78)
    {
        printf("结论：小端模式（低字节存储在低地址）\n");
    }
    else
    {
        printf("结论：大端模式（高字节存储在低地址）\n");
    }
    printf("大多数嵌入式 MCU（如 STM32）为小端模式。\n\n");
}

// ==================== 4. 模拟硬件寄存器操作（STM32 GPIO 风格） ====================
// 定义 GPIO 寄存器结构体（位段 + 普通成员混合）
// 实际 STM32 中，GPIOx_MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR 等寄存器
// 这里简化模拟一个控制寄存器 CR，包含使能、方向、输出数据等位段

typedef union
{
    uint32_t reg; // 原始寄存器值
    struct
    {
        uint32_t enable : 1;    // bit0: 使能
        uint32_t direction : 1; // bit1: 0=输入,1=输出
        uint32_t pull : 2;      // bit2-3: 00无,01上拉,10下拉
        uint32_t output : 1;    // bit4: 输出值（当方向为输出时）
        uint32_t reserved : 27; // 保留
    } bits;
} GPIOReg;

// 模拟硬件寄存器（实际应定义为 volatile 指针指向特定地址）
volatile GPIOReg GPIOA_CR = {0};

void demo_register_ops(void)
{
    printf("========== 4. 位运算 & 位段操作硬件寄存器 ==========\n");
    printf("模拟 STM32 GPIO 控制寄存器操作：\n");

    // 方法1：使用位段直接修改
    GPIOA_CR.bits.enable = 1;
    GPIOA_CR.bits.direction = 1; // 输出模式
    GPIOA_CR.bits.pull = 1;      // 上拉
    GPIOA_CR.bits.output = 1;    // 输出高电平
    printf("通过位段设置后，寄存器值 = 0x%08X\n", GPIOA_CR.reg);
    printf("  使能=%u, 方向=%s, 上拉=%u, 输出=%u\n",
           GPIOA_CR.bits.enable,
           GPIOA_CR.bits.direction ? "输出" : "输入",
           GPIOA_CR.bits.pull,
           GPIOA_CR.bits.output);

    // 方法2：使用传统位运算修改（嵌入式开发常用宏）
    // 例如：将输出位清零
    CLEAR_BIT(GPIOA_CR.reg, 4);
    printf("通过 CLEAR_BIT 宏清除输出位后，寄存器值 = 0x%08X\n", GPIOA_CR.reg);
    printf("  输出位现在 = %u\n", GET_BIT(GPIOA_CR.reg, 4));

// 批量修改：设置速度位段（假设速度位段在 bit5-6）
// 实际项目中会定义掩码 SHIFT 值，这里仅演示
#define SPEED_MASK (0x3 << 5)
    GPIOA_CR.reg &= ~SPEED_MASK; // 清零速度位段
    GPIOA_CR.reg |= (2 << 5);    // 设置为 2 (10b)
    printf("设置速度位段后，寄存器值 = 0x%08X\n", GPIOA_CR.reg);
    printf("速度位段值 = %u\n", (GPIOA_CR.reg >> 5) & 0x3);

    printf("\nSTM32 开发核心：通过位段或位运算直接操作寄存器，\n");
    printf("实现高效的硬件控制。实际代码中寄存器地址需通过指针映射。\n");
}

// ==================== 1. 宏定义基础 ====================
#define PI 3.14159f           // 常量宏
#define SQUARE(x) ((x) * (x)) // 带参数的宏（注意括号）
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// 宏与函数的区别示例
int max_func(int a, int b)
{
    return a > b ? a : b;
}

// 演示宏的副作用
#define SQUARE_BAD(x) x *x // 错误的宏，缺少括号

void demo_macro_basic(void)
{
    printf("========== 1. 宏定义基础 ==========\n");
    float r = 2.0f;
    printf("PI = %f, 面积 = %f\n", PI, PI * r * r);

    int a = 5, b = 3;
    printf("SQUARE(%d) = %d\n", a, SQUARE(a));
    printf("MAX(%d, %d) = %d\n", a, b, MAX(a, b));

    // 宏的副作用：参数多次求值
    int x = 2;
    int y = SQUARE(x++); // 展开为 ((x++) * (x++))，x 自增两次
    printf("SQUARE(x++) 后: x = %d, 结果 = %d\n", x, y);

    // 错误的宏定义缺少括号
    int z = 3;
    int wrong = SQUARE_BAD(z + 1); // 展开为 z + 1 * z + 1 = 3+1*3+1=7
    printf("SQUARE_BAD(3+1) = %d (应为 16)\n", wrong);

    // 宏与函数的区别
    printf("\n宏与函数区别:\n");
    printf("  max_func(++a, b) = %d\n", max_func(++a, b)); // 参数只求值一次
    // 宏: MAX(++a, b) 会多次求值，产生副作用
    a = 5;
    b = 3;
    printf("  MAX(++a, b) = %d, a = %d (a 自增两次)\n", MAX(++a, b), a);
    printf("总结：宏在预处理阶段展开，不进行类型检查，可能多次求值；函数在运行时调用，类型安全。\n\n");
}

// ==================== 2. 宏进阶：do-while(0)、# 和 ## ====================
// 多语句宏的安全封装（do-while(0) 技巧）
#define LOG_INFO(msg)        \
    do                       \
    {                        \
        printf("[INFO] ");   \
        printf("%s\n", msg); \
    } while (0)

// # 运算符：将宏参数转换为字符串
#define STR(x) #x
// ## 运算符：连接两个标识符
#define CONCAT(a, b) a##b

// 使用 ## 创建变量名
int var_abc = 123;

void demo_macro_advanced(void)
{
    printf("========== 2. 宏进阶用法 ==========\n");
    // do-while(0) 宏可以安全地放在 if 等语句中，末尾加分号
    int condition = 1;
    if (condition)
        LOG_INFO("条件成立"); // 宏展开后是 do{...}while(0); 安全
    else
        printf("条件不成立\n");

    // # 运算符
    int value = 42;
    printf("STR(value) = %s\n", STR(value));             // 输出 "value" 字符串
    printf("STR(Hello World) = %s\n", STR(Hello World)); // 带空格也可以

    // ## 运算符
    int abc = CONCAT(var_, abc); // 展开为 var_abc
    printf("CONCAT(var_, abc) = %d\n", abc);

// 还可以用于生成函数名或变量名
#define FUNC_NAME(prefix, suffix) prefix##_##suffix
    int FUNC_NAME(my, var) = 888; // 创建变量 my_var
    printf("my_var = %d\n", my_var);
    printf("\n");
}

// ==================== 3. 条件编译 ====================
#define DEBUG_LEVEL 2
// 定义宏来开关调试信息
// #define ENABLE_DEBUG      // 注释掉即可关闭调试输出

// 条件编译演示
void demo_conditional_compile(void)
{
    printf("========== 3. 条件编译 ==========\n");

#ifdef ENABLE_DEBUG
    printf("调试模式：变量值等调试信息\n");
#else
    printf("发布模式：无调试信息\n");
#endif

// #if 根据常量表达式
#if DEBUG_LEVEL > 1
    printf("详细调试信息（DEBUG_LEVEL=%d）\n", DEBUG_LEVEL);
#elif DEBUG_LEVEL == 1
    printf("普通调试信息\n");
#else
    printf("无调试信息\n");
#endif

// #ifndef 常用于头文件保护
#ifndef MAX_LEN
#define MAX_LEN 100
#endif
    printf("MAX_LEN = %d\n", MAX_LEN);

    // 预定义宏
    printf("当前文件: %s, 行号: %d, 编译日期: %s, 时间: %s\n",
           __FILE__, __LINE__, __DATE__, __TIME__);
    printf("\n");
}

// ==================== 4. #include 规则说明 ====================
// 此部分无法通过运行代码完全展示，但通过注释说明

/*
 * #include 规则：
 * - #include <header.h> ：从系统标准路径搜索头文件（如 /usr/include）
 * - #include "header.h" ：先从当前源文件所在目录搜索，再到系统路径
 * - 头文件保护：使用 #ifndef HEADER_H / #define HEADER_H / #endif 防止重复包含
 * - 重复包含可能导致重复定义错误，保护宏可避免
 */

// 演示一个简单的头文件保护宏（通常写在 .h 文件中）
#ifndef MY_HEADER_H
#define MY_HEADER_H
// 声明函数等
extern int global_counter;
#endif

int global_counter = 0;

void demo_include_rules(void)
{
    printf("========== 4. #include 规则 ==========\n");
    printf("<> 用于系统头文件，\"\" 用于用户自定义头文件。\n");
    printf("头文件保护宏（#ifndef ... #define ... #endif）防止重复包含。\n");
    printf("例如：上面的 MY_HEADER_H 保护了这个区域的重复包含。\n");
    printf("global_counter = %d\n", global_counter);
}
// ==================== 辅助函数：错误处理 ====================
void check_file(FILE *fp, const char *filename, const char *mode)
{
    if (fp == NULL)
    {
        perror("文件打开失败");
        printf("无法打开文件: %s (模式: %s)\n", filename, mode);
        exit(EXIT_FAILURE);
    }
}

// ==================== 1. 字符读写（fgetc/fputc） ====================
void demo_char_rw(void)
{
    printf("\n========== 1. 字符读写（fgetc/fputc） ==========\n");
    const char *filename = "char_test.txt";

    // 写入字符
    FILE *fp = fopen(filename, "w");
    check_file(fp, filename, "w");
    const char *msg = "Hello C File I/O";
    for (int i = 0; msg[i] != '\0'; i++)
    {
        fputc(msg[i], fp);
    }
    fclose(fp);
    printf("写入完成: %s\n", msg);

    // 读取字符并显示
    fp = fopen(filename, "r");
    check_file(fp, filename, "r");
    printf("读取内容: ");
    int ch;
    while ((ch = fgetc(fp)) != EOF)
    {
        putchar(ch);
    }
    putchar('\n');
    fclose(fp);
}

// ==================== 2. 字符串读写（fgets/fputs） ====================
void demo_string_rw(void)
{
    printf("\n========== 2. 字符串读写（fgets/fputs） ==========\n");
    const char *filename = "string_test.txt";

    // 写入多行字符串
    FILE *fp = fopen(filename, "w");
    check_file(fp, filename, "w");
    fputs("Line 1: Hello\n", fp);
    fputs("Line 2: World\n", fp);
    fputs("Line 3: End\n", fp);
    fclose(fp);
    printf("写入三行文本完成。\n");

    // 按行读取
    fp = fopen(filename, "r");
    check_file(fp, filename, "r");
    char buffer[100];
    printf("读取内容:\n");
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        printf("  %s", buffer); // fgets 会保留换行符
    }
    fclose(fp);
}

// ==================== 3. 二进制读写（fread/fwrite） ====================
typedef struct
{
    int id;
    float value;
    char name[20];
} Record;

void demo_binary_rw(void)
{
    printf("\n========== 3. 二进制读写（fread/fwrite） ==========\n");
    const char *filename = "binary_test.dat";

    // 准备数据
    Record records[] = {
        {1, 10.5f, "SensorA"},
        {2, 20.3f, "SensorB"},
        {3, 30.8f, "SensorC"}};
    int count = sizeof(records) / sizeof(records[0]);

    // 写入二进制
    FILE *fp = fopen(filename, "wb");
    check_file(fp, filename, "wb");
    size_t written = fwrite(records, sizeof(Record), count, fp);
    printf("写入 %zu 条记录（共 %zu 字节）\n", written, written * sizeof(Record));
    fclose(fp);

    // 读取二进制
    fp = fopen(filename, "rb");
    check_file(fp, filename, "rb");
    Record read_buf[10];
    size_t read_cnt = fread(read_buf, sizeof(Record), 10, fp);
    printf("读取 %zu 条记录:\n", read_cnt);
    for (size_t i = 0; i < read_cnt; i++)
    {
        printf("  ID=%d, Value=%.1f, Name=%s\n",
               read_buf[i].id, read_buf[i].value, read_buf[i].name);
    }
    fclose(fp);
}

// ==================== 4. 随机读写（fseek/ftell/rewind） ====================
void demo_random_access(void)
{
    printf("\n========== 4. 随机读写（fseek/ftell/rewind） ==========\n");
    const char *filename = "random_test.txt";

    // 创建一个有内容的文件
    FILE *fp = fopen(filename, "w");
    check_file(fp, filename, "w");
    fprintf(fp, "0123456789ABCDEFGHIJ");
    fclose(fp);

    // 随机读取
    fp = fopen(filename, "r");
    check_file(fp, filename, "r");

    // 移动到第5个字符（从0开始）
    fseek(fp, 5, SEEK_SET);
    long pos = ftell(fp);
    int ch = fgetc(fp);
    printf("从偏移 %ld 处读取: '%c'\n", pos, ch);

    // 从当前位置向后移动3个字节
    fseek(fp, 3, SEEK_CUR);
    ch = fgetc(fp);
    printf("再向后移动3字节，读取: '%c'\n", ch);

    // 回到文件开头
    rewind(fp);
    ch = fgetc(fp);
    printf("rewind 后读取第一个字符: '%c'\n", ch);

    // 获取文件大小
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    printf("文件总大小: %ld 字节\n", size);

    fclose(fp);
}

// ==================== 5. 错误处理与场景模拟 ====================
// 场景1：记录日志（追加模式）
void log_message(const char *level, const char *msg)
{
    FILE *log = fopen("system.log", "a"); // 追加模式
    if (log == NULL)
    {
        perror("日志文件打开失败");
        return;
    }
    fprintf(log, "[%s] %s\n", level, msg);
    fclose(log);
}

// 场景2：读取配置文件（简单的键值对）
typedef struct
{
    char key[32];
    char value[64];
} ConfigItem;

int load_config(const char *filename, ConfigItem *items, int max_items)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("配置文件打开失败");
        return -1;
    }
    int count = 0;
    char line[128];
    while (fgets(line, sizeof(line), fp) && count < max_items)
    {
        // 忽略空行和注释
        if (line[0] == '\n' || line[0] == '#')
            continue;
        // 解析 key=value 格式
        char *eq = strchr(line, '=');
        if (eq != NULL)
        {
            *eq = '\0';
            strncpy(items[count].key, line, sizeof(items[count].key) - 1);
            strncpy(items[count].value, eq + 1, sizeof(items[count].value) - 1);
            // 去除换行符
            items[count].value[strcspn(items[count].value, "\n")] = '\0';
            count++;
        }
    }
    fclose(fp);
    return count;
}

void demo_scenarios(void)
{
    printf("\n========== 5. 适用场景：日志存储 & 参数配置 ==========\n");

    // 日志场景
    log_message("INFO", "系统启动");
    log_message("WARN", "电池电压低");
    log_message("ERROR", "电机过热");
    printf("日志已写入 system.log，请查看文件内容。\n");

    // 配置文件场景：先创建一个示例配置文件
    FILE *cfg = fopen("robot.cfg", "w");
    if (cfg)
    {
        fprintf(cfg, "# 机器人配置文件\n");
        fprintf(cfg, "speed=100\n");
        fprintf(cfg, "turn_speed=50\n");
        fprintf(cfg, "battery_min=11.5\n");
        fclose(cfg);
    }

    ConfigItem configs[10];
    int cnt = load_config("robot.cfg", configs, 10);
    if (cnt > 0)
    {
        printf("读取到 %d 个配置项:\n", cnt);
        for (int i = 0; i < cnt; i++)
        {
            printf("  %s = %s\n", configs[i].key, configs[i].value);
        }
    }
    else
    {
        printf("配置文件读取失败或为空。\n");
    }
}
// ==================== 1. errno 全局变量 ====================
void demo_errno(void)
{
    printf("\n========== 1. errno 错误码机制 ==========\n");

    // 尝试打开一个不存在的文件
    FILE *fp = fopen("nonexistent.txt", "r");
    if (fp == NULL)
    {
        printf("fopen 失败，errno = %d\n", errno);
        // 使用 perror 打印错误信息（自动添加描述）
        perror("perror 输出");
        // 使用 strerror 获取错误描述字符串
        printf("strerror 输出: %s\n", strerror(errno));
    }
    else
    {
        fclose(fp);
    }

    // 其他可能设置 errno 的函数，如 strtol
    errno = 0; // 调用前清空 errno
    char *endptr;
    long val = strtol("abc", &endptr, 10);
    if (errno == ERANGE)
    {
        printf("数值超出范围\n");
    }
    else if (errno != 0)
    {
        printf("转换错误: %s\n", strerror(errno));
    }
    else if (endptr == "abc")
    {
        printf("没有数字被转换\n");
    }
}

// ==================== 2. perror / strerror ====================
void demo_perror_strerror(void)
{
    printf("\n========== 2. perror / strerror 用法 ==========\n");
    // 故意制造一个错误：无效的指针分配大小
    void *p = malloc((size_t)-1); // 请求超大内存
    if (p == NULL)
    {
        perror("malloc 失败");
        printf("详细错误: %s\n", strerror(errno));
    }
    free(p); // 安全释放（p 为 NULL 时 free 无害）
}

// ==================== 3. assert 断言调试 ====================
// 计算平方根（仅演示参数检查）
double safe_sqrt(double x)
{
    assert(x >= 0); // 调试模式下，若 x<0 则终止并报告位置
    // 实际计算平方根（略）
    return x >= 0 ? x : 0;
}

void demo_assert(void)
{
    printf("\n========== 3. assert 断言 ==========\n");
    double a = 4.0;
    double b = -1.0;
    printf("safe_sqrt(%.1f) = %.1f\n", a, safe_sqrt(a));
    // 下面这行会触发断言失败（调试模式下），正式版可定义 NDEBUG 禁用
    // printf("safe_sqrt(%.1f) = %.1f\n", b, safe_sqrt(b));
    printf("断言在调试阶段很有用，可快速定位逻辑错误。\n");
    printf("若需要关闭断言，在编译时定义 NDEBUG 宏: gcc -DNDEBUG ...\n");
}

// ==================== 4. 常见 bug 定位方法 ====================
// 4.1 数组越界检测
void demo_array_bound_check(void)
{
    printf("\n--- 4.1 数组越界检测 ---\n");
    int arr[5] = {0, 1, 2, 3, 4};
    int index = 5; // 越界索引
    // 方法：使用条件判断或 assert
    if (index >= 0 && index < 5)
    {
        printf("arr[%d] = %d\n", index, arr[index]);
    }
    else
    {
        printf("错误：索引 %d 越界（数组大小 5）\n", index);
    }
    // 使用 assert 强制检查（调试模式）
    assert(index >= 0 && index < 5);
}

// 4.2 野指针检测
void demo_wild_pointer(void)
{
    printf("\n--- 4.2 野指针检测 ---\n");
    int *ptr = NULL; // 初始化为 NULL
    // 使用前检查
    if (ptr != NULL)
    {
        *ptr = 10;
        printf("指针赋值成功\n");
    }
    else
    {
        printf("警告：指针为空，避免解引用\n");
    }
    // 释放内存后置 NULL 是防止野指针的好习惯
    int *data = (int *)malloc(sizeof(int));
    *data = 100;
    free(data);
    data = NULL; // 避免野指针
    // 再次检查
    if (data != NULL)
    {
        printf("data = %d\n", *data);
    }
    else
    {
        printf("data 已释放并置 NULL，安全\n");
    }
}

// 4.3 栈溢出检测（演示递归过深或大数组）
void recursive_depth(int n)
{
    char buf[1024]; // 每层占用 1KB 栈空间
    printf("深度 %d, buf 地址: %p\n", n, buf);
    recursive_depth(n + 1); // 无限递归，很快栈溢出
}

void demo_stack_overflow(void)
{
    printf("\n--- 4.3 栈溢出检测 ---\n");
    printf("栈溢出通常由递归过深或超大局部数组引起。\n");
    printf("下面演示危险递归（已注释），如需观察可取消注释，但程序会崩溃。\n");
    // recursive_depth(1);   // 危险！会栈溢出，默认注释
    printf("安全做法：避免无限递归，或改用迭代；局部数组不要过大。\n");
    // 示例：使用静态或全局数组代替大局部数组
    static char large_buffer[1024 * 1024]; // 1MB，在静态区，不占栈
    printf("大缓冲区放在静态区，避免栈溢出。\n");
}

// 综合演示常见 bug 定位工具
void demo_debug_tools(void)
{
    printf("\n========== 4. 常见 bug 定位方法 ==========\n");
    printf("推荐工具：\n");
    printf("  - 静态分析：gcc -Wall -Wextra，clang-tidy\n");
    printf("  - 动态检测：valgrind（内存泄漏/越界），AddressSanitizer（gcc -fsanitize=address）\n");
    printf("  - 调试器：gdb 查看栈帧、断点\n");
    demo_array_bound_check();
    demo_wild_pointer();
    demo_stack_overflow();
}
// 模拟一个硬件寄存器地址（实际中为绝对地址）
#define UART_STATUS_REG ((volatile uint32_t *)0x40004000) // 假设地址
#define UART_DATA_REG ((volatile uint32_t *)0x40004004)

// 演示资源受限：避免使用大缓冲区，尽量复用
#define SMALL_BUF_SIZE 32

void embedded_features(void)
{
    printf("\n========== 1. 嵌入式 C 特点 ==========\n");
    printf("- 资源受限：RAM/ROM 小，代码需精简\n");
    printf("- 直接操作硬件：通过指针访问寄存器\n");
    printf("- 实时性要求：中断优先级、响应时间\n");
    printf("- 跨平台：使用 stdint.h 固定宽度类型\n");

    // 模拟直接写寄存器（实际会写入绝对地址）
    // 以下代码仅演示，在普通 PC 上运行时可能崩溃，故注释
    // *UART_STATUS_REG = 0x01;
    // *UART_DATA_REG = 'A';
    printf("模拟：通过指针访问硬件寄存器地址。\n");
}

// ==================== 2. 中断服务函数（模拟） ====================
// 实际中断函数需遵循特定编译器语法，例如 __interrupt 或 __attribute__((interrupt))
// 这里用普通函数模拟中断处理逻辑

volatile uint32_t g_timer_overflow = 0; // 全局中断标志（需 volatile）

// 模拟定时器中断服务函数（ISR）
void Timer_ISR(void)
{
    // 注意事项：
    // 1. 保持简短，快速执行
    // 2. 避免调用不可重入函数（如 printf、malloc）
    // 3. 访问共享变量时要使用 volatile 并考虑原子操作
    // 4. 清除中断标志
    g_timer_overflow++;
    // 清除中断标志（假设的寄存器操作）
    // *TIMER_INT_FLAG = 0;
}

// 模拟主程序中使用中断标志
void demo_interrupt(void)
{
    printf("\n========== 2. 中断服务函数 ==========\n");
    printf("中断函数注意事项：\n");
    printf("- 使用 volatile 修饰共享变量\n");
    printf("- 函数尽可能短，避免复杂操作\n");
    printf("- 不可重入函数（如 printf）可能导致死锁\n");
    printf("- 某些平台需要特定函数属性（如 __interrupt）\n");

    // 模拟中断触发
    Timer_ISR();
    printf("模拟中断触发后，g_timer_overflow = %u\n", g_timer_overflow);
}

// ==================== 3. volatile 关键字演示 ====================
// 模拟硬件状态寄存器（假设由外部硬件改变）
volatile int hardware_flag = 0; // 不加 volatile 可能被编译器优化

void delay_loop(int n)
{
    for (int i = 0; i < n; i++)
        ;
}

void demo_volatile(void)
{
    printf("\n========== 3. volatile 关键字 ==========\n");
    // 模拟等待硬件标志变化（实际硬件会异步修改 hardware_flag）
    printf("等待硬件标志（模拟 2 秒）...\n");
    // 假设硬件在某个时刻将 hardware_flag 置为 1
    // 此处用另一个线程或中断修改，这里简单延时后手动修改模拟
    // 注意：在真实场景中，hardware_flag 被中断或硬件改变
    delay_loop(200000000); // 模拟等待
    hardware_flag = 1;     // 模拟硬件置位

    // 编译器如果不加 volatile，可能优化为：if (hardware_flag != 0) 直接读取一次
    if (hardware_flag)
    {
        printf("检测到硬件标志，继续执行。\n");
    }
    else
    {
        printf("未检测到（不应发生）。\n");
    }
    printf("volatile 告诉编译器该变量可能被外部改变，禁止优化掉看似多余的读取。\n");
}

// ==================== 4. static 关键字进阶用法 ====================
// 4.1 限制作用域：static 函数只在当前文件可见
static int helper_add(int a, int b)
{
    return a + b;
}

// 4.2 静态局部变量：生命周期贯穿整个程序，但作用域在函数内
void counter_demo(void)
{
    static int call_count = 0; // 只初始化一次，值在函数调用间保持
    call_count++;
    printf("counter_demo 被调用了 %d 次\n", call_count);
}

// 4.3 静态全局变量：只能在本文件内访问，避免命名冲突
static int module_private_data = 100;

void demo_static(void)
{
    printf("\n========== 4. static 关键字进阶 ==========\n");
    printf("static 作用：\n");
    printf("1. 修饰局部变量：生命周期变为程序运行期间，值在函数调用间保持。\n");
    counter_demo();
    counter_demo();
    printf("2. 修饰全局变量/函数：限制作用域为本文件，避免符号冲突。\n");
    printf("   模块私有数据: %d\n", module_private_data);
    printf("   调用静态函数 helper_add(3,5)=%d\n", helper_add(3, 5));
}

int main(void)
{
    // ========== 1. const 修饰指针的三种场景 ==========
    printf("========== 1. const 修饰指针三种场景 ==========\n");

    int a = 10, b = 20;

    // 场景1：const int *p  指向常量的指针（不能通过 p 修改所指向的值）
    const int *p1 = &a;
    printf("p1 指向 %d\n", *p1);
    // *p1 = 30;          // 错误！不能通过 p1 修改 a
    p1 = &b; // 可以修改 p1 本身的指向
    printf("p1 改为指向 %d\n\n", *p1);

    // 场景2：int * const p  指针常量（不能修改指针本身的指向）
    int *const p2 = &a;
    *p2 = 30; // 可以修改所指向的值
    printf("通过 p2 修改 a = %d\n", a);
    // p2 = &b;           // 错误！不能修改 p2 的指向

    // 场景3：const int * const p  指向常量的指针常量（都不能改）
    const int *const p3 = &a;
    // *p3 = 40;          // 错误！不能修改值
    // p3 = &b;           // 错误！不能修改指向
    printf("p3 只能读，指向的值 = %d\n\n", *p3);

    // ========== 2. void* 通用指针 ==========
    printf("========== 2. void* 通用指针 ==========\n");

    int num = 100;
    float pi = 3.14f;

    void *vptr; // 通用指针，可以指向任意类型
    vptr = &num;
    printf("vptr 指向 int，值: %d\n", *(int *)vptr); // 必须强转回 int*

    vptr = &pi;
    printf("vptr 指向 float，值: %.2f\n", *(float *)vptr);

    // 典型场景：内存拷贝函数（如 memcpy）的参数使用 void* 实现泛型
    printf("void* 常用于内存操作函数（如 memcpy），实现与类型无关的代码。\n\n");

    // ========== 3. 指向结构体的指针 ==========
    printf("========== 3. 指向结构体的指针 ==========\n");

    Point pt = {5, 10}; // 普通结构体变量
    Point *ppt = &pt;   // 指针指向结构体

    // 通过指针访问成员（两种方式）
    printf("通过指针访问: (%d, %d)\n", (*ppt).x, (*ppt).y);
    printf("使用箭头运算符: (%d, %d)\n", ppt->x, ppt->y);

    // 通过指针修改结构体成员
    ppt->x = 99;
    ppt->y = 100;
    printf("修改后: (%d, %d)\n", pt.x, pt.y);

    // 结构体指针的典型用途：作为函数参数（传址，避免拷贝）
    void print_point(const Point *p)
    {
        printf("Point: (%d, %d)\n", p->x, p->y);
    }
    print_point(ppt);

    // ========== 1. 结构体初始化与访问 ==========
    Robot r1 = {"Turtle", {0, 0}, 100, FORWARD};
    printf("机器人: %s, 位置(%d,%d), 速度=%d, 状态=%d\n",
           r1.name, r1.pos.x, r1.pos.y, r1.speed, r1.state);

    // 修改成员
    r1.speed = 150;
    strcpy(r1.name, "Explorer");
    printf("修改后: %s, 速度=%d\n\n", r1.name, r1.speed);

    // ========== 2. 结构体指针 ==========
    Robot *p = &r1;
    p->speed = 200; // 等价于 (*p).speed
    printf("通过指针修改速度: %d\n\n", r1.speed);

    // ========== 3. 结构体作为函数参数 ==========
    changeByValue(r1);
    printf("传值调用后速度: %d (不变)\n", r1.speed);
    changeByPointer(&r1);
    printf("传址调用后速度: %d (已改)\n\n", r1.speed);

    // ========== 4. 结构体数组 ==========
    Sensor sensors[3] = {{1, 25.5}, {2, 26.0}, {3, 24.8}};
    printf("传感器数组:\n");
    for (int i = 0; i < 3; i++)
    {
        printf("  ID=%d, 值=%.1f\n", sensors[i].id, sensors[i].value);
    }
    printf("\n");

    // ========== 5. 内存对齐演示 ==========
    printAlign("char", sizeof(char));
    printAlign("int", sizeof(int));
    printAlign("Robot", sizeof(Robot)); // 因对齐可能大于成员大小之和
    printf("说明: 编译器可能填充字节，使结构体大小为最大成员的整数倍\n\n");

    // ========== 5. 内存对齐演示 ==========
    printAlign("char", sizeof(char));
    printAlign("int", sizeof(int));
    printAlign("Robot", sizeof(Robot)); // 因对齐可能大于成员大小之和
    printf("说明: 编译器可能填充字节，使结构体大小为最大成员的整数倍\n\n");
    // ========== 6. 联合体（共享内存） ==========
    DataUnion u;
    u.i = 65;
    printf("联合体: int=%d, 字符=%c, float=%f\n", u.i, u.c[0], u.f);
    u.f = 3.14f;
    printf("赋值float后: int=%d, float=%f\n", u.i, u.f);
    printf("联合体大小: %zu 字节 (最大成员的大小)\n\n", sizeof(DataUnion));
    // ========== 7. 枚举用法 ==========
    RobotState s = BACKWARD;
    printf("枚举值 BACKWARD = %d\n", s);
    printf("枚举提高代码可读性，实际是整数常量\n");

    int num = 10;//二进制：1010
    num = num | (1 << 3); //把第3位设为1
    num = num & ~(1 << 1);//把第一位设为0
    num = num ^ (1 << 2);//翻转第2位

    //位状态判断(检查是0还是1)
    int num = 10;
    if(num&(1<<1)){
        printf("第一位是1\n");//会执行
    }else{
        printf("第一位是0\n");
    }
    demo_bitwise_ops();
    demo_bitfield();
    demo_endianness();
    demo_register_ops();
    demo_macro_basic();
    demo_macro_advanced();
    demo_conditional_compile();
    demo_include_rules();
    demo_char_rw();
    demo_string_rw();
    demo_binary_rw();
    demo_random_access();
    demo_scenarios();
    demo_errno();
    demo_perror_strerror();
    demo_assert();
    demo_debug_tools();
    embedded_features();
    demo_interrupt();
    demo_volatile();
    demo_static();
    return 0;
}