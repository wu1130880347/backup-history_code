在大伙的帮助和启发下完成的第一个SKILL，把源代码分享给大家，代码比较简单，思路：通过创建批处理文件，然后向批处理文件中写入删除代码，再调用该批处理删除文件，刚学编写SKILL，代码的注释不知是否正确，有误的地方望指教!
axlCmdRegister( "cam_clear" 'camclear  ?cmdType "general")  ;注册allegro命令"cam_clear",调用函数"camclear"，命令类型为"general",当调用命令时会立即执行命令，立即停止allegro当前正在执行的命令。
defun( camclear ()     ;定义函数"camclear"
p = axlDMOpenFile("TEMP" "./temp.bat" "w");在当前目录创建打开一个批处理文件"temp.bat"
fprintf(p "del /f /s /q \".\\*.art\"\n")  ;向temp.bat中写入删除语句：del /f /s /q ".\*.art"，删除当前目录下后缀为.art的文件
fprintf(p "del /f /s /q \".\\*.log\"\n")  ;删除当前目录下后缀为.lon的文件
fprintf(p "del /f /s /q \".\\*.tag\"\n")  ;删除当前目录下后缀为.tag的文件
fprintf(p "del /f /s /q \".\\*.drl\"\n")  ;删除当前目录下后缀为.drl的文件
fprintf(p "del /f /s /q \".\\*.dxf\"\n")  ;删除当前目录下后缀为.dxf的文件
fprintf(p "del /f /s /q \".\\*.ipc\"\n")  ;删除当前目录下后缀为.ipc的文件
fprintf(p "del /f /s /q \".\\*.jrl,1\"\n") ;删除当前目录下后缀为.jrl,1的文件
fprintf(p "del /f /s /q \".\\art_aper.txt\"\n") ;删除当前目录下的art_aper.txt
fprintf(p "del /f /s /q \".\\art_param.txt\"\n") ;删除当前目录下的art_param.txt
fprintf(p "del /f /s /q \".\\nc_param.txt\"\n")  ;删除当前目录下的nc_param.txt
fprintf(p "rd /s /q signoise.run\n") ;删除当前目录下的signoise.run文件夹