#include <mpi.h>
#include <iostream>

using namespace std;

//1. 

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int local = rank;
    int global;

    MPI_Reduce(&local, &global, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Bcast(&global, 1, MPI_INT, 0, MPI_COMM_WORLD);

    cout << "The sum of all ranks using MPI_Reduce and MPI_Bcast is " << global << endl;

    //the code above adds all together and sends to each processor
    //now using all reduce (going to subtract all this time)
    int send = -rank;
    int recv;

    MPI_Allreduce(&send, &recv, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    cout << "The subtraction of all the ranks using MPI_AllReduce is " << recv << endl;


    MPI_Finalize();
    return 0;
}