using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ParseANUData
{
    public class ANUFilename
    {
        //Class to deal with the filenames created by the LabView VI which is used for DAQ.
        public string Filename { get { return sFilename; } }
        private string sFilename;

        public DateTime FileNameTime { get { return dtFileNameTime; } }
        private DateTime dtFileNameTime;

        public DateTime LastModifiedTime { get { return dtLastModifiedTime; } }
        private DateTime dtLastModifiedTime;

        public bool IsBFile { get { return bIsBFile; } }
        private bool bIsBFile;

        public int FileNumber { get { return iFileNumber; } }
        private int iFileNumber;

        public double RunTimeSeconds { get { return dRunTimeSeconds; } }
        private double dRunTimeSeconds;

        public ANUFilename(string _ANUFilename)
        {
            string[] _splits; //string array to hold pilts of the filename
            char[] _delimiters = new char[] { ' ', '\\', ',', '.' }; //char array to hold delimiters for splitting filename
            sFilename = _ANUFilename; //set filename
            _splits = sFilename.Split(_delimiters); //split filename into component parts
            foreach(string s in _splits)
            {
                Console.WriteLine(s);
                //Console.ReadKey();
            }
            string _Date = _splits[2];
            string _Time = _splits[4];
            string _FileNumber = _splits[11];

            string[] _DateSplits = _Date.Split('-');

            string _sHour, _sMinute;
            _sHour = _Time.Remove(2, 2);
            _sMinute = _Time.Remove(0, 2);
            int _iDay, _iMonth, _iYear, _iHour, _iMinute;
            int.TryParse(_DateSplits[0], out _iDay);
            int.TryParse(_DateSplits[1], out _iMonth);
            int.TryParse(_DateSplits[2], out _iYear);
            int.TryParse(_sHour, out _iHour);
            int.TryParse(_sMinute, out _iMinute);
            _iHour = 0;
            _iMinute = 0;
            dtFileNameTime = new DateTime(_iYear, _iMonth, _iDay, _iHour, _iMinute, 0);

            if (_FileNumber.Contains('B'))
            {
                bIsBFile = true;
                string _tempFileNumber = _FileNumber.Remove((_FileNumber.Length - 1), 1);
                int.TryParse(_tempFileNumber, out iFileNumber);
            }
            else
            {
                int.TryParse(_FileNumber, out iFileNumber);
            }
            dtLastModifiedTime = System.IO.File.GetLastWriteTime(_ANUFilename);
            TimeSpan runtime = dtLastModifiedTime - dtFileNameTime;
            dRunTimeSeconds = runtime.TotalSeconds;
        }



    }
}
