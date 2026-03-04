#include <mpi.h>
#include <iostream>

#ifndef NUM_ROUNDS
#define NUM_ROUNDS 1
#endif

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    
    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int integer = 0;
    for (int i=0; i < NUM_ROUNDS; i++){
      if(rank != 0){
      MPI_Recv(&integer, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      std::cout << "Process " << rank << " recieved integer " << integer << " from process " << rank - 1 << std::endl;
      }

      MPI_Send(&integer, 1, MPI_INT, (rank +1)% size, 0, MPI_COMM_WORLD);

      if (rank == 0){
        MPI_Recv(&integer, 1, MPI_INT, size-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Process " << rank << " recieved integer " << integer << " from process " << size - 1 << std::endl;
        integer++;
      }

    }
    MPI_Finalize();
    return 0;
}