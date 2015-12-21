#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>

using namespace std;

int main(){
  int i;
  string line;
  int c;
  vector<int> v;
  ifstream cdfile("cdfreal.txt");
  ofstream cdffinal("cdf1.txt");
  if(cdfile.is_open()){
    while(getline(cdfile,line)){
      istringstream iss(line);
      while(iss>>c){
	v.push_back(c);
      }
    }
  }
  sort(v.begin(),v.end());
  c=v.size();
  for(i=0;i<c;i++)cout<<v[i]<<'\n';
  int cdf[600]={0};
  for(i=0;i<c;i++)cdf[v[i]]++;
  for(i=1;i<600;i++)cdf[i]+=cdf[i-1];
  for(i=0;i<600;i++)cdffinal<<cdf[i]<<'\n';
  
  return 0;
}
      
