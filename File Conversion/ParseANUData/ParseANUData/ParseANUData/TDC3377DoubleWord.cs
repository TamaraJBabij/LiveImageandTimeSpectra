using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ParseANUData
{
    class TDC3377DoubleWord
    {
        public UInt16 Word { get { return ui16Word; } }
        private UInt16 ui16Word;

        public bool IsHeader { get { return IsWordHeader(); } }

        public bool IsLSB { get { return IsWordLSB(); } }

        public int Channel { get { return GetChannel(); } }

        public int Time { get { return GetTime(); } }

        public bool IsLeadingEdge { get { return IsWordLeadingEdge(); } }

        public bool IsLeadingEdgeRecording { get { return IsWordLeadingEdgeRecording(); } }

        public int ModuleID { get { return GetModuleID(); } }

        public int EventSerial { get { return GetEventSerial(); } }

        public bool IsDoubleWord { get { return IsWordDouble(); } }

        public int TwoBitResolution { get { return Get2bitResoluation(); } }

        public TDC3377DoubleWord(UInt16 _ui16Word)
        {
            ui16Word = _ui16Word;
        }
        
        private bool IsWordHeader()
        {
            bool bIsHeader = false;
            UInt16 ui16Mask = 32768;
            if ((ui16Word & ui16Mask) == ui16Mask)
                bIsHeader = true;
            return bIsHeader;
        }

        private bool IsWordLSB()
        {
            bool bIsLSB = true;
            UInt16 ui16Mask = 256;
            if ((ui16Word & ui16Mask) == ui16Mask)
                bIsLSB = false;
            return bIsLSB;
        }

        private int GetChannel()
        {

            int iChannel = -1;
            UInt16 ui16Mask = 31744;
            iChannel = (int)((ui16Mask & ui16Word)>>10);
            return iChannel;
        }

        private int GetTime()
        {
            int iTime = -1;
            UInt16 ui16Mask = 255;
            if (IsWordLSB())
                iTime = (int)(ui16Mask & ui16Word);
            else
                iTime = (int)((ui16Mask & ui16Word) << 8);
            return iTime;
        }

        private bool IsWordLeadingEdge()
        {
            bool bIsLeadingEdge = false;
            UInt16 ui16Mask = 512;
            if ((ui16Mask & ui16Word) == 0)
                bIsLeadingEdge = true;
            return bIsLeadingEdge;
        }

        private int GetModuleID()
        {
            int iModuleID = 0;
            UInt16 ui16Mask = 255;
            iModuleID = (int)(ui16Mask & ui16Word);
            return iModuleID;
        }

        private bool IsWordLeadingEdgeRecording()
        {
            bool bIsWordLER = false;
            UInt16 ui16Mask = 1024;
            if ((ui16Mask & ui16Word) == 0)
                bIsWordLER = true;
            return bIsWordLER;
        }

        private int GetEventSerial()
        {
            int iEventSerial = 0;
            UInt16 ui16Mask = 14336;
            iEventSerial = (int)(((ui16Word & ui16Mask) >> 11)%8);
            return iEventSerial;
        }

        private bool IsWordDouble()
        {
            bool bIsDouble = false;
            UInt16 ui16Mask = 16384;
            if ((ui16Mask & ui16Word) == ui16Mask)
                bIsDouble = true;
            return bIsDouble;
        }

        private int Get2bitResoluation()
        {
            int i2bitResolution = 0;
            UInt16 ui16Mask = 768;
            i2bitResolution = (int)((ui16Mask & ui16Word) >> 8);
            return i2bitResolution;
        }

        public override string ToString()
        {
            string sOutput;
            if(IsHeader)
            {
                sOutput = "======== Header Word ========\r\n Mod ID: " + ModuleID.ToString() + "\t2 bit Res: " + TwoBitResolution.ToString();
                if (IsLeadingEdge)
                    sOutput = sOutput + "\tEdge Rec: L";
                else
                    sOutput = sOutput + "\tEdge Rec: B";
                sOutput = sOutput + "\tEvent Serial: " + EventSerial.ToString();
                if (IsDoubleWord)
                    sOutput = sOutput + "\tPrec: D";
                else
                    sOutput = sOutput + "\tPrec: S";
            }
            else
            {
                sOutput = "Data Word --->"; 
                if (IsLSB)
                    sOutput = sOutput + "\tBit: LS";
                else
                    sOutput = sOutput + "\tBit: MS";
                if (IsLeadingEdge)
                    sOutput = sOutput + "\tEdge: L";
                else
                    sOutput = sOutput + "\tEdge: T";
                sOutput = sOutput + "\tC: " + Channel.ToString() + "\tT: " + Time.ToString();
            }
            return sOutput;
        }

    }
}
