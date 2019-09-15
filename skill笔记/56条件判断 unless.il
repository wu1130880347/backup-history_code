unless
格式：
  当condition为假值时执行代码块expA，否则跳过
  
  unless( condition
    expA
  ); end unless

  
实例
unless(3<1 printf("True"))  =>True

使用 unless 语句可避免无效的条件，unless 语句可避免判定条件的混乱。最后返回nil.

unless( shapeType == "rect" || shapeType == "line"
println( "Shape ismiscellaneous" )
++miscCount
) ; unless