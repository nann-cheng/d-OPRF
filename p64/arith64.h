#ifndef ARITHMETIC_64_H
#define ARITHMETIC_64_H

#include "../arith.h"


// Reduction modulo p
void f_red(f_elm_t a);

// Generate a random field element
void f_rand(f_elm_t a);

// Addition of two field elements
void f_add(const f_elm_t a, const f_elm_t b, f_elm_t c);

// Subtraction of two field elements
void f_sub(const f_elm_t a, const f_elm_t b, f_elm_t c);

// Negation of a field element
void f_neg(const f_elm_t a, f_elm_t b);

// Multiplication of two multiprecision words (without reduction)
void mp_mul(const digit_t* a, const digit_t* b, digit_t* c);

// Montgomery form reduction after multiplication
void mont_rdc(const digit_t* a, digit_t* c);

// Multiplication of field elements
void f_mul(const f_elm_t a, const f_elm_t b, f_elm_t c);

// Convert a number from value to Montgomery form  (a -> aR)
void to_mont(const digit_t* a, f_elm_t b);

// Convert a number from Montgomery form into value (aR -> a)
void from_mont(const f_elm_t a, digit_t* b);

// Multiplicative inverse of a field element
void f_inv(const f_elm_t a, f_elm_t b);

// Legendre symbol of a field element
void f_leg(const f_elm_t a, unsigned char *b);


#endif