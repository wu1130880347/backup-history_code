调用SKILL函数
你可以通过 键盘、窗体、菜单、命令解释器、skill程序等方式去执行skill代码来实现功能。
Bindkeys	一个Bindkeys关联键盘事件，发送Skill功能
Forms	有些功能需要您填写字段，在弹出的表单中提供数据。
Menus	当您在菜单中选择一个项目时，系统会发送相关的Skill功能。
CIW	你可以直接输入Skill功能到CIW,进行立即评估。
Skill process	您可以启动一个单独的UNIX的过程 ，可以直接提交skill功能的解释。
Skill Interpreter	您可能通过回载skill源码文件，对提交的skill功能进行集中评估。



您可以通过加载SKILL源代码文件提交的SKILL功能的集合进行评估。

CIW命令解释器窗口
你可以在allegro PCB编辑器的命令行里直接运行AXL-SKILL函数，语法如下：
skill (<function><arguments>)