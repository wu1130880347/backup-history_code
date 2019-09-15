概述
        Cadence Skill 语言为“嵌入式”高级计算机语言，基于Lisp 语言，支持一套类似C语言的语法，大大降低了初学者学习的难度。程序员可以使用类似Lisp语言的全部功能，所以Skill 是一款使用简单而功能强大的编程语言，可以用作开发Allegro的任何应用。
        SKILL可以与底层系统交互，也提供了访问Cadence各个工具的丰富接口。在Cadence环境下，允许Skill自定义用户界面，并包含强大的跟踪、调试和分析工具。Skill还可以让你访问和控制所有的工具、配置用户环境、设计数据库。用户可以通过Skill语言来访问，并且可以开发自己的基于Cadence平台的快捷、实用工具。

语法格式
         在Skill 里，函数调用可写成下面两种语法格式。一种是代数语法格式，现在大多计算机语言采用这种方式，即func( arg1 arg2 ...)，

    procedure( fibonacci(n)
    if( (n == 1 || n == 2) then 
      1 
    else fibonacci(n-1) + fibonacci(n-2) 
    )
  )

另一种是前缀语法格式，类似于Lisp语言，即(func arg1 arg2 ...)。
  (defun fibonacci (n)
    (cond 
      ((or (equal n 1) (equal n 2)) 1) 
        (t (plus (fibonacci (difference n 1)) 
          (fibonacci (difference n 2))))
    )
  )
        这里可以看到类似Lisp语言的表示法后面有很多右括号，而且函数和参数容易混淆，所以一般推荐还是用常用的类C语言代数表示法。