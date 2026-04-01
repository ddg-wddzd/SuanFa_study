#include<stdio.h>
//全局变量：静态存储器，整个程序运行期间存在，作用域为整个文件
int global_var = 10;
//静态全局变量：作用域仅限于本文件，其它文件无法访问
static int static_global = 20;
void func(void){
    //局部变量：自动存储器，每次进入函数时创建，退出时销毁
    int local = 30;
    //静态局部变量：静态存储器，只在第一次进入时初始化，值在函数调用时保持
    static int static_local = 0;
    static_local++;
    printf("func:local=%d,static_local=%d,global_val=%d\n",
           local, static_local, global_var);
}
//const用法示例
void const_example(void){
    const int a = 100;//只读变量，不可修改
    //a = 200; //编译错误，不能修改const变量
    //常量的正确用法：用宏或枚举
    #define SIZE 10
    int arr[SIZE]; //宏在预处理阶段替换，符合常量要求
    printf("const a=%d,array size=%d\n", a, SIZE);
}
//宏定义：全大写，下划线分隔
#define MAX_STUDENTS 100
#define PI 3.14159
//结构体类型：首字母大写，或加-t后缀
typedef struct Student{
    char name[50];
    int age;
} Student_t; //_t后缀表示类型
//全局变量：加g_前缀
int g_student_count = 0;
//函数：小写字母+下划线
void print_student_info(const Student_t *student)
{
    //局部变量：小写字母+下划线
    int temp_id = 0;
    printf("Name:%d,Age:%d\n", student->name, student->age);
}
int main(void) //void表示不接受参数
{
    //unsigned 修饰符的影响
    unsigned int u = 0;
    printf("u=%u\n", u);
    u = u - 1;//无符号整数下溢，结果为最大值4294967295
    printf("u-1=%u\n", u);
    //有符号与无符号混用的陷阱
    int signed_num = -1;
    // 超级重点：C语言的【隐式类型转换】规则
    // 有符号数 和 无符号数 比较时，有符号数会自动转为无符号数
    // signed_num = -1 转为无符号数后，结果也是 4294967295
    // 所以实际比较的是：4294967295 < 4294967295 → 条件不成立
    if(signed_num<u){//这里发生隐式类型转换，-1被转换为无符号整数，结果为4294967295，因此条件为假
        printf("-1 < u\n");
    }else{
        //程序会执行这里！
        printf("-1<%u为假（注意：因为比较时-1被当作无符号数处理）\n", u);
    }
    printf("Hello\n");
    printf("char    :%zu 字节\n", sizeof(char));//%zu是C17中用于打印size_t类型的格式说明符，，char通常1字节
    print("short   :%zu 字节\n", sizeof(short));//short通常2字节
    printf("int  :%zu字节\n", sizeof(int));//int通常4字节
    print("long :%zu字节\n", sizeof(long));//long通常4字节（32位系统）或8字节（64位系统）
    print("long long ：%zu字节\n", sizeof(long long));//long long通常8字节
    printf("float :%zu字节\n", sizeof(float));//float通常4字节
    printf("double :%zu字节\n", sizeof(double));//double通常8字节
    printf("main:global_var=%d\n", global_var);
    func();///static_local 变为1
    func();//sstatic_local 变为2
    const_example();
    //变量命名：清晰，有意义
    int student_age = 20;
    float average_score = 85.5f;
    Student_t stu = {"Alice", student_age};
    print_student_info(&stu);
    //使用宏
    printf("Max students:%d\n", MAX_STUDENTS);
    return 0;
}
