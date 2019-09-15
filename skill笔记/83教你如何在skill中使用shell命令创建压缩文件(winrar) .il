
前提，电脑必须安装有winrar，如果安装在C盘，那么请到C:/Program Files/WinRAR文件夹找到winrar.exe文件，然后存放于任何不带空格的文件目录下。
建议存放于C:/WINDOWS/system32目录。
下面对shell中使用winrar命令做一个介绍。以下是其基本格式：

shell("winrar路径 执行命令 压缩文件名 需要压缩的文件或文件夹")

1.winrar路径绝对路径!
    假如放于“e:/winrar.exe”，那么该参数就为e:/winrar。假如已经存放于C:/WINDOWS/system32目录，那么该参数可直接使用winrar!

2.执行命令- E
    可指定以下命令) 
    a 添加文件到压缩文件: 
    -k 锁定压缩文件
    -s 产生固体存档,这样可以增大压缩比
    -r 包括子目录
    -m* 设置压缩比
注：m*请参考以下参数- e# ?! L+ F, }) H. S* }
    -m0   存储      添加到压缩文件时不压缩文件。
    -m1   最快      使用最快方式(低压缩)
    -m2   较快      使用快速压缩方式+
    -m3   标准      使用标准(默认)压缩方式(
    -m4   较好      使用较好压缩方式(较好压缩，但是慢)
    -m5   最好      使用最大压缩方式(最好的压缩，但是最慢)
如果只压缩单个文件，该参数直接指定为"a"即可，如果压缩某个文件夹下所有文件，且该文件夹下包含有子目录，并需要一起压缩，那么参数指定为"

3.压缩包文件的文件名
     可指定绝对路径。
     比如现在想压缩某个文件，并在相同目录下产生压缩包，压缩包名字为1.zip，那么该参数直接为"1.zip"。如果指定压缩包存放在d盘下，那么该参数就指定为"d:/1.zip".

4.需要压缩的文件或文件夹。
      比如压缩当前目录下所有文件，那么该参数指定为"./"，如果压缩当前目录下的1.brd文件，那么就指定"1.brd"。如果需要压缩所有扩展名为brd的文件，那么就指定该参数为"*.brd"

下面举几个简单的例子说明如何使用（默认winrar已存放于system32目录中）：

1.直接压缩当前brd文件，压缩包名字为2.zip，可输入以下命令：
shell("winrar.exe a 2.zip brdName.brd")
复制代码
2.压缩当前目录下所有文件并包含子目录文件夹:
shell("winrar.exe a 2.zip ./")
复制代码
3.以最快方式压缩当前目录下所有"il"文件，并将压缩包名字命为为"il.zip"，存放在d盘根目录下：
shell("winrar.exe a -m1 d:/il.zip ./*.il")
复制代码
4.直接压缩当前打开的brd文件并存于同一目录，brd文件名命名压缩包文件名。
brd_Name = strcat(axlCurrentDesign() ".brd")
zip_Name = strcat(axlCurrentDesign() ".zip")
winrar_brdzip = strcat("winrar.exe a " zip_Name " "
shell(winrar_brdzip)