1-11 手工修改菜单_allegro.mem    

    日后会介绍如何设置动态菜单，在管理员安装机器时，先手工修改好菜单文件，然后复制公司内部统一菜单文件是必要的。或是如果只是一两处需要修改，使用手工菜单修改还是蛮方便的。) y* R, d+ L! u( i


先了解一下allegro 菜单格式如下

//注释信息								; //为注释语句
POPUP "主菜单名"                  		;显示在Allegro菜单栏上面
BEGIN   								;主菜单开始，在BEGIN 与END间定义菜单
  MENUITEM "菜单名", "执行的命令"		;菜单项
  ……
  MENUITEM SEPARATOR					;横线
  ……
  POPUP "二级菜单名"					;二级菜单，可嵌入二、三级菜单……
  BEGIN   								;二级菜单开始
   MENUITEM "菜单名", "执行的命令"		;三级菜单项
   ……									;
  END									;二级菜单结束
END   									;主菜单结束


    Allegro带有一个菜单配置文件allegro.mem在每次启动时会调用该文件加载菜单
可输入 echo $menupath命令查找菜单文件的位置

Command > echo $menupath
D:\SPB_Data16.5\pcbenv\cuimenus\.C:/Cadence/SPB_16.5/share/local/pcb/menus ;查找结果可能会有多个目录，第一个就是启动时调用位置，

用文本编辑器（如记事本、notepad++) 打开编缉D:\SPB_Data16.5\pcbenv\cuimenus\allegro.men， 

       通常大家会在help菜单后添加菜单，免乱了allegro已有菜单，那就在最后一行"END"前加代码，
来个实例吧，跟着糟菜一起学修改菜单:在help后添加“Mytools”，修改前请先备份菜单配置文件后再修改。
	// -----MyTools menu for Allegro Begin-----
POPUP "&MyTools"
BEGIN
    MENUITEM "Hello &world", "palHelloWord"
    MENUITEM "&Open Home","palOpenHome"
    POPUP "&Help"
        BEGIN
        MENUITEM "&About","echo 'MyTools 1.0'"
        MENUITEM SEPARATOR
        MENUITEM "&BBS", "http http://www.allegroskill.com/forum.php"
        MENUITEM "Autho&r","http http://wpa.qq.com/msgrd?V=1&uin=16831621"
        END
END
// -----MyTools menu for Allegro end-----

       笔者一般为能类似"// -----MyTools menu for Allegro Begin/end-----" 这样一对注释做自定义菜单的开始与结尾，方便日后再次修改时容易找到上次的修改处。

        由于您的不熟练操作，本操作就存在一定风险，如果你在修改菜单后，再次打开allegro编辑器时发现没有看到菜单，说明菜单修改有误。须认真查找格式不对之处。

        本文需与《 §1-10 加载 skill 程序_allegro.ilinit 》篇配合完成！