#include <Kokkos_Core.hpp>
#include <iostream>

int main(int argc, char** argv){
  Kokkos::initialize(argc, argv);
  {
    int n = 10, m = 10;
    Kokkos::View<double**> A("A", n, m);
    for (int i = 0; i < n; i++){
      for (int j = 0; j < m; j++){
      A(i, j) = 1000 * i * j;
    }
  }

    for (int i = 0; i < n; i++){
      std::cout << std::endl;
      for (int j = 0; j < m; j++){
      std::cout << A(i, j) << " ";
    }
    std::cout << std::endl;
  }

}
  Kokkos::finalize();
  return 0;
}