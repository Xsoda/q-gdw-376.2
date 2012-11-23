using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace MainUI
{
    #region User define function
    class UserNativeFunction
    {
        [DllImport( "cbridge.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl )]
        public extern static int InitializeComponent();

        [DllImport( "cbridge.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl )]
        public extern static int ReleaseComponent();

        [DllImport( "cbridge.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl )]
        public extern static int DispatchCommand( IntPtr command, IntPtr arg1, int arg2 );

        [DllImport( "serialport.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl )]
        public static extern IntPtr SerialPort_Proc( int msg, IntPtr lParam, IntPtr wparam );

    }
    #endregion

    #region Windows API
    class WinNativeFunction
    {
    }
    #endregion
}
