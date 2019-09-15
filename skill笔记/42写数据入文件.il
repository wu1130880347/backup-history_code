向文件中写入数据
向文件中写入数据，可通过以下步骤：
1.使用outfile函数，获取一个文件的输出端口
2. 为print和println函数提供一个可选的端口参数写入文件，或者提供一个端口参数给fprintf函数写入文件
3. 使用close函数关闭输出端口。

print和println可接受一个可选的输出端口参数，使用outfile获取一个文件的输出端口。然后写入文件，最后使用close函数关闭输出端口。
myPort = outfile( "myFile.txt" )
=>port:"myFile.txt";定义输出文件端口2 
for( i 1 3  println( list( "Number:" i) myPort ) ) 
close( myPort ) ;关闭释放输出文件。

写这些数据到myFile.txt
("Number:" 1)
("Number:" 2)
("Number:" 3)

请注意outfile返回值为port:"myFile.txt"，说明文件读取或创建成功。请注意SKILL显示端口：
myPort = outfile( "/tmp/myFile" )
port:"/tmp/myFile"

如果返回值为nil,说明该文件只读或在指定的目录中无法创建。在使用print或println时，出错提示：
E- *Error* print/println: cannot send output to a closed port -port:"myFile.txt"
ERROR


outfile函数需要一个全路径，当未获取输出文件端口或文件未创建时，outfile函数返回nil。
当其端口为nil，print和println函数返回一个错误，请看下面例子：
println( “Hello”nil )
Message: *Error* println: argument #2 should be an I/Oport
(type template = “gp”) – nil
与print和println函数不一样，printf函数不需要一个可选的端口参数。
使用fprintf函数向一个文件写入“格式控制”内容。fprintf函数的第一个参数必须为一个文件的输出端口。请看下面例子：
myPort = outfile( “/tmp/myFile”)
for( i 1 3
fprintf(myPort “Number: %d\n” i )
)
close(myPort )
Number: 1
Number: 2
Number: 3
	
	更多实例：

创建文件并输出数据到该文件的函数

procedure( DataOut()
    lData = list(
                '("just" 10 5.5)
                '("for" 20 10.2)
                '("test" 30 15.0)
                '("use" 40 17.8)
                '("done" 50 1.2)
          ); 定义数据列表lData
fDataOut = strcat( getTempDir() "./" "data.txt") 
; 输出文件名和路径，%temp%/data.txt。
    pDataOut = outfile( fDataOut "w" ); 建立输出端口
    foreach( item lData
        fprintf( pDataOut "%s %d %0.2f\n"  ; 输出数据到文件端口
                    car(item) cadr(item) caddr(item)  
        ; item=>每一个在lData中的列表
        ); end fprintf
    ); end foreach
    close( pDataOut )  ; 关闭端口
printf( "I- output data fromlData list to file %s, done!" fDataOut )
; 输出提示信息
); end procedure
	