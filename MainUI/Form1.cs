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
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            // Initialize the serial port list
            IntPtr lpPortList = new IntPtr();
            uint count = new uint();
            UserNativeFunction.SerialPort_GetPortNames( ref lpPortList, ref count );
            if (count != 0)
            {
                IntPtr com = lpPortList;
                for (uint i = 0; i < count; i++)
                {
                    cb_serialport.Items.Add(Marshal.PtrToStringAuto(Marshal.ReadIntPtr(com)));
                    com = (IntPtr)(com.ToInt64() + Marshal.SizeOf(com));
                }
                //UserNativeFunction.SerialPort_FreePortNameList(lpPortList);
                cb_serialport.SelectedItem = 0;
            }
        }

        private void btn_openserial_Click( object sender, EventArgs e )
        {
            IntPtr serialport = new IntPtr();
            IntPtr command = Marshal.StringToHGlobalAnsi( Properties.Settings.Default.OpenSerial );
            if ( cb_serialport.SelectedItem != null )
            {
                serialport = Marshal.StringToHGlobalAnsi( cb_serialport.SelectedItem.ToString() );
                UserNativeFunction.DispatchCommand( command, serialport, 0 );
            }
            else
            {
                MessageBox.Show( "请选择一个串口~~", "错误", MessageBoxButtons.OKCancel, MessageBoxIcon.Warning );
                UserNativeFunction.DispatchCommand(Marshal.StringToHGlobalAnsi("OPEN"), Marshal.StringToHGlobalAnsi("COM0"), 0);
            }
        }
    }
}
