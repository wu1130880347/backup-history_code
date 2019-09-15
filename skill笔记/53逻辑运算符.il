逻辑运算符
    SKILL将 nil 默认为是"假"，其它值都被认为是"真"。如果需要确定其返回值时，SKILL将会返回其第二个参数的值。
功能     函数     运算符    语法     例子    返回值    
与       and        &&    x1 && x2   3&&5      t  
                                     5&&3      t
                                     t&&nil   nil
                                     nil&&t   nil
或       or         ||    x1 || x2   3||5      3
                                     5||3      5 
                                     t||nil    t
                                     nil||t    t
not      非         ！     !expr     !nil      t


    &&和 || 运算符返回其最后一次计算的值。因此，&& 和 || 运算符可以用于避免 if 和 when 运算符赘余的表达式。

使用 &&
当 SKILL 创建一个变量时，将会附一个 unbound 初值给变量表明该变量还未初始化。可使用 boundp 函数判断一个变量是否是bound。如果变量为 bound ，返回 t 否则返回 nil
boundp(s_arg [ e_environment ])=> t | nil

    假如你想返回变量 args 的值，如果args 值是一个 unbound，结果将会产生一个错误，否则，使用以下表达式：
Skill > args = 'unbound=>unbound  ;args变量清空
Skill > boundp( 'args) => nil  ;不是bound
Skill > boundp( 'args) && args => nil ;空值时避免出错提示
Skill >args="full" "full"
Skill > boundp( 'args) && args=>"full" ;非空值时返回变量值


使用 ||
    假如你有一个默认名称，比如 noName。假如你有一个变量，比如 userName。如果userName是 nil ，那么就使用默认名称。可使用以下表达式：
username || "noName" =>"noName"


使用 ！
nil为假，（非nil为真）
!nil=>t
!3=>nil ;非nil为真