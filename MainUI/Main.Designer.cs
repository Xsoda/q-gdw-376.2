namespace MainUI
{
    partial class Main
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose( bool disposing )
        {
            if ( disposing && ( components != null ) )
            {
                components.Dispose();
            }
            base.Dispose( disposing );
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.CB_FLOW_CONTROL = new System.Windows.Forms.ComboBox();
            this.CB_CHECK = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.CB_BAUD = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.BTN_CLOSE_SERIAL_PORT = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.CB_CHOSE_SERIAL_PORT = new System.Windows.Forms.ComboBox();
            this.BTN_OPEN_SERIAL_PORT = new System.Windows.Forms.Button();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.文件ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.退出ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.设置ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.串口参数设置ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.数据抄读项设置ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.关于ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.STATUS = new System.Windows.Forms.StatusStrip();
            this.STATUS_SERIAL_PORT = new System.Windows.Forms.ToolStripStatusLabel();
            this.tableLayoutPanel1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.STATUS.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.tabControl1, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.menuStrip1, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.STATUS, 0, 2);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 3;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.Size = new System.Drawing.Size(727, 387);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(3, 27);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(721, 335);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.CB_FLOW_CONTROL);
            this.tabPage1.Controls.Add(this.CB_CHECK);
            this.tabPage1.Controls.Add(this.label4);
            this.tabPage1.Controls.Add(this.label3);
            this.tabPage1.Controls.Add(this.CB_BAUD);
            this.tabPage1.Controls.Add(this.label2);
            this.tabPage1.Controls.Add(this.BTN_CLOSE_SERIAL_PORT);
            this.tabPage1.Controls.Add(this.label1);
            this.tabPage1.Controls.Add(this.CB_CHOSE_SERIAL_PORT);
            this.tabPage1.Controls.Add(this.BTN_OPEN_SERIAL_PORT);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(713, 309);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "串口参数设置";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // CB_FLOW_CONTROL
            // 
            this.CB_FLOW_CONTROL.FormattingEnabled = true;
            this.CB_FLOW_CONTROL.Items.AddRange(new object[] {
            "None",
            "CTS/RTS",
            "CTS/DTR",
            "DSR/RTS",
            "DSR/DTR",
            "XON/XOFF"});
            this.CB_FLOW_CONTROL.Location = new System.Drawing.Point(81, 136);
            this.CB_FLOW_CONTROL.Name = "CB_FLOW_CONTROL";
            this.CB_FLOW_CONTROL.Size = new System.Drawing.Size(99, 20);
            this.CB_FLOW_CONTROL.TabIndex = 9;
            // 
            // CB_CHECK
            // 
            this.CB_CHECK.FormattingEnabled = true;
            this.CB_CHECK.Items.AddRange(new object[] {
            "None",
            "Odd",
            "Even",
            "Mark",
            "Space"});
            this.CB_CHECK.Location = new System.Drawing.Point(81, 95);
            this.CB_CHECK.Name = "CB_CHECK";
            this.CB_CHECK.Size = new System.Drawing.Size(99, 20);
            this.CB_CHECK.TabIndex = 8;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(20, 139);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(65, 12);
            this.label4.TabIndex = 7;
            this.label4.Text = "流程控制：";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(18, 95);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(53, 12);
            this.label3.TabIndex = 6;
            this.label3.Text = "校验位：";
            // 
            // CB_BAUD
            // 
            this.CB_BAUD.FormattingEnabled = true;
            this.CB_BAUD.Items.AddRange(new object[] {
            "300",
            "600",
            "1200",
            "2400",
            "4800",
            "9600",
            "14400",
            "19200",
            "33600",
            "38400",
            "57600",
            "115200"});
            this.CB_BAUD.Location = new System.Drawing.Point(81, 53);
            this.CB_BAUD.Name = "CB_BAUD";
            this.CB_BAUD.Size = new System.Drawing.Size(99, 20);
            this.CB_BAUD.TabIndex = 5;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(18, 56);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 12);
            this.label2.TabIndex = 4;
            this.label2.Text = "波特率：";
            // 
            // BTN_CLOSE_SERIAL_PORT
            // 
            this.BTN_CLOSE_SERIAL_PORT.Location = new System.Drawing.Point(105, 204);
            this.BTN_CLOSE_SERIAL_PORT.Name = "BTN_CLOSE_SERIAL_PORT";
            this.BTN_CLOSE_SERIAL_PORT.Size = new System.Drawing.Size(75, 23);
            this.BTN_CLOSE_SERIAL_PORT.TabIndex = 3;
            this.BTN_CLOSE_SERIAL_PORT.Text = "关闭串口";
            this.BTN_CLOSE_SERIAL_PORT.UseVisualStyleBackColor = true;
            this.BTN_CLOSE_SERIAL_PORT.Click += new System.EventHandler(this.BTN_CLOSE_SERIAL_PORT_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(18, 14);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(41, 12);
            this.label1.TabIndex = 2;
            this.label1.Text = "串口：";
            // 
            // CB_CHOSE_SERIAL_PORT
            // 
            this.CB_CHOSE_SERIAL_PORT.FormattingEnabled = true;
            this.CB_CHOSE_SERIAL_PORT.Location = new System.Drawing.Point(81, 11);
            this.CB_CHOSE_SERIAL_PORT.Name = "CB_CHOSE_SERIAL_PORT";
            this.CB_CHOSE_SERIAL_PORT.Size = new System.Drawing.Size(99, 20);
            this.CB_CHOSE_SERIAL_PORT.TabIndex = 1;
            // 
            // BTN_OPEN_SERIAL_PORT
            // 
            this.BTN_OPEN_SERIAL_PORT.Location = new System.Drawing.Point(20, 204);
            this.BTN_OPEN_SERIAL_PORT.Name = "BTN_OPEN_SERIAL_PORT";
            this.BTN_OPEN_SERIAL_PORT.Size = new System.Drawing.Size(75, 23);
            this.BTN_OPEN_SERIAL_PORT.TabIndex = 0;
            this.BTN_OPEN_SERIAL_PORT.Text = "打开串口";
            this.BTN_OPEN_SERIAL_PORT.UseVisualStyleBackColor = true;
            this.BTN_OPEN_SERIAL_PORT.Click += new System.EventHandler(this.BTN_OPEN_SERIAL_PORT_Click);
            // 
            // tabPage2
            // 
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(713, 309);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "数据抄读设置";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.文件ToolStripMenuItem,
            this.设置ToolStripMenuItem,
            this.关于ToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(727, 24);
            this.menuStrip1.TabIndex = 2;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // 文件ToolStripMenuItem
            // 
            this.文件ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.退出ToolStripMenuItem});
            this.文件ToolStripMenuItem.Name = "文件ToolStripMenuItem";
            this.文件ToolStripMenuItem.Size = new System.Drawing.Size(43, 20);
            this.文件ToolStripMenuItem.Text = "文件";
            // 
            // 退出ToolStripMenuItem
            // 
            this.退出ToolStripMenuItem.Name = "退出ToolStripMenuItem";
            this.退出ToolStripMenuItem.Size = new System.Drawing.Size(98, 22);
            this.退出ToolStripMenuItem.Text = "退出";
            // 
            // 设置ToolStripMenuItem
            // 
            this.设置ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.串口参数设置ToolStripMenuItem,
            this.数据抄读项设置ToolStripMenuItem});
            this.设置ToolStripMenuItem.Name = "设置ToolStripMenuItem";
            this.设置ToolStripMenuItem.Size = new System.Drawing.Size(43, 20);
            this.设置ToolStripMenuItem.Text = "设置";
            // 
            // 串口参数设置ToolStripMenuItem
            // 
            this.串口参数设置ToolStripMenuItem.Name = "串口参数设置ToolStripMenuItem";
            this.串口参数设置ToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.串口参数设置ToolStripMenuItem.Text = "串口参数设置";
            // 
            // 数据抄读项设置ToolStripMenuItem
            // 
            this.数据抄读项设置ToolStripMenuItem.Name = "数据抄读项设置ToolStripMenuItem";
            this.数据抄读项设置ToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.数据抄读项设置ToolStripMenuItem.Text = "数据抄读项设置";
            // 
            // 关于ToolStripMenuItem
            // 
            this.关于ToolStripMenuItem.Name = "关于ToolStripMenuItem";
            this.关于ToolStripMenuItem.Size = new System.Drawing.Size(43, 20);
            this.关于ToolStripMenuItem.Text = "关于";
            // 
            // STATUS
            // 
            this.STATUS.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.STATUS_SERIAL_PORT});
            this.STATUS.Location = new System.Drawing.Point(0, 365);
            this.STATUS.Name = "STATUS";
            this.STATUS.Size = new System.Drawing.Size(727, 22);
            this.STATUS.TabIndex = 1;
            this.STATUS.Text = "statusStrip1";
            // 
            // STATUS_SERIAL_PORT
            // 
            this.STATUS_SERIAL_PORT.Name = "STATUS_SERIAL_PORT";
            this.STATUS_SERIAL_PORT.Size = new System.Drawing.Size(0, 17);
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(727, 387);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "Main";
            this.Text = "Form1";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.STATUS.ResumeLayout(false);
            this.STATUS.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.ComboBox CB_CHOSE_SERIAL_PORT;
        private System.Windows.Forms.Button BTN_OPEN_SERIAL_PORT;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem 文件ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 退出ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 设置ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 串口参数设置ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 数据抄读项设置ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 关于ToolStripMenuItem;
        private System.Windows.Forms.StatusStrip STATUS;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button BTN_CLOSE_SERIAL_PORT;
        private System.Windows.Forms.ComboBox CB_FLOW_CONTROL;
        private System.Windows.Forms.ComboBox CB_CHECK;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox CB_BAUD;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ToolStripStatusLabel STATUS_SERIAL_PORT;


    }
}

