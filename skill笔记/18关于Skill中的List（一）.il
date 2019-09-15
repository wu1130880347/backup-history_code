Skill中的list是一个收集的SKILL数据的对象，和数组有点类似，但是list不会指定list的大小。在SKILL编程中，list会经常使用到。

list中的元素可以是任意数据类型。一个list可以包含任意多个对象（包括空）。比如，一个list可以包含变量及其他list。list必须在一对括号中。

下面举几个简单的list例子：
(a b c) ;这是一个list，包含三个字符 a 、b、c,
(1);一个list，包含单个元素 1
( )；空list1
(1 (2 3) 4);这是一个list，该list的第二个元素包含另外一个list

下面介绍如何创建list：
1.使用单引号逐个指定元素创建list。
2.使用list函数指定参数来传递元素的值
3.使用cons函数向已经存在的list中添加一个元素
4.使用append函数合并两个list

append与cons之间区别：
两个函数都将创建一个新的list。必须将新创建的list存放在一个变量中，使用该变量调用新创建的list。
但是cons仅需要第二个参数为一个list，append则要求两个参数都为list。
当list包含元素比较多时，append执行会比cons慢很多。



以下是创建list的几个例子:

通过给定每个元素值来创建list ("=>"为返回值)
‘(1 2 3) =>(1 2 3)
a=1 =>1
b=2 =>2
list(a b 3 ) =>(1 2 3)

使用cons函数在一个list的最前面添加新的元素
result = '( 2 3 ) => ( 2 3 )* T  A, A
result = cons( 1 result ) => ( 1 2 3 )

使用append函数合并两个list
oneList = '( 4 5 6 ) => ( 4 5 6 )
aList = '( 1 2 3 ) => ( 1 2 3 )5 l! C! x% e( P, ^5 D# _
bList = append( oneList aList) => ( 4 5 6 1 2 3 )