读取一个文件，需要以下步骤：
使用infile获取一个文件的输入端口
使用gets函数一次读取文件的一行，或者使用fscanf函数转换文本输入。
使用close函数关闭文件输入端口

gets函数读取文件的下一行，gets函数的第一个参数变量为gets函数读取文件一行的值。gets函数返回一个字符串，当遇到文件结尾时，返回nil值。
请看下面例子：
inPort = infile( "myFile.txt" ) ;定义输入文件端口
when( inPort
while( gets( nextLine inPort )
println( nextLine )
)) ;输出文件中的每一行 =>"(\"Number:\" 1)\n" =>"(\"Number:\" 2)\n" =>"(\"Number:\" 3)\n" close( inPort ) ;关闭释放输入文件。

fscanf函数通过“格式控制”读取文件内容”。查找文件中的下一个数据，
格式：fscanf(inPort "%conversion_code" x)
fscanf函数主要输入参数如下：
输入端口
格式控制字符串
接收结果的变量
fscanf函数返回结果为相符数据项的数量。

一些常见格式的指令
格式规范	数据类型	Scans 输入端口
%d	整数	下一个整数
%f	浮点	下一个浮点
%s	文本字符串	下一个文本字符串
%e	E浮点	下一个E浮点
%g	G浮点	下一个G浮点


inPort = infile( "myFile.txt" ) ;定义输入文件端口
when( inPort while( fscanf( inPort "%s" word ) 
          println( word ))) ;输出文件中的每一字符串
=>"(\"Number:\""
=>"1)"
=>"(\"Number:\""
=>"2)"
=>"(\"Number:\""
=>"3)"
close( inPort ) ;关闭释放输入文件。