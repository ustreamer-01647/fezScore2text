using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenCvSharp;
using System.Runtime.InteropServices;

namespace ss2textCS
{
    class Program
    {
        static void Main(string[] args)
        {
            using (IplImage image = new IplImage(128, 128, BitDepth.U8, 1))
            {
                image.Zero();
                for (int x = 0; x < image.Width; x++)
                {
                    for (int y = 0; y < image.Height; y++)
                    {
                        int offset = y * image.WidthStep + x;
                        byte value = (byte)(x + y); 
                        Marshal.WriteByte(image.ImageData, offset, value);

                    }
                }
                using (CvWindow window = new CvWindow(image))
                {
                    CvWindow.WaitKey();
                }
            }
        }
    }
}
