using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using OpenCvSharp;

namespace fezScore2text
{
    /// <summary>
    /// Window1.xaml の相互作用ロジック
    /// </summary>

    public partial class PreviewWIndow : Window
    {
        public PreviewWIndow()
        {
            InitializeComponent();
        }

        public void preview ( CvMat mat )
        {
            BitmapImage bImage;
            bImage = BitmapImage.Create( mat.Cols, mat.Rows, 96, 96,
                System.Drawing.Imaging.PixelFormat.Format24bppRgb,
                BitmapPalettes.BlackAndWhite,
                );
            PreviewArea.Source;
        }
    }
}
