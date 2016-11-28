using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lin_sniffer
{
	public class LinMessageService : ILinMessageService
	{
		public void PrintIncomintMessage(List<byte> messageArray)
		{
			Console.WriteLine(messageArray);
		}
	}
}
