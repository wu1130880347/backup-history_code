大纲：
    （一）、SKILL基础
    （二）、利用AXL接口操作Allegro PCB对象
    （三）、注册和使用自定义Allegro命令
    （四）、Allegro UI界面设计和响应

Email: TrizDev@yahoo.cn   (以前的邮箱丢了)

三、注册和使用自定义Allegro命令
1、注册Allegro命令
    axlCmdRegister("CmdTest"  'RegisterCmdTestFun)
    
    procedure( RegisterCmdTestFun()
    prog( ()
        axlUIConfirm("Allegro Cmd Register Test...")
        
        return(t)
        
    ))
    
2、赋快捷键
    alias ~1 CmdTest                            （使用Allegro alias命令）
    axlSetAlias("~1"  "CmdTest")                （使用AXL接口函数axlSetAlias）
    
3、菜单编辑
    点击Setup - User Preferences...菜单，选择UI_Paths类别中的menupath，查看Allegro菜单可以存在的路径
    Allegro软件系统的菜单文件都位于%CDSTOOT%\share\pcb\text\cuimenus文件夹下
    
    当前常用的Editor涉及两个菜单，一个是allegro.men（打开brd文件使用）；另一个是xlibsymbol.men（打开
    dra文件使用）
    
    //描述菜单的起始（其中//为注释）
    BEGIN
    
    END
    
    //描述弹出下拉子菜单
    POPUP
        BEGIN
    
        END
        
    //描述一个菜单项（&表示热键，第一个字母为菜单名，中间一个逗号，后面为命令）
    MENUITEM    "$New...",      "new"
    
    //描述菜单分栏
    MENUITEM    SEPARATOR
    
    //如果你定义了快捷键，那么菜单上就会显示出来， 这个是系统的功能
    
    另外， 惨淡格式支持条件加载（见#ifndef或#ifdef）, 他们后面紧跟一个环境变量名， 探索吧... ...
    
4、SKILL文件的自动加载
    getSkillPath函数返回的路径列表下的Allegro.ilinit文件中添加load文件的命令，对应的文件在Allegro启动
    时自动加载，基于这个功能，我们可以不同每次在使用自定义功能前，先加载SKILL文件。
    
5、参考文档
    %CDSROOT%\doc
    
    例子：
    D:\DesignAdvance\Cadence\SPB_15.2\doc
    
    参考手册列表：
    algroskill          AXL接口函数参考手册
    
6、课后习题
    将第二课课后习题实现的功能注册命令，并使用CTROL + D可以快捷键加亮，同时配置到工具里面