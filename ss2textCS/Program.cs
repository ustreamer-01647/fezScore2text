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
        void showrite(String s, CvMat image)
        {
            CvWindow.ShowImages(image);
            image.SaveImage(s);
        }

        static void Main(string[] args)
        {
            // スコア
            List<Score> scores;
            // ss 読み込み
            using ( CvMat ss = new CvMat ( "ss3.png" )
            {
                // スコア読み出し
                recognize( ss, ref scores );

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
