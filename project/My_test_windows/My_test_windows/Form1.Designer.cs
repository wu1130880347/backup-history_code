namespace My_test_windows
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.openSerial_btn = new System.Windows.Forms.Button();
            this.showLine_btn = new System.Windows.Forms.Button();
            this.ready_btn = new System.Windows.Forms.Button();
            this.showData_btn = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.label3 = new System.Windows.Forms.Label();
            this.labe14 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.portName_cbx = new System.Windows.Forms.ComboBox();
            this.baudRate_cbx = new System.Windows.Forms.ComboBox();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.clearData_btn = new System.Windows.Forms.Button();
            this.default_btn = new System.Windows.Forms.Button();
            this.start_btn = new System.Windows.Forms.Button();
            this.clearLine_btn = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.setParameter_btn = new System.Windows.Forms.Button();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.showPwm_btn = new System.Windows.Forms.Button();
            this.showTemp_btn = new System.Windows.Forms.Button();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.label14 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            this.label20 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.label21 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.SuspendLayout();
            // 
            // openSerial_btn
            // 
            resources.ApplyResources(this.openSerial_btn, "openSerial_btn");
            this.openSerial_btn.BackColor = System.Drawing.Color.Red;
            this.openSerial_btn.Name = "openSerial_btn";
            this.openSerial_btn.UseVisualStyleBackColor = false;
            this.openSerial_btn.Click += new System.EventHandler(this.button1_Click);
            // 
            // showLine_btn
            // 
            resources.ApplyResources(this.showLine_btn, "showLine_btn");
            this.showLine_btn.Name = "showLine_btn";
            this.showLine_btn.UseVisualStyleBackColor = true;
            this.showLine_btn.Click += new System.EventHandler(this.button2_Click);
            // 
            // ready_btn
            // 
            resources.ApplyResources(this.ready_btn, "ready_btn");
            this.ready_btn.BackColor = System.Drawing.Color.Red;
            this.ready_btn.Name = "ready_btn";
            this.ready_btn.UseVisualStyleBackColor = false;
            this.ready_btn.Click += new System.EventHandler(this.button3_Click);
            // 
            // showData_btn
            // 
            resources.ApplyResources(this.showData_btn, "showData_btn");
            this.showData_btn.Name = "showData_btn";
            this.showData_btn.UseVisualStyleBackColor = true;
            this.showData_btn.Click += new System.EventHandler(this.button5_Click);
            // 
            // label1
            // 
            resources.ApplyResources(this.label1, "label1");
            this.label1.Name = "label1";
            // 
            // label2
            // 
            resources.ApplyResources(this.label2, "label2");
            this.label2.Name = "label2";
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 50;
            // 
            // label3
            // 
            resources.ApplyResources(this.label3, "label3");
            this.label3.Name = "label3";
            // 
            // labe14
            // 
            resources.ApplyResources(this.labe14, "labe14");
            this.labe14.Name = "labe14";
            // 
            // label5
            // 
            resources.ApplyResources(this.label5, "label5");
            this.label5.Name = "label5";
            // 
            // label6
            // 
            resources.ApplyResources(this.label6, "label6");
            this.label6.Name = "label6";
            // 
            // label7
            // 
            resources.ApplyResources(this.label7, "label7");
            this.label7.Name = "label7";
            // 
            // label8
            // 
            resources.ApplyResources(this.label8, "label8");
            this.label8.Name = "label8";
            // 
            // portName_cbx
            // 
            resources.ApplyResources(this.portName_cbx, "portName_cbx");
            this.portName_cbx.FormattingEnabled = true;
            this.portName_cbx.Name = "portName_cbx";
            this.portName_cbx.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // baudRate_cbx
            // 
            resources.ApplyResources(this.baudRate_cbx, "baudRate_cbx");
            this.baudRate_cbx.FormattingEnabled = true;
            this.baudRate_cbx.Name = "baudRate_cbx";
            this.baudRate_cbx.SelectedIndexChanged += new System.EventHandler(this.comboBox2_SelectedIndexChanged);
            // 
            // serialPort1
            // 
            this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort1_DataReceived);
            // 
            // richTextBox1
            // 
            resources.ApplyResources(this.richTextBox1, "richTextBox1");
            this.richTextBox1.Name = "richTextBox1";
            // 
            // clearData_btn
            // 
            resources.ApplyResources(this.clearData_btn, "clearData_btn");
            this.clearData_btn.Name = "clearData_btn";
            this.clearData_btn.UseVisualStyleBackColor = true;
            this.clearData_btn.Click += new System.EventHandler(this.button4_Click);
            // 
            // default_btn
            // 
            resources.ApplyResources(this.default_btn, "default_btn");
            this.default_btn.Name = "default_btn";
            this.default_btn.UseVisualStyleBackColor = true;
            this.default_btn.Click += new System.EventHandler(this.default_btn_Click);
            // 
            // start_btn
            // 
            resources.ApplyResources(this.start_btn, "start_btn");
            this.start_btn.BackColor = System.Drawing.Color.Red;
            this.start_btn.Name = "start_btn";
            this.start_btn.UseVisualStyleBackColor = false;
            this.start_btn.Click += new System.EventHandler(this.button7_Click);
            // 
            // clearLine_btn
            // 
            resources.ApplyResources(this.clearLine_btn, "clearLine_btn");
            this.clearLine_btn.Name = "clearLine_btn";
            this.clearLine_btn.UseVisualStyleBackColor = true;
            this.clearLine_btn.Click += new System.EventHandler(this.button8_Click);
            // 
            // textBox1
            // 
            resources.ApplyResources(this.textBox1, "textBox1");
            this.textBox1.Name = "textBox1";
            // 
            // textBox2
            // 
            resources.ApplyResources(this.textBox2, "textBox2");
            this.textBox2.Name = "textBox2";
            // 
            // textBox3
            // 
            resources.ApplyResources(this.textBox3, "textBox3");
            this.textBox3.Name = "textBox3";
            // 
            // setParameter_btn
            // 
            resources.ApplyResources(this.setParameter_btn, "setParameter_btn");
            this.setParameter_btn.Name = "setParameter_btn";
            this.setParameter_btn.UseVisualStyleBackColor = true;
            this.setParameter_btn.Click += new System.EventHandler(this.setParameter_btn_Click);
            // 
            // label9
            // 
            resources.ApplyResources(this.label9, "label9");
            this.label9.Name = "label9";
            // 
            // label10
            // 
            resources.ApplyResources(this.label10, "label10");
            this.label10.Name = "label10";
            // 
            // label11
            // 
            resources.ApplyResources(this.label11, "label11");
            this.label11.Name = "label11";
            // 
            // showPwm_btn
            // 
            resources.ApplyResources(this.showPwm_btn, "showPwm_btn");
            this.showPwm_btn.BackColor = System.Drawing.Color.Red;
            this.showPwm_btn.Name = "showPwm_btn";
            this.showPwm_btn.UseVisualStyleBackColor = false;
            this.showPwm_btn.Click += new System.EventHandler(this.button10_Click);
            // 
            // showTemp_btn
            // 
            resources.ApplyResources(this.showTemp_btn, "showTemp_btn");
            this.showTemp_btn.BackColor = System.Drawing.Color.Red;
            this.showTemp_btn.Name = "showTemp_btn";
            this.showTemp_btn.UseVisualStyleBackColor = false;
            this.showTemp_btn.Click += new System.EventHandler(this.button11_Click);
            // 
            // textBox4
            // 
            resources.ApplyResources(this.textBox4, "textBox4");
            this.textBox4.Name = "textBox4";
            // 
            // label12
            // 
            resources.ApplyResources(this.label12, "label12");
            this.label12.Name = "label12";
            // 
            // label13
            // 
            resources.ApplyResources(this.label13, "label13");
            this.label13.Name = "label13";
            // 
            // textBox5
            // 
            resources.ApplyResources(this.textBox5, "textBox5");
            this.textBox5.Name = "textBox5";
            // 
            // label14
            // 
            resources.ApplyResources(this.label14, "label14");
            this.label14.ForeColor = System.Drawing.Color.Red;
            this.label14.Name = "label14";
            // 
            // label15
            // 
            resources.ApplyResources(this.label15, "label15");
            this.label15.Name = "label15";
            // 
            // label16
            // 
            resources.ApplyResources(this.label16, "label16");
            this.label16.Name = "label16";
            // 
            // label17
            // 
            resources.ApplyResources(this.label17, "label17");
            this.label17.Name = "label17";
            // 
            // label18
            // 
            resources.ApplyResources(this.label18, "label18");
            this.label18.Name = "label18";
            // 
            // label19
            // 
            resources.ApplyResources(this.label19, "label19");
            this.label19.ForeColor = System.Drawing.Color.Fuchsia;
            this.label19.Name = "label19";
            // 
            // label20
            // 
            resources.ApplyResources(this.label20, "label20");
            this.label20.ForeColor = System.Drawing.Color.Red;
            this.label20.Name = "label20";
            // 
            // label4
            // 
            resources.ApplyResources(this.label4, "label4");
            this.label4.Name = "label4";
            // 
            // progressBar1
            // 
            resources.ApplyResources(this.progressBar1, "progressBar1");
            this.progressBar1.Name = "progressBar1";
            // 
            // label21
            // 
            resources.ApplyResources(this.label21, "label21");
            this.label21.ForeColor = System.Drawing.Color.Fuchsia;
            this.label21.Name = "label21";
            // 
            // panel1
            // 
            resources.ApplyResources(this.panel1, "panel1");
            this.panel1.BackColor = System.Drawing.Color.Black;
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel1.Name = "panel1";
            this.panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            // 
            // Form1
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.AutoValidate = System.Windows.Forms.AutoValidate.EnablePreventFocusChange;
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.label21);
            this.Controls.Add(this.progressBar1);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label20);
            this.Controls.Add(this.label19);
            this.Controls.Add(this.label18);
            this.Controls.Add(this.label17);
            this.Controls.Add(this.label16);
            this.Controls.Add(this.label15);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.textBox5);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.textBox4);
            this.Controls.Add(this.showTemp_btn);
            this.Controls.Add(this.showPwm_btn);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.setParameter_btn);
            this.Controls.Add(this.textBox3);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.clearLine_btn);
            this.Controls.Add(this.start_btn);
            this.Controls.Add(this.default_btn);
            this.Controls.Add(this.richTextBox1);
            this.Controls.Add(this.baudRate_cbx);
            this.Controls.Add(this.portName_cbx);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.labe14);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.showData_btn);
            this.Controls.Add(this.clearData_btn);
            this.Controls.Add(this.ready_btn);
            this.Controls.Add(this.showLine_btn);
            this.Controls.Add(this.openSerial_btn);
            this.Cursor = System.Windows.Forms.Cursors.Arrow;
            this.ForeColor = System.Drawing.Color.RoyalBlue;
            this.Name = "Form1";
            this.Tag = "  ";
            this.TransparencyKey = System.Drawing.Color.Silver;
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button openSerial_btn;
        private System.Windows.Forms.Button showLine_btn;
        private System.Windows.Forms.Button ready_btn;
        private System.Windows.Forms.Button showData_btn;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label labe14;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.ComboBox portName_cbx;
        private System.Windows.Forms.ComboBox baudRate_cbx;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Button clearData_btn;
        private System.Windows.Forms.Button default_btn;
        private System.Windows.Forms.Button start_btn;
        private System.Windows.Forms.Button clearLine_btn;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.Button setParameter_btn;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Button showPwm_btn;
        private System.Windows.Forms.Button showTemp_btn;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.TextBox textBox5;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.Label label21;
        public System.Windows.Forms.Panel panel1;
    }
}

