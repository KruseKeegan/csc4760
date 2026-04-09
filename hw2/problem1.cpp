#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>

std::vector<int> merge(const std::vector<int>& left, const std::vector<int>& right){
  std::vector<int> newVector;
  for (int i = 0; i < left.size(); i++){
    newVector.push_back(left[i]);
  }

  for (int i=0; i < right.size(); i++){
    for (int j=0; j < newVector.size(); j++){
      if (right[i] <= newVector[j]){
        newVector.insert(newVector.begin()+j, right[i]);
        break;
      }
      else if(j == newVector.size()-1){
        newVector.push_back(right[i]);
        break;
      }
    }
  }

  return newVector;
}

int main (int argc, char** argv){
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  const int N = 16;
  int piece = N/size;

  std::vector<int> unsort_array(N);
  std::vector<int> local_piece(piece);

  //create array
  if (rank ==0){
    srand(time(NULL));
    std::cout << "Unsorted array: ";
    for (int i=0; i < N; i++){
      unsort_array[i] = rand() %50;
      std::cout << unsort_array[i] << " ";
    }
    std::cout << std::endl;
  }

  MPI_Scatter(unsort_array.data(), piece, MPI_INT, local_piece.data(), piece, MPI_INT, 0, MPI_COMM_WORLD);
  std::sort(local_piece.begin(), local_piece.end());

  int step = 1;
  while (step < size){
    if (rank % (2 * step) == 0){
      int partner = rank + step;
      if (partner < size){
        int recv_size;
        MPI_Recv(&recv_size, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::vector<int> recv_data(recv_size);
        MPI_Recv(recv_data.data(), recv_size, MPI_INT, partner, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        local_piece = merge(local_piece, recv_data);
      }
    }
    else{
      int partner = rank - step;
      int send_size = local_piece.size();
      MPI_Send(&send_size, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
      MPI_Send(local_piece.data(), send_size, MPI_INT, partner, 1, MPI_COMM_WORLD);
      break;
    }
    step *=2;
  }

  if (rank == 0){
    std::cout << "Sorted array: ";
    for (int i=0; i < local_piece.size(); i++){
      std::cout << local_piece[i] << " ";
    }
    std::cout << std::endl;
  }


  MPI_Finalize();
  return 0;
}