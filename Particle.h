#ifndef Particle_H
#define Particle_H


#include "Vect.h"


#ifdef USEVECEXT
template <typename T, template<typename> class Vec=Vec4D >
#else
template <typename T, template<typename> class Vec=Vector3D >
#endif
class Particle {
public:

  using value = typename std::remove_reference<T>::type;
  using ref = typename std::add_lvalue_reference<T>::type;


  using T3D = Vec<T>;

#ifdef USEVECEXT
  using V3D = Vec<value>;
  using C3D = Vec<value>;
#else
  using V3D = Vec<value>;
  using C3D = Vec<value const &>;
  using L3D = Vec<ref>;
#endif

  using V4V = extvec::Vec4D<value>;
  using V3V = Vector3D<value>;

  void load(T m, V4V pos, V4V vel, V4V acc) {
    using VA = typename extvec::ExtVecTraits<T,4>::typeA;
    *(VA *)(&m_pos) = pos;
    *(VA *)(&m_vel) = vel;
    *(VA *)(&m_acc) = acc;
    m_mass=m;  
  }
  void load(T m, V3V pos, V3V vel, V3V acc) {
    using VA = typename extvec::ExtVecTraits<T,4>::typeA;
    m_pos= *(VA *)(&pos); m_pos[3]=0;
    m_vel= *(VA *)(&vel);m_vel[3]=0;
    m_acc= *(VA *)(&acc);m_acc[3]=0;
    m_mass=m;  
  }
  
  

  Particle(){}
  Particle(T m, T3D pos, T3D vel, T3D acc) : m_pos(pos), m_vel(vel)
				  , m_acc(acc)
				  , m_mass(m) {}


  void fill (value m, V3D pos, V3D vel, V3D acc) {
    m_pos=pos; m_vel=vel; m_acc=acc; m_mass=m;
  }


  value mass() const { return m_mass;}
  T3D & position() { return m_pos;}
  T3D const & position() const { return m_pos;}
  T3D const & velocity() const { return m_vel;}
  T3D & velocity() { return m_vel;}

  T3D const & acceleration() const { return m_acc;}
  T3D & acceleration() { return m_acc;}

  void update(value dt=value(1)) {
    auto a =  value(0.5)*dt*dt*m_acc;
    m_pos += dt*m_vel + a;
    m_vel += dt*m_acc;

  }


  void scatter(unsigned int i, value wallPos) {
    m_pos[i] = wallPos - (m_pos[i]-wallPos);
    m_vel[i] = -m_vel[i];

  }

private:
  T3D m_pos;
  T3D m_vel;
  T3D m_acc;
  T m_mass;
  // T m_charge;

};

// #define  USESOA

#ifdef USESOA
template<typename T>
class Particles {
public:

  using Float = T;
  using CP = Particle<Float const &>;
  using RP = Particle<Float &&>;
  using LP = Particle<Float&>;
  using Soa = SOA3D<Float>;
  using uint = unsigned int;

  Particles(){}
  explicit Particles(uint n) : m_pos(n), m_vel(n),m_acc(n),m_mass(n), m_charge(n), m_n(n){} 

  void resize(uint n) { m_pos.resize(n); m_vel.resize(n); m_acc.resize(n); m_mass.resize(n); m_charge.resize(n); m_n=n;} 
  uint size() const { return m_n;}

  CP operator[](uint i) const { return CP(m_mass[i],m_pos[i],m_vel[i],m_acc[i]); }
  LP operator[](uint i)       { return LP(m_mass[i],m_pos[i],m_vel[i],m_acc[i]); }
  


private:
  Soa m_pos;
  Soa m_vel;
  Soa m_acc;
  AVector<Float> m_mass;
  AVector<Float> m_charge;
  uint m_n;

};

#else
template<typename T> using Particles = AVector<Particle<T>>; 
#endif



#endif
