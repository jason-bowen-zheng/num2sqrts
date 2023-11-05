from typing import Tuple as _Tuple, Optional as _Optional
from fractions import Fraction as _Frac
import math as _math


def _num2sqrts(n: float, max_num=1000) -> _Optional[_Tuple[int]]:
    if n >= 0:
        mid = _math.floor((n / 2) ** 2) + 0.5
    elif n < 0:
        mid = _math.ceil(-((n / 2) ** 2)) - 0.5

    def fsqrt(n):
        return _math.copysign(_math.sqrt(_math.fabs(n)), n)

    actual_mid = n / 2
    t = 0.5
    while True:
        a = fsqrt(mid + t)
        d = _math.fabs(a - actual_mid)
        b = actual_mid - d
        b = fsqrt(_math.copysign(round(b**2), b))
        if abs(a**2) > max_num or abs(b**2) > max_num:
            return None
        if _math.isclose(a + b, n):
            return int(round(_math.copysign(a**2, a))), int(
                round(_math.copysign(b**2, b))
            )
        t += 1


def _simplify(value):
    """
    Simplify a radical form of `sqrt(value)`,
    `value` is a positive integer in the root sign.
    """
    flag = 1 if value > 0 else -1
    value = abs(value)
    if value == 1:
        return (flag, 1)
    i, cache, inner, outer = 2, [], value, 1
    while inner != 1:
        for m in [i, i - 1]:
            if cache.count(m) == 2:
                outer *= m
                cache.remove(m)
                cache.remove(m)
        if _math.gcd(inner, i) != 1:
            inner //= i
            cache.append(i)
        else:
            i += 1
    if cache.count(i) == 2:
        outer *= i
        cache.remove(i)
        cache.remove(i)
    return flag * outer, _math.prod(cache)


def num2str(value, max_num={"frac": 1000, "sqrts": 1000}, arcus="acos", twice=False):
    """Converting floating point numbers to math expressions.

    max_num : dict
        Precision control.
    arcus : str
        Which inverse trignometric function to use, in `asin`, `acos` or `atan`.
    twice : bool
        Prevent recursion, please don't change it to `True`.
    """
    assert arcus in ["asin", "acos", "atan"]
    if int(value) == value:
        return str(int(value))
    flag = "" if value > 0 else "-"
    a, b = _Frac(value).limit_denominator(max_num["frac"]).as_integer_ratio()
    if _math.isclose(value, a / b):
        return f"{a}/{b}"
    a, b = _Frac(value**2).limit_denominator(max_num["frac"]).as_integer_ratio()
    if _math.isclose(value**2, a / b):
        if b == 1:
            outer, inner = _simplify(a)
            return f"{flag}{'' if outer == 1 else outer}sqrt({inner})"
        elif (a == 1) and (b != 1):
            return f"{flag}sqrt({b})/{b}"
        else:
            outer, inner = _simplify(a * b)
            fact = _math.gcd(outer, b)
            a, b = int(outer / fact), int(b / fact)
            return f"{flag}{'' if a == 1 else a}sqrt({inner})/{b}"
    if twice:
        return None
    if (s := num2str(value / _math.pi, max_num=max_num, twice=True)) is not None:
        pi = chr(960)
        if s.startswith("-1/") or s.startswith("1/"):
            return s.replace("1/", pi + "/")
        elif "/" in s:
            return s.replace("/", pi + "/")
        else:
            if s in ["1", "-1"]:
                s = flag
            return s + pi
    f = getattr(_math, arcus[1:])
    if (s := num2str(f(value), max_num=max_num, twice=True)) is not None:
        return f"{arcus}({s})"
    for c in range(1, 101):
        if (l := _num2sqrts(value * c, max_num=max_num["frac"])) is not None:
            outer_a, inner_a = _simplify(l[0])
            outer_b, inner_b = _simplify(l[1])
            flag = ""
            if c != 1 and outer_a < 0 and outer_b < 0:
                flag = "-"
                outer_a, outer_b = -outer_a, -outer_b
            if (inner_b == 1 and inner_a != 1) or (
                inner_a < inner_b and not l[0] > l[1]
            ):
                outer_a, outer_b = outer_b, outer_a
                inner_a, inner_b = inner_b, inner_a
            if inner_a != 1:
                part_a = f"{'' if outer_a > 0 else '-'}{'' if abs(outer_a) == 1 else abs(outer_a)}sqrt({inner_a})"
            else:
                part_a = str(outer_a)
            if inner_b != 1:
                part_b = f"{'+' if outer_b > 0 else '-'}{'' if abs(outer_b) == 1 else abs(outer_b)}sqrt({inner_b})"
            else:
                part_b = ("+" if outer_b > 0 else "") + str(outer_b)
            expr = part_a + part_b
            if c == 1:
                return expr
            else:
                return f"{flag}({expr})/{c}"
    return str(value)


__all__ = "num2str"
