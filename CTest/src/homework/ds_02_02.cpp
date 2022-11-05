/*
 * @Descripttion: 
 * @version: 
 * @Author: Yihui
 * @Date: 2019-10-04 20:13:47
 * @LastEditors: Yihui
 * @LastEditTime: 2019-10-05 09:21:16
 */
//基本思路:让前半部分与从栈出来的后半部分进行比较,如果符合模式要求,则两者应该相等
#include<string>
#include<iostream>
#include<cstdio>
#include<cstdlib>
using namespace std;
#define maxSize 100
typedef struct{
    char data[maxSize];
    int top;
}SqStack;
void iniStack(SqStack &st){
    st.top=-1;
}
int pop(SqStack &st,string str,int count){
    if(st.top==maxSize-1)   return 0;
    for(int i=0;i<count;++i){
        st.data[++st.top]=str[i+count+1];
    }
    return 1;
}
char push(SqStack &st){
    char x;
    if(st.top==-1)  return 0;
    x=st.data[st.top--];
    return x;
}

int main(){
    string str;
    cout<<"Please enter the sequence:\n";
    getline(cin,str,'#');int count;
    //求出序列a的长度
    for(int i=0;i<maxSize;++i){
        if(str[i]=='&') break;
        count++;
    }
    SqStack st;iniStack(st); pop(st,str,count);
    int flag=1;
    for(int i=0;i<count;++i){
        if(str[i]!=push(st)){
            flag=0;break;
        }
    }
    //str仍然以'\0'结尾,应该是输入后自动加上去的
    if(flag && str[count*2+1]=='\0'){
            printf("It is a pattern sequence.");
    }else   cout<<"It is not a pattern sequence."<<endl;

    return 0;
}