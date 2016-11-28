using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace lin_sniffer
{
	public interface IMainForm
	{
		event EventHandler ConnectClick;
	}

	public partial class MainForm : Form, IMainForm
	{
		public MainForm()
		{
			InitializeComponent();
			connectButton.Click += ConnectButton_Click;
		}

		private void ConnectButton_Click(object sender, EventArgs e)
		{
			ConnectClick?.Invoke(this, EventArgs.Empty);
		}
		#region IMainForm mplementation
		public event EventHandler ConnectClick;
		#endregion
	}
}
