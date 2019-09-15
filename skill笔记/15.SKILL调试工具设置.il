SKILL调试工具设置
1.首先从附件下载文件
2.进入到%CDSROOT%\share\local\pcb\skill目录（注：%CDSROOT%变量为Cadence的安装目录），将下载的skill_debuger.il文件存放到其中，并在改文件夹下找到allegro.ilinit文件，如果没有，则在该目录下新建一个文本文档，文件名修改为allegro.ilinit。
3.打开allegro.ilinit文件，在其中添加如下代码：
load("skill_debuger.il" "www.allegro-skill.com")
4.进入到CDSROOT%share/pcb/text/cuimenus文件夹,拉到最后，找到allegro.men并打开 POPUP "&Help"并在其前面添加如下代码：
POPUP "My Skill"
BEGIN
        MENUITEM "Skill Debuger", "skill_debuger"
END

