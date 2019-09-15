using System;  
using System.Collections.Generic;  
using System.Text;  
using System.Drawing;  
using System.Drawing.Imaging;  
  
namespace My_test_windows
{
    /// 说明：实时图片生成类，在本例中横向坐标上每个像素都会有一个控制点  

    public class RealTimeImageMaker
    {
        private int width;         //要生成曲线图的宽度
        private int height;        //要生成曲线图的高度
        private int N = 560;       //取560个关键点
        private int M = 1;         //每M个像素点取一个关键点
        private Point[] pointList; //用来绘制曲线的关键点，依次将这些点连接起来即得到曲线
        private Bitmap currentImage;//当前要绘制的图片
        private Color backColor;    //图片背景色
        private Color foreColor;    //图片前景色
        public int T_num = 0;       //温度数据
        

        int sinCode = 250;    //初始化默认值
        //图像高度
            private int Height
        {
            get { return height; }
            set { height = value; }
        }
        //图像宽度
        private int Width
        {
            get { return width; }
            set { width = value; }
        }
        //构造函数1
        public RealTimeImageMaker(int width, int height)
            : this(width, height, Color.Gray, Color.Blue) { }

       //构造函数2
        public RealTimeImageMaker(int width, int height, Color backColor, Color foreColor)
        {
            this.width = width;
            this.height = height;
            this.backColor = backColor;
            this.foreColor = foreColor;
            pointList = new Point[N];
            Point tempPoint;

            //初始化曲线上所有的点
            for (int i = 22; i < N; i++)
            {
                tempPoint = new Point();
                tempPoint.X = i * M;
                tempPoint.Y = sinCode;
                pointList[i] = tempPoint;
            }
        }

        public Image GetCurrentCurve()                //将曲线绘制成图片的具体方法
        {
            currentImage = new Bitmap(width, height);//实例化一个图片
            Point p;
            Graphics g = Graphics.FromImage(currentImage);
            g.DrawCurve(new Pen(backColor), pointList);
            //将当前定位曲线图的坐标点后移，并且将横坐标加1，  
            //这样做的效果相当于移除当前最后一个点  
            for (int i = N - 1; i > 0; i--)
            {
                p = pointList[i - 1];
                pointList[i] = new Point(p.X + M, p.Y);
            }
            Point newPoint = new Point();
            //新生成曲线图定位点的第一个点的坐标  
            newPoint.X = 0;
            //曲线上每个点的纵坐标随机生成，但保证在显示区域之内  
            newPoint.Y = (int)(-0.8*(this.T_num) + 250);
            //在最后再添加一个新坐标点  
            pointList[0] = newPoint;
            Point point = new Point(0, 0);//表示在二维平面中定义点的整数 X 和 Y 坐标的有序对。
            g.Clear(backColor);
            //绘制曲线图
            g.DrawCurve(new Pen(foreColor), pointList);
            g.Dispose();
            return currentImage;
        }
    }
}