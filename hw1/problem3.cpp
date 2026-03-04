#include <mpi.h>
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

int main(int argc, char** argv){
  MPI_Init(&argc, &argv);

  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  double startTime, endTime;
  int vectorSize;
  if (rank == 0){
    cout << "Enter Vector Size: ";
    cin >> vectorSize;
    startTime = MPI_Wtime();
  }
  
  MPI_Bcast(&vectorSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int chunkSize = vectorSize / size;
  int leftOver = vectorSize % size;

  int minPos = chunkSize*rank;
  int maxPos = (chunkSize*(rank+1))-1;

  int extra = -1;
  if (rank < leftOver){
    extra = vectorSize - leftOver + rank;
  }
  //cout << minPos << " " << maxPos  << " " << extra << endl;

  // Vector A = 1
  // Vector B = 2

  vector<int> vectorA(vectorSize);
  vector<int> vectorB(vectorSize);

  int sum = 0;
  if (extra != -1){
    for (int i=minPos; i <= maxPos; i++){
      vectorA[i] = i;
      vectorB[i] = 2;
      sum += vectorA[i] * vectorB[i];
    }

    vectorA[extra] = 1;
    vectorB[extra] = 2;
    sum += vectorA[extra] * vectorB[extra];

  }else{
    for (int i=minPos; i <= maxPos; i++){
      vectorA[i] = 1;
      vectorB[i] = 2;
      sum += vectorA[i] * vectorB[i];
    }
  }


  for (int i=1; i < size; i *= 2){
    int tempVal = sum;
    if (rank % (2*i) == 0){
      if (rank + i < size){
        MPI_Recv(&tempVal, 1, MPI_INT, rank + i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        sum += tempVal;
      }
    }else{
      MPI_Send(&tempVal, 1, MPI_INT, rank - i, 0, MPI_COMM_WORLD);
      //cout << "SEND RANK " << rank << " to " << rank-i << endl; 
      break;
    }
  }



  if (rank ==0){
    endTime = MPI_Wtime();
    cout << endTime - startTime << endl;
    cout << sum << endl;
  }

  MPI_Finalize();
  return 0;
}
