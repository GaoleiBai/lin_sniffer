using System;
using System.Windows.Forms;

namespace lin_sniffer
{
	class MessageService : IMessageService
	{
		public void ShowError(String message)
		{
			MessageBox.Show(message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
		}

		public void ShowMessage(String message)
		{
			MessageBox.Show(message, "Message", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

		public void ShowExclamation(String message)
		{
			MessageBox.Show(message, "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
		}
	}
}
