#include<stdio.h>
/*How to use symbols or codes to present the essence or process of how to solve the problem that might write in paper or think in brain?
Firstly,to write a block to represent the essencial opration
Secondly,to make sure the end condition; just do it! 
Thirdly,to optimize
*/
int N=5;
void BubbleSort(int a[],int n);
int main()
{
    int a[5]={0};
    int i;
    for(i=0;i<N;i++){
        scanf("%d",&a[i]);
    }
    BubbleSort(a,N);
    for(i=0;i<N;i++){   
        printf("a[%d]=%d\n",i,a[i]);
    }
    return 0;
}
void BubbleSort(int a[],int n){
    int i,j;
    int tmp;
    for(i=1;i<n;i++)//外层循环控制次数
    {
        for(j=0;j<n-i;j++){//内层循环控制元素从a[0]开始走到有效区域为止,也因此-i
            if(a[j]>a[j+1]){
                tmp=a[j];
                a[j]=a[j+1];
                a[j+1]=tmp;
            }
        }
    }
}