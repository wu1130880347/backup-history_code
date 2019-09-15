显示数据可通过以下两种方式
1）  使用print和println函数
2）  使用printf函数
print 和 println函数
Skill 编译器对每种不同数据都有默认的显示格式。print和println函数将使用默认格式显示数据。
以下是显示格式样本
数据类型	              默认格式例子
integer（整型）	             5
floating point（浮点型）	1.3
text string（字符型）	  “Mary learned SKILL”
variable(变量)	          bBox
list（列表）	          (1 2 3)

注：print 和println函数只能显示单个数据值。println 功能与print一样，但在输出结果后面会自动换行。
for( i 1 3 print( “hello” )) ;输出三次“hello”.
=>
“hello”"hello”"hello”
for( i 1 3 println( “hello” )) ;输出三次“hello”.
=>
“hello”
“hello”
“hello”

printf函数
格式：printf(格式控制,输出表列)
# Q, n2 \& w/ p  I  B2 ^
printf函数通过格式控制输出结果:

printf(
      "\n%-15s %-15s %-10d %-10d%-10d %-10d"
      layerName purpose rectCountlabelCount lineCount miscCount
)

第一个参数是转换控制字符串，包含各种指令：
%[-][width][.precision]conversion_code
[-] = 左对齐
[width] =最小字符数
[.precision] = 需要输出的字符数
d – 整数
f – 浮点数
s –字符串 or symbol
c – 字节
n – 数字
L –列表（忽略宽度和精度方面，数据的默认形式输出。）
P –点列表（忽略宽度和精度方面，数据的默认形式输出。）
B – 边框列表（忽略宽度和精度方面，数据的默认形式输出。）
%L指令指定默认格式，该指令可方便的方式散布应用程序特定的默认格式。printf函数返回值为t

aList = '(1 2 3)
printf( "\nThis is a list:%L" aList ) => t 
This is a list: (1 2 3) 
aList = nil 
printf( "\nThis is a list:%L" aList ) => t 
This is a list: nil

如果转换控制指令是不恰当的数据项，printf输出结果显示错误。

printf( "%d %d" 5 nil )
Message: *Error*fprintf/sprintf: 
      format spec.incompatible with data - nil
示例：
printf( "hello, %s" "Willy" )=> "hello, Willy"
printf("folat num is%0.2f" 10.2)=>"float num is 10.20。
;保留小数点后两位                       
printf( "%d == %x" 20 20 )=> "20 == 14
;把一个十进制书转换成16
sprintf( sMsg "hello, %s" "You" )=> sMsg="hello, You"
fprintf( port "1st line, %dwords" 10 )
; 输出"1st line, 10 words"到文件
printf( "file is \"%s\"" "c:\\text.il"); 输出file is"c:\text.il"，这里带了双引号
