#include<iostream>
#include<algorithm>
#include<vector>
#include<fstream>

using namespace std;

int main(){
  int i;
  ofstream myfile;
  myfile.open("out.txt");
  float j;
  vector<float> vec;
  for(i=0;i<951;i++){
    cin>>j;
    vec.push_back(j);
  }
  sort(vec.begin(),vec.end());
  myfile<<vec[0]<<'\n';
  for(i=1;i<951;i++){
    if(vec[i]!=vec[i-1]){
      myfile<<vec[i];
      myfile<<'\n';
    }
  }
  myfile.close();
  return 0;
}
