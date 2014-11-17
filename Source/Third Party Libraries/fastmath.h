#ifndef FASTMATH_H
#define FASTMATH_H

// Copyright (c) 2007, Nils Jonas Norberg, See COPYING.txt for details

#include <math.h>
#include <stdlib.h>

#define __forceinline inline

// a little collection of fast functions
__forceinline float fast_clamp_mul2(float x, float min, float max)
{
	float x1 = fabsf(x-min);
	float x2 = fabsf(x-max);
	x = x1+min+max-x2;
	return x;
}

// will convert 0..1023 -> 0..1023
__forceinline unsigned int fast_ftol1023( float v /* 0..1023 */ )
{
	v += 1024.f; // make range [1024..2047)
	int r = *((int*)(&v));
	r &= ~0xFF800000; // remove exponent
	r >>= (23-3); // had 23 bits of precision, want 10
	return r;
}

#endif // FASTMATH_H