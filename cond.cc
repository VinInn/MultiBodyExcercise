void cond0(float * a, float const * b, float * c, int N) {
#pragma GCC ivdep
  for (decltype(N) i=0; i<N; ++i) 
    if (a[i]<b[i]) a[i]+=b[i]*c[i];
    else a[i]=b[i];
}

void cond1(float * a, float const * b, float * c, int N) {
#pragma GCC ivdep
  for (decltype(N) i=0; i<N; ++i) {
    auto tc = c[i];
    a[i] = (a[i]<b[i]) ? a[i]+b[i]*tc : b[i];
  }
}
