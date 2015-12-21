#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "lp_lib.h"
//all definitions can be changed according to the given case
#define nspot 50   
#define tc 0.1
#define tr 0.2
#define W 15
#define td 100
#define num 100


int rand_num(){
  int r = rand();
  return (1+r%nspot);
}

int rand_ptpm(REAL** P,int b){
  REAL* c;
  int i;
  c=(REAL*)malloc(nspot*sizeof(REAL));
  for(i=0;i<nspot;i++){
    c[i]=P[b-1][i];
  }
  for(i=1;i<nspot;i++){
     c[i]+=c[i-1];
  }
  for(i=0;i<nspot;i++){
    c[i]*=1000;
  }
  int r=rand();
  r=(r%1000);
  for(i=0;i<nspot;i++){
    if(c[i]>=r)break;
  }
  return (i+1);
}

int** test1(REAL** P){    //creating num test-cases on which we test our program
  int** a;
  int i,j;
  a=(int**)malloc(num*sizeof(int*));
  for(i=0;i<num;i++){
    a[i]=(int*)malloc(2*td*sizeof(int));
  }
  for(i=0;i<num;i++){
    a[i][0]=rand_num();
    //random number from 1 to nspot
    // printf("%d\n",a[i][0]);
    for(j=1;j<2*td;j++){
       a[i][j]=rand_ptpm(P,a[i][j-1]);   //random value of spot price based on j-1 using PTPM
    }
  }
  return a;
}


int main(){
  REAL** P=(REAL**)malloc(nspot*sizeof(REAL*));
  int i,j;
  REAL* row;
  srand(time(NULL));
  for(i=0;i<nspot;++i){
    P[i]=(REAL*)malloc(nspot*sizeof(REAL));
  }
  for(i=0;i<nspot;++i){    //entering the given matrix for testing, to be modified later as per transition matrix
    for(j=0;j<nspot;++j){
      if(i==j)P[i][j]=0.51;
      else P[i][j]=0.01;
    }
  }    
  //row=amazing(P);
  int** tester;
  tester=test1(P);
  for(i=0;i<num;i++){
    for(j=0;j<2*td;j++){
      printf("%d ",tester[i][j]);
    }
    printf("\n");
  }

  printf("%d",rand_num());
  return 0;
}
