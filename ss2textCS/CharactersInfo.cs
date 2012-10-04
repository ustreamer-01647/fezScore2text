using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenCvSharp;

namespace ss2textCS
{
    class CharactersInfo
    {
        // 文字列認識最低幅要件
        const int LowestWidth = 4;
        // 位置
        List<CvRect> positions;
        // 画像
        CvMat image;

        public CharactersInfo(CvMat _image)
        {
            // 画像コピー
            image = _image.Clone();

            // 文字列認識
            // 輝点列検索フラグ．falseなら暗点列を探す
            bool searchingBright = true;
            // 始点
            int left = 0;
            // 終点
            int right;

            for (int col = 0; col < image.Cols; col++)
            {
                // 列の輝点数
                int nonzero = image.GetCol(col).CountNonZero();

                // 探索モードによる分岐
                if ( true == searchingBright)
                {
                    // 輝点列探索中
                    if (0 < nonzero)
                    {
                        // 輝点が1個以上あったなら
                        left = col;
                        // フラグ切り替え
                        searchingBright = false;
                    }
                }
                else
                {
                    // 暗点列探索中
                    if (0 == nonzero)
                    {
                        // 暗点列だったなら
                        right = col;
                        // LowestWidth を満足するか
                        if (CharactersInfo.LowestWidth > right - left)
                        {
                            // 条件を満たさない場合は探索やり直し
                            searchingBright = true;
                            continue;
                        }

                        // 文字と認める
                        CvMat character = image.GetCols( left, right );
                        // 上端輝点行を探す
                        int top = 0;
                        for (int row = 0; row < character.Rows; row++)
                        {
                            if (0 < character.GetRow(row).CountNonZero())
                            {
                                // 輝点発見
                                top = row;
                                break;
                            }
                        }
                        // 下端輝点行を探す
                        int bottom = character.Rows - 1;
                        for (int row = bottom; row > top; row--)
                        {
                            if (0 < character.GetRow(row).CountNonZero())
                            {
                                // 輝点発見
                                bottom = row + 1;
                                break;
                            }
                        }
                        // 文字領域確定
                        positions.Add(new CvRect(left, top, right - left, bottom - top));
                    }
                    // 探索フラグ切り替え
                    searchingBright = true;
                }
            }
        }

        // 文字数
        public int size()
        {
            return positions.Count;
        }

        // 文字画像
        public CvMat characterImage(int n)
        {
            // 存在しない場合は原画像を返す
            if (size() < n)
            {
                return image;
            }
            return image.GetSubArr(out image, positions[n]);
        }

        // 数字認識
        public int getDigit(int n)
        {
            if (0 == size())
                return 0;

            int value = recognizeDigit(characterImage(n));
            return value;
        }

    }
}
