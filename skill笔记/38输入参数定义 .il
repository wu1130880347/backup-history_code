
                                   （输入参数定义）
输入参数有2种必然情况和3种可选情况。
1.以下是2种必然情况：
(1).不含参数的函数
如 procedure( hello() println( “hello, world!”) ) 运行它 hello=>”hello,world!”
(2).含参数的函数! 
如 procedure( hello( arg ) printf(“hello, how are you %s” arg) )
运行它 hello=>函数调用错误，必须提供一个参数 hello(“Emily”)=>”hello,how are you Emily”

以下是3种可选的情况：
可选,有3种可选的情况，@rest，@optional，@key
(1).@rest
@rest选项允许任意数量的参数以列表的形式传入函数，跟在@rest后面的参数的名字是任意的，请参考下例
procedure( trTrace( fun @rest args ) let( ( result ) printf( "\nCalling %s passing %L" fun args ) result = apply( fun args ) printf( "\nReturning from %s with %L\n" fun result ) result ) ; let ) ; procedure 调用函数 trTrace( 'plus 1 2 3 ) => 6，其实是运行了apply( plus 1 2 3 )。
(2).@optional6 P5 }, V( i% |6 a" A! g
@optional选项允许设置一些有默认值的输入参数，如果调用函数的过程中提供了这些参数的值，新值将替换默认值。
procedure( trBuildBBox( height width @optional ( xCoord 0 ) ( yCoord 0 ) ) list( xCoord:yCoord ;;; lower left xCoord+width:yCoord+height ;;; upper right ) ) ; procedure 在这个示例中xCoord和yCoord的默认值都是0。运行* w7 ]) {, S! h6 k: F, R
trBuildBBox( 1 2 ) => ((0 0) (2 1)) ; xCoord，yCoord值保持0 trBuildBBox( 1 2 4 ) => ((4 0) (6 1)) ; xCoord值改为4，yCoord值保持0 trBuildBBox( 1 2 4 10) => ((4 10) (6 11)) ; xCoord值改为4，yCoord值改为10
(3).@key
@key作用和@optional相同，只是@key中的参数可以以任意的顺序调用。
procedure( trBuildBBox(
@key ( height 0 ) ( width 0 ) ( xCoord 0 ) ( yCoord 0 ) )
list( 
xCoord:yCoord ;;; lower left 
xCoord+width:yCoord+height ;;; upper right
) 
) ; procedure 
运行
trBuildBBox() => ((0 0) (0 0)) ; 什么默认值都不改变
trBuildBBox( ?height 10 ) => ((0 0) (0 10)) ;只改变height的值 
trBuildBBox( ?width 5 ?xCoord 10 ) => ((10 0) (15 0));改变width和xCoord