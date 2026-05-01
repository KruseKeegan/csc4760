#include <iostream>
#include <cstdlib>

using namespace std;

//linear load balance distribution 
int linearInverseDistribution(int M, int P, int p, int i){
  int q = M / P; //base block size
  int r = M % P; //number of processes that get one extra element

  //check for processes that get 1 extra
  int startLocation;
  if (p < r){
    startLocation = p * q + p;
  }else{
    startLocation = p * q + r;
  }

  //global index
  int I = startLocation + i;

  cout << "Process p = " << p << ", local index i = " << i << ", global index I = " << I << "." << endl;

  return I;
}

//given global index, compute process and local index
void scatterMappedIndex(int P, int I){
  int p, i;
  p = I % P;
  i = I / P;

  cout << "Global I = " << I << ", process p' = " << p << ", local index i' = " << i << "." << endl;
}

int main(int argc, char *argv[]){
  if (argc != 5){
    return 1;
  }

  int M = atoi(argv[1]);  //vec length
  int P = atoi(argv[2]);  //num processes
  int p = atoi(argv[3]);  //process rank
  int i = atoi(argv[4]);  //local index of p

  int I = linearInverseDistribution(M, P, p, i);
  scatterMappedIndex(P, I);

  return 0;
}