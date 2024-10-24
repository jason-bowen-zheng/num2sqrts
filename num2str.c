#include "num2str.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUM 1000.0
#define MAX_DENOMINATOR 10000
#define PI 3.141592653589793

double ssqrt(double x) { return copysign(sqrt(fabs(x)), x); }

long gcd(long a, long b) {
    while (b != 0) {
        long tmp = a;
        a = b;
        b = tmp % b;
    }
    return a;
}

// This function will find p, q such that p/q is very close to x,
// where q < MAX_DENOMINATOR.
void rationalize(double x, long *p, long *q) {
    // The first part is to find these two integers p and q, no matter how big
    // they are. The code for this part is copied from cpython's implementation
    // of `float.as_integer_ratio`.
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

    // The second part simplifies the fraction computed in the first part so
    // that the denominator is less than MAX_DENOMINATOR. The code for this part
    // is rewritten from the `Fraction.limit_denominator` method in the
    // fractions module of the Python standard library.
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
        mid = floor(x * x / 4) + 0.5;
    } else {
        mid = ceil(-x * x / 4) - 0.5;
    }

    double actual_mid = x / 2, t = 0.5, i, j, k;
    while (1) {
        i = ssqrt(mid + t);
        j = fabs(i - actual_mid);
        k = actual_mid - j;
        k = ssqrt(copysign(round(k * k), k));
        if (fabs(i * i) > MAX_NUM || fabs(j * j) > MAX_NUM) {
            return 0;
        }
        if (fabs(i + k - x) < 1e-16) {
            *a = (long)round(copysign(i * i, i));
            *b = (long)round(copysign(k * k, k));
            return 1;
        }
        t += 1.0;
    }
}

void simplify(long x, long *outer, long *inner) {
    long flag = x > 0 ? 1 : -1;
    x = llabs(x);
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

void num2str(double x, char *s) {
    if (isinf(x)) {
        if (x > 0) {
            sprintf(s, "inf");
        } else {
            sprintf(s, "-inf");
        }
        return;
    }
    if (isnan(x)) {
        sprintf(s, "nan");
        return;
    }

    if (fabsl((long)x - x) < 1e-16) {
        sprintf(s, "%ld", (long)x);
        return;
    }

    long a, b;
    rationalize(x, &a, &b);
    if (fabs(x - (double)a / b) < 1e-12) {
        sprintf(s, "%ld/%ld", a, b);
        return;
    }

    rationalize(x * x, &a, &b);
    if (fabs(x * x - (double)a / b) < 1e-12) {
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
            fact = gcd(labs(outer), b);
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

    rationalize(x / PI, &a, &b);
    if (fabs(x - a * PI / b) < 1e-12) {
        if (b == 1) {
            sprintf(s, "%ld*pi", a);
        } else if (a == 1) {
            sprintf(s, "pi/%ld", b);
        } else if (a == -1) {
            sprintf(s, "-pi/%ld", b);
        } else {
            sprintf(s, "%ld*pi/%ld", a, b);
        }
        return;
    }

    for (int c = 1; c <= 100; ++c) {
        if (num2sqrts(x * c, &a, &b)) {
            long outer_a, inner_a;
            long outer_b, inner_b;
            long t;
            char part_a[512], part_b[256];
            simplify(a, &outer_a, &inner_a);
            simplify(b, &outer_b, &inner_b);
            if (c != 1 && outer_a < 0 && outer_b < 0) {
                outer_a = -outer_a;
                outer_b = -outer_b;
            }
            if ((inner_a != 1 && inner_a == 1) ||
                (inner_a < inner_b || !(labs(a) > labs(b)))) {
                t = outer_a;
                outer_a = outer_b;
                outer_b = t;
                t = inner_a;
                inner_a = inner_b;
                inner_b = t;
            }
            if (inner_a != 1) {
                if (outer_a < 0) {
                    strcat(part_a, "-");
                }
                if (labs(outer_a) != 1) {
                    sprintf(&part_a[strlen(part_a)], "%ld", labs(outer_a));
                }
                sprintf(&part_a[strlen(part_a)], "sqrt(%ld)", inner_a);
            } else {
                sprintf(part_a, "%ld", outer_a);
            }
            if (inner_b != 1) {
                if (outer_b > 0) {
                    strcat(part_b, "+");
                } else {
                    strcat(part_b, "-");
                }
                if (labs(outer_b) != 1) {
                    sprintf(&part_b[strlen(part_b)], "%ld", labs(outer_b));
                }
                sprintf(&part_b[strlen(part_b)], "sqrt(%ld)", inner_b);
            } else {
                if (outer_b > 0) {
                    strcat(part_b, "+");
                }
                sprintf(&part_b[strlen(part_b)], "%ld", outer_b);
            }
            strcat(part_a, part_b);
            if (c == 1) {
                sprintf(s, "%s", part_a);
            } else {
                if (c != 1 && outer_a < 0 && outer_b < 0) {
                    sprintf(s, "-(%s)/%d", part_a, c);
                } else {
                    sprintf(s, "(%s)/%d", part_a, c);
                }
            }
            return;
        }
    }

    sprintf(s, "%g", x);
}
