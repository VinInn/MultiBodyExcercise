#ifndef SoaIterator_H
#define SoaIterator_H

#include<type_traits>

template<typename TREF, typename Container> 
class SoaIterator {
public:
 
  using ContainerP = typename std::add_pointer<Container>::type;
  using ContainerR = typename std::add_lvalue_reference<Container>::type;


  // Lifecycle:
public:
  SoaIterator() {}
  explicit SoaIterator(ContainerR c, unsigned int ind=0) : m_cont(&c),m_index(ind){}
  SoaIterator(const SoaIterator &rhs) = default;
  SoaIterator& operator=(const SoaIterator &rhs)  = default;
   SoaIterator(SoaIterator &&rhs) = default;
  SoaIterator& operator=(SoaIterator &&rhs)  = default;
  template<typename Iter> 
  SoaIterator(const Iter &rhs) : m_cont(&rhs.cont()), m_index(rhs.index()) {}
  template<typename Iter> 
  SoaIterator& operator=(const Iter &rhs) { m_cont=&rhs.cont(); m_index = rhs.index(); return *this;}

  ContainerR cont() const {return *m_cont;}
  unsigned int index() const { return m_index;}
  
  // Operators : misc
public:
  TREF operator*() {return  cont()[m_index];}
  // Type* operator->() {return m_index;}
  TREF operator[](const int& rhs) {return  cont()[m_index+rhs];}
  
  // Operators : arithmetic
public:
  SoaIterator& operator++() {++m_index; return *this;}
  SoaIterator& operator--() {--m_index; return *this;}
  SoaIterator operator++(int) {SoaIterator tmp(*this); ++m_index; return tmp;}
  SoaIterator operator--(int) {SoaIterator tmp(*this); --m_index; return tmp;}
  SoaIterator& operator+=(int rhs) {m_index += rhs; return *this;}
  SoaIterator& operator-=(int rhs) {m_index -= rhs; return *this;}

  SoaIterator operator+(const SoaIterator& rhs) const {return SoaIterator(cont(), m_index+rhs.m_index);}
  SoaIterator operator-(const SoaIterator& rhs) const {return SoaIterator(cont(), m_index-rhs.m_index);}
  SoaIterator operator+(int rhs) const {return SoaIterator(cont(),m_index+rhs);}
  SoaIterator operator-(int rhs) const {return SoaIterator(cont(),m_index-rhs);}
  friend  SoaIterator operator+(int lhs, const SoaIterator& rhs) {return SoaIterator(rhs.cont(),lhs+rhs.m_index);}
  friend  SoaIterator operator-(int lhs, const SoaIterator& rhs) {return SoaIterator(rhs.cont(),lhs-rhs.m_index);}
  
  // Operators : comparison
public:
   bool operator==(const SoaIterator& rhs) const {return m_index == rhs.m_index;}
   bool operator!=(const SoaIterator& rhs) const {return m_index != rhs.m_index;}
   bool operator>(const SoaIterator& rhs) const {return m_index > rhs.m_index;}
   bool operator<(const SoaIterator& rhs) const {return m_index < rhs.m_index;}
   bool operator>=(const SoaIterator& rhs) const {return m_index >= rhs.m_index;}
   bool operator<=(const SoaIterator& rhs) const {return m_index <= rhs.m_index;}
  
  // Data members
private:
  ContainerR cont()  {return *m_cont;}
  ContainerP m_cont=nullptr;
  unsigned int m_index=0;
};


#endif
