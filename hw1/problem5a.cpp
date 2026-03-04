#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
      //first split
      MPI_Comm subset;
      int P, Q = 2;
      int color = rank / 2;
      MPI_Comm_split(MPI_COMM_WORLD, color, rank, &subset);
      int newsize, newrank;
      MPI_Comm_size(subset, &newsize);
      MPI_Comm_rank(subset, &newrank);
      cout << rank << ": I am now in a second communicator (subset): " << newsize << "," << newrank << endl;
      MPI_Barrier(subset);
      MPI_Comm_free(&subset);
    

    MPI_Finalize();
    return 0;
}