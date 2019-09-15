文件名              功能                     全文件名
allegro.men     Allegro菜单配置     %localenv%\menus\allegro.mem或：%cdsroot%\share\local\pcb\menus\allegro.men
env             Allegro用户设置     %localenv%\env
allegro.ilinit  加载skill文件       %localenv%\allegro.ilinit

还是先获取个人设置目录 pcbenv 的路径在哪，以笔者机器为例：
command>echo $localenv=>D:/SPB_Data16.5/pcbenv
    依上表所示，笔者16.5版的三个文件为：
文件名              功能                     全文件名
allegro.men     Allegro菜单配置     D:\SPB_Data16.5\pcbenv\cuimenus\allegro.mem
env             Allegro用户设置     D:\SPB_Data16.5\pcbenv\env
allegro.ilinit  加载skill文件       D:\SPB_Data16.5\pcbenv\allegro.ilinit
注:如果您没有在用户设置目录中找到allegro.ilinit,可使用记事本创建一个空文件，*.ilinit文件与 *.il 格式是一样的，同是skill源码文件
注：Allegro 14版的菜单文件为 adlayout.men，Allegro 15版以上为 allegro.mem

  当你启动Allegro PCB编辑器时，会读取allegro PCB编辑器的env文件，然后运行allegro.ilinit文件。最后自动加载菜单配置文件allegro.men，
注：不能在env文件中插入skill命令。通常会在$home/pcbenv/allegro.ilinit文件中插入加载 skill 文件的语句。
