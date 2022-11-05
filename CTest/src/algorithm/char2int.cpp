/*
 * @Description:
 * @version: 
 * @Author: Yihui
 * @Date: 2019-08-10 22:30:58
 * @LastEditors: Yihui
 * @LastEditTime: 2019-08-10 22:30:58
 */
#include<stdio.h>
#include<stdlib.h>

int char2int(char *str, char *t);

int main(int argc, char const *argv[]) {
    char *str = NULL;
    int n;
    char *temp = NULL;
    printf("Enter the designed number of chars:");
    scanf("%d", &n);
    str = (char *) malloc(n * sizeof(char));
    gets(temp);
    gets(str);
    int i = char2int(str, temp);
    if (*temp) printf("%d", i);
    free(str);
    return 0;
}
/**
 *
 * @param str
 * @param t
 * @return
 */
int char2int(char *str, char *t) {
    int num = 0;
    if (str[0] != '0') {
        while (*str != '\0') {
            if (*str >= '0' && *str <= '9') {
                num = (*str - '0') + num * 10;
                *t = 1;
            } else {
                printf("illegal number");
                *t = 0;
                break;
            }
            str++;
        }
    } else {
        printf("illegal number");
        *t = 0;
    }
    return num;
}