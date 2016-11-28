using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lin_sniffer
{
	public interface ILinMessageService
	{
		void PrintIncomintMessage(List<byte> messageArray);

	}
}
