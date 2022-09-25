/*
 * @Description:
 * @version: 
 * @Author: Yihui
 * @Date: 2019-10-13 10:04:45
 * @LastEditors: Yihui
 * @LastEditTime: 2019-10-15 14:46:05
 */
#include<string>//注意<string.h>和<cstring>为了string则都是错误的
#include<cstring>//这是为了和C兼容
#include<assert.h>
#include<iostream>
using namespace std;
int *Next(string P){
    int m = P.length();
    assert(m>0);//要是空数组玩不了
    int *N = new int[m];
    assert(N!=0);//要是没有内存了玩不了
    N[0]=0;
    for(int i=1;i<m;i++){
        //如果i不从1开始,访问就会越界,然后程序就崩了
        int k =N[i-1];//第(i-1)位置的最长前缀串长度,k至少从0开始
        //以下while语句递推决定合适的前缀位置k
        while (k>0 && P[i]!=P[k])
        {
            k = N[k-1];//与手算不同的点在于,k每一次从上一次的后缀长度继续开始
        }
        if(P[i] == P[k])    N[i]=k+1;//最长前缀后缀相等串加个1
        else N[i]=0;
    }
    return N;
}
int KMP_FindPat(string S,string P, int *Next,int startIndex){
    int lastIndex = S.length() -P.length();
    if((lastIndex - startIndex)<0)  return -1;
    int i;
    int j=0;
    for (i=startIndex;i<S.length();i++){
        while(P[j]!=S[i] && j>0)   j=Next[j-1];
        if(P[j]==S[i]) j++;
        if(j==P.length()-1) return i-P.length()+1;
    }
    return -1;
}
int main(){
    string pat ="abac";
    // string txt;getline(cin,txt);
    string txt;cin>>txt;
    int *t=Next(pat);
    for(int i=0;i<pat.length();i++){
        cout<<t[i];
    }
    cout<<"123"<<endl;
    cout<<"\n"<<endl;
    cout<<"下标是:"<<KMP_FindPat(txt,pat,t,0)<<endl;
    return 0;
}
