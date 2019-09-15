大家集思广益看怎么写一个和excel中一样强大的查找替换string中的一个或者几个char，附上我写的一个功能不完备的skill函数。要替换的函数只能在string出现一次。如果出现多次就会出现问题。大家一起想办法完善一下这个子函数吧。也好供大家以后编程使用。原因是使用的函数遍历string的元素，发现相同的ｃｈａｒ便会执行操作。不能区分多个在一起的ｃｈａｒ

例如 2_R2我想换成3_R2或者是２＿Ｒ３我会使用
_replaceStringElement ＂２＿Ｒ２＂＂２＿＂　＂３＿＂)　但是结果会是３＿Ｒ３＿
_replaceStringElement ＂２＿Ｒ２＂＂Ｒ２＂　＂Ｒ３＂)　　＝＞("R3_R3")



defun(_replaceStringElement (_originalString _originalchar _replacechar)
let((_tempString _breakedString _combinedString _finalString)

_combinedTempString=""
_tempString=strcat("#" _originalString "#")

_breakedString=parseString(_tempString _originalchar)

_combinedString=buildString(_breakedString _replacechar)

_finalString=foreach(x parseString(_combinedString "#") strcat(_combinedTempString x))
)
)