#include "trig.c" //because it's damn ugly

#define abs(val) (val ^ (val >> 31)) + ((val >> 31)&1)
float sin(int x) {if (x < 0) x = 360 + x;return sintab[x % 360];}
float cos(int x) {if (x < 0) x = 360 + x;return costab[x % 360];}
float tan(int x) {if (x < 0) x = 360 + x;return tantab[x % 360];}

// sqrt via newton's approximation
float sqrt(float val) {
	float ret;
	__asm__("sqrt.s %[out], %[in]" : [out] "=f" (ret) : [in] "f" (val));
	return ret;
}

