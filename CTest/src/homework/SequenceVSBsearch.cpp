#include <cstdio>
#include <iostream>
#include <math.h>
using namespace std;
int Seqsearch(int a[],int length,int key,int *s);
int Bsearch(int a[],int low,int high,int key,int *s);
void Quicksort(int a[],int low,int high);
#define Num 10000
int main(){
    int a[Num];
    for(int i=0;i<Num;i++)
        a[i] = ceil(1+rand()%20000);

    Quicksort(a,0,Num);
    double sum_Seq = 0.0,sum_B=0.0;
    int successNum_Seq=0;
    int successNum_B=0;
    int temp;
    for(int i=0;i<Num;i++){
        temp = ceil(1+rand()%20000);// ���������������̫���������ȫ������Ȱ�....
        sum_Seq += Seqsearch(a,Num,temp,&successNum_Seq);
        sum_B += Bsearch(a,0,Num,temp,&successNum_B);
    }
    cout <<"�����ܴ���:"<<Num<<endl;
    cout<<"˳������ɹ��Ĵ���:"<<successNum_Seq<<endl;
    cout<<"���ּ����ɹ��Ĵ���:"<<successNum_B<<endl;
    cout<<"˳������ɹ��İٷֱ�:"<<successNum_Seq*100.0/Num<<"%"<<endl;
    cout<<"���ּ����ɹ��İٷֱ�:"<<successNum_B*100.0/Num<<"%"<<endl;
    cout<<"˳�������ƽ���Ƚϴ���:"<<sum_Seq/Num<<endl;
    cout<<"���ּ�����ƽ���Ƚϴ���:"<<sum_B/Num<<endl;

    return 0;
}
int Seqsearch(int a[],int length,int key,int *s){
    int count=0;
    for(int i=0;i<length;i++){
        count++;
        if(a[i] == key){
            (*s)++;
            return count;
        }
    }
    return length;
}
int Bsearch(int a[],int low,int high,int key,int *s){
    int mid;int count=0;
    // ������Ҳ�൱�ڵݹ���?!!
    while(low <= high){
        count++;
        mid = (low+high)/2;//�Զ�����ȡ��
        if(a[mid]== key){
            (*s)++;
            return count;
        } 
        else if(a[mid]<key) low=mid+1;//k�����������ұ���
        else high=mid-1;
    }
    return count;
}
void Quicksort(int a[],int low,int high){
    int i=low,j=high;
    if(low<high){
        int pivot=a[low];
        while(i<j){ 
            while(a[j]>=a[low] && i<j) --j;
            if(a[j] < a[low] && i<j){
                a[i]=a[j];
                ++i;
            }
            while(a[i]<a[low] && j>i) ++i;
            if(a[i] > a[low] && j>i){
                a[j]=a[i];
                --j;
            }
        }
        a[i] = pivot;
        Quicksort(a,low,i-1);
        Quicksort(a,i+1,high);
    }
}