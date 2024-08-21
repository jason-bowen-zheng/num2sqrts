# num2str
**num2str** is a tiny Python library to convert a floating number into some specific mathematical expressions. The algorithm, which can be used in numerical calculation tools, is simple and efficient.

The following example shows how to use this library:
```python
>>> from math import *
>>> from num2str import num2str
>>> num2str(3 / 5)
'3/5'
>>> num2str(pi / 2)
'π/2'
>>> num2str(-2 * sqrt(3))
'-2sqrt(3)'
>>> num2str(sqrt(2) + sqrt(3))
'sqrt(2)+sqrt(3)'
```

It supports the following formats:

1. $\frac{a}{b}$
2. $\frac{\sqrt{a}}{b}$
3. $\frac{\sqrt{a}+\sqrt{b}}{c}$
4. $\text{some value}\times\pi$

We use a new algorithm to calculate the (3) format, You can find more information about the algorithm in [this blog](https://zhengxyz123.github.io/coding/num2sqrts).
