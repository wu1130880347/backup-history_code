* when
    when语句与 if 语句类似，同样先判定给定的条件是否满足，如果判定的结果满足要求，则执行给出的操作。when 等同于 if语句中只有 then 而没有 else的情况。
格式；
  当condition为真值时执行代码块expA，否则跳过
  
   when( condition  ;
              expA
           ); end when
  
  	  
  

  实例，注意最后都返回nil.
when(3>1 printf("True"))  =>True

when( shapeType == "rect"
println( "Shape is arectangle" )
++rectCount
) ; when
when( shapeType == "ellipse"
println( "Shape is anellipse" )
++ellipseCount
) ; when