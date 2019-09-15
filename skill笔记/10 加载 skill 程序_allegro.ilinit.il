1-10 加载 skill 程序_allegro.ilinit

        想加载Skill 程序，总得有份 xxx.il 文档，网上神人也很多，听说 Skill 语言已被开发很深了，再开发那就得走创新用法与内部需求路线。去各大论坛找找还是有收获得，本论坛可能会比较无耻，将会收集各大论坛的帖子与附件，帖子整理一下放在“采集整理”版块，Skill 程序去重一下放在“软件库”版块，方便大家恶心自己。


. Skill 函数

> axlCmdRegister(); register Skill函数为allegro的命令
如果之前命令已被注册，那么将会用新的命令替换之前的。

axlCmdRegister(t_allegroCmd  ts_callback  ?cmdType t_cmdType  ?doneCmd ts_doneCmd  ?cancelCmd s_cancelCmd )
? t/nil

t_allegroCmd：该处为在allegro使用的命令
ts_callback：该处为调用SKILL中的哪个函数
?cmdType：命令类型，共分为两类。"interactive"，为默认类型；"general" 当命令调用时会立即执行命令，会立即停止allegro正在执行的命令。一般用于含有有弹出窗口的情况下。可选项
?doneCmd：当执行DONE命令后调用的Skill函数，可选项
?cancelCmd：当执行CANCEL命令后调用的Skill函数。可选项。

当返回值为t时说明命令注册成功，否则失败，请检查各个参数是否正确。

下面是一个简单的例子：
axlCmdRegister( "pal_via_net" 'ch_via_net ?cmdType "interactive" ?doneCmd 'viadone ?cancelCmd 'viacancel)

   可使用该函数为Skill注册一个allegro 命令，如果之前命令已被注册，那么将会用新的命令替换之前的。因此笔者见意使用一个统一的个性前缀，如本节中使用的"pal"。自成一个命令系列必避allegro自带或他人命令冲突。

. 执行外部程序命令 system sh
    system  跳出Allegro 直接执行 cmd 命令
    sh         等待执行 cmd 的命令结束后，返回至程序继继下一条语句。

.. 跟学写两个Skill 小程序
      先跟着笔者自己动手写一个所有计算机语言开始的学习的第一个程序 “Hello Word” 小Skill 程序吧！
新建文本文档 d:\MyTools\HelloWorld.il skill 源码如下
;神奇的Hello World
axlCmdRegister( "palHelloWorld" 'palHelloWorld)
defun(palHelloWorld ()
axlUIConfirm("Hello World!")
axlUIConfirm(strcat("This is my first Skill Program _" axlGetVariable("username")))
)

再来写个新建文本文档 d:\MyTools\OpenHome.il skill 源码如下
;选定当前工作图档
axlCmdRegister( "palOpenHome" 'palOpenHome)
defun(palOpenHome()
FileName=buildString(parseString( axlGetDrawingName() "/") "\\")
expBoard=strcat("explorer.exe/select,/n,/e," FileName)
axlShell(strcat("system " expBoard))
)

        热心的朋友看这到可能会提意：“可以在语句后添加适当的注释”，哈，这里仅是加载 skill 程序。不过还是要简介几条常见的，- l  X9 n6 I* t/ s. a

.. 加载skill
.  skill加密
    加密SKILL程序，即是给SKILL程序加上密码，使用命令格式：
格式： encrypt(inputFileoutputFile passwd)   ;其中passwd可以省略：
skill>encrypt("test.il" "test.ile" "pass")skill>load("test.ile" "pass") 

好像cadence没有发布加密后的skill如何反编译。但如果加密时没有使用密码，还是可以使用compress命令进行反编译的，只是换行格式没了。不过好像很少见有没有密码的加密档哦。

.  源码压缩

压缩文件即可以减少文件大小
格式：compress(inputFileoutputFile)

该命令主要会去除标识符如：空格，缩进等，去除注解等。命令产生文件为ASCII格式，所以也可以对加密文件进行压缩。

如果你的源码打算加密，那见意就先压一下，再加密吧

skill> load("D:/welcome.il") ;加载skill> compress("D:/welcome.il"  "D:/welcome.ilc")  ;压缩文件% g, Q" k- A% v7 E! U# F
skill> encrypt("D:/welcome.ilc" "D:/welcome.ile" "www.allegroskill.com");加密文件
skill> load("D:/welcome.ile" "www.allegroskill.com") ;加载有密文的skill文件

. 跨平台函数axlShell 与命令skill
command>skill skill函数
skill>axlShell ("command命令")

. 加载 skill 程序
大家都使用 load("") 这程C语言格式调用skill，其它还可以使用Lisp 语言格式调用 (load "")例：在allegro.ilinit 加入刚刚创建的两个skill 小源码

setSkillPath( append( "d:/MyTools" getSkillPath() ) )
(load "OpenHome.il")
(load "HelloWorld.il ")
注意例子中的斜杠方向。Allegro的前身是从工作站LUNIX 系统移至windows 系统的