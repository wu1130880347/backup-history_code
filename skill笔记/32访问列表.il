Lists在内部被存储为一系列的分支点，可以想象成左边第一个元素为左分支，右边其余的元素为右分支（更多的分支点）。可以使用 car 函数获取左分支的元素，cdr获取右分支的元素（除去第一个元素剩下的右边所有元素）。
例如：有一个 list(a b c)，此一list在内部储存的方式可以下图表示。
检索 List 的第一个元素(car)
car 函数返回list的第一元素
numbers = ‘( 1 2 3 ) => ( 1 2 3 )
car( numbers ) => 1

检索list的尾部元素(cdr)
numbers = ‘( 1 2 3 ) => ( 1 2 3 )
cdr( numbers ) => ( 2 3 )

通过索引检索一个元素(nth)
nth(0 numbers) 相当于car( numbers).
numbers = ‘( 1 2 3 ) => ( 1 2 3 )
nth( 1 numbers ) => 2

确定一个数据是否在list中 (member)
member 函数不能搜索所有分级的list，只能查找顶层元素。将查找到的元素及其右分支的所有元素输出。
numbers = ‘( 1 2 3 ) => ( 1 2 3 )
member( 4 numbers ) => nil
member( 2 numbers ) => ( 2 3 )

统计list的元素 (length)
length determines the length of a list, array, or association table.
length 函数可确定一个list、数组、群丛表的长度
numbers = ‘( 1 2 3 ) => ( 1 2 3 )
length( numbers ) => 3