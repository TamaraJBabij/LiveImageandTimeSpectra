using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ParseANUData
{
    class Group
    {
        public int GroupID { get { return iGroupID; } }
        public List<int> Channel;
        public List<int> Time;
        private int iGroupID;
        private int iNumberOfHits;
        public int NumberOfHits { get { return iNumberOfHits; } }

        public Group(List<TDC3377DoubleWord> _Words, int _GroupID)
        {
            List<int> iChannel = new List<int>();
            List<int> iTime = new List<int>();
            _Words.RemoveRange(0, 1); // remove the header word;
            iNumberOfHits = (int)((double)_Words.Count / 2.0);
            for(int iCount = 0; iCount < _Words.Count-1; iCount=iCount+2)
            {
                iChannel.Add(_Words[iCount].Channel);
                iTime.Add((_Words[iCount].Time + _Words[iCount + 1].Time));
            }
            Channel = iChannel;
            Time = iTime;
            iNumberOfHits = iChannel.Count;
            iGroupID = _GroupID;
        }

        public override string ToString()
        {
            string sOutput="";
            for(int iCount = 0; iCount < iNumberOfHits; iCount++)
                sOutput = sOutput + iGroupID.ToString() +"\t"+ Channel[iCount].ToString() + "\t" + Time[iCount].ToString() + "\r\n";
            return sOutput;
        }

    }
}
