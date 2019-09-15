如何访问一个函数
要访问一个函数，需要函数名与参数，还有一对圆括号。
strcat( "Mary" " had" " a" " little" " lamb" )
=> "Mary had a little lamb"
复制代码

需注意：
。函数名称也是区分大小写的
。函数名和左括号之间不允许有空格
    需要注意的是函数和第一个括号之间没有空格，否则会报错,如
Skill >strcat ( "Hello" "," " everyone" "!" )
=> *Error* eval: not a function - "Hello": 

函数表达式在源代码文件中可以多行或在命令行中可跨行
strcat(
"Mary" " had" " a"
" little" " lamb" )
=> "Mary had a little lamb"
一行可允许多个函数调用。系统只显示从最终函数调用的返回结果。