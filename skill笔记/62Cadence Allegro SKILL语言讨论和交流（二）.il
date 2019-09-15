1、如何在PCB Editor环境下启动SKILL命令行解释器界面？
直接输入set  telskill ， 然后你就会看到一个新的界面， 这个界面就是用来运行SKILL函数和命令的解释器。当然，如果你不觉得界面太小，可以直接在命令行中输入skill ， 然后运行SKILL函数和命令， 也可以正常执行，退出可以直接输入exit。

2、如何执行写在文件中的SKILL函数和命令？
首先，我们按照上面的方法运行SKILL命令行解释器，然后输入getSkillPath()， 回车后，可以看到返回一个路径的列表，如果我们的SKILL文件放在这些路径下，就可以直接使用load()函数加载使用，如果你的SKILL文件没有在其中的一个路径下，就需要带上绝对路径，这个地方大家需要注意‘/’和‘\’的区别，我不想做什么理论描述，看看下面的例子：

假如你有一个文件放在C：下，名字为test.il， 一般SKILL文件都以.il为后缀
使用Ultra-Editor打开文件，输入如下内容：
procedure( tr_Example_1()
prog( ()
      ;打印一串字符
      printf("Let's go today.\n")

     ;弹出一个对话框
      axlUIConfirm("Hello, SKILL...\n")

    return(t)

))

然后在SKILL解释器界面下加载SKILL文件如下：
load("C:/test.il")
或者
load("C:\\test.il")

接下来运行tr_Example_1()， 回车，看看什么结果。

3、总结上面的例子（我们学到了什么？）
一、SKILL文件的扩展名一般为.il，通过load()函数可以加载SKILL文件
二、SKILL文件放在一些特定的路径下可以直接加载，不需要输入绝对路径，直接输入文件名即可, load("test.il")
三、带绝对路径加载SKILL文件，一般输入一个/来隔开路径， 如果需要使用\，则需要输入两个，\代表转义，\\表示一个\。
四、使用procedure可以定义一个SKILL函数，上面的例子， 函数名为tr_Example_1，注意函数名后的括号和函数名之间不要有空格。
五、我们可以很方便的在SKILL解释器中单步调试SKILL语句，例如：上面函数体中的printf("Let's go today.\n")可以直接在SKILL解释器下运行，当然axlUIConfirm("Hello, SKILL...\n")也可以，其中函数体中的注释为以分号开始的行，此为单行注释。
六、我们还可以使用像C语言的注释一样/* */来进行模块注释，但是不要出现两个/* */注释的嵌套。

4、我写这么清楚，相信大家还会输入错误，下面有一个附件，大家可以下载直接加载， 哈哈， 注意一切都是英文的逗号，引号！

5、和大家开个玩笑，如果让我知道了SKILL可以单行运行，我就会将它的所有函数在SKILL命令行运行一次，他的函数在什么地方， 请直接参考文档：（在你Allegro的安装路径下搜索如下路径）
D:\Cadence\SPB_15.5.1\doc\sklangref\sklangref.pdf

初学者文档：
D:\Cadence\SPB_15.5.1\doc\sklanguser\sklanguser.pdf