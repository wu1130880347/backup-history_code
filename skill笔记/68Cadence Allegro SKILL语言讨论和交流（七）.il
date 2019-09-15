
大纲：
    （一）、SKILL基础
    （二）、利用AXL接口操作Allegro PCB对象
    （三）、注册和使用自定义Allegro命令
    （四）、Allegro UI界面设计和响应

Email: TrizDev@yahoo.cn   (以前的邮箱丢了)


四、Allegro UI界面设计和响应
1、简介
    界面是交互设计中不可缺少的一部分，很多时候我们无法只让程序孤芳自赏地运行
    
2、基于文本描述的SKILL界面设计
    例子：Example.form（简单的几段文字已经勾勒出我们想要的界面）
    
    FILE_TYPE=FORM_DEFN VERSION=2
    FORM 
    FIXED
    PORT 26 20
    
    HEADER "FINDER..."
    TILE
    TEXT "COMPONENT LIST"
    TLOC 1 1
    FSIZE 25 3
    ENDTEXT
    
    FIELD Placed_List
    FLOC 1 3
    LIST "" 24 16
    OPTION sort
    ENDFIELD
    FIELD done
    FLOC 9 33
    MENUBUTTON "  OK  " 10 3
    ENDFIELD
    ENDTILE
    ENDFORM
    
3、测试Form的方法
    Form测试接口函数axlFormTest(s_formPath)
    
    示例：
        axlFormTest("C:\\Example.form")
    
    功能：
        利用这个函数可以检查Form文件的语法以及Form的显示效果， 编辑界面时不断使用，使你的界面更加漂亮
        
    Tips:
        A、Allegro所有的Form文件都放在Setup - User Preferences...菜单，选择UI_Path列别中的formpath。
           查看Allegro所有界面文件，很好的界面重用资源
           
        B、将自定义的Form文件放在formpath下可以不用写绝对路径调用，不信试试
        
4、界面工具开发ABCDE
    A、创建界面文件并使用axlFormCreate或axlMiniStatusLoad打开Form句柄（注意卸磨杀驴）
    B、使用axlFormSetField初始化界面区域
    C、axlFormDisplay显示界面
    D、在axlFormCallback中定义用户交互和区域响应
    E、使用axlFormClose关闭界面窗口
    
    /*
        功能：通过选择界面列表框中的器件名称浏览器件
        命令：OverviewComps
    */
    
    ;注册命令
    axlCmdRegister("OverviewComps"  'TestFormAppMainModule)
    
    ;创建界面文件并显示界面（主模块）
    procedure( TestFormAppMainModule()
    prog( (FileName  FilePort)
        ;写文件生成界面文件
        FileName = "TestForm.form"
        FilePort = outfile(FileName  "w")
                
        fprintf(FilePort  "FILE_TYPE=FORM_DEFN VERSION=2\n")
        fprintf(FilePort  "FORM\n") 
        fprintf(FilePort  "FIXED\n")
        fprintf(FilePort  "PORT 26 20\n\n")
        
        fprintf(FilePort  "HEADER \"FORM TEST...\"\n\n")
        
        fprintf(FilePort  "TILE\n\n")
        
        fprintf(FilePort  "TEXT \"ComponentList:\"\n\n")
        fprintf(FilePort  "TLOC 1 1\n")
        fprintf(FilePort  "FSIZE 25 3\n")
        fprintf(FilePort  "ENDTEXT\n\n")
        
        fprintf(FilePort  "FIELD ComponentList\n")
        fprintf(FilePort  "FLOC 1 3\n")
        fprintf(FilePort  "LIST \"\" 24 16\n")
        fprintf(FilePort  "OPTION sort\n")
        fprintf(FilePort  "ENDFIELD\n\n")
        
        fprintf(FilePort  "FIELD Quit\n")
        fprintf(FilePort  "FLOC 9 33\n")
        fprintf(FilePort  "MENUBUTTON \"EXIT\" 10 3\n")
        fprintf(FilePort  "ENDFIELD\n\n")
        
        fprintf(FilePort  "ENDTILE\n\n")
        
        fprintf(FilePort  "ENDFORM\n\n")
        
        close(FilePort)
        
        ;创建界面句柄，删除界面文件（卸磨杀驴...）
        fw = axlFormCreate(gensym()  FileName  '("E"  "OUTER")  'TestFormAppCallBack  t)
        deleteFile(FileName)
        
        ;界面区域初始化
        TestFormFieldInit(fw)
        
        ;显示界面
        axlFormDisplay(fw)
        
        return(t)
        
    ))
    
    ;界面区域初始化
    procedure( TestFormFieldInit(fw)
    prog( (CompsNameList)
        ;获取所有的器件名称
        CompsNameList = axlDBGetDesign()->components~>name
        
        ;设置器件列表区域
        axlFormSetField(fw  "ComponentList"  CompsNameList)
        
        return(t)
    ))
    
    ;界面区域响应回调函数
    procedure( TestFormAppCallBack(fw)
    prog( (CompName  CompId)
        case(fw->curField
            ("ComponentList"
                CompName = fw->curValue
                CompId = axlSelectByName("COMPONENT"  CompName)
                
                ;定位到对象
                axlZoomToDbid(CompId  t)
                
                ;显示对象详细信息
                axlShowObject(CompId)
            )
            
            ("Quit"
                ;清理现场
                axlClearSelSet()
                axlSetFindFilter(?enabled "NOALL"  ?onButtons "NOALL")
                
                axlFormClose(fw)
            )
        )  
        
        return(t)
        
    ))
    
5、参考文档
    %CDSROOT%\doc
    
    例子：
    D:\DesignAdvance\Cadence\SPB_15.2\doc
    
    参考手册列表：
    algroskill          AXL接口函数参考手册
    
    Tips:
        看看下面的路径下是什么东西，保证乐死你... ...
        %CDSROOT%\share\pcb\example\skill
        
6、课后习题
    搞懂%CDSROOT%\share\pcb\example\skill路径下所有的程序代码，然后恭喜你入门了... ...