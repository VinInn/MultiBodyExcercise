// #include "Box.h"
#include "BoxVect.h"



#include<iostream>
#include<cstring>


#include <omp.h>


int 
main(int argc, char* argv[]){
  
  if (argc != 2) {
    std::cout << "please provide number of bodies\n" << std::endl;
    exit(-1);
  }
  
  int nbody = ::atoi(argv[1]);
  
  
  Box box(nbody);
  
  box.check();
  
  std::cout << "start " << nbody << std::endl;
  
  for (int k=0; k<10; ++k) {
    auto t = -omp_get_wtime();
    for (int i=0; i<10; ++i) box.oneStep();
    t+=omp_get_wtime();
    std::cout << "time " << t << ' ' << box.temperature() << std::endl;
    box.check();
  }
  
  return 0;
  
}
