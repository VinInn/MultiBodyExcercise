#ifndef AlignAllocator_H
#define AlignAllocator_H
#include <memory>
#include <cstdlib>

template <typename T, int N>
class align_allocator: public std::allocator<T>
{
public:
  using Base = std::allocator<T>;
  using pointer = typename Base::pointer;
  using size_type = typename Base::size_type;



  template<typename _Tp1>
  struct rebind
  {
    typedef align_allocator<_Tp1, N> other;
  };

  pointer allocate(size_type n, const void *hint=0)
  {
    void * p=0;  if(!(0==posix_memalign(&p,N,sizeof(T)*n))) throw std::bad_alloc(); return (pointer)(p);
  }
  
  void deallocate(pointer p, size_type n)
  {
    free(p);
  }
  
  align_allocator() = default;
  align_allocator(align_allocator const&)=default;
  align_allocator(align_allocator &&)=default;

  template <class U>                    
  align_allocator(const align_allocator<U,N> &a) noexcept: std::allocator<T>(a) { }
};

#endif
