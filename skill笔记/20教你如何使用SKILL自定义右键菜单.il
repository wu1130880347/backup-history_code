在每个SKILL编程中，右键菜单是经常使用到的。下面就如何添加自定义右键菜单做一个简单介绍。8 h5 d1 O3 ~. |, d0 [0 S0 ^

需要用到的axl函数
     axlUIPopupDefine，用于定义右键菜单的各个选项；
     axlUIPopupSet，将定义好的菜单设置到右键中。
     
axlUIPopupDefine函数结构如下：
    axlUIPopupDefine(    r_popup
   ts_pairs
   )

第一个参数 r_popup 为之前存在的菜单handle ，如果创建新的菜单，那么这个参数为 nil

第二个为菜单的项目。注意使用list来定义。一般结构为((“菜单名”  需要调用的函数)( b9 d% z8 x7 y2 ?1 t9 |1 E

axlUIPopupDefine函数定义菜单成功后会返回一个菜单ID

axlUIPopupSet函数结构如下：
axlUIPopupSet(
r_popup
)
t/nil
其传递参数为定义菜单的ID，可通过获取axlUIPopupDefine函数的返回值来确定具体ID

下面是一个简单的菜单例子：
axlCmdRegister( "popuptest" 'popuptest ?cmdType "interactive")   
procedure(popuptest()
        notdone = t

        popTest = axlUIPopupDefine(nil (list
        (list "Button 1" 'poppBP1 )
        (list "Button 2" 'poppBP2 )
        (list "Button 3" 'poppBP3 )
        (list "Exit" 'poppBPE ))
        )
        axlUIPopupSet(popTest)
        while( notdone
        axlSingleSelectPoint()
        );end while
)        
procedure(poppBP1()
        axlUIConfirm("You have pressed Button 1")
        notdone = nil
);end procedure
procedure(poppBP2()
        axlUIConfirm("You have pressed Button 2")
        notdone = nil
);end procedure
procedure(poppBP3()
        axlUIConfirm("You have pressed Button 3")
        notdone = nil
);end procedure

procedure(poppBPE()
        axlUIConfirm("You have pressed Exit")
        notdone = nil
);end procedure
复制代码
当每次点击任何个菜单项时，会弹出相应的对话框。