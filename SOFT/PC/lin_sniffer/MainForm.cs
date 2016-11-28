﻿using System;
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
		void SetPortList(List<string> portList);

		event EventHandler ConnectClick;
		event EventHandler PortListClick;
	}

	public partial class MainForm : Form, IMainForm
	{
		public MainForm()
		{
			InitializeComponent();

			connectButton.Click += ConnectButton_Click;
			portListBox.Click += PortListBox_Click;

		}

		private void ConnectButton_Click(object sender, EventArgs e)
		{
			ConnectClick?.Invoke(this, EventArgs.Empty);
		}

		private void PortListBox_Click(Object sender, EventArgs e)
		{
			if (PortListClick != null)
				PortListClick(this, EventArgs.Empty);
		}

		#region IMainForm mplementation
		public void SetPortList(List<String> portList)
		{
			portListBox.Items.Clear();
			foreach (var p in portList)
			{
				portListBox.Items.Add(p);
			}
		}


		public event EventHandler ConnectClick;
		public event EventHandler PortListClick;
		#endregion
	}
}
