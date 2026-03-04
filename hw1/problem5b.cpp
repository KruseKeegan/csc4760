#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
      
    //second split
        MPI_Comm subset2;
        int P, Q = 2;
        int color = rank % Q;
        MPI_Comm_split(MPI_COMM_WORLD, color, rank, &subset2);
        int newsize, newrank;
        MPI_Comm_size(subset2, &newsize);
        MPI_Comm_rank(subset2, &newrank);
        cout << rank << ": I am now in a second communicator (subset2): " << newsize << "," << newrank << endl;
        MPI_Barrier(subset2);
        MPI_Comm_free(&subset2);
    
    
    

    MPI_Finalize();
    return 0;
}