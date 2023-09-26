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

int isGreater(int x, int y) {
  int mask=(1<<31);
  int minus=y+((~x)+1);
  int sig_m=mask&minus;

  return !(!sig_m);
}


int test_isGreater(int x, int y)
{
  return x > y;
}


int main()
{
    int val1 = isGreater(-134217728, 2080374784);
    int val2 = test_isGreater(-134217728, 2080374784);
    printf("isGreater(-134217728, 2080374784) --> %d [0x%x]\n", val1, val1);
    printf("test_isGreater(-134217728, 2080374784) --> %d [0x%x]\n", val2, val2);
    if (val1 == val2) {
	printf(".. False negative\n");
    } else
	printf(".. A genuine counterexample\n");
}
