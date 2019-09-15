using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;

namespace My_test_windows
{
    


    public partial class Form1 : Form
    {
        Thread thread;
        RealTimeImageMaker rti;//= new RealTimeImageMaker();
        RealTimeImageMaker my_data;
        Color backColor = Color.Black;//指定绘制曲线图的背景色 
        bool show_pwm = false;
        bool show_temp = false;


        private void setPoints()//在panel上绘制坐标及坐标轴
        {
            #region 画坐标轴线
            Graphics curveGph1 = panel1.CreateGraphics();
            Pen curvePen1 = new Pen(Color.White,1);
            curvePen1.DashStyle = System.Drawing.Drawing2D.DashStyle.Dot;
            Point point = new Point(22,10);

            for (int locx = 20; locx < 610;)//画竖线
            {
                curveGph1.DrawLine(curvePen1, locx, point.Y, locx, 370);
                locx += 60;
            }
            for (int locy = 10; locy < 410;)//画横线
            {
                curveGph1.DrawLine(curvePen1, point.X, locy, 560, locy);
                locy += 40;
            }
            curveGph1.Dispose();
            curvePen1.Dispose();
            #endregion

            #region 画坐标值
            Graphics curveGph2 = panel1.CreateGraphics();
            Font font1 = new Font("楷体", 12, FontStyle.Italic);
            Font font2 = new Font("楷体", 12, FontStyle.Italic);
            curveGph2.DrawString("0", font2, new SolidBrush(Color.Red), 10, 410);
            for (double locx = 0; locx <= 432; locx += 48)
            {
                curveGph2.DrawString(locx.ToString(), font1, new SolidBrush(Color.Red), (float)locx * 5/4, 240);
            }
            curveGph2.DrawString("(T/s)", font2, new SolidBrush(Color.Red), (float)10.8*50, 260);

            curveGph2.DrawString("-15", font2, new SolidBrush(Color.Red), 1, 400 - 40);
            curveGph2.DrawString("-10", font2, new SolidBrush(Color.Red), 1, 400 - 80);
            curveGph2.DrawString("-5", font2, new SolidBrush(Color.Red), 1, 400 - 120);
            curveGph2.DrawString("0", font2, new SolidBrush(Color.Red), 1, 400 - 160);
            curveGph2.DrawString("5", font2, new SolidBrush(Color.Red), 1, 400 - 200);
            curveGph2.DrawString("10", font2, new SolidBrush(Color.Red), 1, 400 - 240);
            curveGph2.DrawString("15", font2, new SolidBrush(Color.Red), 1, 400 - 280);
            curveGph2.DrawString("20", font2, new SolidBrush(Color.Red), 1, 400 - 320);
            curveGph2.DrawString("25", font2, new SolidBrush(Color.Red), 1, 400 - 360);
            curveGph2.DrawString("30 (t/°C)", font2, new SolidBrush(Color.Red), 1, 400 - 400);


            curveGph2.Dispose();
            #endregion
        }
        private void Run()
        {
            
            while (true)                                         //开始标志位
                {
                if (show_line_F)
                    try
                    {
                        Image image = rti.GetCurrentCurve();     //新建图片对象
                        Graphics g = panel1.CreateGraphics();    //在panel上新建GDI
                        g.DrawImage(image, 0, 0);                //绘制图片
                        this.setPoints();                        //刷新图像第一点
                        g.Dispose();                             //释放GDI
                                                                //每0.8秒钟刷新一次（线程以毫秒为单位）  
                        Thread.Sleep(800);
                    }
                    catch (Exception) { }
                else
                { }
                }
        }
        public Form1()
        {

            InitializeComponent();
            rti = new RealTimeImageMaker(Width, Height, backColor, Color.Green);
            thread = new Thread(new ThreadStart(Run));
            thread.Start();
        }
        //定义标志变量
        bool serial_start_F = false;
        bool mcu_ready_F = false;
        bool start_F = false;
        bool show_data_F = false;
        bool show_line_F = false;
        //定义工作函数
        byte[] sendcontrol = new byte[] {0x5A,0x5A,0x00,0x00,0x9A,0x00,0x00,0x00,0x00,0xA5}; //2字节标记位+1字节指令+2字节温度+1字节P+1字节I+1字节D+1字节T+1字节标记
        


        private void Form1_Load(object sender, EventArgs e)
        {
            portName_cbx.Items.Add("COM1");
            portName_cbx.Items.Add("COM2");
            portName_cbx.Items.Add("COM3");
            portName_cbx.Items.Add("COM4");
            portName_cbx.Items.Add("COM5");
            portName_cbx.Items.Add("COM6");
            portName_cbx.Items.Add("COM7");
            portName_cbx.Items.Add("COM8");
            portName_cbx.Items.Add("COM9");
            portName_cbx.Items.Add("COM10");
            portName_cbx.Text = "COM1";

            baudRate_cbx.Items.Add("9600");
            baudRate_cbx.Items.Add("76800");
            baudRate_cbx.Items.Add("115200");
            baudRate_cbx.Items.Add("921600");
            baudRate_cbx.Items.Add("1382400");
            baudRate_cbx.Text = "4800";

            textBox1.Text = "0";
            textBox2.Text = "0";
            textBox3.Text = "0";
            textBox4.Text = "0";
            textBox5.Text = "25.0";
            bool existPort_f = false;
            richTextBox1.AppendText("可用串口：");
            string[] portNames = SerialPort.GetPortNames();
            foreach (string str in portNames)
            {
                richTextBox1.AppendText(str);
                existPort_f = true;
            }
            richTextBox1.AppendText("\n");
            if (existPort_f) { serialPort1.PortName = portNames[0]; portName_cbx.Text = portNames[0]; }
            else serialPort1.PortName = portName_cbx.Text;
            serialPort1.BaudRate = int.Parse(baudRate_cbx.Text);
            serialPort1.DataBits = 8;
            serialPort1.StopBits = StopBits.One;
            serialPort1.Parity = Parity.None;
            serialPort1.ReadBufferSize = 1024*1024;              //设置串口读缓冲区大小数值
            serialPort1.WriteBufferSize = 1024*1024;             //设置串口写缓冲区大小数值
            serialPort1.ReceivedBytesThreshold = 10;              //引发DataReceived事件时输入缓冲区字节数
        }

        bool data_btn5 = true;
        private void button5_Click(object sender, EventArgs e)
        {
            if (data_btn5)
            {
                show_data_F = true;
                data_btn5 = false;
                richTextBox1.AppendText("显示数据...\n");
            }
            else
            {
                show_data_F = false;
                data_btn5 = true;
                richTextBox1.AppendText("隐藏数据...\n");
            }
           
        }
        public void buttonUnlock()
        {
            ready_btn.Enabled = true;
            start_btn.Enabled = true;
            showTemp_btn.Enabled = true;
            showLine_btn.Enabled = true;
            showPwm_btn.Enabled = true;
            setParameter_btn.Enabled = true;
            default_btn.Enabled = true;
            clearLine_btn.Enabled = true;
            baudRate_cbx.Enabled = false;
            portName_cbx.Enabled = false;
        }
        private void buttonLock()
        {
            ready_btn.Enabled = false;
            start_btn.Enabled = false;
            showTemp_btn.Enabled = false;
            showPwm_btn.Enabled = false;
            setParameter_btn.Enabled = false;
            clearLine_btn.Enabled = false;
            default_btn.Enabled = false;
            showLine_btn.Enabled = false;
            baudRate_cbx.Enabled = true;
            portName_cbx.Enabled = true;
            ready_btn.BackColor = Color.Red;
            start_btn.BackColor = Color.Red;
            showTemp_btn.BackColor = Color.Red;
            showPwm_btn.BackColor = Color.Red;
        }
        private void button1_Click(object sender, EventArgs e)
        {
            if (!serialPort1.IsOpen)
            {
                try
                {

                    serialPort1.Open();
                    openSerial_btn.Text = "关闭串口";
                    openSerial_btn.BackColor = Color.Lime;
                    serial_start_F = true;
                    richTextBox1.AppendText("串口："+serialPort1.PortName.ToString()+"已经打开！！！\n");
                    buttonUnlock();
                }
                catch
                {
                    MessageBox.Show("串口" + serialPort1.PortName + "打开失败了！\n\n可能是串口已被占用。");
                }
            }
            else
            {
                serialPort1.Close();
                openSerial_btn.Text = "打开串口";
                openSerial_btn.BackColor = Color.Red;
                richTextBox1.AppendText("串口：" + serialPort1.PortName.ToString() + "已被关闭！！！\n");
                buttonLock();
                thread.Abort();
                serial_start_F = false;

            }
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            
            Graphics g = e.Graphics;
            Pen p = new Pen(Color.Lime, 6);
            
            //定义笔的颜色为蓝色，大小为2
            g.DrawRectangle(p, 10, 10, 610, 410);
            g.Dispose();//释放由Graphics使用的资源
        }

        private void textBox1_TextChanged_1(object sender, EventArgs e)
        {

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            serialPort1.PortName = portName_cbx.Text;
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            serialPort1.BaudRate = int.Parse(baudRate_cbx.Text);
        }
        public static string byteToHexStr(byte[] bytes)
        {
            string returnStr = "";
            if (bytes != null)
            {
                for (int i = 0; i < bytes.Length; i++)
                {
                    returnStr += "0x"+bytes[i].ToString("X2") + " ";
                }
            }
            returnStr += "\n";
            return returnStr;
        }
        public static int pre_data = 0;
        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            Byte[] readBytes = new Byte[10];
            int SDataTemp = this.serialPort1.Read(readBytes, 0, 10);
            
                this.richTextBox1.Invoke(
                    new MethodInvoker(
                        delegate
                        {
                            
                            if ((readBytes[0] == 0xAA) && (readBytes[9] == 0xAA))
                            {
                                if(readBytes[2] == 0x01)richTextBox1.AppendText("连接成功...\n");
                                else richTextBox1.AppendText("断开成功...\n");
                                start_F = true;
                                this.richTextBox1.AppendText(byteToHexStr(readBytes));
                                //   showFlag = false;
                            }
                            else if ((readBytes[0] == 0x5A) && (readBytes[9] == 0xA5))//(showFlag)
                            {

                                //byteToHexStr(readBytes);
                                 if (readBytes[2] == 0x02)
                                 {
                                    richTextBox1.AppendText("开始成功...\n");
                                 }
                                 else if (readBytes[2] == 0x03)
                                 {
                                     //start_F = false;
                                     richTextBox1.AppendText("结束成功...\n");
                                 }

                                 else if (readBytes[2] == 0x04)
                                 {
                                     richTextBox1.AppendText("设置成功...\n");
                                 }//start_F = false;
                                 else { }
                                 
                                 if (show_data_F)
                                this.richTextBox1.AppendText(byteToHexStr(readBytes));
                                pre_data = my_data.T_num;
                                my_data.T_num = ((int)readBytes[3] << 8 | (int)readBytes[4]);
                                if (my_data.T_num == 0) my_data.T_num = pre_data;
                                if (show_temp)
                                {
                                    // this.richTextBox1.AppendText(my_data.T_num.ToString());
                                    label20.Text = ((float)my_data.T_num / 10).ToString();
                                }
                                else
                                { show_temp = false; label20.Text = "0.00"; }
                                if (show_pwm)
                                {
                                    progressBar1.Value = readBytes[8];
                                    label21.Text = readBytes[8].ToString();
                                }
                            else progressBar1.Value = 0;
                                
                                //this.richTextBox1.AppendText(Encoding.GetEncoding("gb2312").GetString(readBytes));

                                richTextBox1.SelectionStart = richTextBox1.TextLength;
                            }
                            serialPort1.DiscardInBuffer();
                        }
                )
                );  
        }
        private void button4_Click(object sender, EventArgs e)
        {
            richTextBox1.Clear();
            
            richTextBox1.AppendText("清除数据...\n");
        }
        byte btn10_f = 0;
        private void button10_Click(object sender, EventArgs e)
        {
            if(btn10_f == 0)
            {
                showPwm_btn.BackColor = Color.Lime;
                showPwm_btn.Text = "断开实时PWM";
                btn10_f = 1;
                show_pwm = true;
                richTextBox1.AppendText("显示实时PWM...\n");
            }
            else
            {
                showPwm_btn.BackColor = Color.Red;
                showPwm_btn.Text = "显示实时PWM";
                btn10_f = 0;
                show_pwm = false;
                richTextBox1.AppendText("断开实时PWM...\n");
            }
        }

        byte btn3_f = 0;
        bool start = false;
        private void button3_Click(object sender, EventArgs e)
        {
            
            if (serial_start_F)
            {

                if (btn3_f == 0)
                {
                    ready_btn.BackColor = Color.Lime;
                    ready_btn.Text = "断开连接";
                    btn3_f = 1;
                    sendcontrol[2] = 0x01;
                    start = true;
                    serialPort1.Write(sendcontrol, 0, sendcontrol.Length);
                    richTextBox1.AppendText("请求连接...\n");
                }
                else
                {
                    ready_btn.BackColor = Color.Red;
                    ready_btn.Text = "请求连接";
                    btn3_f = 0;
                    sendcontrol[2] = 0x00;
                    start = false;
                    serialPort1.Write(sendcontrol, 0, sendcontrol.Length);
                    richTextBox1.AppendText("断开连接...\n");
                }
                if (mcu_ready_F)
                {
                    start_F = true;
                }
                else start_F = false;
            }
            else
            {
                MessageBox.Show("串口未打开，不能操作!!!!");
            }
            
        }
        byte btn7_f = 0;
        private void button7_Click(object sender, EventArgs e)
        {
            if (start_F && start )
            {

                if (btn7_f == 0)
                {
                    sendcontrol[2] = 0x02;

                    serialPort1.Write(sendcontrol, 0, sendcontrol.Length);
                    start_btn.BackColor = Color.Lime;
                    start_btn.Text = "结束运行";
                    btn7_f = 1;
                    richTextBox1.AppendText("开始运行...\n");
                }
                else
                {
                    sendcontrol[2] = 0x03;

                    serialPort1.Write(sendcontrol, 0, sendcontrol.Length);
                    start_btn.BackColor = Color.Red;
                    start_btn.Text = "开始运行";
                    btn7_f = 0;
                    richTextBox1.AppendText("结束运行...\n");
                }
            }
            else
            {
                MessageBox.Show("下位机为准备好，不可以开始运行！！！");
            }

        }
        byte btn11_f = 0;
        private void button11_Click(object sender, EventArgs e)
        {
            if(btn11_f == 0)
            {
                showTemp_btn.BackColor = Color.Lime;
                showTemp_btn.Text = "断开实时温度";
                btn11_f = 1;
                show_temp = true;
                richTextBox1.AppendText("显示实时温度...\n");
            }
            else
            {
                showTemp_btn.BackColor = Color.Red;
                showTemp_btn.Text = "显示实时温度";
                btn11_f = 0;
                show_temp = false;
                richTextBox1.AppendText("断开实时温度...\n");
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            show_line_F = true;
            richTextBox1.AppendText("显示曲线...\n");
            sendcontrol[2] = 0x03;
            serialPort1.Write(sendcontrol,0,10);
        }

        private void button8_Click(object sender, EventArgs e)
        {
            show_line_F = false;
            richTextBox1.AppendText("暂停显示...\n");

        }

        private void default_btn_Click(object sender, EventArgs e)
        {
            textBox3.Text = "0";
            textBox2.Text = "0";
            textBox1.Text = "0";
            textBox4.Text = "0";
            textBox5.Text = "25.0";

            richTextBox1.AppendText("恢复默认...\n");
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {
            rti = new RealTimeImageMaker(panel1.Width, panel1.Height, Color.Black, Color.Blue);//实例化一幅图片
            my_data = rti;
            thread = new Thread(new ThreadStart(Run));//实例化一个线程（调用Run方法）
            Control.CheckForIllegalCrossThreadCalls = false;
            thread.Start();//开始线程
            this.setPoints();

        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            //在窗体即将关闭之前中止线程  
            thread.Abort();
            System.Environment.Exit(0);
        }

        private void setParameter_btn_Click(object sender, EventArgs e)
        {
            sendcontrol[2] = 0x04;
            sendcontrol[3] = (byte)((int)(float.Parse(textBox5.Text) * 10) >> 8);
            sendcontrol[4] = (byte)((int)(float.Parse(textBox5.Text) * 10));
            sendcontrol[5] = (byte.Parse(textBox3.Text));
            sendcontrol[6] = (byte.Parse(textBox2.Text));
            sendcontrol[7] = (byte.Parse(textBox1.Text));
            sendcontrol[8] = (byte.Parse(textBox4.Text));
            serialPort1.Write(sendcontrol, 0, sendcontrol.Length);
            richTextBox1.AppendText("设置参数...\n");
        }
    }
    
}

