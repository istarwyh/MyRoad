/*
 * @Description:
 * @version: 
 * @Author: Yihui
 * @Date: 2019-10-13 10:04:45
 * @LastEditors: Yihui
 * @LastEditTime: 2019-10-15 14:46:05
 */
#include<string>//ע��<string.h>��<cstring>Ϊ��string���Ǵ����
#include<cstring>//����Ϊ�˺�C����
#include<assert.h>
#include<iostream>
using namespace std;
int *Next(string P){
    int m = P.length();
    assert(m>0);//Ҫ�ǿ������治��
    int *N = new int[m];
    assert(N!=0);//Ҫ��û���ڴ����治��
    N[0]=0;
    for(int i=1;i<m;i++){
        //���i����1��ʼ,���ʾͻ�Խ��,Ȼ�����ͱ���
        int k =N[i-1];//��(i-1)λ�õ��ǰ׺������,k���ٴ�0��ʼ
        //����while�����ƾ������ʵ�ǰ׺λ��k
        while (k>0 && P[i]!=P[k])
        {
            k = N[k-1];//�����㲻ͬ�ĵ�����,kÿһ�δ���һ�εĺ�׺���ȼ�����ʼ
        }
        if(P[i] == P[k])    N[i]=k+1;//�ǰ׺��׺��ȴ��Ӹ�1
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
    cout<<"�±���:"<<KMP_FindPat(txt,pat,t,0)<<endl;
    return 0;
}
