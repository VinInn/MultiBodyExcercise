#include<vector>
#include<random>
#include "Particle.h"


#include<iostream>
#include<cstring>

#include <omp.h>
#include "rdtscp.h"


#include<fstream>

int 
main(int argc, char* argv[]){
  
  using Float = vect3d::Float;

  using Part = Particle<Float>;
  using PartV = Particle<Float, extvec::Vec4D>;

  using V3D = Part::V3D;

  std::cout << "Size of particle " << sizeof(Part) << std::endl;


  // std::vector<Part> particles;
  Particles particles;

  if (argc < 2) {
    std::cout << "please provide number of bodies, time step, coupling-costant, file name\n" << std::endl;
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

  constexpr Float speed = 0.1;

  for (auto i=0U; i< nBody; ++i)
    particles[i].fill(mass,V3D{rgen(eng),rgen(eng),rgen(eng)},
		      speed*V3D{rgen(eng),rgen(eng),rgen(eng)},zeroV);


  long long t = 0;

  auto iprobe = 15U;
  particles[iprobe].position() = zeroV;
  std::cout << "time/position " << t << ' ' << particles[iprobe].position()
	    << ' ' << particles[iprobe].velocity()<< std::endl;



  auto nLoop = (unsigned int)(10000/deltaT);


  Float aveV=zero;

  std::ofstream probeTraj(argc > 4 ? argv[4] :  "probeTraj.txt");

  for(auto k=0U; k<nLoop; ++k) {
    t -= rdtscp();
#pragma GCC ivdep
    for (auto i=0U; i< nBody; ++i) {
      // PartV part; part.load(particles[i].mass(), particles[i].position(), particles[i].velocity(), particles[i].acceleration());
      auto && part = particles[i];
      part.update(deltaT);
      // part.position()+=deltaT*part.velocity();
      
      // #ifdef  USESOA
#ifndef  USEVECEXT
	/*
      for (unsigned int k=0; k<3; ++k) {
	if (part.position()[k] > wallPos) part.scatter(k,wallPos);
	if (part.position()[k] < -wallPos) part.scatter(k,-wallPos);
	}*/

	if (part.position().x() > wallPos) part.scatter(0,wallPos);
	if (part.position().x() < -wallPos) part.scatter(0,-wallPos);
	if (part.position().y() > wallPos) part.scatter(1,wallPos);
	if (part.position().y() < -wallPos) part.scatter(1,-wallPos);
	if (part.position().z() > wallPos) part.scatter(2,wallPos);
	if (part.position().z() < -wallPos) part.scatter(2,-wallPos);

           
#else
      auto p = extvec::abs(part.position());
      PartV::V4V outside = p>wallPos;
      auto msk = extvec::mask(outside);
      if (msk) {
	part.velocity() = (p<wallPos) ? part.velocity() : -part.velocity();
	p = (p<wallPos) ? p : (wallPos - (p-wallPos));
	part.position() = (part.position()>0) ? p : -p;
      }
#endif
      
      // particles[i].load(part.mass(), part.position(), part.velocity(), part.acceleration());
    }
    t +=rdtscp();

    aveV +=mag(particles[iprobe].velocity());

    if (1==k%100){
      std::cout << "time/position " << t << ' ' << particles[iprobe].position() 
		<< ' ' << particles[iprobe].velocity()
		<< ' ' << mag(particles[iprobe].velocity())
		<< std::endl;
    }
    
#ifdef USEVECEXT
   probeTraj << particles[iprobe].position()[0]  << ' '<< particles[iprobe].position()[1]  << ' '<< particles[iprobe].position()[2] << '\n';
#else
   probeTraj << particles[iprobe].position().x()  << ' '<< particles[iprobe].position().y()  << ' '<< particles[iprobe].position().z() << '\n';
#endif
   
   }


  std::cout << "time/position/velocity/ave-vel " << 1.e-6*double(t)/nLoop << ' ' << particles[iprobe].position() 
	    << ' ' << particles[iprobe].velocity() << ' ' << mag(particles[iprobe].velocity())
	    << ' '<< aveV/nLoop  << std::endl;

  return 0;

}


