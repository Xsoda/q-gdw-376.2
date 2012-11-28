using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace MainUI
{
    public partial class Main : Form
    {
        const int SPM_GETPORTNAMES = 0x02;
        private Output op = new Output();
        public Main()
        {
            InitializeComponent();
            BTN_CLOSE_SERIAL_PORT.Enabled = false;
            // Initialize the serial port list
            IntPtr lpCount = Marshal.AllocCoTaskMem(sizeof(uint));
            IntPtr lpPortList = UserNativeFunction.SerialPort_Proc(SPM_GETPORTNAMES, lpCount, IntPtr.Zero);
            int count = Marshal.ReadInt32(lpCount);
            if (count != 0)
            {
                IntPtr com = lpPortList;
                for (uint i = 0; i < count; i++)
                {
                    CB_CHOSE_SERIAL_PORT.Items.Add(Marshal.PtrToStringAuto(Marshal.ReadIntPtr(com)));
                    com = (IntPtr)(com.ToInt64() + Marshal.SizeOf(com));
                }
                CB_CHOSE_SERIAL_PORT.SelectedItem = 0;
            }
            op.Height = tabPage1.Height;
            op.Width = tabPage1.Width - 210;            
            op.Show();
        }

        protected override void OnMove(EventArgs e)
        {
            base.OnMove(e);
            op.Left = this.Left + 210;
            op.Top = this.Top + this.Height - tabPage1.Height - STATUS.Height - menuStrip1.Height + 8;
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);
            op.Height = tabPage1.Height;
            op.Width = tabPage1.Width - 200;
            this.OnMove(e);
        }

        private void BTN_OPEN_SERIAL_PORT_Click(object sender, EventArgs e)
        {
            IntPtr serialport = new IntPtr();
            IntPtr command = Marshal.StringToHGlobalAnsi(Properties.Settings.Default.OpenSerial);
            int arg = 0;
            if (CB_CHOSE_SERIAL_PORT.SelectedItem != null)
            {
                serialport = Marshal.StringToHGlobalAnsi(CB_CHOSE_SERIAL_PORT.SelectedItem.ToString());                
                if (CB_BAUD.SelectedItem == null)
                    CB_BAUD.SelectedIndex = 5;
                arg |= CB_BAUD.SelectedIndex;
                arg <<= 8;
                if (CB_CHECK.SelectedItem == null)
                    CB_CHECK.SelectedIndex = 2;
                arg |= CB_CHECK.SelectedIndex;
                arg <<= 8;
                if (CB_FLOW_CONTROL.SelectedItem == null)
                    CB_FLOW_CONTROL.SelectedIndex = 0;
                arg |= CB_FLOW_CONTROL.SelectedIndex;
                if (UserNativeFunction.DispatchCommand(command, serialport, arg) == 1)
                {
                    CB_BAUD.Enabled = false;
                    CB_CHECK.Enabled = false;
                    CB_CHOSE_SERIAL_PORT.Enabled = false;
                    CB_FLOW_CONTROL.Enabled = false;
                    BTN_OPEN_SERIAL_PORT.Enabled = false;
                    BTN_CLOSE_SERIAL_PORT.Enabled = true;
                    STATUS_SERIAL_PORT.Text = "串口打开成功";
                }
                else
                    STATUS_SERIAL_PORT.Text = "串口打开失败";
            }
            else
            {
                MessageBox.Show("请选择一个串口~~", "错误", MessageBoxButtons.OKCancel, MessageBoxIcon.Warning);
            }
        }

        private void BTN_CLOSE_SERIAL_PORT_Click(object sender, EventArgs e)
        {
            IntPtr command = Marshal.StringToHGlobalAnsi(Properties.Settings.Default.CloseSerial);
            if (UserNativeFunction.DispatchCommand(command, IntPtr.Zero, 0) == 1)
            {
                CB_BAUD.Enabled = true;
                CB_CHECK.Enabled = true;
                CB_CHOSE_SERIAL_PORT.Enabled = true;
                CB_FLOW_CONTROL.Enabled = true;
                BTN_OPEN_SERIAL_PORT.Enabled = true;
                BTN_CLOSE_SERIAL_PORT.Enabled = false;
                STATUS_SERIAL_PORT.Text = "串口关闭成功";
            }
            else
                STATUS_SERIAL_PORT.Text = "串口关闭失败";
        }
    }
}
