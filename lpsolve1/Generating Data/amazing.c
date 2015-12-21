#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "lp_lib.h"

//all definitions can be changed according to the given case
#define nspot 50
#define tc 0.1
#define tr 0.2
#define W 100
#define td 5000
#define num 1000

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
  }
  
  if(ret==0){
    //Note: the naming scheme is as follows- 0 to nspot is 111, nspot to 2*nspot is 110
    //2*nspot to 3*nspot is 011,3 to 4 is 010, 4 to 5 is 101,5 to 6 is 100
    //6 to 7 is 001 and 7 to 8 is 000.
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
  }
  
  //all constraints for the LP have been created
  //now it is time to set the objective function, which we then minimise

  if(ret == 0) {
    set_add_rowmode(lp, FALSE);
    j = 0;
    for(j=0;j<4*nspot;j++){
      row[j]=j%nspot+1;
      row[j]*=td;
    }
    /* set the objective in lpsolve */
    if(!set_obj_fnex(lp, Ncol, row, colno))
      ret = 4;
  }

  if(ret == 0){
    /* set the object direction to minimize */
    set_minim(lp);    
    write_LP(lp,fp);
    set_verbose(lp, IMPORTANT);//only important messages
    /* Now let lpsolve calculate a solution */
    ret = solve(lp);
    if(ret == OPTIMAL)
      ret = 0;
    else
      ret = 5;
  }

  if(ret == 0) {
    //printing the solved lp values
    printf("E[M]: %f\n", get_objective(lp));
    //fprintf(output,"E[M]: %f\n", get_objective(lp));
    temp1=get_objective(lp);
    /* variable values */
    get_variables(lp, row);
    for(j = 0; j < Ncol; j++){
      //printf("%s: %f\n", get_col_name(lp, j + 1), row[j]);
      //fprintf(fp,"%s: %f\n", get_col_name(lp, j + 1), row[j]);
    }

    /* we are done now */
  }
  return row;
}



int main(){
  FILE* output1;
  FILE* output2;
  output2=fopen("lpout3.txt","a");
  output1=fopen("lpout1.txt","a");
  
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
  row=amazing(P);
  int netsum[num]={0};
  tester=test1(P);
  int price;
  int* output;
  int sum;
  float avg=0;
  float avgt=0;
  int avgtime[num]={0};
  output=(int*)malloc(100*td*sizeof(int));
  for(i=0;i<num;i++){
    sum=2;
    output[0]=1;
    output[1]=1;
    
    for(j=2;j<100*td;j++){
      price=tester[i][j];
      //printf("%d ",price);
      //printf("price:%d\n ",price);
      if(output[j-1]==1&&output[j-2]==1){
	output[j]=rand_choose(row[price-1],row[price+nspot-1]);
	//printf("state=i,i\n");
      }
      else if(output[j-1]==1&&output[j-2]==0){
	output[j]=rand_choose(row[2*nspot+price-1],row[3*nspot+price-1]);
	//printf("state=i,o\n");
      }
      else if(output[j-1]==0&&output[j-2]==1){
	output[j]=rand_choose(row[4*nspot+price-1],row[5*nspot+price-1]);
	//printf("state=o,i\n");
      }
      else if(output[j-1]==0&&output[j-2]==0){
	output[j]=rand_choose(row[6*nspot+price-1],row[7*nspot+price-1]);
	//printf("state=o,o\n");
      }
      if(output[j]==1){
	sum++;
	netsum[i]+=price;
      }
      //printf("price:%d Bidding option taken:%d\n",price,output[j]);
      if(sum==W){
	avgtime[i]=j;
	break;
      }
    }
    //printf("\n\n");
    //printf("\n");
    //int sum=0;
    netsum[i]+=(tester[i][0]+tester[i][1]);
    //printf("netsum:\n%d",netsum[i]);
    //printf("\n\n\n");
    avg+=(netsum[i]);
    avgt+=avgtime[i];
  }
  avg/=(float)num;
  avgt/=(float)num;
  printf("E[M]: %f Actual: %f",temp1,avg);
  fprintf(output1,"Actual: %f\n",avg);
  float e=1-(avg/temp1);
  fprintf(output1,"Error Rate: %f\n\n",e);
  printf("\nmod expected:%f avg time:%f\n",temp1*avgt/td,avgt);
  fprintf(output2,"W:%d\ntd: %d\nE[M]: %f\nactual cost: %f\nactual time: %f",W,td,temp1,avg,avgt); 
  // printf("Actual: %d",rand_num());
  return 0;
}
