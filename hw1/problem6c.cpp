#include <mpi.h>
#include <iostream>

using namespace std;

//2. 

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int send[size], recv[size];
    for (int i=0; i < size; i++){
      send[i] = i * rank + 2*rank;
    }
    MPI_Alltoall(&send, 1, MPI_INT, &recv, 1, MPI_INT, MPI_COMM_WORLD);

    for (int i = 0; i < size; i++){
      cout << "Processor " << rank << " recieved " << recv[i] << endl;
    }

    MPI_Finalize();
    return 0;
}