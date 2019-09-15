1-5 文件操作函数

常见文件
这里只列举小些与 Allegro Skilll 语言相关的几个常见文件：

类型	  				 功能	    		 源码格式
.il	    			 skill 源码				 skill
.form				 表单文件				 BNF
.color				 颜色配置文件	  		 skill
.clp	   		     剪切板文件     		 skill
allegro.ilinit  	随allegro启动自动加载	 skill
env					随allegro启动自动加载	 command

检测文件
文件的读/写均为UNIX文本文件。

文件存在	isFile，isFileName	检测文件是否存在函数
isFileName("c:/test/1.txt")  =>nil ;c:/test/1.txt 实际不存在
可被读	isReadable	检测文件或文件夹的是否可以被读取的函数
isReadable("c:/test/1.txt")  =>nil
可读写	isExecutable	判断文件是否可读写。
isExecutable("c:/test/1.txt") =>nil
可被写	isWritable	检测文件或文件夹的是否可以被写入的函数
isWritable("c:/test/1.txt")
大文件检查	isLargeFile	检测文件是否大于2GB或更大
isLargeFile( "largeFile" ) => t
文件长度	fileLength	返回文件长度
fileLength( "largeFile" ) => 3072000000
删除文件	deleteFile	删除指定文件
deleteFile("c:/test/1.txt")
修改文件名	renameFile	重命名文件 renameFile(S_old S_new) => t | nil