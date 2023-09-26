#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Convert from bit level representation to floating point number */
float u2f(unsigned u) {
  union {
    unsigned u;
    float f;
  } a;
  a.u = u;
  return a.f;
}

/* Convert from floating point number to bit-level representation */
unsigned f2u(float f) {
  union {
    unsigned u;
    float f;
  } a;
  a.f = f;
  return a.u;
}

/* Fix any NaN to be NaN value 0x7FC00000 */
float fix_nan(float f)
{
  if (isnanf(f))
    return u2f(0x7FC0000);
  else
    return f;
}

int float64_f2i(unsigned uf1, unsigned uf2) {
  int exp=((uf2&(0x7ff00000))>>20);
  int sig=uf2&(0x80000000);
  int ans=0x40000000|((uf1&0xffc00000)>>22)|((uf2&0xfffff)<<10);
  int offset=1053-exp;
  if(exp>1053){
    return 0x80000000;
  }
  else if(exp<1023){
    return 0;
  }else{
    ans=ans>>offset;
    if(!sig){
      ans=-ans;
    }
  }
  return ans;
}


int test_float64_f2i(unsigned uf1, unsigned uf2) {
  unsigned sign = uf2 >> 31;
  unsigned exp = (uf2 >> 20) & 0x7FF;
  unsigned val = 0x80000000u | ((uf2 & 0xFFFFF) << 11) | (uf1 >> 21);
  if (exp < 1023) {
    return 0;
  }
  if (exp > 1054)
    return 0x80000000u;
  val = val >> (1054 - exp);
  if (sign) {
    return val > 0x80000000u ? 0x80000000u : -val;
  } else {
    return val > 0x7FFFFFFF ? 0x80000000u : val;
  }
}


int main()
{
    int val1 = float64_f2i(0, 1103101952);
    int val2 = test_float64_f2i(0, 1103101952);
    printf("float64_f2i(0, 1103101952) --> %d [0x%x]\n", val1, val1);
    printf("test_float64_f2i(0, 1103101952) --> %d [0x%x]\n", val2, val2);
    if (val1 == val2) {
	printf(".. False negative\n");
    } else
	printf(".. A genuine counterexample\n");
}
