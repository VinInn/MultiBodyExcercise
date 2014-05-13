void mirror( float & x, float w) {
  x = w - (x-w);
}
void mirror2( float & x, float w) {
  x = 2.f*w - x;
}

void mirror3( float & x, float w) {
  x -= 2.f*(x - w);
}

/* compare
 c++ -std=c++11 -O2 -Wall -S mirror.cc -mavx2; cat mirror.s
 c++ -std=c++11 -O2 -Wall -S mirror.cc -mavx2 -mfma; cat mirror.s
 c++ -std=c++11 -Ofast -Wall -S mirror.cc -mavx2 -mfma; cat mirror.s
 c++ -std=c++11 -Ofast -Wall -S mirror.cc -mavx2; cat mirror.s
*/

