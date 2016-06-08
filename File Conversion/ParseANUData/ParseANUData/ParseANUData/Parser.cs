using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ParseANUData
{
    class Parser
    {
        private string sDataDirectory;
        private double dRunTimeSeconds;

        public Parser(string _DataDirectory)
        {
            sDataDirectory = _DataDirectory;
            ReadFiles();
        }

        public void ReadFiles()
        {
            System.IO.StreamReader thisReader;
            System.IO.StreamWriter thisWriter;
            string sOutputFilename = sDataDirectory+"//parsed.tsv";
            string[] sFilenames;
            sFilenames = System.IO.Directory.GetFiles(sDataDirectory); //Reads all filenames in the specified directory
            int iGroupNumber = 0; //Set the overall group number to 0, this will associate all hits in a group so it's important to track! 
            thisWriter = new System.IO.StreamWriter(sOutputFilename);
            foreach (string FileName in sFilenames)
            {
                Console.WriteLine("File is called " + FileName);
                ANUFilename thisName = new ANUFilename(FileName);
                if (thisName.RunTimeSeconds > dRunTimeSeconds)
                    dRunTimeSeconds = thisName.RunTimeSeconds; //Calculate total run time (+/- 60s if the data has been copied)
                if (thisName.IsBFile == true)
                {
                    Console.WriteLine("And it's a B file!");
                    //Open the file
                    thisReader = new System.IO.StreamReader(FileName);
                    while (thisReader.EndOfStream != true)
                    {
                        string sLine = thisReader.ReadLine();
                        string[] splits = sLine.Split('\t');
                        UInt16[] values = Array.ConvertAll(splits, UInt16.Parse);
                        List<TDC3377DoubleWord> TDC3377Words = new List<TDC3377DoubleWord>(); 
                        foreach (UInt16 thisUI16 in values) //We have all the values as UInt16's but it would be nice to get them as TDC3377DataWords so lets convert
                        {
                            if(thisUI16 > 0) //Remove the useless 0's 
                            {
                                TDC3377DoubleWord tempWord = new TDC3377DoubleWord(thisUI16);
                                if (tempWord.Channel > 15) //Remove the noise on channels <16
                                    TDC3377Words.Add(tempWord); //Put the word in the list 
                            }
                        }

                        if (TDC3377Words.Count > 0) //if we have data put it into a group class
                        {
                            Group thisGroup = new Group(TDC3377Words, iGroupNumber);
                            //Console.Write(thisGroup.ToString());
                            thisWriter.Write(thisGroup.ToString());
                            iGroupNumber++;
                        }
                        
                    }
                    thisReader.Close();
                }

            }
        }
    }
}
