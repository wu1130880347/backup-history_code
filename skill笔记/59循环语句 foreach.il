foreach

foreach是简化版的for，上面的例子用foreach写如下
         alist = ‘(1 2 3 4 5)
         foreach( item alist
               println( item )
         )

1) shapeType 是一个索引变量，在执行foreach 语句前，SKILL首先保存当前shapeType 的值，当执行一次循环后，shapeType 保存 shapeTypeList 中的下一个元素值。) q  u/ x2 @* e, `
2) shapeTypeList 包含list中的值，当执行完一次循环后，SKILL将 shapeTypeList 的值分配给变量shapeType
3)foreach 的语句一次循环相当于单个的 case 语句。
4)foreach 的返回值是一个list ，包含 shapeTypeList. 中的所有元素。
5)foreach通常用于操作列表，因为它会遍历列表的每一个元素