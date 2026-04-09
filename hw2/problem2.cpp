#include <Kokkos_Core.hpp>
#include <iostream>

int main(int argc, char** argv){
  Kokkos::initialize(argc, argv);
  {
    int n = 8;
    Kokkos::View<double*> check("check", n);
    std::cout << check.label() << std::endl;
  }
  Kokkos::finalize();
  return 0;
}