# num2str
正如它的名字所描述的那样，这是一个没有什么含金量的、默默无闻的pythhon库。

它的功能不太好描述，那我们先看演示吧：
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
'sqrt(3)+sqrt(2)'
```

就是这样，它能使一些浮点数转换为人类可读的字符串：

1. 分子、分母都为整数的分数
2. `±sqrt(a)/b`型的数（a、b是整数）
3. `(±sqrt(a)±sqrt(b))/c`型的数（a、b、c都是整数）
4. 如上述`(1)`或`(2)`型的数乘以π
5. 对上述`(1)`或`(2)`型的数施加反三角运算

