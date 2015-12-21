#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "lp_lib.h"

//all definitions can be changed according to the given case
#define nspot 33
#define tc 0
#define tr 0
#define W 100
#define td 200
#define num 100
float temp1;
REAL* amazing(REAL** P) //P is the transition matrix
{
  //FILE* fp;
  //FILE* output;  
  //output=fopen("lpout1.txt","a");
  //fprintf(output,"td: %d\nW: %d\n",td,W);
  //fp=fopen("lpout.txt","a");
  lprec *lp;
  int Ncol, *colno = NULL, j,i,k, ret = 0;
  REAL *row = NULL;
  Ncol=8*nspot;
  lp=make_lp(0,Ncol);
  if(lp==NULL)ret=1;
  if(ret==0){
    colno=(int*)malloc(Ncol*sizeof(*colno));
    row=(REAL*)malloc(Ncol*sizeof(*row));
    if((colno==NULL)||(row==NULL))ret=2;
  }
  if(ret==0){
    //create the 1st constraint : rho(xi,ai)>=0 for all xi and ai
    set_add_rowmode(lp,TRUE);
    
    for(j=0;j<Ncol;++j){
      colno[j]=j+1;
      row[j]=0;
    }
    row[0]=1;
    for(j=1;j<Ncol;++j){
      if(!add_constraintex(lp, Ncol, row, colno, GE, 0))ret = 3;
      row[j-1]=0;
      row[j]=1;
    }
    if(!add_constraintex(lp, Ncol, row, colno, GE, 0))ret = 3;    
  }
  
  if(ret==0){
    //the third constraint: sum(rho(xi,ai))=1
    for(j=0;j<Ncol;++j){
      row[j]=1;
    }
    if(!add_constraintex(lp, Ncol, row, colno, EQ, 1))ret = 3;
    //write_LP(lp,stdout);  
  }

  
  if(ret==0){
    //Note: the naming scheme is as follows- 0 to nspot is 111, nspot to 2*nspot is 110 etc
    //where 1 is in bid, 0 is out of bid, 1st digit for previous, middle digit for current and last digit for bid option taken(B,G)
    //creating the 2nd constraint which says average expected execution progress per instance hour is greater than W/td
    for(j=0;j<Ncol;++j){
      row[j]=0;
    }
    for(j=0;j<nspot;++j){
      row[j]=1;
    }
    for(j=nspot;j<2*nspot;++j){
      row[j]=1-tc;
    }
    for(j=2*nspot;j<3*nspot;++j){
      row[j]=1-tr;
    }
    for(j=3*nspot;j<4*nspot;++j){
      row[j]=1-tc-tr;
    }
    float z=(float)W/td;
    
    if(!add_constraintex(lp, Ncol, row, colno, GE, z))ret = 3;
    //write_LP(lp,stdout);
  }
  //the next 4 entries to the lp together constitute the last constraint
  //which states that incoming and outgoing rate for a state must be equal
  if(ret==0){
    for(j=0;j<Ncol;j++){
      row[j]=0;
    }
    for(j=0;j<nspot;j++){
      row[j]=1;
      row[j+nspot]=1;
      for(i=0;i<nspot;++i){
	row[i]-=P[i][j];
      }
      for(i=2*nspot;i<3*nspot;++i){
       	row[i]-=P[i%nspot][j];
      }
      if(!add_constraintex(lp, Ncol, row, colno, EQ, 0))ret = 3;
      for(k=0;k<Ncol;++k){
	row[k]=0;
      }
    }
    //write_LP(lp,stdout);
  }
  if(ret==0){
    for(j=0;j<Ncol;j++){
      row[j]=0;
    }
    for(j=2*nspot;j<3*nspot;j++){
      row[j]=1;
      row[j+nspot]=1;
      for(i=4*nspot;i<5*nspot;++i){
	row[i]-=P[i%nspot][j%nspot];
      }
      for(i=6*nspot;i<7*nspot;++i){
	row[i]-=P[i%nspot][j%nspot];
      }
      if(!add_constraintex(lp, Ncol, row, colno, EQ, 0))ret = 3;
      for(k=0;k<Ncol;k++){
	row[k]=0;
      }
    }
    //if(ret==0) write_LP(lp,stdout);
  }
  if(ret==0){
    for(j=0;j<Ncol;j++){
      row[j]=0;
    }
    for(j=4*nspot;j<5*nspot;j++){
      row[j]=1;
      row[j+nspot]=1;
      for(i=nspot;i<2*nspot;++i){
	row[i]-=P[i%nspot][j%nspot];
      }
      for(i=3*nspot;i<4*nspot;i++){
	row[i]-=P[i%nspot][j%nspot];
      }
      if(!add_constraintex(lp, Ncol, row, colno, EQ, 0))ret = 3;
      for(k=0;k<Ncol;k++){
	row[k]=0;
      }
    }
    //if(ret==0) write_LP(lp,stdout);
  }
  if(ret==0){
    for(j=0;j<Ncol;j++){
      row[j]=0;
    }
    for(j=6*nspot;j<7*nspot;j++){
      row[j]=1;
      row[j+nspot]=1;
      for(i=5*nspot;i<6*nspot;++i){
	row[i]-=P[i%nspot][j%nspot];
      }
      for(i=7*nspot;i<8*nspot;i++){
	row[i]-=P[i%nspot][j%nspot];
      }
      if(!add_constraintex(lp, Ncol, row, colno, EQ, 0))ret = 3;
      for(k=0;k<Ncol;k++){
	row[k]=0;
      }
    }
    //if(ret==0) write_LP(lp,stdout);
  }  
  //now it is time to set the objective function, which we then minimise

  if(ret == 0) {
    FILE* prices;
    prices=fopen("out.txt","r");
    set_add_rowmode(lp, FALSE);
    j = 0;
    for(j=0;j<nspot;j++){
      float f;
      fscanf(prices,"%f",&f);
      f*=1000;
      row[j]=f;
      row[j+nspot]=f;
      row[j+2*nspot]=f;
      row[j+3*nspot]=f;
    }
    /* set the objective in lpsolve */
    if(!set_obj_fnex(lp, Ncol, row, colno))
      ret = 4;
  }

  if(ret == 0){
    /* set the object direction to minimize */
    set_minim(lp);    
    write_LP(lp,stdout);
    //set_verbose(lp, IMPORTANT);//only important messages
    /* Now let lpsolve calculate a solution */
    ret = solve(lp);
    //if(ret == OPTIMAL)
    //ret = 0;
    //else
    //ret = 5;
  }

  if(ret == 0) {
    //printing the solved lp values
    printf("E[M]: %f\n", get_objective(lp));
    //fprintf(output,"E[M]: %f\n", get_objective(lp));
    temp1=get_objective(lp);
    /* variable values */
    get_variables(lp, row);
    for(j=0;j<Ncol;j++)printf("%s: %f\n", get_col_name(lp, j + 1), row[j]);

    /* we are done now */
  }
  return row;
}

int rand_choose(float a,float b){
  //gives 1 or 0 randomly based on their probabilities
  //here a is the probability of 1 and b is the probability of 0
  float p1=a/(a+b);  
  p1*=1000000;  
  int r=rand();
  r%=1000000;
  if(r<p1)return 1;
  return 0;
}

int main(){
  FILE* cdfreal;
  cdfreal=fopen("cdfreal.txt","a");
  REAL** P=(REAL**)malloc(nspot*sizeof(REAL*));
  int i,j;
  REAL* row;
  srand(time(NULL));
  for(i=0;i<nspot;i++){
    P[i]=(REAL*)malloc(nspot*sizeof(REAL));
  }
  for(i=0;i<nspot;++i){
    for(j=0;j<nspot;j++){
      if(i==j)P[i][j]=0.3;
      else P[i][j]=0.7/32;
    }
  }
  row=amazing(P);  
  int sum=0;
  int* output;
  output=(int*)malloc(100*td*sizeof(int));
  FILE* prices;
  prices=fopen("prices.txt","r");
  sum=2;
  output[0]=1;
  output[1]=1;
  float price;
  float netsum=0;
  int a[56]={0};
  for(i=0;i<951;i++){
    scanf("%f",&price);
    int temp=price*500-32;
    a[temp]++;
  }
  int count=0;
  for(i=0;i<56;i++){
    if(a[i])count++;
  }
  int index1=0,index2=0,k=0;
  //scanf("%f",&price);
  float avgtime=0;
  int time=0;
  float netsum1=0;
  for(i=0;i<70;i++){
    for(k=0;k<10*i;++k)fscanf(prices,"%f",&price);
    for(j=2;j<951;j++){      
      fscanf(prices,"%f",&price);
      printf("%f\n",price);
      int temp=(price*500)-32;
      for(k=0;k<temp;k++)if(a[k])index1++;
      printf("%d\n",index1);
      //printf("%d ",price);
      //printf("price:%d\n ",price);
      if(output[j-1]==1&&output[j-2]==1){
	output[j]=rand_choose(row[index1],row[index1+nspot]);
	//printf("state=i,i\n");
      }
      else if(output[j-1]==1&&output[j-2]==0){
	output[j]=rand_choose(row[2*nspot+index1],row[3*nspot+index1]);
	//printf("state=i,o\n");
      }
      else if(output[j-1]==0&&output[j-2]==1){
	output[j]=rand_choose(row[4*nspot+index1],row[5*nspot+index1]);
	//printf("state=o,i\n");
      }
      else if(output[j-1]==0&&output[j-2]==0){
	output[j]=rand_choose(row[6*nspot+index1],row[7*nspot+index1]);
	//printf("state=o,o\n");
      }
      index1=0;
      if(output[j]==1){
	sum++;
	netsum1+=price;
      }
      printf("%d ",output[j]);
      //printf("price:%d Bidding option taken:%d\n",price,output[j]);
      if(sum==W){
	//fprintf(output4,"%d\n",j);
	time+=j+1;
	//k=-1;
	break;
      }
      
    }
    avgtime+=time;
    netsum+=netsum1;
    fprintf(cdfreal,"%f\n",netsum1);
    time=0;
    netsum1=0;
    fseek(prices,0,SEEK_SET);
    if(j==950)index2++;
    sum=0;
  }
  //if(k!=-1)avgtime=-1;
  FILE* realout;
  realout=fopen("finalresult.txt","a");
  printf("%d %f %f\n",index2,avgtime/70,netsum/70);
  fprintf(realout,"%d %f %f\n",td,avgtime/70,netsum/70);
  return 0;
}
  
