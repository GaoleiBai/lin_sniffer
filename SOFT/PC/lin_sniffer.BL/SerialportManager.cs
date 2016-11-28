using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Management;
using System.Text.RegularExpressions;
using System.IO.Ports;

namespace lin_sniffer.BL
{
	public interface ISerialportManager
	{
		void PortInit();
		bool PortState { get; }
		List<string> GetPortList();
		string setPortName(string longPortName);
		bool opnePort(string portName);
		bool closePort();
		void writeData(byte[] data);
		void readData(byte[] data);
		event EventHandler dataRecived;
	}

	public class SerialportManager : ISerialportManager
	{
		private bool _portState = false;
		SerialPort sp1 = new SerialPort("COM1", 38400, Parity.None, 8, StopBits.One);
		Regex regex = new Regex("COM");
		/// <summary>
		/// Initialisation events of port.
		/// </summary>
		public void PortInit()
		{
			sp1.DataReceived += sp1_DataReceived;
			sp1.RtsEnable = true;
			sp1.DtrEnable = true;
		}

		/// <summary>
		/// Return all aviable serial ports with PID info.
		/// </summary>
		/// <returns></returns>
		public List<string> GetPortList()
		{
			List<string> portList = new List<string>();
			string query = "SELECT * FROM Win32_PnPEntity WHERE ConfigManagerErrorCode = 0";

			// Don't work in Window 10
			/*ManagementObjectSearcher searcher = new ManagementObjectSearcher(query); 
				foreach (ManagementObject service in searcher.Get())
				{
					if (regex.IsMatch(service["Name"].ToString()))
					{
						portList.Add(service["Name"].ToString());
					}
				}*/
			portList = SerialPort.GetPortNames().ToList();

			return portList;
		}

		/// <summary>
		/// Set port name. Incapsulate "PORT NAME" of line.
		/// </summary>
		/// <param name="longPortName"></param>
		/// <returns>Port nmae string with cendor info</returns>
		public string setPortName(string longPortName)
		{
			string shortPortName = "";
			string pattern = @"COM\d+"; // Паттерн для выделения имени COM порта из строки
			try
			{
				MatchCollection matchs = Regex.Matches(longPortName, pattern); // Процесс выделения имени COM порта из строки
				foreach (Match match in matchs)
					shortPortName = match.Value;
			}
			catch
			{

			}
			return shortPortName;
		}

		/// <summary>
		/// Try open port
		/// </summary>
		/// <returns>Port state - boolean</returns>
		public bool opnePort(string portName)
		{
			bool result = false;
			if (!sp1.IsOpen)
			{
				sp1.PortName = portName;
				try
				{
					sp1.Open();
					_portState = true;
					result = true;
				}
				catch (Exception)
				{
					result = false;
				}
			}
			return result;
		}

		/// <summary>
		/// Try close port
		/// </summary>
		/// <returns>Port state - boolean</returns>
		public bool closePort()
		{

			bool result = false;
			if (sp1.IsOpen)
			{
				try
				{
					sp1.Close();
					_portState = false;
					result = true;
				}
				catch (Exception)
				{
					result = false;
				}
			}
			return result;
		}

		/// <summary>
		/// Write data in port
		/// </summary>
		/// <param name="data"></param>
		public void writeData(byte[] data)
		{
			if (sp1.IsOpen)
			{
				sp1.Write(data, 0, data.Length);
			}
		}

		/// <summary>
		/// Read data  and write them in an array
		/// </summary>
		/// <param name="data"> Target array</param>
		public void readData(byte[] data)
		{
			Array.Clear(data, 0, data.Length);
			sp1.Read(data, 0, data.Length);
		}

		/// <summary>
		/// Data recived handler
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void sp1_DataReceived(object sender, SerialDataReceivedEventArgs e)
		{
			if (dataRecived != null)
				dataRecived(this, EventArgs.Empty);
		}

		public bool PortState
		{
			get { return _portState; }
		}

		public event EventHandler dataRecived;
	}
}
