关系运算符
    关系运算符是逻辑运算中比较简单的一种。关系运算实际上是比较运算。将两个值进行比较，判断其比较结果是否符合给定的条件。使用以下运算符比较两个数据值。如果比较的数据类型不一致，SKILL通常会返回一个错误信息。所有这些运算符将返回t 或者 nil 。
函数              操作符       例       结果
Lessp（小于）       <         8<9        t
Leqp（小等于）      <=        2<=5       t
Greaterp（大于）    >         8>7        t
Geqp(大等于）       >=        5>=1       t
Equal(等于)         ==        3.0==3     t
                          "abc"=="ABc"  nil
Nequal(不等于)      !=        3!=2       t
                          "abc"!="ABc"   t

    当SKILL运行出错时，会提示函数名称。而不是运算符。比如：
1 > "abc"
Message: *Error* greaterp: can’t handle (1 > "abc")