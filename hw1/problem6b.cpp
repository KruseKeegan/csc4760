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
    
    int local = rank * 2;
    int sum =0;

    int sub_local[size];
    MPI_Gather(&local, 1, MPI_INT, sub_local, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
      for (int i=0; i < size; i++){
         sum += sub_local[i];
      }
    }

    MPI_Bcast(&sum, 1, MPI_INT, 0, MPI_COMM_WORLD);

    cout << "The sum of all ranks using MPI_Gather and MPI_Bcast is " << sum << endl;

    int send = -rank;
    int recv[size];

    MPI_Allgather(&send, 1, MPI_INT, &recv, 1, MPI_INT, MPI_COMM_WORLD);
    sum = 0;
    for (int i=0; i < size; i++){
      sum += recv[i];
    }
    cout << "The subtraction of all the ranks using MPI_Allgather is " << sum << endl;

    MPI_Finalize();
    return 0;
}