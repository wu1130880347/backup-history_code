using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ZWSD_NameRule
{
    
    public partial class Form1 : Form
    {
        string nameStr = "ZWXXS0CA002CF0R";
        NameSpace NS = new NameSpace();
        public Form1()
        {
            InitializeComponent();
            

        }
        static public void RefreshWin()
        {

        }
        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged_1(object sender, EventArgs e)
        {

        }

        private void textBox5_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox4_TextChanged(object sender, EventArgs e)
        {

        }
    }

    public class NameSpace
    {
        string[] type1 = new string[] { "ZW" };
        string [] type2= new string[] { "0R","0C","0L" };
        string[] type3 = new string[] { "SMD", "DIP" };
        string[] type4 = new string[] { "0402", "0603","0805","1206" };
        string[] type5 = new string[] { "金属膜" ,"陶瓷","电解"};
        string[] type6 = new string[] { "1/16W", "1/10W", "1/8W","1/6W","1/4W" };

    }
}
