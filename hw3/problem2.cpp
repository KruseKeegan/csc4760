#include <mpi.h>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char **argv){
  MPI_Init(&argc, &argv);

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
  int P, Q, M;
  int share[3];
  if (rank == 0){
    do {
      cout << "Enter P and Q (P * Q multiplies out to the size = " << size << "): ";
      cin >> P >> Q;
      cout << "Enter size of vector M: ";
      cout << "Size: ";
      cin >> M;
    } while (P*Q != size || M < 1);
    share[0] = P;
    share[1] = Q;
    share[2] = M;
  }

  //share P Q M
  MPI_Bcast(share, 3, MPI_INT, 0, MPI_COMM_WORLD);
  P = share[0];
  Q = share[1];
  M = share[2];

  //position in P X Q grid
  int row_split = rank / Q;
  int col_split = rank % Q;

  //row and column communicator
  MPI_Comm row_communicate, col_communicate;

  MPI_Comm_split(MPI_COMM_WORLD, row_split, col_split, &row_communicate);
  MPI_Comm_split(MPI_COMM_WORLD, col_split, row_split, &col_communicate);

  //linear load balance block
  int col_q = M / P;
  int col_r = M % P;

  int start_location;
  if (row_split < col_r){
    start_location =row_split  * col_q + row_split;
  }else{
    start_location = row_split * col_q + col_r;
  }

  //num of elemetns owned
  int block_size;
  if (row_split < col_r) block_size = col_q+1;
  else block_size = col_q;

  vector<int> col_vec(M, 0);


  if (rank ==0) {
    for (int i=0; i < M; i++){
      col_vec[i] = i;
    }
  }

  MPI_Bcast(col_vec.data(), M, MPI_INT, 0, col_communicate);

  //each chunk takes it data from vector
  vector<int> local(block_size, 0);
  for (int i=0; i < block_size; i++){
    local[i] = col_vec[start_location +i];
  }

  //parallel copy
  vector<int> row_vec(block_size, 0);
  MPI_Allreduce(local.data(), row_vec.data(), block_size, MPI_INT, MPI_SUM, row_communicate);

  MPI_Bcast(local.data(), block_size, MPI_INT, 0, row_communicate);

  for (int p = 0; p < size; p++) {
    if (rank == p) {
      cout << "Rank " << rank << " (" << row_split << "," << col_split << ")"
           << "  x=[";
      for (int i = 0; i < block_size; i++)
        cout << local[i] << (i+1 < block_size ? "," : "");
      cout << "]  y=[";
      for (int i = 0; i < block_size; i++)
        cout << row_vec[i] << (i+1 < block_size ? "," : "");
      cout << "]" << endl;
      cout.flush();
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Comm_free(&row_communicate);
  MPI_Comm_free(&col_communicate);

  MPI_Finalize();
  return 0;
}