#ifndef Particle_H
#define Particle_H


#include "Array3D.h"

class Particle {
public:

  using V3D = Vect3D;
  using Float = vect3d::Float;

  Particle(Float m, V3D pos, V3D vel) : m_pos(pos), m_vel(vel)
#ifndef NOACC
				      , m_acc(vect3d::ZERO())
#endif
				      , m_mass(m) {}

  Float mass() const { return m_mass;}
  V3D position() const { return m_pos;}
  V3D velocity() const { return m_vel;}

#ifndef NOACC
  V3D acceleration() const { return m_acc;}
  V3D & acceleration() { return m_acc;}

  void update() {
    update(m_acc);
  }
#endif  

  void update(V3D acc) {
    m_pos+= m_vel + Float(0.5f)*acc;
    m_vel+= acc;
  }


  void scatter(unsigned int i, Float wallPos) {
    m_pos[i] = wallPos - (m_pos[i]-wallPos);
    m_vel[i] = -m_vel[i];

  }

private:
  V3D m_pos;
  V3D m_vel;
#ifndef NOACC
  V3D m_acc;
#endif
  Float m_mass;

};


#endif
