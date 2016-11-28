using System;
using System.Windows.Forms;
using lin_sniffer.BL;

namespace lin_sniffer
{
	static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);

			MainForm form = new MainForm();
			MessageService service = new MessageService();
			LinMessageService linService = new LinMessageService();
			SerialportManager portManager = new SerialportManager();
			Presentor presentor = new Presentor(form, service, linService, portManager);

			Application.Run(form);
		}
	}
}
