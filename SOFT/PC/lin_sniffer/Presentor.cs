using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using lin_sniffer.BL;

namespace lin_sniffer
{
	public class Presentor
	{
		private readonly IMainForm view;
		private readonly IMessageService messageService;
		private readonly ISerialportManager portManager;
		private readonly ILinMessageService linMessageService;

		public Presentor(IMainForm form, IMessageService msgService, ILinMessageService linMessageService, ISerialportManager serialPortManager)
		{
			view = form;
			messageService = msgService;
			this.linMessageService = linMessageService;
			portManager = serialPortManager;

			view.ConnectClick += View_ConnectClick;
			view.PortListClick += View_PortListClick;
		}

		private void View_PortListClick(Object sender, EventArgs e)
		{
			view.SetPortList(portManager.GetPortList());
		}

		private void View_ConnectClick(object sender, EventArgs e)
		{
			var portName = view.GetPortName();
			if (portName != null && portName != string.Empty)
			{
				if (!portManager.PortState)
				{
					if (portManager.opnePort(portName))
					{
						Console.WriteLine("Serial port opened");
					}
				}
				else
				{
					portManager.closePort();
					//	view.SetConnectionOff();
					Console.WriteLine("Serial port closed");
				}

			}
			else
			{
				messageService.ShowExclamation("Please select port");
			}

		}
	}
}
