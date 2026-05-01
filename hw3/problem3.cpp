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

  MPI_Bcast(share, 3, MPI_INT, 0, MPI_COMM_WORLD);
  P = share[0];
  Q = share[1];
  M = share[2];

  int row_split = rank / Q;
  int col_split = rank % Q;

  MPI_Comm row_communicate, col_communicate;

  MPI_Comm_split(MPI_COMM_WORLD, row_split, col_split, &row_communicate);
  MPI_Comm_split(MPI_COMM_WORLD, col_split, row_split, &col_communicate);

  int col_q = M / P;
  int col_r = M % P;

  int start_location;
  if (row_split < col_r){
    start_location =row_split  * col_q + row_split;
  }else{
    start_location = row_split * col_q + col_r;
  }

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

  vector<int> local(block_size, 0);
  for (int i=0; i < block_size; i++){
    local[i] = col_vec[start_location +i];
  }

  MPI_Bcast(local.data(), block_size, MPI_INT, 0, row_communicate);

  //scatter distribition
  int row_q = M/Q;
  int row_r = M%Q;
  int row_size;
  if (col_split < row_r) row_size = row_q+1;
  else row_size = row_q;
  
  //assemble full x and P row-chunks so each process can get its part
  vector<int> recvcounts(P), rdispls(P);
  for (int p = 0; p < P; p++){
    rdispls[p]    = p * col_q + min(p, col_r);
    recvcounts[p] = col_q + (p < col_r ? 1 : 0);
  }
  vector<int> full_x(M, 0);
  MPI_Allgatherv(local.data(), block_size, MPI_INT,
                 full_x.data(), recvcounts.data(), rdispls.data(), MPI_INT,
                 col_communicate);

  //extract slice or section
  vector<int> row_vec(row_size, 0);                    
  for (int j = 0; j < row_size; j++)
    row_vec[j] = full_x[j * Q + col_split];

  for (int p = 0; p < size; p++) {
    if (rank == p) {
      cout << "Rank " << rank << " (" << row_split << "," << col_split << ")"
           << "  x=[";
      for (int i = 0; i < block_size; i++)
        cout << local[i] << (i+1 < block_size ? "," : "");
      cout << "]  y=[";
      for (int i = 0; i < row_size; i++)
        cout << row_vec[i] << (i+1 < row_size ? "," : "");
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