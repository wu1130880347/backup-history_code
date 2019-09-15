/* 调试 */
/* 提供printf子程序 */

#include	"DEBUG.H"

/* 检查操作状态,如果错误则显示错误代码并停机,应该替换为实际的处理措施,例如显示错误信息,等待用户确认后重试等 */
void	mStopIfError( UINT8 iError )
{
	if ( iError == USB_INT_SUCCESS ) return;  /* 操作成功 */
	while ( 1 ) {
            PGout(5)=1;
		mDelaymS( 200 );
PGout(5)=0;
		mDelaymS( 200 );
	}
}

/* 为printf和getkey输入输出初始化串口 */
void	mInitSTDIO( void )
{
}
