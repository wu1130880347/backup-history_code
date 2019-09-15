Cadence Allegro SKILL语言讨论和交流（三）
好久没在EDA365论坛发帖子了， 可是我一直关注着大家，正如大家对我那两个不怎么样的老贴子的关注， 谢谢大家，下面是我奉献给大家的一碟小炒肉， 请大家享用，后面针对Allegro，我将按照第一个主题和大家展开讨论， 或许这些你知道， 但真的是这样吗？请先看看下面的几点：

1、Allegro Automation应该掌握的知识，你知道吗？
        ENV File
        Keyboard Shortcuts
                Single key or combinations followed by <Enter>
                Less used functions or windows (C+<Enter> for Color and Visibility Palette popup window)
        Function Keys
                Instantaneous commands
                Typically for most used functions (Toggle, Next, Oops, etc.)
        Scripts
                Record a linear series of repetitive routines
                Play back later
                Playback using keyboard shortcut or function key
        Menu alterations
        Extracta Command
        Strokes
        SKILL

2、Sub-Drawing文件的内容就是SKILL脚本，你知道吗？ 
        /*在"REF DES/SILKSCREEN_TOP"层创建了一条线，归属于'line _clp_sym对象*/
        _clpDBCreateLine(
                list(_clpAdjustPt(0:0 _clp_cinfo)  _clpAdjustPt(20:20 _clp_cinfo) ) 
                _clpMKSConvert(15.000000 _clp_cinfo->t_from_units _clp_cinfo->t_to_units) 
                "REF DES/SILKSCREEN_TOP" 
                'line _clp_sym
        )

3、经常要查看Brd所在的文件夹，如何才能快速找到？
        奉献一个快捷键，看到的人或许会用到：
        alias ~E 'shell explorer .'

4、一键切换PCB光标大小显示的快捷键？
        很像格点显示的切换，其实很简单，你对Allegro了解有多少
        alias ~1 'settoggle pcb_cursor infinite cross'

5、修改了ENV文件，如何在不启动Allegro的情况下让更改生效？
        看看你对Allegro的命令和变量了解多少
        source -q $localenv/env