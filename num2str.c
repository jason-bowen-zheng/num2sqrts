#include "num2str.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM 1000.0
#define MAX_DENOMINATOR 10000

double ssqrt(double x) { return copysign(sqrt(fabs(x)), x); }

long gcd(long a, long b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

void rationalize(double x, long *p, long *q) {
    int exponent;
    double float_part;
    long numerator, denominator, the_exponent;
    float_part = frexp(x, &exponent);
    for (int i = 0; i < 300 && float_part != floor(float_part); ++i) {
        float_part *= 2.0;
        --exponent;
    }
    numerator = (long)float_part;
    denominator = 1;
    the_exponent = (long)abs(exponent);
    if (exponent > 0) {
        numerator <<= the_exponent;
    } else {
        denominator <<= the_exponent;
    }

    if (denominator < MAX_DENOMINATOR) {
        *p = numerator;
        *q = denominator;
        return;
    }
    long p0 = 0, q0 = 1, p1 = 1, q1 = 0, q2, a, k, t;
    long n = numerator, d = denominator;
    while (1) {
        a = n / d;
        q2 = q0 + a * q1;
        if (q2 > MAX_DENOMINATOR) {
            break;
        }
        t = p0;
        p0 = p1;
        q0 = q1;
        p1 = t + a * p1;
        q1 = q2;
        t = n;
        n = d;
        d = t - a * d;
    }
    k = (MAX_DENOMINATOR - q0) / q1;

    if (2 * d * (q0 + k * q1) <= denominator) {
        *p = p1;
        *q = q1;
    } else {
        *p = p0 + k * p1;
        *q = q0 + k * q1;
    }
}

int num2sqrts(double x, long *a, long *b) {
    double mid;
    if (x > 0) {
        mid = floorl(x * x / 4) + 0.5;
    } else {
        mid = ceill(-x * x / 4) - 0.5;
    }

    double actual_mid = x / 2, t = 0.5, i, j, k;
    while (1) {
        i = ssqrt(mid + t);
        j = fabs(i - actual_mid);
        k = actual_mid - j;
        k = ssqrt(copysign(round(k * k), k));
        if (fabs(i * i) > MAX_NUM || fabs(j * j) > MAX_NUM) {
            return -1;
        }
        if (fabs(i + k - x) < 1e-16) {
            *a = (long)round(copysign(i * i, i));
            *b = (long)round(copysign(k * k, k));
            return 0;
        }
        t += 1.0;
    }
}

void simplify(long x, long *outer, long *inner) {
    long long flag = x > 0 ? 1 : -1;
    x = llabs(x);
    if (x == 1) {
        *outer = flag;
        *inner = 1;
    }
    *outer = 1;
    *inner = x;
    long long i = 2, cache = 1;
    while (*inner != 1) {
        for (long long m = i - 1; m <= i; ++m) {
            if (cache % (m * m) == 0) {
                *outer *= m;
                cache /= m * m;
            }
        }
        if (gcd(*inner, i) != 1) {
            *inner /= i;
            cache *= i;
        } else {
            ++i;
        }
    }
    if (cache % (i * i) == 0) {
        *outer *= i;
        cache /= i * i;
    }
    *outer = flag * (*outer);
    *inner = cache;
}

void num2str(double x, char *s) {
    if (isinf(x)) {
        sprintf(s, "inf");
        return;
    }
    if (isnan(x)) {
        sprintf(s, "nan");
        return;
    }
    if (fabsl((long long)x - x) < 1e-16) {
        sprintf(s, "%lld", (long long)x);
        return;
    }

    long a, b;
    rationalize(x, &a, &b);
    if (fabsl(x - (long double)a / b) < 1e-12) {
        sprintf(s, "%ld/%ld", a, b);
        return;
    }

    rationalize(powl(x, 2), &a, &b);
    if (fabsl(powl(x, 2) - (long double)a / b) < 1e-12) {
        long outer, inner, fact;
        if (x < 0) {
            a = -a;
        }
        if (b == 1) {
            simplify(a, &outer, &inner);
            if (outer == 1) {
                sprintf(s, "sqrt(%ld)", inner);
            } else if (outer == -1) {
                sprintf(s, "-sqrt(%ld)", inner);
            } else {
                sprintf(s, "%ld*sqrt(%ld)", outer, inner);
            }
        } else if (labs(a) == 1 && b != 1) {
            if (a == 1) {
                sprintf(s, "sqrt(%ld)/%ld", b, b);
            } else {
                sprintf(s, "-sqrt(%ld)/%ld", b, b);
            }
        } else {
            simplify(a * b, &outer, &inner);
            fact = gcd(outer, b);
            a = outer / fact;
            b = b / fact;
            if (a == 1) {
                sprintf(s, "sqrt(%ld)/%ld", inner, b);
            } else if (a == -1) {
                sprintf(s, "-sqrt(%ld)/%ld", inner, b);
            } else {
                sprintf(s, "%ld*sqrt(%ld)/%ld", a, inner, b);
            }
        }
        return;
    }
    sprintf(s, "%g", x);
}