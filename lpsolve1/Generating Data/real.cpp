#include<iostream>
using namespace std;
int main(){
  int a[56]={0};
  int i;
  float j;
  for(i=0;i<951;i++){
    cin>>j;
    j*=1000;
    j/=2;
    j-=32;
    int temp=j;
    a[temp]++;
  }
  int count=0;
  for(i=0;i<56;i++){
    if(a[i])count++;
  }
  //cout<<count;
  int index1=0,index2=0,k;
  float sum1;
  cout<<count<<endl;
  float ptpm[count][count];
  for(i=0;i<count;i++){
    for(k=0;k<count;k++)ptpm[i][k]=0;
  }
  cin>>j;
  int temp=(j*500)-32;
  for(k=0;k<temp;k++)if(a[k])index1++;
  for(i=1;i<951;i++){    
    cin>>j;
    temp=(j*500)-32;
    for(k=0;k<temp;k++)if(a[k])index2++;
    ptpm[index1][index2]++;
    index1=index2;
    index2=0;
  }
  
  for(i=0;i<count;i++){
    for(k=0;k<count;k++)cout<<ptpm[i][k]<<' ';
    cout<<'\n';
  }
  /*
  for(i=0;i<count;i++){
    sum1=0;
    for(k=0;k<count;k++){
      sum1+=ptpm[i][k];
    }
    for(k=0;k<count;k++){
      ptpm[i][k]/=sum1;
    }
  }
  
  cout<<'\n';
  for(i=0;i<count;i++){
    for(k=0;k<count;k++)cout<<ptpm[i][k]<<' ';
    cout<<'\n';
  }
  */
  return 0;
}
