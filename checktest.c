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
  return 2;
}


unsigned test_float_i2f(int x) {
  float f = (float) x;
  return f2u(f);
}


int main()
{
    int val1 = float_i2f(0);
    int val2 = test_float_i2f(0);
    printf("float_i2f(0) --> %d [0x%x]\n", val1, val1);
    printf("test_float_i2f(0) --> %d [0x%x]\n", val2, val2);
    if (val1 == val2) {
	printf(".. False negative\n");
    } else
	printf(".. A genuine counterexample\n");
}
