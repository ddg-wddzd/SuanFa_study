#include<stdio.h>
typedef struct{
    char name[20];
    int score;
} Student;
int main(){
    Student students[3]{
        {"张三", 50};
        {"李四", 60};
        {"老五", 70};
    };
    Student *p = students;
    int sum = 0;
    int count = sizeof(students) / sizeof(students[0]);
    for (int i = 0; i < count;i++){
        sum += p->score;
        printf("%s 的分数:%d\n", p->name, p->score);
        p++;
    }
    printf("平均分：%.2f\n", (float)sum / count);
    return 0;
}