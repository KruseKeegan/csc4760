#include <Kokkos_Core.hpp>
#include <iostream>

int main(int argc, char** argv){
  Kokkos::initialize(argc, argv);
  {
    int a = 3, b = 3, c = 3;
    double result;
    Kokkos::View<double**> A("2D", a, b);
    Kokkos::View<double*> B("1D", c);

    
    A(0,0)=130; A(0,1)=147; A(0,2)=115;
    A(1,0)=224; A(1,1)=158; A(1,2)=187;
    A(2,0)=54;  A(2,1)=158; A(2,2)=120;

    B(0)=221; B(1)=12; B(2)=157;


    Kokkos::parallel_for("Row Sum", a, KOKKOS_LAMBDA(const int& i){
      for (int j =0; j < b; j++){
        A(i, j) = A(i,j) + B(j);
      }
    });
    Kokkos::fence();
    
    for (int i=0; i < a; i++){
      std::cout << std::endl;
      for (int j =0; j < b; j++){
        std::cout << A(i,j) << " ";
      }
      std::cout << std::endl;
    }

  }
  Kokkos::finalize();
  return 0;
}