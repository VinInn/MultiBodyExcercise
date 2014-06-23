#ifndef UltimateSOA_H
#define UltimateSOA_H

#include<vector>
#include <tuple>
#include <utility>
#include <type_traits>
#include "align_allocator.h"
#include "SoaIterator.h"

namespace details_UltimateSoa {
  template<typename T> using AVector = std::vector<T,align_allocator<T,32>>;
}

template<typename T>
struct UltimateSoaTraits {
  using VAL =  typename T::VAL;
  using CREF = typename T::CREF;
  using REF =  typename T::REF;
  using SOATUPLE = typename T::SOATUPLE;
};

template<typename T, bool=!std::is_arithmetic<T>::value>
class UltimateSoa {};


template<typename T>
class UltimateSoa<T, true> {
public:
  using VAL =  typename UltimateSoaTraits<T>::VAL;
  using CREF = typename UltimateSoaTraits<T>::CREF;
  using REF =  typename UltimateSoaTraits<T>::REF;
  using Data = typename UltimateSoaTraits<T>::SOATUPLE;

  using iterator = SoaIterator<REF, UltimateSoa<T, true>>;
  using const_iterator = SoaIterator<CREF, UltimateSoa<T, true>>;

   template<std::size_t... I>
   void resize_impl(unsigned int n, std::index_sequence<I...>) { 
     using swallow = int[];
     (void)swallow{0, ((void)(std::get<I>(m_data).resize(n)),0)...};
   }
   
  void resize(unsigned int n) {
    m_n = n;
    resize_impl(n,std::make_integer_sequence<std::size_t,std::tuple_size<Data>::value>{});
  }
  
  UltimateSoa(){}
   explicit UltimateSoa(unsigned int n) {
     resize(n);
   }

  auto size() const { return m_n;}

  iterator begin() { return iterator(*this);}  
  iterator   end() { return iterator(*this,size());}  

  const_iterator begin() const { return const_iterator(*this);}  
  const_iterator   end() const { return const_iterator(*this,size());}  


  template<typename V, std::size_t... I>
  V t2r_impl(unsigned int j, std::index_sequence<I...>) {
    return V(std::get<I>(m_data)[j] ...); 
  }

  REF operator[](unsigned int j) {
    return t2r_impl<REF>(j,std::make_integer_sequence<std::size_t,std::tuple_size<Data>::value>{});
  }

  CREF operator[](unsigned int j) const {
   return t2r_impl<CREF>(j,std::make_integer_sequence<std::size_t,std::tuple_size<Data>::value>{});
  }


  static void swap(REF a, REF b) {
    VAL tmp = a;
    a = b; b=tmp;
  } 

  Data m_data;
  unsigned int m_n=0;
};


template<typename T>
class UltimateSoa<T, false> : public details_UltimateSoa::AVector<T> {
public:
  using Storage = details_UltimateSoa::AVector<T>;
  
  using value = typename std::remove_reference<T>::type;
  using ref = typename std::add_lvalue_reference<T>::type;
  using cref = typename std::add_lvalue_reference<typename std::add_const< T>::type>::type;
  using VAL = value;
  using CREF = cref;
  using REF = ref;
  
  template<typename ... Args>
  UltimateSoa(Args... args) : Storage(args...){}

  static void swap(REF a, REF b) {
    VAL tmp = a;
    a = b; b=tmp;
  } 

};

/* there is no way this can ever work...
template<template<typename, bool> class UltimateSoa, typename T>
void swap(typename UltimateSoa<T,true>::REF a, typename UltimateSoa<T,true>::REF b) {
  UltimateSoa<T,true>::swap(a,b);
}
*/

template<typename T, template<typename> class V>
void swap(V<T&> a, V<T&> b) {
  V<T> tmp = a;
  a = b; b=tmp;
};



#endif


