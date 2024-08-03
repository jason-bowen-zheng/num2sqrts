import math
import random
from time import time


def _fsqrt(n: float) -> float:
    return math.copysign(math.sqrt(math.fabs(n)), n)


def normal_one(n: float) -> tuple[int, int] | None:
    for x in range(101):
        for y in range(101):
            if math.isclose(_fsqrt(x) + _fsqrt(y), n, rel_tol=1e-16):
                return x, y


def num2sqrts(
    n: int | float, max_num: int = 1000, count_loops: bool = False
) -> int | tuple[int, int] | None:
    if n >= 0:
        mid = math.floor((n / 2) ** 2) + 0.5
    else:
        mid = math.ceil(-((n / 2) ** 2)) - 0.5
    actual_mid = n / 2
    loops = 1
    t = 0.5
    while True:
        a = _fsqrt(mid + t)
        d = math.fabs(a - actual_mid)
        b = actual_mid - d
        b = _fsqrt(math.copysign(round(b**2), b))
        if abs(a**2) > max_num or abs(b**2) > max_num:
            if count_loops:
                return loops
            return
        if math.isclose(a + b, n, rel_tol=1e-12):
            if count_loops:
                return loops
            return int(round(math.copysign(a**2, a))), int(
                round(math.copysign(b**2, b))
            )
        loops += 1
        t += 1


def compare():
    random.seed(100)
    total_no = 0
    for _ in range(5000):
        x, y = random.randint(0, 100), random.randint(0, 100)
        now = time()
        normal_one(_fsqrt(x) + _fsqrt(y))
        total_no += time() - now

    random.seed(100)
    total_ns = 0
    for _ in range(5000):
        x, y = random.randint(0, 100), random.randint(0, 100)
        now = time()
        num2sqrts(_fsqrt(x) + _fsqrt(y))
        total_ns += time() - now

    print(f"normal_one : {total_no}")
    print(f"num2sqrt   : {total_ns}")
    print(f"rate       : {total_no / total_ns}")


def perform():
    import numpy as np
    import matplotlib.pyplot as plt

    mat = np.zeros((201, 201))
    for x in range(-100, 101):
        for y in range(-100, 101):
            if x == y:
                mat[x + 100][-y + 100] = 1
            else:
                mat[x + 100][-y + 100] = num2sqrts(
                    _fsqrt(x) + _fsqrt(y), count_loops=True
                )
    fig, ax = plt.subplots()
    img = ax.matshow(mat)
    fig.colorbar(img, ax=ax)
    ax.set_xlabel("x")
    ax.xaxis.set_major_formatter(lambda x, pos: int(x - 100))
    ax.set_ylabel("y")
    ax.yaxis.set_major_formatter(lambda y, pos: int(-y + 100))
    fig.savefig("perform.pdf")


if __name__ == "__main__":
    from sys import argv

    if argv[1] == "perform":
        perform()
    elif argv[1] == "compare":
        compare()
