using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lin_sniffer.BL
{
	public class LIN
	{
		public const int IncomingArrayLenght = 12;

		byte Header { get; set; }
		byte ID { get; set; }
		byte DataLenght { get; set; }
		byte[] Data { get; set; }
		byte CRC { get; set; }

		public LIN(byte header, byte id, byte dataLenght, byte[] data, byte crc)
		{
			Header = header;
			ID = id;
			DataLenght = dataLenght;
			Data = data;
			CRC = crc;
		}
	}
}
