using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ParseANUData
{
    class Program
    {
        static void Main(string[] args)
        {
            string directory;

            if (args.Length != 0)
            {
                directory = args[0];
            }
            else
            {
                Console.WriteLine("Enter directory to load: ");
                //directory = Console.ReadLine();
                directory = @"H:\2015\9-10-2015, 1157";
                //Console.WriteLine("Usage ParseANUData [Directory]");
            }

            Parser thisParser = new Parser(directory);
        }
    }
}
