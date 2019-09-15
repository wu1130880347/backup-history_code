函数调用方式

有三种不同的格式，类似 c ， lisp , Pascal 语言格式
strcat( "Mary" " had" " a" " little" " lamb" )    ;类似C语言格式          无空格函数
( strcat "Mary" " had" " a" " little" " lamb" )   ;Lisp语言格式           有空格函数
strcat "Mary" " had" " a" " little" " lamb"       ;去除最外层圆括号格式   有空格函数
   返回的结果都是=> "Mary had a littlelamb"

    推荐使用第一种方式，如果软件没响应，比如在CIW中输入段代码，软件没有反映，什么结果也没有，一般是因为( )或者" "不成对造成的，一般可以通过键入 ] 来解决，它表示补充完不对称的括号(可以代替任意多个右括号)，如果还没有响应，键入"] 这时大部分情况下，系统会有响应b. 数据类型不匹配，如：
Skill >strcat( "Mary had a" 5 )
=>*Error* strcat:argument #2 should be either a string or a symbol (type template = "S") – 5 
代码排版格式!
    编写代码的任何程序结构与代码格式都是为了他人或日后自已可快速浏览并明白正在查看的程序段要执行什么？所以个人以为要养成按界面表单的顺序来按列代码模块，与严格的上下代码段左缩进对齐显行犹为重要