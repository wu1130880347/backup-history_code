在allegro环境下使用SKILL,必须要先载入后才能使用相关的命令，一种方式是直接在COMMAND窗口输入skill load("SKILL文件名")来载入，这种方法只针对当前开启的ALLEGRO才有效，另外一种方法是将载入语句写入PCBENV目录下的allegro.ilinit文件内来实现ALLEGRO启动时自动加载，这种是比较通用的方法，下面以此方法为例。

一，查看环境变量所在目录
1.通过查看电脑环境变量中的HOME变量值. 例如HOME变量为d:\ ，那实际的allegro.ilinit文件路径应该放在d:\pcbenv目录下
2.直接在ALLEGRO 命令行输入echo $localenv 也可以得到实际的环境变量目录。
在环境变量目录下如果没有allegro.ilinit文件，可以新建一个文本文件，然后修改文件名即可，allegro在启动时会在环境变量目录下查找allegro.ilinit文件，并执行其中的SKILL语句。可以把allegro.ilinit理解成一个ALLEGRO自动启动的SKILL文件。

二，allegro.ilinit文件的编写
大家经常见到的allegro.ilinit文件内容如下：
;--------------------------------------------------------
setSkillPath(buildString(append1(getSkillPath() "D:/skill"))) 
load("xxx.il")
;--------------------------------------------------------

以上代码都是标准的SKILL语句，下面介绍所使用到的函数:

getSkillPath() ;函数，作用是获取ALLEGRO的SKILL PATH列表，此列表所包含路径中的SKILL文件可直接使用相对路径调用。
append1 ;函数，在列表中添加内容，append1(getSkillPath() "D:/skill"))) 的作用就是将D:/SKILL这个目录添加进SKILL 路径里，这样只要123.il文件放在D:/skill目录下就可以直接使用load("123.il")来调用。
buildString ;将LIST类型组成字符串，此函数可以省略
setSkillPath ;函数，根据重新组成的路径设置SKILL PATH.
load ;载入函数,如果SKILL带有密码，写法为load("xxx.il" "password");另外也可以使用loadi函数。

使用以上代码后，可以将SKILL文件放入指定文件夹以方便管理，
另外，因为PCBENV目录默认包括在SKILL PATH中，所以如果不使用上面语句，也可以将SKILL文件放入PCBENV目录下,使用相对路径调用 ，如load("test.il");
如果在其它位置也可以使用绝对路径调用，例如load("d:/skill/test.il")

保存allegro.ilinit文件后, 在ALLEGRO命令行里执行skill load("allegro.ilinit") 所有加载的SKILL即可生效，当然也可以重开ALLEGRO.

三，在allegro.ilinit的中定义快捷键
使用axlSetAlias和axlSetFunckey函数

axlSetAlias("F2" "oops")
