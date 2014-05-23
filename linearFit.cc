template<typename T> 
void linearFit( T const  * __restrict__ x, T const  * __restrict__ y, int ndat,
		T const  * __restrict__ sigy2,  
		T & slope, T & intercept,
		T & covss, T & covii, T & covsi) {
  T g1 = 0, g2 = 0;
  T s11 = 0, s12 = 0, s22 = 0;
  for (int i = 0; i != ndat; i++) {
    T sy2 = T(1)/sigy2[i];
    g1 += y[i] *sy2;
    g2 += x[i]*y[i] * sy2;
    s11 += sy2;
    s12 += x[i] * sy2;
    s22 += x[i]*x[i] * sy2;
  }
  
  T d = T(1)/(s11*s22 - s12*s12);
  intercept = (g1*s22 - g2*s12) * d;
  slope =     (g2*s11 - g1*s12) * d;
  
  covii =  s22 * d;
  covss =  s11 * d;
  covsi = -s12 * d;
}


//----------------------------------------------

template<typename T>
struct IntType {
  using type = T;
};

template<>
struct IntType<float> {
  using type = int;
};
template<>
struct IntType<double> {
  using type = long long;
};


template<typename T, int N>
struct ExtVecTraits {
  typedef T __attribute__( ( vector_size( N*sizeof(T) ) ) ) type;
  typedef T __attribute__( ( vector_size( N*sizeof(T) ), aligned(alignof(T)) ) ) typeA;


  static type load(T const * p) { return *(typeA const *)(p);}
  static void load(T *p, type const & v) { *(typeA *)(p) = v; }

  static typeA & bind(T * p) { return *(typeA *)(p);}
  static typeA & bind(T const * p) { return *(typeA const *)(p);}

  typedef typename IntType<T>::type __attribute__( ( vector_size( N*sizeof(typename IntType<T>::type) ) ) ) itype;


};



template<typename T, int N> using ExtVec =  typename ExtVecTraits<T,N>::type;
template<typename T, int N> using ExtVecI =  typename ExtVecTraits<T,N>::itype;

template<typename T> using Vec4D = ExtVec<T,4>;
template<typename T> using VecI4D = ExtVecI<T,4>;





template<typename T> struct Point { T x,y,z, wx,wy;};

template<typename T> 
struct LinearFit{

  using Vec = Vec4D<T>;
  using VecI = VecI4D<T>;
  Vec sol;
  Vec g; // gx1,gx2,gy1,gy2;
  Vec snn; // sx22,sx11,sy22,sy11
  Vec s12;  // T s12x, s12y;

  void add(Point<T> const & p) {
    // Vec w{p.wx,p.wx,p.wy,p.wy};  // load and shuffle??
    // Vec xy{T(1),p.x,T(1),p.y};  // load and shuffle???


    Vec w = ExtVecTraits<T,4>::load(&p.wx);  //can be very unsafe
    w = __builtin_shuffle(w,VecI{0,0,1,1});

    Vec one{T(1),T(1),T(1),T(1)};
    Vec xy = ExtVecTraits<T,4>::load(&p.x);
    xy =  __builtin_shuffle(xy,one,VecI{4,0,5,1});

    auto tmp = w*xy;
    g += p.z*tmp;
    s12 +=  __builtin_shuffle(tmp,VecI{1,1,3,3});
    tmp*=xy;
    snn +=__builtin_shuffle(tmp,VecI{1,0,3,2});
    /*
    g[0] +=     (p.z * p.wx);
    g[1] += p.x*(p.z * p.wx);
    g[2] +=     (p.z * p.wy);
    g[3] += p.y*(p.z * p.wy);
    snn[1] += p.wx;
    snn[0] += p.x*(p.x*p.wx);
    snn[3] += p.wy;
    snn[2] += p.y*(p.y*p.wy);
    s12x += p.x*p.wx;
    s12y += p.y*p.wy;
    */

  }


  void remove(Point<T> const & p) {
    Vec w = ExtVecTraits<T,4>::load(&p.wx);  //can be very unsafe
    w = __builtin_shuffle(w,VecI{0,0,1,1});

    Vec one{T(1),T(1),T(1),T(1)};
    Vec xy = ExtVecTraits<T,4>::load(&p.x);
    xy =  __builtin_shuffle(xy,one,VecI{4,0,5,1});

    auto tmp = w*xy;

    s12 -= __builtin_shuffle(tmp,VecI{1,1,3,3});
    snn -= __builtin_shuffle(tmp,VecI{1,0,3,2});
 
  }

  void solve() {
    //  T dx = snn[1]*snn[0]; // - s12x*s12x;
    // T dy = snn[3]*snn[2]; // - s12y*s12y;
    // Vec d ={dx,dx,dy,dy};

    // Vec s12{s12x,s12x,s12y,s12y};
    Vec d =  __builtin_shuffle(snn,VecI{1,1,3,3})* __builtin_shuffle(snn,VecI{0,0,2,2});
    d -= s12*s12;


    // Vec g2{g[1],g[0],g[3],g[2]};
    Vec g2 = __builtin_shuffle(g,VecI{1,0,3,2});
    sol = (g*snn - g2*s12)/d;
  }

};

void add(Point<float> const & p, LinearFit<float> & f) { f.add(p);}
void solve(LinearFit<float> & f) { f.solve();}



void add(Point<double> const & p, LinearFit<double> & f) { f.add(p);}
void solve(LinearFit<double> & f) { f.solve();}
