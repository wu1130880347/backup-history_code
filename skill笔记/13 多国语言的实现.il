以上篇中提到的动态修改菜单方式一为例，实现多国语言的菜单
先请看下面的l中文转义字符例子：
printf("Cadence\n\tskill\n\t\t\270\372\316\322\321\247")
Cadence
   skill
      跟我学
t

其中Cadence\n\tskill\n每输出一行换行后再添加相应的水平制表符，再输出另外的内容。
最后一行"\270\372\316\322\321\247"代表的是八进制的ASCII码，该部分ASCII码为汉子"跟着学"的八进制代码。

可通过转码软件获取汉字的转义字符
Skill > axlUIPrompt("Code:",sprintf(nil,"%L",axlUIPrompt("Please Input Chinese:")))

   因为这里可能会用到较多的变量，所以再次重申一下变量的定义。
    Skill的变量不需要事先声明，Skill第一次用到是会自动生成变量。变量可以由字符、数字、"_" 和 "?" 组成，注意第一个字符不能是数字和"?"。由于Cadence所开发的Skill中的变量、函数都是第一个字母小写，以_为开头的是Cadence的专用函数，为了避免冲突，建议函数和变量命名都以大写字母开头。有时code package前头使用三个小写字母更清晰些。

;在allegro.inilit中添加(load "D:/mySkill/UserMenu3.il"）

;把所有菜单项名称均用一个变量来表示。
strMytools = "MyTools" ;我的工具
strHelloWorld = "Hello &world" ;第一个程序
strOpenHome = "&Open Home" ;打开工作目录
strHelp = "&Help" ;使用说明
strAbout = "About" ;关于
strBBS = "BBS" ;跟着学论坛
strAuthor = "Autho&r" ;联系作者

defun(loadMyMenu (t_menuName)
palMenu = axlUIMenuFind( nil -1)
palMenu1 = axlUIMenuInsert(palMenu 'popup "MyTools")
palMenu2 = axlUIMenuInsert(palMenu1 "Hello &world","palHelloWorld")
palMenu2 = axlUIMenuInsert(palMenu1 "&Open Home","palOpenHome")
axlUIMenuInsert(palMenu2'end )
palMenu2 = axlUIMenuInsert(palMenu1 'popup "Help")
axlUIMenuInsert(palMenu2"About","echo 'MyTools1.0'")
axlUIMenuInsert(palMenu2"BBS","httphttp://www.allegroskill.com/forum.php?")
axlUIMenuInsert(palMenu2"Autho&r","httphttp://wpa.qq.com/msgrd?V=1&uin=16831621")
axlUIMenuInsert(plMenu2'separator )
axlUIMenuInsert(palMenu2'end )
axlUIMenuInsert(palMenu1'end )
);defun
axlTriggerSet('menu 'loadMyMenu)
将语言配置放置另一个文件内， 如 "D:/mySkill/Language.ini"
修改LanguageType的值"Traditional Chinese"就可在下次重启allegro时动态切换菜单语言，这里示例 英文，简体中文，繁体中文，你还可以按自已的需求自定义其它国家的语种。