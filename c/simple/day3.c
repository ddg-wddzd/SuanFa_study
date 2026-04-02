#include <stdio.h>
int main(void){
    int option;
    printf("请输入选项(1-3)：");
    scanf("%d", &option);
    switch(option){
        case 1:
            printf("选择了选项1\n");
            break;
        case 2:
            printf("选择了选项2\n");
            break;
        case 3:
            printf("选择了选项3\n");
            break;
        default:
            printf("这是default分支，通常处理未匹配的值\n");
            break;
    }
    char grade = 'B';
    printf("\n参加等级%c:", grade);
    switch(grade){
        case'A':
        case'B':
        case'C':
            printf("通过\n");
            break;
        case'D':
            printf("不通过\n");
            break;
        default:
            printf("无效等级\n");
        }
        return 0;
}