using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenCvSharp;

namespace ss2textCS
{
    partial class Program
    {
        static int showiteCounter = 0;

        static void showrite(String s, CvMat image)
        {
            CvWindow window = new CvWindow(showiteCounter.ToString() + s);
            window.ShowImage(image);
            image.SaveImage(showiteCounter.ToString() + s + ".png");

            showiteCounter++;
        }

        static void Main(string[] args)
        {
            // スコア
            List<Score> scores;
            // ss 読み込み
            using ( CvMat ss = new CvMat ( "ss3.png" ) )
            {
                // スコア読み出し
                recognize(ss, out scores);

                for (int n = 0; n < scores.Count; n++)
                {
                    scores[n].dump();
                }
                Cv.WaitKey();
            }

        }
    }
}
