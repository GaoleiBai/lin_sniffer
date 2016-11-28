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

		public Presentor(IMainForm form, IMessageService msgService, ISerialportManager serialPortManager)
		{
			view = form;
			messageService = msgService;
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

					}
				}
				else
				{
					portManager.closePort();
					//	view.SetConnectionOff();
				}

			}
			else
			{
				messageService.ShowExclamation("Please select port");
			}

		}
	}
}
