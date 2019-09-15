Skill List是Skill基于Lisp(List Processing)语言的表现，它是skill数据对象的一个有序集合，skill数据甚至程序本身都可以看作是一个list，这是C语言中所没有的概念。
        Skill中的list是一个收集的SKILL数据的对象，和数组有点类似，可以把有多种类型的常量组成的列表。理解为一个数据结构表，但是不会指定list的大小。在SKILL编程中，list会经常使用到。   
       Skill 分板器规定极限6000个元素
         list中的元素可以是任意数据类型。一个list可以包含任意多个对象（包括空）。比如，一个list可以包含变量及其他list。list必须在一对括号中。

下面举几个简单的list例子：

(a b c)   ;这是一个list，包含三个字符 a 、b、c
(1)         ;一个list，包含单个元素 1
( )         ；空list
(1 (2 3) 4);这是一个list，该list的第二个元素包含另外一个list
'( 1 2 a b c "PCB" ) ;List中包含的数据有整数、字符、字符串
'( 1 ( 2 a a ) b )    ;在List中也可以包含List类型的数据