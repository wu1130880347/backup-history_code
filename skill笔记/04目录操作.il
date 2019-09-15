绝对路径：
    是指目录下的绝对位置，直接到的目标位置。Unix中以"/"开始的路径，Windows中以磁盘"C:,D:,..."开始的路径，网络邻居的电脑以 "\\" 开始。

相对路径
    不是绝对路径的情况就是相对路径 ，相对路径就是指由这个文件所在的路径引起的跟其它文件（或文件夹）的路径关系。
    使用相对路径可以为我们带来非常多的便利。
    以"~/"开始的路径表示被查询的路径是用户的home文件夹
    以"~username"开始的路径，如果username正好是一个用户的名字，那么被查询的路径将是该用户的home文件夹;
    以"./"开始的路径表示被查询的位置是当前的工作目录. 
    以"../"开始的路径表示被查询的位置为当前工作目录的父文件夹

常用路径
       Allegro 安装时会带有两个目录：安装目录(cdsroot)与工作目录(home)
       先了解一下你机器上的allegro的几个比较常用到的目录。
      Allegro命令：Echo 显示各种预设目录
      下表以笔者本机allegro 16.5为例，帮您展现各文件与目录所在：

  目录名称         查看命令              例3     			不要在SKILL中输入
安装目录       echo $cdsroot        C:\Cadence\SPB_16.5
工作目录       echo $home           D:/SPB_Data16.56 \0
pcbenv目录     echo $localenv       D:/SPB_Data16.5/pcbenv- 
菜单目录       echo  $menupath      D:\SPB_Data16.5\pcbenv\cuimenus\"

一般我们会在SKILL初始化的时候通过setSkillPath 和 getSkillPath 建立目录。
设置skill查询路径的函数setSkillPath 
setSkillPath("." "~" "C:/Skill")，

获取skill查询路径列表的函数getSkillPath，如   需要在skill中查找
setSkillPath("." "~" "C:/Skill")； 
getSkillPath()=>("." "~" "C:/Skill")

就设置了3个skill查询路径，先后顺序分别为"."－当前工作路径，"~"－home文件夹，"C:/Skill"－用户自定义文件夹。
实例：设置一个新路径到目录列表中
setSkillPath(append(trSamplesPath getSkillPath()))   ;trSamplesPath改为自己想添加的目录。然后将代码直接放在allegro.ilinit 中。之后在自己添加的目录就可以使用相对路径加载。


    #endif




