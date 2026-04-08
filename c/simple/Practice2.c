#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// 1.反转字符串(递归实现)
void reverse_string_recursive(char *str, int left, int right)
{
    if (left >= right)
        return;
    char temp = str[left];
    str[left] = str[right];
    str[right] = temp;
    reverse_string_recursive(str, left + 1, right - 1);
}
// 2.统计字符串，单词，行数
void count_string_info(const char *str, int *chars, int *words, int *lines)
{
    *chars = strlen(str);
    *words = 0;
    *lines = 1;
    int in_word = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
        {
            if (in_word)
            {
                (*words)++;
                in_word = 0;
            }
            if (str[i] == '\n')
            {
                (*lines)++;
            }
        }
        else
        {
            in_word = 1;
        }
    }
    if (in_word)
    {
        (*words)++;
    }
}
// 3.查找子串（返回位置，-1表示未找到）
int find_substring(const char *str, const char *sub)
{
    int str_len = strlen(str);
    int sub_len = strlen(sub);
    for (int i = 0; i <= str_len - sub_len; i++)
    {
        int j;
        for (j = 0; j < sub_len; j++)
        {
            if (str[i + j] != sub[j])
            {
                break;
            }
        }
        if (j == sub_len)
        {
            return i; // 找到子串
        }
    }
    return -1; // 未找到
}

//4.动态创建字符串副本
char *duplicate_string(const char *str){
    if(str==NULL)
        return NULL;
    int len = strlen(str);
    char *copy = (char *)malloc((len + 1) * sizeof(char));
    if(copy==NULL){
        printf("内存分配失败！\n");
        return NULL;
    }
    strcpy(copy, str);
    return copy;
}
int main(){
    char str[] = "Hello World\nThis is a test\nString operations";
    printf("原始字符串:\n%s\n\n", str);
    //1.反转字符串
    char *reversed = duplicate_string(str);
    if(reversed){
        reverse_string_recursive(reversed, 0, strlen(reversed) - 1);
        printf("反转后的字符串:\n%s\n\n", reversed);
        free(reversed);
    }
    //2.统计信息
    int chars, words, lines;
    count_string_info(str, &chars, &words, &lines);
    printf("字符串统计:\n");
    printf("  字符数: %d\n", chars);
    printf("  单词数: %d\n", words);
    printf("  行数: %d\n\n", lines);
    //3.查找子串
    char *sub = "test";
    int pos = find_substring(str, sub);
    if (pos != -1)
    {
        printf("子串 \"%s\" 在位置 %d 找到\n\n", sub, pos);
    }
    else
    {
        printf("子串 \"%s\" 未找到\n\n", sub);
    }
    //4.测试动态内存分配
    char *copy = duplicate_string(str);
    if(copy){
        printf("字符串副本：\n%s\n", copy);
        free(copy);
    }
    return 0;
    
}