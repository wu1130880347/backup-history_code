开发Skill Function.
开发SKILL功能包括下列任务。
？分组几个SKILL语句成一个单一的SKILL声明
？声明的程序功能的SKILL功能
？定义函数的参数
？保持你的源代码
？加载你的技能的源代码
？重新定义SKILL功能

合成skill 语句
有时可以很方便的将几个skill语名合成一个单一的skill来声明，使用大括号｛｝来集合skill语句至一个单一的skill声明中。单一语句的返回值是该组中的最后一个Skill语句的返回值。您可以将这个返回值保存至一个变量中。
这个例子计算BBOX的像素高度，
bBoxHeight = {
      bBox = list( 100:150 250:400)
      ll = car( bBox )
      ur = cadr( bBox )
      lly = yCoord( ll )
      ury = yCoord( ur )
      ury - lly }


.声明skill 语句

  要通过引用函数名，使用声明一个过程与函数体关联，该函数体和函数名构成了一个SKILL功能。
procedure( ComputeBBoxHeight( )
      bBox = list( 100:150 250:400)
      ll = car( bBox )
      ur = cadr( bBox )
      lly = yCoord( ll )
      ury = yCoord( ur )
      ury - lly }
      ) ; procedure
bBoxHeight = ComputeBBoxHeight()
;注，要执行的函数体，其函数名后紧跟().


.定义带参数函数

  为了使您的函数更回灵活，可以在函数体加形式参数识别某些变量。
procedure( ComputeBBoxHeight( bBox )
      ll       = car( bBox )
      ur       = cadr( bBox )
      lly      = yCoord( ll )
      ury      = yCoord( ur )
      ury - lly 
      ) ; procedure
bBox = list( 100:150 250:400)
bBoxHeight = ComputeBBoxHeight( bBox )
;要执行你的函数，你必须提供的参数值。


.为您的功能定义前缀

   本手册中的很多例子都使用"tr"的前缀，表示他们正在以练习(try)为目的创建的
  例如用于技术文件管理功能都带有 "tc"前缀。这些前缀跨越Cadence的工具各不相同
Cadence 都使用小写字母为前缀，建议您在建立函数名时使用大写字母表明自已独特的前缀。

.维护SKILL源代码
   Cadence的环境，可以很容易地调用你选择的编辑器。设置SKILL变量编辑器为UNIX命令行可以启动你的编辑器。

editor = "xterm -e vi"
    该编辑功能调用您选择的编辑器。如果您选择使用的EDL功能，系统加载文件，当您退出编辑器。

ED（“myFile.il”）
另外，您也可以使用编辑器独立于Cadence的环境。

.加载你的skill 源码

这个加载功能
评估在源码中的每条语句
通常用于定义函数集
返回t,表明所有表达式计算没有错误
如果有任何错误就中止之后的表达式计算。

.设定相对路径

当你使用相对路径加载skill时，系统使用list设定skill 路径。
setSkillPath 设置skill路径list
getSkillPath 搜索目录list
prependInstallPath 获取安装目录

trSamplesPath = list(
            prependInstallPath( "etc/context" )
            prependInstallPath( "local" )
            prependInstallPath( "samples/local" )
            )
setSkillPath( append( trSamplesPath getSkillPath() ) )


.输入Function

有时，你需要定义一个函数，在编辑器中输入命令。

.重新定义 Skill 函数

   用户应保护，，避免在不经意间重新定义的函数。使用writeProtect关闭虚拟内存定义以防止在运行间被重定义。, m( B, w# I: ^( |

sstatus( writeProtect t )        ;sets it to t
sstatus( writeProtect nil )      ;sets it to nil

这个例子试图重新定义trReciprocal防止除以0。
sstatus( writeProtect t ) => t
procedure( trReciprocal( x ) 1.0 / x ) => trReciprocal
procedure( trReciprocal( x ) when( x != 0.0 1.0 / x ))
*Error* def: function name is write protected
      and cannot be redefined - trReciprocal