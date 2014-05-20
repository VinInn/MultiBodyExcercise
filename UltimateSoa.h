#ifndef UltimateSOA_H
#define UltimateSOA_H

#include<vector>
#include <tuple>
#include <utility>
#include <type_traits>
#include "align_allocator.h"


namespace details_UltimateSoa {
  template<typename T> using AVector = std::vector<T,align_allocator<T,32>>;
}

template<typename T>
struct UltimateSoaTraits {
  using CREF = typename T::CREF;
  using REF =  typename T::REF;
  using SOATUPLE = typename T::SOATUPLE;
};

template<typename T, bool>
class UltimateSoa {};


template<typename T>
class UltimateSoa<T, true> {
public:
  using CREF = typename UltimateSoaTraits<T>::CREF;
  using REF =  typename UltimateSoaTraits<T>::REF;
  using Data = typename UltimateSoaTraits<T>::SOATUPLE;

   template<std::size_t... I>
   void resize_impl(unsigned int n, std::index_sequence<I...>) { 
     using swallow = int[];
     (void)swallow{0, ((void)(std::get<I>(data).resize(n)),0)...};
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

  template<typename V, std::size_t... I>
   V t2r_impl(unsigned int j, std::index_sequence<I...>) {
   return V(std::get<I>(data)[j] ...); 
 }

  REF operator[](unsigned int j) {
   return t2r_impl<REF>(j,std::make_integer_sequence<std::size_t,std::tuple_size<Data>::value>{});
 }

  CREF operator[](unsigned int j) const {
   return t2r_impl<CREF>(j,std::make_integer_sequence<std::size_t,std::tuple_size<Data>::value>{});
 }

  Data data;
  unsigned int m_n=0;
};


template<typename T>
class UltimateSoa<T, false> : public details_UltimateSoa::AVector<T> {
public:
  using Storage = details_UltimateSoa::AVector<T>;
  /*
  using value = typename std::remove_reference<T>::type;
  using ref = typename std::add_lvalue_reference<T>::type;
  using VAL = UltimateSoa <value,false>;
  using CREF = UltimateSoa <value const &,false>;
  using REF = UltimateSoa<ref,false>;
  */  

  template<typename ... Args>
  UltimateSoa(Args... args) : Storage(args...){}

};


#endif


