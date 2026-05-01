#include <iostream>
#include <cuda_runtime.h>

using namespace std;

//GPU kernal, adds vector b to every row of matrix A
__global__ void rowAddition(int *A, int *B, int rows, int cols){
  int row = blockIdx.x * blockDim.x + threadIdx.x;
  int col = blockIdx.y * blockDim.y + threadIdx.y;

  if (row < rows && col < cols){
    A[row * cols + col] += B[col];
  }
}

int main(){
  const int rows = 3, cols = 3;
  const int matSize = rows * cols * sizeof(int);
  const int vecSize = cols *sizeof(int);

  int A[rows * cols] = {130, 147, 115,
                        224, 158, 187,
                         54, 158, 120};
  
  int B[cols] = {221, 12, 157};

  //device memory
  int *d_A, *d_B;
  cudaMalloc(&d_A, matSize);
  cudaMalloc(&d_B, vecSize);

  //copy mem from host to device
  cudaMemcpy(d_A, A, matSize, cudaMemcpyHostToDevice);
  cudaMemcpy(d_B, B, vecSize, cudaMemcpyHostToDevice);

  dim3 blockDim(16,16);
  dim3 gridDim((rows +blockDim.x -1) / blockDim.x, (cols + blockDim.y -1) / blockDim.y);

  rowAddition<<<gridDim, blockDim>>>(d_A, d_B, rows, cols);

  //sync
  cudaDeviceSynchronize();
  
  //mem back to host
  cudaMemcpy(A, d_A, matSize, cudaMemcpyDeviceToHost);
  
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            cout << A[i * cols + j] << " ";
        cout << endl;
    }

  return 0;
}