#include "VectorN.h"


#include<iostream>

int main() {


  VectorN<float,1> a1;
  VectorN<float,3> a3;



  std::get<0>(a1) = 1.f;

  std::get<2>(a3) = std::get<0>(a1);

  std::cout << std::get<2>(a3) << std::endl;

  return 0;
};
