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

unsigned float_i2f(int x) {
  int sig=0,exp=0,mag=0;
  unsigned int cnt=0xffffffff,x_=x;
  int flag=0,tmp=0,tail=0,lastBit=0,offset=0;
  sig=0x80000000&x;
  if(!x){
    return 0;
  }
  if(sig){
    x=-x;
  }
  while(x_){
    cnt=cnt+1;
    x_=(x_>>1);
  }
  offset=(1<<cnt);
  exp=((127+cnt)<<23);
  mag=(x&((offset-1)));
  if(cnt<24){
    mag=mag<<(23-cnt);
  }
  else{
    tmp=cnt-23;
    lastBit=(1<<tmp);
    flag=lastBit>>1;
    tail=(flag-1)&mag;
    flag=flag&mag;
    if(flag){
      if((tail)||((lastBit&mag))){
        mag=mag+lastBit;
        if(mag&(offset)){
          exp=exp+0x800000;
        }
      }
    }
    mag=(mag>>tmp)&0x7fffff;
  }

  return sig|exp|mag;
}


unsigned test_float_i2f(int x) {
  float f = (float) x;
  return f2u(f);
}


int main()
{
    int val1 = float_i2f(-1073741824);
    int val2 = test_float_i2f(-1073741824);
    printf("float_i2f(-1073741824) --> %d [0x%x]\n", val1, val1);
    printf("test_float_i2f(-1073741824) --> %d [0x%x]\n", val2, val2);
    if (val1 == val2) {
	printf(".. False negative\n");
    } else
	printf(".. A genuine counterexample\n");
}
