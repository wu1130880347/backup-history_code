@rest
@rest允许任意多个参数传递个函数，@rest的参数保存在单个的list中
procedure(trTrace(fun @rest args)
let((result)
printf(“\nCalling %s passing %L” fun args)
result=apply(fun args)
printf(“\nReturning from %s with %L\n” fun args)
result
);end let
);end procedure
复制代码
上面声明一个函数名为 trTrace的函数，该函数传递一个不确定的参数args。
当以以下方式调用函数时，返回值为6。
trTrace( ‘plus 1 2 3 ) => 6
Calling plus passing (1 2 3)
Returning from plus with 6
以上函数调用函数plus，并将第二个参数中的值全部传递给plus函数。
@optional
@optional是另外一个可以指定任意个参数的选项，一般，在使用@optional时需要给参数一个默认的值，
当函数传递时未使用该参数传递值是，那么将使用该参数的默认值。
procedure(trBuildBox(height width @optional (xCoord 0) (yCoord 0)):
list(xCoord:yCoord xCoord+width:yCoord+height)
);end procedure<
复制代码
以上代码声明一个函数名为trBuildBox的函数，该函数有四个参数，height、width、xCoord、yCoord。
这四个参数中，xCoord和yCoord是可选参数，默认值都为0。当未给这两个参数传递值时，函数将使用其
默认的值。请参考以下调用函数的结果。
trBuildBox( 1 2 ) => ((0 0) (2 1)) ;指定第一个和第二个参数的值，xCoord和yCoord使用默认值0。
trBuildBox( 1 2 4 ) => ((4 0) (6 1)) ;指定第一个和第二个参数的值，xCoord值为4，yCoord使用默
认,0。
trBuildBox( 1 2 4 10) => ((4 10) (6 11)) ;同时指定四个参数的值。
@key
@key与@option类似，在为指定参数时，都使用默认值，但是在函数调用时@key必须要求指定确定的参数
。
procedure(trBuildBox(@key (height 0) (width 0) (xCoord 0) (yCoord 0))
list(xCoord:yCoord xCoord+width:yCoord+height)
);end procedure
复制代码
请参考以下函数调用的结果:
trBuildBBox() => ((0 0) (0 0))
trBuildBBox( ?height 10 ) => ((0 0) (0 10))
trBuildBBox( ?width 5 ?xCoord 10 ) => ((10 0) (15 0))