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
            // ss 読み込み
            using ( CvMat ss = new CvMat ( "ss3.png" )
            {
                // スコア読み出し
                List<Score> scores;

            }
            //            // ss読み込み
            //cv::Mat ss = cv::imread("ss3.png");

            //// スコア読み出し
            //std::vector<Score> scores;
            //recognize( ss, scores );

            //for ( size_t n = 0; n < scores.size(); n++ )
            //{
            //    std::cout << scores[n].toString() << std::endl;
            //}

            //// 表示
            //cv::waitKey();

        }
    }
}
