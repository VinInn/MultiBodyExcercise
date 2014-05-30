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

  using Part = Particle<Float>;
  using V3D = Part::V3D;

  // std::vector<Part> particles;
  Particles particles;

  if (argc < 2) {
    std::cout << "please provide number of bodies, time step, coupling-costant\n" << std::endl;
    exit(-1);
  }
  

  unsigned int nBody = ::atoi(argv[1]);

  Float deltaT = 1.f;
  Float fact = 1.e-8;

  if (argc > 2) deltaT = ::atof(argv[2]);
  if (argc > 3) fact = 1.e-9*::atof(argv[3]);

  std::cout << "start " << nBody << ' ' << deltaT << ' ' << fact<< std::endl;

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
  V3D maxDA = zeroV;
  Float maxDV=zero;
  Float aveDV=zero;
  Float aveV=zero;
  auto iprobe = 15U;
  particles[iprobe].position() = zeroV;
  std::cout << "time/position " << t << ' ' << particles[iprobe].position()
	    << ' ' << particles[iprobe].velocity()<< std::endl;

  auto force = [=](auto a, auto b) -> V3D {
    constexpr Float eps = 0.001*0.001;
    
    auto && delta = b.position()-a.position();
    // very long range force
    // auto d = dist(b.position(),a.position())+eps;
      // return delta*(fact/(d*std::sqrt(d)));
      // long range force
      // auto d2 = dist2(b.position(),a.position())+eps;
      // return delta*(Float(10)*fact/d2);
      // coulomb
    auto d2 = dist2(b.position(),a.position())+eps;
    return delta*(fact/(std::sqrt(d2)*d2));
    
  };
  

  auto nLoop = (unsigned int)(1000/deltaT);

  // heat up
  
  for(auto k=0U; k<nLoop/10; ++k) {
    forceTot=zeroV;
    for (auto i=0U; i< nBody; ++i) 
      forceTot += force(particles[i],particles[iprobe]);
    particles[iprobe].acceleration()=forceTot;
    particles[iprobe].update(deltaT);

  }
  

  for(auto k=0U; k<nLoop; ++k) {
    t -= rdtscp();
    V3D forceT=zeroV;
    //#pragma omp parallel for simd
    //#pragma omp simd
#pragma GCC ivdep
    for (auto i=0U; i< nBody; ++i) {
      auto && f = force(particles[iprobe],particles[i]);
      forceT -= f;
    }
    forceTot=forceT;
    auto dd = abs((forceTot-particles[iprobe].acceleration())/mag(particles[iprobe].velocity()));
    maxDA = max(maxDA,dd);
    maxDV = std::max(maxDV,mag(dd));
    aveDV +=mag(dd);
    aveV +=mag(particles[iprobe].velocity());

    particles[iprobe].acceleration()=forceTot;
    particles[iprobe].update(deltaT);
    t +=rdtscp();

   if (1==k%100){
     std::cout << "time/force " << t << ' ' << forceTot << std::endl;
     std::cout << "time/position " << t << ' ' << particles[iprobe].position() 
	       << ' ' << particles[iprobe].velocity()
	       << ' ' << mag(particles[iprobe].velocity())
	       << std::endl;
   }

  }
  
  std::cout  << std::endl << "time/force/maxDA " << t << ' ' << forceTot
	     << ' '<< deltaT*maxDA << ' '<< deltaT*maxDV 
	     << ' '<< deltaT*aveDV/nLoop
	     << ' '<< aveV/nLoop  << std::endl;
  std::cout << "time/position " << double(t)/nLoop << ' ' << particles[iprobe].position() 
	    << ' ' << particles[iprobe].velocity() << ' ' << mag(particles[iprobe].velocity())<< std::endl;
 

  return 0;
};
