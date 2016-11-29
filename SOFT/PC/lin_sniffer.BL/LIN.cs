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

		public LIN(byte[] incomingData)
		{
			Header = incomingData[0];
			ID = incomingData[1];
			DataLenght = incomingData[2];
			Data = incomingData.Skip(3).Take(DataLenght).ToArray();
			CRC = incomingData.Last();
		}

		public override string ToString()
		{
			return ($"{Header}  {ID}  {DataLenght}  {BitConverter.ToString(Data)}  {CRC,18}");
		}
	}
}
