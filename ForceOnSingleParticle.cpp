#include<vector>
#include<random>
#include "Particle.h"


#include<iostream>
#include<cstring>

#include <omp.h>
#include "rdtscp.h"



int 
main(int argc, char* argv[]){
  
    using Float = float;
  // using Float = double;

  using V3D = Vector3D<Float>;
  using Part = Particle<Float>;

  std::vector<Part> particles;

  if (argc != 2) {
    std::cout << "please provide number of bodies\n" << std::endl;
    exit(-1);
  }
  
  unsigned int nBody = ::atoi(argv[1]);

  std::cout << "start " << nBody << std::endl;

  particles.resize(nBody);

  constexpr Float wallPos = 1.f;

  std::mt19937 eng;
  std::uniform_real_distribution<float> rgen(-wallPos,wallPos);

  Float zero = 0;
  const V3D zeroV{zero,zero,zero};
  constexpr Float mass = 1.;

  for (auto i=0U; i< nBody; ++i)
    particles[i].fill(mass,V3D{rgen(eng),rgen(eng),rgen(eng)},zeroV,zeroV);



  long long t = 0;
  V3D forceTot=zeroV;
  auto iprobe = 15U;
  particles[iprobe].position() = zeroV;
  std::cout << "time/position " << t << ' ' << particles[iprobe].position() << std::endl;


  for(auto k=0U; k<1000; ++k) {
    auto force = [=](auto a, auto b) -> V3D {
      constexpr Float eps = 0.0001;
      constexpr Float fact = .1e-7;
      
      auto && delta = b.position()-a.position();
      // very long range force
      // auto d = dist(b.position(),a.position())+eps;
      // return delta*(fact/(d*std::sqrt(d)));
      // long range force
      // auto d2 = dist2(b.position(),a.position())+eps;
      // return delta*(Float(10)*fact/d2);
      // coulomb
      auto d2 = dist2(b.position(),a.position())+eps;
      return delta*(-fact/(std::sqrt(d2)*d2));
      
    };
    t -= rdtscp();
    forceTot=zeroV;
    for (auto i=0U; i< nBody; ++i) 
      forceTot += force(particles[i],particles[iprobe]);
    particles[iprobe].acceleration()=forceTot;
    particles[iprobe].update();
    t +=rdtscp();

   if (1==k%100){
     std::cout << "time/force " << t << ' ' << forceTot << std::endl;
     std::cout << "time/position " << t << ' ' << particles[iprobe].position() << std::endl;
   }

  }
  
  std::cout << "time/force " << t << ' ' << forceTot << std::endl;
  std::cout << "time/position " << t << ' ' << particles[iprobe].position() << std::endl;

  return 0;
};
