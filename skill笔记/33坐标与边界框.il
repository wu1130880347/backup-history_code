坐标与边界框
以下函数操作的变量将不会改变自身值或者参数新的变量
坐标
一个xy坐标是通过包含两个元素的list表示。”:” 可通过x值和y值的创建一个坐标。
xValue = 300      ;x值
yValue = 400      ;y值
aCoordinate = xValue:yValue => ( 300 400 )
xCoord函数和yCoord函数可分别获取坐标的x值和y值
xCoord( aCoordinate ) => 300
yCoord( aCoordinate ) => 400

可通过单引号(‘)或者list函数创建一个包含坐标的list
可通过使用car获取x坐标，car(cdr(…))获取y坐标。
边界框

顾名思义，边界框是一个方形区域，可通过一个list表示出来，该list包含两个坐标——左下角和右上角。使用list函数可创建一个边界框：

_ 通过“:”表示坐标 ( : ).
bBox = list( 300:400 500:450 )

_ 通过变量表示坐标.
lowerLeft = 300:400
upperRight = 500:450
bBox = list( lowerLeft upperRight )

可通过使用单引号(‘)创建一个bounding box，但需要逐个给出lists
bBox = ‘(( 300 400 ) ( 500 450 ))

使用 car  cdr  操作 Bounding Boxes

函数      意义                        例子              描述
car    car( … )                   ll = car( bBox)      左下角坐标
cadr   car( cdr( … ) )            ur= cadr( bBox)      右上角坐标
caar   car( car( … ) )            llx = caar( bBox)    左下角X坐标
cadar  car( cdr( car( … ) ) )     lly = cadar( bBox)   左下角Y坐标
caadr  car( car( cdr( … ) ) )     urx = aadr( bBox)    右上角X坐标
cadadr car( cdr( car( cdr( ...]	  ury =cadadr( bBox)   右上角Y坐标