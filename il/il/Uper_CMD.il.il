
;说明：此脚本功能用于兼容 输入x ix iy命令错为大写，仅需安装在skill路径并声明即可。
; load("Uper_CMD.il")
axlCmdRegister("X" 'X_CMD ?cmdType "sub_cmd")
axlCmdRegister("IX" 'IX_CMD  ?cmdType "sub_cmd")
axlCmdRegister("IY" 'IY_CMD ?cmdType "sub_cmd")
: v9 F1 ]5 Y/ j# x1 {/ b0 j& \

/*====================== ix||IX =========================*/ 
defun(IX_CMD (@optional (valueX nil) )
        if(valueX  == nil% }! d7 z' _' F# y
                then5 R: x- n6 ?% H+ X9 a: |
                        axlUIWPrint(nil "Somethings Wrong")
                else% j% G) w6 B$ C+ ~: Y* C/ a
                        ix_scr(valueX )
        );
);1 R$ N7 F. J: }2 i
defun( ix_scr (valueX )
        axlShell(sprintf(nil "ipick %L " valueX))
);! }6 Z- Y; l" N: _2 x# X, I- A: ~
/*======================= iy||IY ========================*/ 
defun(IY_CMD (@optional (valueY nil))9 t& F0 m4 C6 d9 g+ w; [
        if(valueY == nil
                then& E: X; T+ v  `
                        axlUIWPrint(nil "Somethings Wrong")
                else
                        iy_scr(valueY)
        );9 P% I% }% R5 i/ [  ?7 L
);
defun( iy_scr (valueY)" h9 Z* F& A  Y" x5 g% a
        axlShell(sprintf(nil "ipick 0 %L"  valueY))# p( p4 B) _" R" ^* K7 Q( f
);& D0 N7 V; B' V& u- `
/*======================= x||X ========================*/ 
defun(X_CMD (@optional (valueX nil) (valueY nil))/ |% j/ b. \- k& C/ z# k3 a
        if(valueX == nil || valueY == nil
                then" P/ U$ o* I8 I: W2 A8 |! d- Z
                        axlUIWPrint(nil "Somethings Wrong")) f5 T$ _& D1 X2 f$ R4 I% x9 ^
                else: ?3 S. Y3 u9 B# H, l: M" |, u0 ^
                        x_scr(valueX valueY)
        );
);
defun( x_scr (valueX valueY)
        axlShell(sprintf(nil "pick %L %L" valueX valueY))8 C- M! l( R) t+ }9 f# Z! [
);