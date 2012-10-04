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
        // 数字認識
        internal static int recognizeDigit(CvMat image)
        {
            int nonzero = 0;

            nonzero = image.GetCols( image.Cols-2, image.Cols ).CountNonZero();
            if ( image.Rows * 2 == nonzero )
                // 1 右端2列がすべて輝点
                return 1;

            nonzero = image.GetRows( image.Rows-2, image.Rows ).CountNonZero();
            if ( image.Cols * 2 == nonzero )
                // 2 下端2行がすべて輝点
                return 2;

            nonzero = image.GetRows ( 0, 2 ).CountNonZero();
            if ( image.Cols * 2 - 2 < nonzero )
                // 7 上端2行がすべて輝点．ただし1ピクセルまで欠けても良い
                return 7;

            nonzero = image.GetCols ( image.Cols-3, image.Cols-1 ).CountNonZero();
            if ( image.Rows * 2 == nonzero )
                // 4 右端の左2列がすべて輝点
                return 4;

            CvRect rect = new CvRect( 0, 0, 1, image.Rows*2/3 );
            CvMat submat;
            nonzero = image.GetSubArr ( out submat, rect ).CountNonZero();
            if ( 0 == nonzero )
                // 3 左端の上部3分の2がすべて暗点
                return 3;

            rect = new CvRect ( 0, image.Rows/2, 3, 2 );
            nonzero = image.GetSubArr ( out submat, rect ).CountNonZero();
            if ( 0 == nonzero )
                // 5 左端の下半分開始すぐのwidth3 height2 がすべて暗点
                return 5;

            rect = new CvRect ( image.Cols/2, image.Rows/2-1, 1, 3 );
            nonzero = image.GetSubArr( out submat, rect ).CountNonZero();
            if ( 0 == nonzero )
                // 0 中央列中央3ピクセルがすべて暗点
                return 0;

            rect = new CvRect ( image.Cols-1, 0, 1, image.Rows*2/5 );
            nonzero = image.GetSubArr( out submat, rect ).CountNonZero();
            if ( 0 == nonzero )
                // 6 右端上部5分の2がすべて暗点
                return 6;

            rect = new CvRect ( image.Cols-1, image.Rows-3, 1, 3 );
            nonzero = image.GetSubArr( out submat, rect ).CountNonZero();
            if ( 0 == nonzero )
                // 右端下部3ピクセルがすべて暗点
                return 9;
            
            // 8 上記条件を満たさない
            return 8;
        }

        // 所属国認識
        static 所属国 recognizeNationality(CvMat image)
        {
	        // 2値画像の輝点数を基に判別する
	        // ces: 25
	        // geb: 52
	        // hor: 99
	        // http://twitpic.com/az9oy2
            int count = image.CountNonZero();
	        if ( count > 80 )
		        return 所属国.ホルデイン;

	        if ( count > 45 )
		        return 所属国.ゲブランド;

	        if ( count > 15 )
		        return 所属国.カセドリア;

	        return 所属国.不明;
        }

        /** 所属国認識2
         * @param image 所属国アイコン部分カラー画像
        */
        static 所属国 recognizeNationality2 ( CvMat image )
        {
            // 閾値130.0で2値化した画像を基にする
            CvMat bin = new CvMat( image.Rows, image.Cols, MatrixType.U8C1);
            image.CvtColor( bin, ColorConversion.BgrToGray );
            bin.Threshold( bin, 130.0, 255.0, ThresholdType.Binary );

            // iel: 7
            // net: 23
            // http://twitpic.com/azbzjh
            int count = bin.GetRows( 2, bin.Rows ).CountNonZero();
            if ( count > 15 )
                return 所属国.ネツァワル;

            if ( count > 3 )
                return 所属国.エルソード;

            return 所属国.不明;
        }

        static int recognizeInteger ( CvMat image )
        {
            // 文字列情報
            CharactersInfo charactersInfo = new CharactersInfo(image);

            // 数値認識
            StringBuilder sb = new StringBuilder();
            for ( int n = 0; n < charactersInfo.size(); n++ )
            {
                sb.AppendFormat("D", charactersInfo.getDigit(n));
            }

            return int.Parse( sb.ToString() );
        }

        // クラス認識
        static クラス recognizeJob ( CvMat image )
        {
            // 文字列情報
            CharactersInfo charactersInfo = new CharactersInfo( image );

            // 5字：ソーサラー，ウォリアー，フェンサー
            if ( 5 == charactersInfo.size() )
            {
                int second = charactersInfo.characterImage(1).CountNonZero();
                int last = charactersInfo .characterImage(4).CountNonZero();
                // 第2字と最終文字の輝点数差が3以下なら同一文字があるソーサラーとみなす
                int diff = second - last;
                if ( diff*diff < 3*3 )
                {
                    return クラス.ソーサラー;
                }
                // 最初文字最上段がすべて輝点ならばフェンサー
                int nonzero = charactersInfo.characterImage(0).GetRows(0, 1 ).CountNonZero();
                if ( nonzero == charactersInfo.characterImage(0).Cols )
                    return クラス.フェンサー;

                return クラス.ウォリアー;
            }
            // 4字：スカウト，セスタス
            if ( 4 == charactersInfo.size() )
            {
                int second = charactersInfo.characterImage(1).CountNonZero();
                int last = charactersInfo .characterImage(3).CountNonZero();
                // 第2字と最終文字の輝点数差が3以下なら同一文字があるセスタスとみなす
                int diff = second - last;
                if ( diff*diff < 3*3 )
                {
                    return クラス.セスタス;
                }
                return クラス.スカウト;
            }


            return クラス.不明;
        }

        /** キャラクタスコア認識
         * @param ss fezのスクリーンショット
         * @param scores 認識したスコアデータ格納先
         */
        static void recognize ( CvMat ss, ref List<Score> scores )
        {
            // 表部分切り出し
            CvMat scoreTable = extractScoreTable(ss);
            // 2値画像
            CvMat stBin = scoreTable2Binary(scoreTable);
            // キャラクタスコア切り出し
            List<CvMat> scoreRows = new List<CvMat>();
            extractScoreRows( stBin, ref scoreRows );

            for ( int n = 0; n < scoreRows.Count; n++ )
            {
                Score score = new Score();
                int integer = 0;
                所属国 nationality = 所属国.不明;
                クラス job = クラス.不明;

                // 順位
                CvRect rect = new CvRect (Score.RankOffset, 0, Score.RankWidth, scoreRows[n].Rows );
                CvMat submat;
                integer = recognizeInteger( scoreRows[n].GetSubArr( out submat, rect ));
                score.rank = integer;
                // キャラクタ名
                // rect = new Rect ( Score.NameOffset, 0, Score.NameWidthm scoreRows[n].Rows );
                // String name = recognizeText( scoreRows[n].GetSubArr( out submat, rect ));
                // score.name = name;
                // 所属国
                rect = new CvRect (Score.NationalityOffset, 0, Score.NationalityWidth, scoreRows[n].Rows );
                nationality = recognizeNationality(scoreRows[n].GetSubArr(out submat, rect));
                if (所属国.不明 == nationality)
                    nationality = recognizeNationality2(extractColorNationality(scoreTable, n));
                score.nationality = nationality;
                // クラス
                rect = new CvRect(Score.JobOffset, 0, Score.JobWidth, scoreRows[n].Rows);
                job = recognizeJob(scoreRows[n].GetSubArr(out submat, rect));
                score.job = job;
                // キル数
                rect = new CvRect (Score.KillOffset, 0, Score.KillWidth, scoreRows[n].Rows );
                integer = recognizeInteger( scoreRows[n].GetSubArr( out submat, rect ));
                score.kill = integer;
                // デッド数
                rect = new CvRect (Score.DeadOffset, 0, Score.DeadWidth, scoreRows[n].Rows );
                integer = recognizeInteger( scoreRows[n].GetSubArr( out submat, rect ));
                score.dead = integer;
                // 貢献度
                rect = new CvRect (Score.ContributionOffset, 0, Score.ContributionWidth, scoreRows[n].Rows );
                integer = recognizeInteger( scoreRows[n].GetSubArr( out submat, rect ));
                score.contribution = integer;
                // PC与ダメージ
                rect = new CvRect (Score.PcDamageOffset, 0, Score.PcDamageWidth, scoreRows[n].Rows );
                integer = recognizeInteger( scoreRows[n].GetSubArr( out submat, rect ));
                score.pcDamage = integer;
                // 建築与ダメージ
                integer = recognizeInteger( scoreRows[n].GetCols( Score.ObjectDamageOffset, scoreRows[n].Cols));
                score.objectDamage = integer;
                
                scores.Add( score );
            }
        }
    }

}
