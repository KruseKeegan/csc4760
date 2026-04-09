#include <Kokkos_Core.hpp>
#include <iostream>

int main(int argc, char** argv){
  Kokkos::initialize(argc, argv);
  {
    int n = 8;
    double result;
    Kokkos::View<double*> A("A", n);

    for (int i=0; i < n; i++){
      A(i) = i * 10;
    }

    Kokkos::parallel_reduce("Max Element", n, KOKKOS_LAMBDA(const int& i, double& max){
      if (A(i) > max){
        max = A(i);
      }
    }, Kokkos::Max<double>(result));
    std::cout << result << std::endl;
  }
  Kokkos::finalize();
  return 0;
}