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
        public extern static int DispatchCommand( IntPtr command, int arg );

        [DllImport( "serialport.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl )]
        public static extern int SerialPort_GetPortNames( ref IntPtr lpPortList, ref uint lpCount );

        [DllImport( "serialport.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl )]
        public static extern void SerialPort_FreePortNameList( IntPtr portList );
    }
    #endregion

    #region Windows API
    class WinNativeFunction
    {
    }
    #endregion
}
