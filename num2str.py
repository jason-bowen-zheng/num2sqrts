from fractions import Fraction as _Frac
import math as _math


def _num2sqrts(n, max_num=1000):
    if n >= 0:
        mid = _math.floor((n / 2) ** 2) + 0.5
    elif n < 0:
        mid = _math.ceil(-(n / 2) ** 2) - 0.5

    def fsqrt(n): return _math.copysign(_math.sqrt(_math.fabs(n)), n)
    actual_mid = n / 2
    t = 0.5
    while True:
        a = fsqrt(mid + t)
        d = _math.fabs(a - actual_mid)
        b = actual_mid - d
        b = fsqrt(_math.copysign(round(b ** 2), b))
        if abs(a ** 2) > max_num or abs(b ** 2) > max_num:
            return None
        if _math.isclose(a + b, n):
            return int(round(_math.copysign(a ** 2, a))), \
                int(round(_math.copysign(b ** 2, b)))
        t += 1


def _simplify(value):
    """化简形如`sqrt(a)`的根式，value是根号内正整数。"""
    flag = 1 if value > 0 else -1
    value = abs(value)
    i, result = 2, []
    while True:
        if value == i:
            result.append(i)
            break
        if _math.gcd(value, i) == i:
            result.append(i)
            value = value // i
            i = 2
            continue
        i += 1
    inner, outter = [], 1
    for i in result:
        if inner.count(i) == 1:
            inner.remove(i)
            outter *= i
        else:
            inner.append(i)
    return flag * outter, _math.prod(inner)


def num2str(value, max_num={"frac": 1000, "sqrts": 1000}, arcus="acos", twice=False):
    """将浮点数转换为数学表达式：

    1. 分子、分母都为整数的分数
    2. ±sqrt(a)/b型的数（a、b为整数）
    3. (±sqrt(a)±sqrt(b))/c型的数（a、b、c都是整数）
    4. 对上述(1)或(2)型的数乘以π
    5. 对上述(1)或(2)型的数施加反三角运算

    max_num : dict
        精度控制：`{"frac": <分子、分母允许的最大值>, "sqrts": <给_num2sqrts的max_num参数>}`
    arcus : str
        使用哪个反三角函数表示，`asin`、`acos`或`atan`之一
    twice : bool
        防止递归，请不要将它改为`True`
    """
    assert arcus in ["asin", "acos", "atan"]
    if int(value) == value:
        return "%d" % value
    flag = "" if value > 0 else "-"
    a, b = _Frac(value).limit_denominator(max_num["frac"]).as_integer_ratio()
    if _math.isclose(value, a / b):
        return "%d/%d" % (a, b)
    a, b = _Frac(
        value ** 2).limit_denominator(max_num["frac"]).as_integer_ratio()
    if _math.isclose(value ** 2, a / b):
        if b == 1:
            outter, inner = _simplify(a)
            return "%s%ssqrt(%d)" % (flag, "" if outter == 1 else outter, inner)
        elif (a == 1) and (b != 1):
            return "%ssqrt(%d)/%d" % (flag, b, b)
        else:
            outter, inner = _simplify(a * b)
            fact = _math.gcd(outter, b)
            a, b = int(outter / fact), b / fact
            return "%s%ssqrt(%d)/%d" % (flag, "" if a == 1 else a, inner, b)
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
        return "%s(%s)" % (arcus, s)
    # 请允许我在这里硬编码一个数据，不然的话就太~慢~啦~
    for c in range(1, 101):
        if (l := _num2sqrts(value * c, max_num=max_num["frac"])) is not None:
            outter_a, inner_a = _simplify(l[0])
            outter_b, inner_b = _simplify(l[1])
            flag_a = "" if outter_a > 0 else "-"
            flag_b = "+" if outter_b > 0 else "-"
            outter_a = "" if abs(outter_a) == 1 else abs(outter_a)
            outter_b = "" if abs(outter_b) == 1 else abs(outter_b)
            if c == 1:
                return "%s%ssqrt(%d)%s%ssqrt(%d)" % (flag_a, outter_a, inner_a, flag_b, outter_b, inner_b)
            else:
                return "(%s%ssqrt(%d)%s%ssqrt(%d))/%d" % (flag_a, outter_a, inner_a, flag_b, outter_b, inner_b, c)
    return str(value)
