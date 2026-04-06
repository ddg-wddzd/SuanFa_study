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
#include <string.h>

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

    return 0;
}