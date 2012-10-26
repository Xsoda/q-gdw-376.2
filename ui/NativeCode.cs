using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace ui
{
    class UserNativeCode
    {
        [DllImport( "cbridge.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl )]
        public extern static int InitializeComponent();

        [DllImport( "cbridge.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl )]
        public extern static int ReleaseComponent();

        [DllImport( "cbridge.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl )]
        public extern static int DispatchCommand( uint command, int arg );
    }
    class Win32NativeCode
    {
        [DllImport( "kernel32.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.StdCall )]
        public static extern uint WaitForMutipleObjects();
    }
}
