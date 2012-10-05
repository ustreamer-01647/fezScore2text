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
using System.Windows.Navigation;
using System.Windows.Shapes;
using OpenCvSharp;
using ss2textCS;

namespace fezScore2text
{
    /// <summary>
    /// MainWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class MainWindow : Window
    {
        // fezスクリーンショット原画像
        CvMat ss;
        // fezスクリーンショットスコア表部分
        CvMat scoreTable;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void MenuExit_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void MenuOpen_Click(object sender, RoutedEventArgs e)
        {
            var dlg = new Microsoft.Win32.OpenFileDialog();
            dlg.Filter = "画像ファイル(*.bmp, *.png, *.jpg, *.jpeg)|*.bmp;*.png;*.jpg;*.jpeg|すべてのファイル(*.*)|*.*";
            
            dlg.Title = "fezスクリーンショットを開く";
            if ( true == dlg.ShowDialog())
            {
                using (ss = new CvMat(dlg.FileName))
                {
                    scoreTable = extractScoreTable(ss);
                    // プレビュー更新
                    previewRefresh(scoreTable);
                }
            }

        }

        private void previewRefresh(CvMat scoreTable)
        {

        }

        private void MenuPreview_Click(object sender, RoutedEventArgs e)
        {
        }

    }
}
