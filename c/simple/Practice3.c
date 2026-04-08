#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX_NAME_LEN 50
#define MAX_STUDENTS 100
//学生结构体
typedef struct{
    int id;
    char name[MAX_NAME_LEN];
    float score;
} Student;
//学生管理系统结构体
typedef struct{
    Student students[MAX_NAME_LEN];
    int count;
} StudentManager;
//初始化管理器
void init_manager(StudentManager *manager){
    manager->count = 0;

}
//添加学生
int add_student(StudentManager *manager,int id,const char *name,float score){
    if(manager->count >=MAX_STUDENTS){
        printf("错误：学生数量已达上限!\n");
        return 0;
    }
    //检查ID是否重复
    for (int i = 0; i < manager->count;i++){
        if(manager->students[i].id==id){
            printf("错误：学号%d已存在！\n", id);
            return 0;
        }
    }
    Student *stu = &manager->students[manager->count];
    stu->id = id;
    strncpy(stu->name, name, MAX_NAME_LEN - 1);
    stu->name[MAX_NAME_LEN - 1] = '\0';
    stu->score = score;
    manager->count++;
    return 1;
}
//删除学生
int delete_student(StudentManager *manager,int id){
    for (int i = 0; i < manager->count;i++){
        if (manager->students[i].id == id){
            // 将后面的学生往前移动
            for (int j = i; j < manager->count - 1; j++)
            {
                manager->students[j] = manager->students[j + 1];
            }
        manager->count--;
        return 1;
        }
    }
    printf("错误：未找到学号为%d的学生\n", id);
    return 0;
}
//查找学生（返回指针）
Student* find_student(StudentManager *manager,int id){
    for (int i = 0; i < manager->count;i++){
        if(manager->students[i].id==id){
            return &manager->students[i];
        }
    }
    return NULL;
}
//显示所有学生
void display_students(StudentManager *manager){
    if(manager->count==0){
        printf("赞无学生信息\n");
        return;
    }
    printf("=== 学生列表（共%d人） ===\n", manager->count);
    printf("%-8s %-20s %-8s\n", "学号", "姓名", "成绩");
    printf("---------------------------------\n");
    for (int i = 0; i < manager->count;i++){
        Student *stu = &manager->students[i];
        printf("%-8d %-20s %-8.1lf\n", stu->id, stu->name, stu->score);
    }
    printf("\n");
}
//按成绩排序(冒泡排序)
void sort_by_score(StudentManager *manager){
    for (int i = 0; i < manager->count - 1;i++){
        for (int j = 0; j < manager->count - i - 1;j++){
            if(manager->students[j].score<manager->students[j+1].score){
                //交换学生
                Student temp = manager->students[j];
                manager->students[j] = manager->students[j + 1];
                manager->students[j + 1] = temp;
            }
        }
    }
}
//计算平均分
float calculate_average(StudentManager *manager){
    if(manager->count==0)
        return 0.0;

    float sum = 0.0;
    for (int i = 0; i < manager->count;i++){
        sum += manager->students[i].score;
    }
    return sum / manager->count;
}
int main(){
    StudentManager manager;
    init_manager(&manager);

    printf("=== 学生管理系统 ===\n\n");

    // 添加学生
    add_student(&manager, 1001, "张三", 85.5);
    add_student(&manager, 1002, "李四", 92.0);
    add_student(&manager, 1003, "王五", 78.5);
    add_student(&manager, 1004, "赵六", 88.0);

    // 显示所有学生
    display_students(&manager);

    // 查找学生
    int search_id = 1002;
    Student *found = find_student(&manager, search_id);
    if (found)
    {
        printf("找到学生: 学号=%d, 姓名=%s, 成绩=%.1f\n\n",
               found->id, found->name, found->score);
    }

    // 计算平均分
    printf("平均分: %.2f\n\n", calculate_average(&manager));

    // 按成绩排序
    printf("=== 按成绩排序后 ===\n");
    sort_by_score(&manager);
    display_students(&manager);

    // 删除学生
    printf("=== 删除学号1003 ===\n");
    delete_student(&manager, 1003);
    display_students(&manager);

    // 尝试添加重复学号
    printf("=== 尝试添加重复学号 ===\n");
    add_student(&manager, 1001, "钱七", 95.0);

    return 0;
}