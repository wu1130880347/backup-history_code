一、什么是转义字符
SKILL中的转义字符与C语言基本一样。它是一种特殊的字符常量，一般通过一个反斜杠和一个或多个字符组成。转义字符具有特定的含义，不同于字符原有的意义，故称“转义”字符。比如我们在编程中常常用到的换行符“\n”就是一个转义字符。转义字符主要用来表示那些用一般字符不便于表示的控制代码。
# l6 X2 ?4 b6 A8 P6 A
二、常用的转义字符表

下面是常用到的转义字符表

字符意义                转义字符
换行                            \n
水平制表符                 \t
垂直制表符                 \v
退格                           \b
回车符                       \r
换页                           \f
反斜杠                       \\
双引号                       \”
ASCII码（八进制）    \ddd


三、例子
比如常见的目录结构，一般为c:\\windows\\，其中的两个反斜杠代表的就是输出一个反斜杠，其输出的目录就为c:\windows。这就是为什么使用反斜杠指定目录时，需要同时使用两个。
请看下面的例子：

Skill > printf("line 1\n\tline 2\n\t\tline 3\n\t\t\tline 4\nCadence Skill\302\333\314\263")
line 1
        line 2
                line 3n
                        line 4
Cadence Skill论坛
t
Skill > axlGetVariable("cdsroot")
"d:\\Cadence\\SPB_16.3"
Skill > 

其中printf("line 1\n\tline 2\n\t\tline 3\n\t\t\tline 4\nCadence Skill\302\333\314\263")函数没输出一行换行后再添加相应的水平制表符，再输出另外的内容。

最后一行“\302\333\314\263”代表的是八进制的ASCII码，该部分ASCII码为汉子“论坛”的八进制代码。