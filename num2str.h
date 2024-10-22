#ifndef _NUM2STR_H
#define _NUM2STR_H

double ssqrt(double x);
long gcd(long a, long b);
void rationalize(double x, long *p, long *q);
int num2sqrts(double x, long *a, long *b);
void simplify(long x, long *outer, long *inner);
void num2str(double x, char *s);

#endif
