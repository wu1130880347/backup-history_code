实现目标：在help后面添加一个菜单，放自己的函数。
实现方式：帖子所述
目前进程：完成1，2。3，4调试不出。

SKILL看了一些关于加载自己菜单的方式，这里总结下思路：（我用的是16.3）
方式一：在.men文件下手动进行修改。
    这个群里有很多，本人已验证，这里略。
方式二：通过axlUIMenuDump() axlUIMenuLoad()加载
    这种方式是将当前的菜单通过axlUIMenuDump()函数读取，存放在一个临时文件中，文件操作，将自己的菜单添加到这个临时文件中，然后通过axlUIMenuLoad()函数，加载这个临时文件。
    这种方式其实感觉更第一种方式差不多。本人已验证，可行。
方式三：通过axlUIMenuFind()axlUIMenuInsert()加载
    这种方式通过axlUIMenuFind找到自己想要添加的菜单位置，axlUIMenuInsert()添加菜单。
    我暂时出不来，不知道哪里出问题  
方式四：通过axlUIMenuRegister()加载
    这种方式通过axlUIMenuRegister()直接进行添加。
    我暂时出不来，不知道哪里出问题

调试进程描述：方式三与方式四都是编译成功的，但是执行就是挑不出菜单，@deargds版主能帮我找下原因吗？
menu.il文件我都看了，就是出不来。

方式三代码：
SKILLToolsMenu = '(
     (popup "SKILL Tools")
     ("test"                        "TEST")
)
defun(MikecMenuTrig ()
        let( (q)
        q = axlUIMenuFind(nil -1)
        res = axlUIMenuInsert(q SKILLToolsMenu)
        )
)
axlTriggerSet('menu  'MikecMenuTrig)

方式四代码：
defun(MikecMenuTrig ()
        let( (q)
        q = axlUIMenuRegister( -1 SKILLToolsMenu )
        )
)