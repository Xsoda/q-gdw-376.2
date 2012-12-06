namespace MainUI
{
    partial class Output
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.RTB_PACKET_PARSE = new System.Windows.Forms.RichTextBox();
            this.RTB_PACKET_CONTENT = new System.Windows.Forms.RichTextBox();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.RTB_PACKET_PARSE, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.RTB_PACKET_CONTENT, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 65.30612F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 34.69388F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(507, 334);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // RTB_PACKET_PARSE
            // 
            this.RTB_PACKET_PARSE.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.RTB_PACKET_PARSE.Dock = System.Windows.Forms.DockStyle.Fill;
            this.RTB_PACKET_PARSE.Location = new System.Drawing.Point(3, 3);
            this.RTB_PACKET_PARSE.Name = "RTB_PACKET_PARSE";
            this.RTB_PACKET_PARSE.ReadOnly = true;
            this.RTB_PACKET_PARSE.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.RTB_PACKET_PARSE.Size = new System.Drawing.Size(501, 212);
            this.RTB_PACKET_PARSE.TabIndex = 0;
            this.RTB_PACKET_PARSE.Text = "";
            // 
            // RTB_PACKET_CONTENT
            // 
            this.RTB_PACKET_CONTENT.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.RTB_PACKET_CONTENT.Dock = System.Windows.Forms.DockStyle.Fill;
            this.RTB_PACKET_CONTENT.Location = new System.Drawing.Point(3, 221);
            this.RTB_PACKET_CONTENT.Name = "RTB_PACKET_CONTENT";
            this.RTB_PACKET_CONTENT.ReadOnly = true;
            this.RTB_PACKET_CONTENT.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.RTB_PACKET_CONTENT.Size = new System.Drawing.Size(501, 110);
            this.RTB_PACKET_CONTENT.TabIndex = 1;
            this.RTB_PACKET_CONTENT.Text = "";
            // 
            // Output
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(507, 334);
            this.Controls.Add(this.tableLayoutPanel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Output";
            this.Opacity = 0.9D;
            this.ShowInTaskbar = false;
            this.Text = "Output";
            this.TopMost = true;
            this.tableLayoutPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.RichTextBox RTB_PACKET_PARSE;
        private System.Windows.Forms.RichTextBox RTB_PACKET_CONTENT;
    }
}