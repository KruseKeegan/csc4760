#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv){
  int size, rank;
  int P, Q;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int share_vector[2];
  if (rank == 0){
    do {
      cout << "Enter grid shape (multiplies out to the size = " << size << "): " << endl;
      cout << "First: ";
      cin >> P;
      cout << "Second: ";
      cin >> Q;
    } while (P*Q != size);
    share_vector[0] = P;
    share_vector[1] = Q;
  }

  //give p and q to all processes
  MPI_Bcast(share_vector, 2, MPI_INT, 0, MPI_COMM_WORLD);
  P = share_vector[0];
  Q = share_vector[1];

  //split1 by row
  //split2 by column
  MPI_Comm split1, split2;

  int rankSplit1, rankSplit2;


  int color1 = rank / Q;
  int key1 = rank % Q;
  int color2 = rank % Q;
  int key2 = rank / Q;

  MPI_Comm_split(MPI_COMM_WORLD, color1, key1, &split1);
  MPI_Comm_split(MPI_COMM_WORLD, color2, key2, &split2);

  MPI_Comm_rank(split1, &rankSplit1);
  MPI_Comm_rank(split2, &rankSplit2);

  //sum ranks within row_communicator
  int sum;
  MPI_Reduce(&rank, &sum, 1, MPI_INT, MPI_SUM, 0, split1);
  if (rankSplit1 == 0){
    cout << "The sum of the world ranks withing the first split is " << sum << endl;
  }


  //all processes in column communicator recieves rankSplit2 world rank
  int value = 0;
  if (rankSplit2 == 0){
    value = rank;
  }
  MPI_Bcast(&value, 1, MPI_INT, 0, split2);

  cout << "SPLIT2 World rank: " << rank << ", split2 rank: " << rankSplit2 << ", recieved " << value << endl;


  MPI_Comm_free(&split1);
  MPI_Comm_free(&split2);

  MPI_Finalize();
  return 0;
}