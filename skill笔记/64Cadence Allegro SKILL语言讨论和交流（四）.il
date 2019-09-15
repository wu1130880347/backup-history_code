大纲：
    （一）、SKILL基础
    （二）、利用AXL接口操作Allegro PCB对象
    （三）、注册和使用自定义Allegro命令
    （四）、Allegro UI界面设计和响应

Email: TrizDev@yahoo.cn   (以前的邮箱丢了)
    
一、SKILL基础
1、简介
    SKILL是基于Cadence软件平台的一种二次开发语言，所有的Cadence软件都可以使用SKILL进行二次开发，同样，我们使用的Allegro PCB软件同样可以。
2、启动和退出SKILL解释窗口
    A、在Allegro命令行输入 set telskill
    B、点击Setup - User Preferences...菜单，勾选SKILL类别中的telskill，选择OK按钮确认
    C、在Allegro命令行输入 skill
    使用 exit 退出SKILL解释窗口
3、SKILL语言的数据类型
    整  型：        integer         x           1
    浮  点：        flonum          f           1.0
    数  字：        number          n           2或2.5
    符  号：        symbol          s           'C
    字符串：        string          S           "Count"
    列  表：        list            l           (1 2 3 "Test")
    I/O端口：       port            p           port:"C:/Test.txt"（使用infile或outfile）
      .
      .
      .
      
    SKILL变量数据类型判断相关的函数有：
    type
    typep
    listp
    floatp
    inportp
    fixp
    integerp
    portp
    stringp
    symbolp
    
    通过help函数在SKILL解释窗口查看帮助，例如：help abs
    arglist('abs)  查看abs函数的参数
    
4、在SKILL解释窗口对变量赋值并进行数学运算
    1
    96.54
    'a
    "I hava a dream."
    
    1 + 0.5         对应        plus(1  0.5)
    96.2 - 1        对应        difference(96.2  1)
    5 ** 2          对应        expt(5  2)
    6.2 * 5.1       对应        times(6.2  5.1)
    3.3 / 5.5       对应        quotient(3.3  5.5)
    
    Number = 1.35
    type(Number)
    numberp(Number)
    Number = Number + 1
    
5、在SKILL命令行显示数据
    printf("Hello, World.\n")
    println(5.9867614)
    
    Variable = 25
    printf("%d\n"  Variable)
    println(Variable)
    
    Variable = 98.25468
    printf("%f\n"  Variable)
    println(Variable)
    
    Variable = "Hello, SKILL, I get it, today."
    printf("%s\n"  Variable)
    println(Variable)
    
6、函数调用及其常见问题
    ;正确调用，函数名和左括号之间没有空格
    strcat("I "  "have "  "a "  "question.")
    
    ;出错了... ...
    strcat ("I "  "have "  "a "  "question.")
    
7、SKILL中最伟大的数据类型之一 list 登场了
    ;创建一个列表
    list(1 2 3 4 5 6)
    '(1 2 3 4 5 6)
    
    ;在列表前插入一个元素
    Variable = list(5 6 4 7)
    Variable = cons("Front"  Variable)
    
    ;在列表后追加一个元素
    Variable = list(5 6 4 7)
    Variable = append1(Variable  "End")
    
    ;合并两个列表
    Variable = list(5 6 4 7)
    Variable = append(Variable  list("End"))
    
    ;统计列表中的元素个数
    Variable = list(5 6 4 7)
    length(Variable)
    
    ;访问列表中的元素
    Variable = list(5 6 4 7)
    car(Variable)
    cdr(Variable)
    nth(0  Variable)
    nthelem(0  Variable)
    last(Variable)
    
    ;怎么返回列表最后一个元素
    Variable = list(5 6 4 7)
    car(last(Variable))
    nth(0  last(Variable))
    nthelem(1  last(Variable))
    
    ;倒序一个列表
    Variable = list(5 6 4 7)
    reverse(Variable)
    
8、开发一个SKILL函数并加载运行
    ;自定义函数模板
    procedure( FunName()
    prog( ()
        ;添加你的功能代码
        
        return(t)
        
    ))
    
    注：prog中可以声明局部变量，在其中返回值
    
    ;例子
    procedure( HelloWorld()
    prog( ()
        ;添加你的功能代码
        printf("Hello, World.\n")
        
        return(t)
        
    ))
    
    使用load函数加载SKILL文件， 假设上面的例子保存在D盘下的Test.il文件中
    load("D:/Test.il")
    load("D:\\Test.il")
    
    放在getSkillPath()路径下的SKILL文件不用写文件路径，直接写名称就可以了
    
    在SKILL解释窗口输入 HelloWorld() 调用函数， 和使用系统函数一样... ...
    
9、在函数中使用条件控制和循环
    ;成绩登记打印
    Variable = list(98  70  68  40  82  60.5  53)
    
    procedure( ScoreLevelPrint(ScoreList)
    prog( (Item)
        ;遍历成绩列表
        foreach(Item  ScoreList
            ;使用cond函数条件判断
            cond(
                ((Item < 60)
                    printf("cond  ==>  %.1f  ==>  D"  Item)
                )
                
                (((Item >= 60) && (Item < 75))
                    printf("cond  ==>  %.1f  ==>  C"  Item)
                )
                
                (((Item >= 75) && (Item < 90))
                    printf("cond  ==>  %.1f  ==>  B"  Item)
                )
                
                ((Item >= 90)
                    printf("cond  ==>  %.1f  ==>  A"  Item)
                )
            )
            
            ;使用if函数条件判断
            if( (Item < 60)
            then
                printf("if  ==>  %.1f  ==>  D"  Item)
            )
            
            if( ((Item >= 60) && (Item < 75))
            then
                printf("if  ==>  %.1f  ==>  C"  Item)
            )
            
            if( ((Item >= 75) && (Item < 90))
            then
                printf("if  ==>  %.1f  ==>  B"  Item)
            )
            
            if( (Item >= 90)
            then
                printf("if  ==>  %.1f  ==>  A"  Item)
            )
        )
        
        return(t)
        
    ))
    
10、文件读写操作
    Variable = list("arc"  "arc"  "cline"  "text"  "via"
                    "arc"  "arc"  "cline"  "text"  "via"
                    "arc"  "arc"  "cline"  "text"  "via")
                    
    procedure( type_count(element_list)
    prog( (cline_count  arc_count  via_count  text_count  NoCline  total_count
            elmnt  FileName  FilePort)
        cline_count = arc_count = via_count = text_count = 0
        NoCline = total_count = 0
    
        ;遍历列表
        foreach(elmnt  element_list
            if( (elmnt == "cline")
            then
                cline_count++
            else
                NoCline++
            )
            
            if( (elmnt == "arc")
            then
                arc_count++
            )
            
            if( (elmnt == "via")
            then
                via_count++
            )
            
            if( (elmnt == "text")
            then
                text_count++
            )
        )
        
        total_count = cline_count + arc_count + via_count + text_count
        
        
        ;结果写入文件
        FileName = "C:\\Test.txt"
        FilePort = outfile(FileName  "w")
        
        ;操作
        fprintf(FilePort  "Total Elements: %d\n"  total_count)
        fprintf(FilePort  "Cline Elements: %d\n"  cline_count)
        fprintf(FilePort  "Arc   Elements: %d\n"  arc_count)
        fprintf(FilePort  "Text  Elements: %d\n"  text_count)
        fprintf(FilePort  "Via   Elements: %d\n"  via_count)
        fprintf(FilePort  "NoCline       : %d\n"  NoCline)
        
        close(FilePort)
        
        ;显示文件中的信息到命令行
        FileName = "C:\\Test.txt"
        FilePort = infile(FileName)
        
        ;临时字符串变量
        TmpString = ""
        
        while( gets(TmpString  FilePort)
            printf(TmpString)
        )
        
        close(FilePort)
        
        return(t)
        
    ))
    
11、SKILL中的注释
    ;行注释
    
    /*
        模块注释
        This is a test.
    */
    
12、参考文档
    %CDSROOT%\doc
    
    ;例子
    D:\DesignAdvance\Cadence\SPB_15.2\doc
    
    参考手册列表
    sklanguser              SKILL用户手册
    sklangref               SKILL函数参考手册(encrypt加密SKILL文件)
    skdevref                SKILL语言开发函数参考手册
    skipcref                SKILL调用其他进程参考手册
    skoopref                SKILL面向对象开发参考手册
    
13、课后习题
    练习写一个自定义的函数，实现一个小功能（例如：计算直角三角形斜边长度的功能）