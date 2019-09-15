与其它编程语言不一样，Skill中的变量可以不用提前声明。当程序在一段代码中遇到一个变量时，Skill 会自动创建该变量。
变量名可包含以下符号：
数字、字母、下划线、问号
注意，变量名的第一个字符不能是数字。
当 SKILL 创建一个变量时，将会附一个 unbound 初值给变量表明该变量还未初始化。可使用 boundp 函数判断一个变量是否是 bound。boundp函数具有以下功能：
如果变量为 bound ，返回 t
如果不是 bound ，返回 nil
使用赋值操作符可以给变量赋一个值。我们可通过变量名获得变量的值。
可通过使用 type 函数判断一个变量当前存储的数据类型。如下
lineCount = 4 => 4
lineCount => 4
type( lineCount ) => fixnum
lineCount = "abc" => "abc"
lineCount => "abc"
type( lineCount ) => string