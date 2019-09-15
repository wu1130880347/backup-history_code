1-8 用户习惯性设定

    用户习惯性设定一般在设置在env文件里。在进行User Perferences Editor 设定后，设定动作均会在env文件里出现相关的set/unset语句。关于User Perferences Editor 部分会在 Allegro-用户配置 版块详解。( |" f' T5 b$ }9 E
》Script 键鼠操作录制文件
     可先录制一段键鼠操作，与类似 office 里宏的类似。比如只单显某布的布线，供日后调用快捷切换层面。
     操作：菜单File -> Script（进入Scripting录制面板） -> 输入待录制文件名 -> 点 [Record] 开始录制
               -> 开始键鼠操作直至得到想要的效果
               -> 再进入Scripting录制面板 -> 点[Stop]按钮结束全程录制。
     录制结果被保存在 "xxx.scr" 文件中，可使用 replay 命令调用。

command>replay("xxx.scr")

》快捷键设定

    >alias 字符串别名
    在Allegro13-14版及其前生，只有 alias 字符串别名设定命令，可将长字符串以较短的字符表示，

比如：将 F2 键设定为布线命令"add connect"，
command>alias F2 "add connect"

    由于F1被系统默认为“help”功能无法进行再设定，一般键盘只能设定F2-F12，home，page up,page down, end, insert, delete, 四个方向键，还有与Ctrl,Alt,Shift相结合。虽可设定的键还蛮多，但都不易于左手单手操作。但传说中有一种专业的键盘，键盘左手边还多出两竖12个功能键，可设定到F13-F24。小生用过，感觉相当爽。

    当然在 Allegro 15 版之后有了 funckey 快捷键设定命令就用普通键盘更好用。

    >funckey 快捷键设定, 
先区分一下 alias 与 funckey 的区别! 
command>alias a "zoom in"  ;在命令行CIW中输入"a" 再按“enter”, 即可放大.
command>funckey b "zoom out" ;在设计工作区按“a”键，即可缩小

由此可见有了 funckey 与 alias 配合使用，可使整个键盘上的除Ctrl,Alt,Shift,win,popup,fn,caps...等特殊键外均可做快捷键设定。
command>alias ;可查看alias 与  funckey 的所有已设定

例：比如单显 top 布线层快捷设定在“1”键：
先录制单显 top 布线层的键鼠操作：先全关闭显于所有层，再勾先显示与 top 布线相关层面（pin via drc 零件框 CnsRgn区 等）
并将文件保存在工作目录下，如：“OnlyDisplayTop.scr"，最后有funckey 进行调用。
command>funckey 1 "replay(OnlyDisplayTop.scr)" 

笔者见意公司内统一设定几个快捷键，统一设定有助于各工程师位客串他人机器时容易上手