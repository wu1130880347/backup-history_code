局部变量的定义,使用let或prog函数，
Let的运行速度要快于prog，当函数中不使用go或者return时，建议最好使用let。
转义字符
其中的两个反斜杠代表的就是输出一个反斜杠
Skill > str="C:\\Cadence\\SPB_16.3\\doc\\skhelp"=>"C:\\Cadence\\SPB_16.3\\doc\\skhelp"
Skill > printf str
=>C:\Cadence\SPB_16.3\doc\skhelp
=>t
Skill > axlGetVariable("cdsroot")
"d:\\Cadence\\SPB_16.3"
Skill >

字符串合成：strcat( s_string1 [s_string2] …)=> t_result
strcat( 'ab "xyz" )        => "abxyz"  ；合成所有提供的字符串生成一个新的字符串


