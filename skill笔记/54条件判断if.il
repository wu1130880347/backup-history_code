* if
if 语句是用来判定给定的条件是否满足，根据判定的结果决定执行给出的两种操作之一。
SKILL 中的if 语句共有两种判定结果，nil 或者 non-nil 。if 语句执行结果的返回值是 if 语句中最后计算过的值。

格式：
  当条件1为真执行expA，否则执行expB
  
  if( condition1 then  
    expA
  else  
    expB
  )；end if


  下面例子请注意if语句结构与C（或其它语言）之间的区别
if( shapeType == "rect"
then
  println( "Shape is a rectangle" )
  ++rectCount
else
  println( "Shape is not a rectangle" )
  ++miscCount
)


    SKILL在计算过程中会检查代码自身的错误，而 if 语句书写错误信息是很模糊的，在使用 if 语句时要注意一下几点：
1）  注意 if 语句括号位置：if( … then … else … )
2）  避免在 if 关键字后面出现空格。
3）  使用 then 和 else 将其逻辑判定结果分配。

在 if 关键词后面多意外的出了一个空格。出错提示
Message: *Error* if: toofew arguments (at least 2 expected, 1 given)…) 

在 if 语句中缺少了 then 关键词，但是包含 else 关键词，该情况会返回一个nil 值。
Message: *Error* if: toomany arguments …