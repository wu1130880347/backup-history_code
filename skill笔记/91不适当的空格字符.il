在函数名和左括号之间切记不要留空格，以下例子在函数名和左括号之间出现了空格，函数执行时，产生报错。
1)使用 strcat 函数合并多个字符串
strcat ( “Mary” ” had” ” a” ” little” ” lamb”)
Message: *Error* eval: not a function – “Mary”
2)将函数值分配给一个变量
greeting = strcat ( “happy” ” birthday” )
Message: *Error* eval: unbound variable – strcat5 