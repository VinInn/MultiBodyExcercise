#ifndef Vector_H
#define Vector_H
#include <tuple>
#include<cmath>
#include<algorithm>
#include<type_traits>
#include <utility>



// Convert array into to tuple
template<typename T, typename Array, std::size_t... I>
T a2t_impl(const Array& a, std::index_sequence<I...>)
{
  return T(a[I]...);
}
 
template<typename T, typename A, std::size_t N, typename Indices = std::make_index_sequence<N>>
auto a2t(const std::array<A, N>& a)
  -> decltype(a2t_impl<T>(a, Indices()))
{
  return a2t_impl<T>(a, Indices());
}


template<typename T, int N>
struct HelperVectorN {
  using value = typename std::decay<T>::type;

  using tuple_type = decltype(std::tuple_cat(HelperVectorN<T,1>::type(), HelperVectorN<T,N-1>::type()));

  static tuple_type & type() {
    static tuple_type loc = a2t<tuple_type>(arr());
    return loc;
  }

  static std::array<value, N> & arr() {
    static std::array<value, N> loc = {};
    return loc;
  }

};

template<typename T>
struct HelperVectorN<T,1> {
  using value = typename std::decay<T>::type;

  using tuple_type = std::tuple<T>;  
  static tuple_type & type() {
    static tuple_type loc = a2t<tuple_type>(arr());
    return loc;
  }

  static std::array<value, 1> & arr() {
    static std::array<value, 1> loc = {};
    return loc;
  }

};




template<typename T, typename TUPLE>
auto cat(T t, TUPLE tuple) { return std::tuple_cat(std::tie(t),tuple);}

template<typename T, int N>
class VectorN : public HelperVectorN<T,N>::tuple_type {
public:
  using tuple_type = typename HelperVectorN<T,N>::tuple_type;
  static constexpr unsigned int DIM=N;
  using value = typename std::remove_const<typename std::remove_reference<T>::type>::type;
  using  ref = typename std::add_lvalue_reference<T>::type;
  using cref = typename std::add_lvalue_reference<typename std::add_const< T>::type>::type;
  using VAL = VectorN<value,DIM>;
  using CREF = VectorN<cref,DIM>;
  using REF = VectorN<ref,DIM>;


  template<typename V, typename W, std::size_t... I>
  static  V t2r_impl(W const &  w, std::index_sequence<I...>) {
    return V(std::get<I>(w) ...); 
  }

  template<typename V, typename W, std::size_t... I>
  static  V t2r_impl(W & w, std::index_sequence<I...>) {
    return V(std::get<I>(w) ...); 
  }


  constexpr VectorN(){}

  template<typename ... Args>
    explicit constexpr  VectorN(T t, Args... args) : tuple_type(t,args...){}
  
  template<typename V>
    constexpr VectorN(V v) : tuple_type(t2r_impl<tuple_type>(v,std::make_integer_sequence<std::size_t,N>{}) ){}
  template<typename V>
    VectorN & operator=(V const & v) { tuple_type::operator=(t2r_impl<tuple_type>(v,std::make_integer_sequence<std::size_t,N>{})); return *this;}

  constexpr VectorN(VectorN const & v) : tuple_type(v){}
  VectorN & operator=(VectorN const & v) { tuple_type::operator=(v); return *this;}

  constexpr VectorN(tuple_type const & v) : tuple_type(v){}




};


/*
template<typename T>
class VectorN<T,1> : public std::tuple<T> {
public:
  using tuple_type = std::tuple<T>;
  static constexpr unsigned int DIM=1;
  using value = typename std::remove_const<typename std::remove_reference<T>::type>::type;
  using  ref = typename std::add_lvalue_reference<T>::type;
  using cref = typename std::add_lvalue_reference<typename std::add_const< T>::type>::type;
  using VAL = VectorN<value,DIM>;
  using CREF = VectorN<cref,DIM>;
  using REF = VectorN<ref,DIM>;

  static constexpr value vdefI = value();
  static constexpr VAL defI = VAL(value());


};
*/

#include "UltimateSoa.h"
template<typename T, int N>
struct UltimateSoaTraits<VectorN<T,N>> {
  using value = typename std::remove_reference<T>::type;
  using ref = typename std::add_lvalue_reference<T>::type;
  using cref = typename std::add_lvalue_reference<typename std::add_const< T>::type>::type;
  using CREF = VectorN<cref,N>;
  using REF = VectorN<ref,N>;
  using VAL = VectorN<value,N>;
  using SOATUPLE = typename VectorN<UltimateSoa<value>,N>::tuple_type;

};

#endif
