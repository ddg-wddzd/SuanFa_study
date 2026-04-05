// 栈帧：每调用一个函数，系统就会在内存栈上给它开一个专属小格子，这个小格子就叫栈帧。
// 栈从高地址向低地址增长，栈帧是连续分配的
// 每次调用函数，就会在栈上压入一个栈帧；
// 一个栈帧里主要存这 3 样东西：形参（函数接收到的参数）,局部变量（函数里定义的变量）,返回地址（执行完要回到哪个函数、哪一行）
#include<stdio.h>
    void func1(int n){
    printf("func1:n=%d,&n=%p\n", (void *)&n);
}
void func2(int m){
    printf("func2:m=%d,&m=%p\n", (void *)&m);
}

// 头文件  .h   告诉编译器：有这个函数，长这样，但具体实现我不写这。
// tool.h （只写声明，不写函数体）
// #ifndef TOOL_H    如果没定义TOOL_H
// #define TOOL_H    就定义它
//上面两行是防止重复包含
// 函数声明
//int max(int a, int b);
//void printLine(char ch, int len);
//#endif
//就是函数声明



// 源文件  .c  写真正实现
//  tool.c
//#include "tool.h"
//#include <stdio.h>
// 函数实现（定义）
//int max(int a, int b)
//{
    //return a > b ? a : b;
//}
//void printLine(char ch, int len)
//{
   // for (int i = 0; i < len; i++)
       //putchar(ch);
   // putchar('\n');
//}




// main.c 调用它们
//  main.c
//#include <stdio.h>
//#include "tool.h"
//int main()
//{
    //printLine('-', 20);
    //printf("max = %d\n", max(10, 20));
    //printLine('-', 20);
   // return 0;
//}


//#include规则
//#include<stdio.h>     找系统自带头文件（库函数）
//#include"tool.h"     先找当前文件夹自己写的头文件


//下面就是main.c调用
int main(){
    int x = 10;
    printf("main:x=%d,&x=%p\n", x, (void *)&x);
    func1(x);
    int a=10;
    printf("a=%d,地址：%p\n", a, &a);
    //指针变量：存放地址
    int *p;//定义指针
    p = &a;//初始化：p指向a
    printf("p指向的地址：%p,值：%d\n", p,* p); //解引用：通过指针访问a
    *p = 99;
    printf("修改后a=%d\n", a);
    int *q = NULL;//空指针，不能解引用
    printf("空指针地址：%p\n", q);

    return 0;
}