using System;

namespace lin_sniffer
{
	public class LinMessageService : ILinMessageService
	{
		public void PrintIncomintMessage(string message)
		{
			Console.ForegroundColor = ConsoleColor.Green;
			Console.WriteLine(message);
		}
	}
}
