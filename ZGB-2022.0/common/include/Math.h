#ifndef MATH_H
#define MATH_H

#include "asm/types.h"

INT16 DespRight(INT16 a, INT16 b);

#define U_LESS_THAN(A, B) ((A) - (B) & 0x8000u)

#define DISTANCE(A, B) (U_LESS_THAN(A, B) ? (B - A) : (A - B))

#define SET_BIT(N, POS)   N |=   1 << POS
#define UNSET_BIT(N, POS) N &= ~(1 << POS)
#define GET_BIT(N, POS)   (1 & DespRight(N, POS))

#define SET_BIT_MASK(N, MASK) N |= MASK
#define UNSET_BIT_MASK(N, MASK) N &= ~MASK
#define GET_BIT_MASK(N, MASK) (N & MASK)

#endif