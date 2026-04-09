#include <Kokkos_Core.hpp>
#include <iostream>

int main(int argc, char** argv){
  Kokkos::initialize(argc, argv);
  {
    int n = 100, m = 100;
    double result;
    Kokkos::View<double**> A("A", n, m);

    for (int i=0; i < n; i++){
      for (int j =0; j < m; j++){
        A(i, j) = (i+1) * 10;
      }
    }

    Kokkos::Timer timer;
    for (int i=0; i < n; i++){
      int sum = 0;
      for (int j=0; j < m; j++){
        sum += A(i,j);
      }
      //std::cout << "Row " << i << ": " << sum << std::endl;
    }
    std::cout << "Seconds: " << timer.seconds() << std::endl;

    timer.reset();

    Kokkos::parallel_for ("Row Sum", n, KOKKOS_LAMBDA(const int& i){
      int sum = 0;
      for (int j = 0; j < m; j++){
        sum += A(i, j);
      }
      //printf("Row %d: %d\n", i, sum);
    });
    Kokkos::fence();
    std::cout << "Parallel time: " << timer.seconds() << " seconds" << std::endl;

  }
  Kokkos::finalize();
  return 0;
}