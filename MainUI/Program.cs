using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace MainUI
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            UserNativeFunction.InitializeComponent();
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault( false );
            Application.Run( new Main() );
            UserNativeFunction.ReleaseComponent();
        }
    }
}
