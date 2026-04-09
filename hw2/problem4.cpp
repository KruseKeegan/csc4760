#include <Kokkos_Core.hpp>
#include <iostream>

int main(int argc, char** argv){
  Kokkos::initialize(argc, argv);
  {
    int n = 8;
    Kokkos::View<double****> A("A",5,7,12,n);
    std::cout << A.label() << std::endl;
  }
  Kokkos::finalize();
  return 0;
}