case是一个多分支选择语句。请看下面的例子：
case( shapeType
( "rect"  ++rectCount  println( "Shape is arectangle" ))
( "line"  ++lineCount  println( "Shape is aline" ))
( "label" ++labelCountprintln( "Shape is alabel" ))
( t        ++miscCount println( "Shape is miscellaneous" ))
) ; case


t 值作为匹配所有值出现在语句的最后。case 语句返回最后一个表达式的值. 在该例子中需要清楚以下几点：
1)    shapeType的值是与 rect, line,label.相比较的，当 shapeType 的值与 rect, line,label中的任意相同，那么就执行该值下面的所有表达式。
2)    如果未找到匹配的值，那么最后一段语句将会执行。
3)    如果一个分支的值是一个 list，那么只要 list 中的任一个值与比较值匹配，该分支下的所有语句将会执行。
case( shapeType
      ( "rect"
          ++rectCount  
          println( "Shape is arectangle"
       )
      ( ( "label" "line" ) 
        ++labelOrLineCount
        println( "Shape is a line ora label" )
       )
       ( t 
          ++miscCount
          println( "Shape is miscellaneous" )
        )
) ; case

case 或caseq
caseq的用法类似cond，功能也类似。
caseq( var
   (condition1 expA)
   (condition2 expB)
    …
) ；case语法结构