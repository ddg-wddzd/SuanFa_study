#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//1.反转字符串(递归实现)
void reverse_string_recursive(char *str,int left,int right){
    if(left>=right)
        return;
    char temp = str[left];
    str[left] = str[right];
    str[right] = temp;
    reverse_string_recursive(str, left + 1, right - 1);
}
//2.统计字符串，单词，行数
void count_string_info(const char *str,int *chars,int *words,int *lines){
    *chars = strlen(str);
    *words = 0;
    *lines = 1;
    int in_word = 0;
    for (int i = 0; str[i] != '\0';i++){
        if(str[i]==' '||str[i]=='\t'||str[i]=='\n'){
            if(in_word){
                (*words)++;
                in_word = 0;
            
            }
            if(str[i]=='\n'){
                (*lines)++;
            }
        }else{
            in_word = 1;
        }
    }
    if(in_word){
        (*words)++;
    }
}
//3.查找子串（返回位置，-1表示未找到）
int find_substring(const char *str,const char *sub){
    int str_len = strlen(str);
    int sub_len = strlen(sub);
    for (int i = 0; i <= str_len - sub_len;i++){
        int j;
        for (j = 0; j < sub_len;j++){
            if(str[i+j]!=sub[j]){
                break;
            }
        }
        if(j==sub_len){
            return i;//找到子串
        }
    }
    return -1;//未找到
}