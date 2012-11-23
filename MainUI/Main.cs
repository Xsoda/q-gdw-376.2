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
            op.Top = this.Top + this.Height - tabPage1.Height - statusStrip1.Height - menuStrip1.Height + 8;
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
                arg <<= 2;
                if (CB_CHECK.SelectedItem == null)
                    CB_CHECK.SelectedIndex = 2;
                arg |= CB_CHECK.SelectedIndex;
                arg <<= 2;
                if (CB_FLOW_CONTROL.SelectedItem == null)
                    CB_FLOW_CONTROL.SelectedIndex = 0;
                arg |= CB_FLOW_CONTROL.SelectedIndex;
                UserNativeFunction.DispatchCommand(command, serialport, arg);
            }
            else
            {
                MessageBox.Show("请选择一个串口~~", "错误", MessageBoxButtons.OKCancel, MessageBoxIcon.Warning);
            }
        }

        private void BTN_CLOSE_SERIAL_PORT_Click(object sender, EventArgs e)
        {
            IntPtr command = Marshal.StringToHGlobalAnsi(Properties.Settings.Default.CloseSerial);
            UserNativeFunction.DispatchCommand(command, IntPtr.Zero, 0);
        }
    }
}
