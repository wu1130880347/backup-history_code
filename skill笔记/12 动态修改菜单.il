《Allegro Skill 跟着学》§1-12 动态修改菜单 本文需与《 §1-10 加载 skill 程序_allegro.ilinit 》篇配合完成！

与手工修改菜单一样添加下图示例菜单。
方法一：适用于逐个添加菜单的程序下面是菜单源文件
;在allegro.inilit中添加(load "D:/mySkill/UserMenu.il")
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

方法二：将菜单建成list 放置于参数文件中，给程序调用
下面是菜单源文件
;在allegro.inilit中添加(load"D:/mySkill/UserMenu.il")
defun(loadMyMenu (t_menuName)
      SKILLToolsMenu = '(
      (popup "MyTools")
       ("Hello&world","palHelloWorld")
       ("&OpenHome","palOpenHome")
       (popup "&Help")
       ("About","echo'MyTools 1.0'")
       (separator)   
       ("BBS","httphttp://www.allegroskill.com/forum.php?")
       ("Autho&r","httphttp://wpa.qq.com/msgrd?V=1&uin=16831621")
        (end)  
      (end)
      )   
     let( (palMenu)
      palMenu = axlUIMenuFind(nil -1)
      axlUIMenuInsert(palMenu SKILLToolsMenu)
     )
)
axlTriggerSet('menu  'loadMyMenu)

注释一下两种方法吧
菜单源码一注释：
;在allegro.inilit中添加(load "D:/mySkill/UserMenu1.il")
defun(loadMyMenu (t_menuName)  ;定义菜单
palMenu = axlUIMenuFind( nil -1) ;定位菜单位置于Allegro主菜单尾部，即"Help"后。
palMenu1 = axlUIMenuInsert(palMenu 'popup "MyTools") ;添加自定义主菜单”MyTools"
palMenu2 = axlUIMenuInsert(palMenu1 "Hello &world","palHelloWorld") ;添加二级子菜单“Hello &world”
palMenu2 = axlUIMenuInsert(palMenu1 "&Open Home","palOpenHome") ;添加二级子菜单“Open Home"
axlUIMenuInsert(palMenu2 'end ) ;添加二级子菜单结果，本语句可忽略
palMenu2 = axlUIMenuInsert(palMenu1 'popup "&Help") ;i添加带有子菜单的二级子菜单 "&Help"
axlUIMenuInsert(palMenu2 "About","echo 'MyTools 1.0'") ;添加三级子菜单 "About"，示例内部命令的直接使用。
axlUIMenuInsert(plMenu2 'separator ) ;添加菜单分组横线
axlUIMenuInsert(palMenu2 "BBS","http http://www.allegroskill.com/forum.php?") ;添加三级子菜单 "BBS"，示例如何访问网页。
axlUIMenuInsert(palMenu2 "Autho&r","http http://wpa.qq.com/msgrd?V=1&uin=16831621") ;添加三级子菜单 "BBS"，示例如何与作者取得联系。
axlUIMenuInsert(palMenu2 'end ) ;添加二级子菜单结果，本语句可忽略
axlUIMenuInsert(palMenu1 'end ) ;添加主菜单结果，本语句可忽略
);defun
axlTriggerSet('menu 'loadMyMenu) ;某个事件触发相应函数，本命令是自动加载菜单的关键！


菜单源码二注释
;在allegro.inilit中添加(load "D:/mySkill/UserMenu2.il")
defun(loadMyMenu (t_menuName)  ;定义菜单
      SKILLToolsMenu = '(  ;定义一个list,
      (popup "MyTools")  ;添加自定义主菜单”MyTools"
       ("Hello &world","palHelloWorld")  ;添加二级子菜单“Hello &world”
       ("&Open Home","palOpenHome") ;添加二级子菜单“Open Home"
       (popup "&Help") ;i添加带有子菜单的二级子菜单 "&Help"
       ("About","echo 'MyTools 1.0'") ;添加三级子菜单 "About"，示例内部命令的直接使用。
       (separator)    ;添加菜单分组横线
       ("BBS","http http://www.allegroskill.com/forum.php?") ;添加三级子菜单 "BBS"，示例如何访问网页。
       ("Autho&r","http http://wpa.qq.com/msgrd?V=1&uin=16831621") ;添加三级子菜单 "BBS"，示例如何与作者取得联系。
        (end) ;添加二级子菜单结果
      (end) ;添加主菜单结果
      )
     let( (palMenu) ;定义局部变量
      palMenu = axlUIMenuFind(nil -1) ;定位菜单位置于Allegro主菜单尾部，即"Help"后。
      axlUIMenuInsert(palMenu SKILLToolsMenu)
     )
)
axlTriggerSet('menu  'loadMyMenu) ;某个事件触发相应函数，本命令是自动加载菜单的关键！