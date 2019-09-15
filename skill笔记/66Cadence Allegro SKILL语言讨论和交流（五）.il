大纲：
    （一）、SKILL基础
    （二）、利用AXL接口操作Allegro PCB对象
    （三）、注册和使用自定义Allegro命令
    （四）、Allegro UI界面设计和响应

Email: TrizDev@yahoo.cn   (以前的邮箱丢了)

二、利用AXL接口操作Allegro PCB对象
1、简介
    Allegro PCB中是以DBID的方式描述相关对象的（例如：dbid:59252472），通常我们可以使用
    两种方法来获取到Brd文件中的相关信息，其中第一种是使用axlDBGetDesign()接口函数获取
    顶层设计，然后不断使用访问属性的方式获取到其他对象；另一种类似Show Element功能一样
    的操作方式，通过设置对象过滤器，选择高亮对象，获取高亮对象三个步骤获取对象列表，接
    下来，我们就通过两种方法来介绍通过AXL接口函数操作Allegro PCB对象。
    
2、使用axlDBGetDesign()方式访问PCB对象（实例：获取单板上所有的二极管器件名称列表）
    需求：
        获取单板上所有的二极管器件名称列表
        
    分析：
        二极管就是名称以D开头和数字结束的器件
        
    设计：
        正则表达式：  "^D[0-9]+$" 模式匹配以D开头和数字结束的字符串
        
    实现：
    
    procedure( GetAllDiodeCompsNameList()
    prog( (Design  Components  Item  CompName  DiodeCompsNameList)
        ;获取等曾设计ID并赋值给Design
        Design = axlDBGetDesign()
        
        ;获取设计中的所有器件（通过 -> 访问标识的属性）
        Components = Design->components
        
        ;遍历所有的器件
        foreach(Item  Components
            ;获取器件爱你的名称
            CompName = Item->name
            
            ;当器件名称满足二极管条件
            if( rexMatchp("^D[0-9]+$"  upperCase(CompName))
            then
                DiodeCompsNameList = cons(CompName  DiodeCompsNameList)
            )
        )
        
        ;翻转DiodeCompsNameList中的元素
        DiodeCompsNameList = reverse(DiodeCompsNameList)
        
        return(DiodeCompsNameList)
        
    ))
    
    
    同样访问网络的方式（课后习题可以实现一个寻找含有GND的网络名称列表，正则表达式匹配模式为： "GND"）
    Nets = Design->nets
    NetNumber = length(Nets)
    
3、通过设置过滤器和Select函数操作brd对象（设置过滤器 -- 设置显示层并选择对象 -- 获取高亮东西 -- 清理现场）
    需求：
        删除ETCH/TOP层所有的TEXT
    
    分析：
        axlDeleteObject接口函数可以删除对象
        
    设计：
        通过使用设置过滤器和Select函数操作brd对象
        
    实现：
    
    procedure( DeleteAllTextsOnEtchTopLayer()
    prog( (CurVis  AllText)
        ;设置过滤器
        axlSetFindFilter(?enabled list("TEXT")  ?onButtons "TEXT")
        
        ;设置显示层并选择对象
        CurVis = axlVisibleGet()
        
        axlVisibleDesign(nil)
        axlVisibleLayer("ETCH/TOP")
        axlUIWUpdate(nil)
        
        axlAddSelectAll()
        
        ;获取高亮东西
        AllText = axlGetSelSet()
        
        ;删除操作
        axlDeleteObject(AllText)
        
        ;清理现场
        axlClearSelSet()
        axlSetFindFilter(?enabled "NOALL"  ?onButtons "NOALL")
        
        axlVisibleSet(CurVis)
        axlUIWUpdate(nil)
        
        return(t)
        
    ))
    
4、参考文档
    %CDSROOT%\doc
    
    例子：
    D:\DesignAdvance\Cadence\SPB_15.2\doc
    
    参考手册列表：
    algroskill          AXL接口函数参考手册
    
5、课后习题
    自定义一个函数，使用AXL接口函数实现根据网络名后缀自动定义差分对的功能（备注：差分先网络后准以P/N或+/-结束，
    其中后面为P或+代表正）
    
    提示：定义差分对可以使用axlDiffPair函数