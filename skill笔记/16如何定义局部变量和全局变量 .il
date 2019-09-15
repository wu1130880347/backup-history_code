定义局部变量：
定义局部变量的2个关键字let和prog。let和prog的区别在于函数的返回值，let的返回值是let()中的最后一句表达式，prog的返回值比较多样，还包括return命令的返回值。相对于let，prog还多了项功能，在prog()中多了一个循环功能，支持go命令。
1. let 如let( ( args ) expX…)，在let中的变量args在let()范围有效，在let()范围外没有意义，不存在。
2. prog 如prog( ( args ) expX…)，在prog中的变量args在prog()范围有效，在prog()范围外没有意义，不存在。
定义全局变量：
不在prog和let里面指定定义变量的地方定义的变量都是全局变量，但是请尽量少使用全局变量。关键字defvar可以用来定义全局变量，如defvar(globalvar)。如上所示，使用let或不使用let和prog的函数返回值都是函数中最后的表达式，使用prog的情况下，会增加因为return语句带来的返回情况。