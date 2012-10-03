using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ss2textCS
{

    // 国家
    enum 所属国 { 不明, ネツァワル, エルソード, ホルデイン, ゲブランド, カセドリア };
    // クラス．classが予約語なので job とする
    enum クラス { 不明, ウォリアー, スカウト, ソーサラー, フェンサー, セスタス };

    class Score
    {
        // 順位
        public int rank { get; set; }
        public const int RankOffset = 0;
        public const int RankWidth = 20;
        // キャラクタ名
        public String name { get; set; }
        public const int NameOffset = 29;
        public const int NameWidth = 114;
        // 所属国
        public 所属国 nationality { get; set; }
        public const int NationalityOffset = 180;
        public const int NationalityWidth = 24;
        // クラス
        public クラス job { get; set; }
        public const int JobOffset = 225;
        public const int JobWidth = 84;
        // キル数
        public int kill { get; set; }
        public const int KillOffset = 323;
        public const int KillWidth = 46;
        // デッド数
        public int dead { get; set; }
        public const int DeadOffset = 386;
        public const int DeadWidth = 40;
        // 貢献度
        public int contribution { get; set; }
        public const int ContributionOffset = 451;
        public const int ContributionWidth = 50;
        // PC与ダメージ
        public int pcDamage { get; set; }
        public const int PcDamageOffset = 523;
        public const int PcDamageWidth = 88;
        // 建物与ダメージ
        public int objectDamage { get; set; }
        public const int ObjectDamageOffset = 629;
        //const int ObjectDamageWidth; // 画像右端

        public Score()
        {
            rank = 0;
            name = "774";
            nationality = 所属国.不明;
            job = クラス.不明;
            kill = 0;
            dead = 0;
            contribution = 0;
            pcDamage = 0;
            objectDamage = 0;
        }

        public void dump()
        {
            Console.WriteLine(
                 rank + ", " + name + ", " + nationality + ", " + job + ", "
                + kill + ", " + dead + ", " + contribution + ", "
                + pcDamage + ", " + objectDamage);
        }

    }
}
