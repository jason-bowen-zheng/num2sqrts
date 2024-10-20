#include "num2str.h"
#include <math.h>
#include <stdlib.h>

#define MAX_NUM 1000.0

double ssqrt(double x) { return copysign(sqrt(fabs(x)), x); }

long gcd(long a, long b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

int num2sqrts(long double x, long *a, long *b) {
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
        if (fabsl(i + k - x) < 1e-12) {
            *a = (long)round(copysign(i * i, i));
            *b = (long)round(copysign(k * k, k));
            return 0;
        }
        t += 1.0;
    }
}

void simplify(long x, long *outer, long *inner) {
    long flag = x > 0? 1: -1;
    x = labs(x);
    if (x == 1) {
        *outer = flag;
        *inner = 1;
    }
    *outer = 1;
    *inner = x;
    long i = 2, cache = 1;
    while (*inner != 1) {
        for (long m = i - 1; m <= i; ++m) {
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
