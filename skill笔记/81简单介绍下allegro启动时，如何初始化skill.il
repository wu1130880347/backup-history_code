allegro在每次启动时，首先去目录里面找到ENV文件（ENV文件主要用于存放allegro的一些配置信息、快捷键等。。。），然后再读取AXL初始化文件，即 allegro.ilinit 文件。一般我们使用load函数添加需要l加载SKILL文件，那么每次启动allegro时，都会加载一次allegro.ilinit中指定的SKILL文件。
allegro会在以下的目录里面查找 allegro.ilinit 文件：
1. <cdsroot>/share/pcb/etc/skill* ]" N! Z8 a- ?5 f& }9 N
2. <cdssite>/pcb/skill: f# j3 F* W7 J" s% r+ e$ E
3. $HOME/pcbenv! H  z8 `- O6 {# ^
注：& N0 R. z# F4 L
<cdsroot>为软件的安装目录，通过环境变量获取。比如现在将Cadence 16.3安装在D盘根目录下，那么<cdsroot>路径为：D:\Cadence\SPB_16.3
<cdssite>默认路径为：<cdsroot>/share/local/pcb/skill ，我们也可以通过添加一个"CDS_SITE"环境变量，并通过其变量值来指定其他目录。若以上文件夹不存在，可手动创建一个。$ ~* j% o# I% l* \
; W' w4 ?! J! [5 e6 t2 R/ H
假如以上文件夹中存在多个 allegro.ilinit 文件，其每个 allegro.ilinit 文件都会被读取。假如你想allegro只读取找到的第一个allegro.ilinit/ T/ C4 _* g" k  V* J4 c
文件，可通过点击allegro菜单 ：Setup -> User Preferences... 找到SKILL，将其中的 skill_old_ilinit打上勾。或者直接在ENV文件中添加如下代码：
set skill_old_ilinit