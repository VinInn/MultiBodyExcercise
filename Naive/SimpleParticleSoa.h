#ifndef Particle_H
#define Particle_H


#include "Vector3D.h"

class Particles {
public:
  using Float = vect3d::Float;
  using V3D = Vector3D<Float>;
  using C3D = Vector3D<Float const &>;
  using R3D = Vector3D<Float &&>;
  using L3D = Vector3D<Float&>;
  using Soa = SOA3D<Float>;
  using uint = unsigned int;

  explicit Particles(uint n) : m_pos(n), m_vel(n),m_acc(n),m_mass(n), m_n(n){} 

  uint size() const { return m_n;}

  void fill(uint i, Float m, V3D pos, V3D vel) {
    m_pos[i]= pos; m_vel[i] = vel;
    m_acc[i] = vect3d::ZERO::toV();
    m_mass[i] = m;
  }

  Float mass(uint i) const { return m_mass[i];}
  C3D position(uint i) const { return m_pos[i];}
  C3D velocity(uint i) const { return m_vel[i];}


  C3D acceleration(uint i) const { return m_acc[i];}
  L3D acceleration(uint i) { return m_acc[i];}

  void update(uint i) {
    C3D a =  m_acc[i]*Float(0.5);
    m_pos[i] += m_vel[i] + a;
    m_vel[i]+= m_acc[i];

  }

  /*
  void update(uint i, V3D acc) {
    m_pos[i] += m_vel[i] + (acc[i]*Float(0.5));
    m_vel[i]+= acc[i];
  }
  */

  void scatter(uint i, unsigned int k, Float wallPos) {
    m_pos[i][k] = wallPos - (m_pos[i][k]-wallPos);
    m_vel[i][k] = -m_vel[i][k];

  }

private:
  Soa m_pos;
  Soa m_vel;
  Soa m_acc;
  std::vector<Float> m_mass;
  uint m_n;

};


#endif
