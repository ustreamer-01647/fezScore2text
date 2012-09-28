#pragma once

#include <iostream>
#include <string>


// 国家
enum Nationality { NUnknown, NNetzawar, NIelsord, NHordaine, NGeburand, NCesedria };
// クラス．classが予約語なので job とする
enum Job { JUnknown, JWarrior, JScout, JSorcerer, JFencer, JCestas };

// スコア
struct Score
{
	// 順位
	int rank;
	static const int RankOffset = 0;
	static const int RankWidth = 20;
	// キャラクタ名
	std::string name;
	static const int NameOffset = 29;
	static const int NameWidth = 114;
	// 所属国
	enum Nationality nationality;
	static const int NationalityOffset = 180;
	static const int NationalityWidth = 24;
	// クラス
	enum Job job;
	static const int JobOffset = 225;
	static const int JobWidth = 84;
	// キル数
	int kill;
	static const int KillOffset = 323;
	static const int KillWidth = 46;
	// デッド数
	int dead;
	static const int DeadOffset = 386;
	static const int DeadWidth = 40;
	// 貢献度
	int contribution;
	static const int ContributionOffset = 451;
	static const int ContributionWidth = 50;
	// PC与ダメージ
	int pcDamage;
	static const int PcDamageOffset = 523;
	static const int PcDamageWidth = 88;
	// 建物与ダメージ
	int objectDamage;
	static const int ObjectDamageOffset = 629;
	//static const int ObjectDamageWidth; // 画像右端

	Score()
	{
		int rank = 0;
		name = "774";
		nationality = NUnknown;
		job = JUnknown;
		kill = 0;
		dead = 0;
		contribution = 0;
		pcDamage = 0;
		objectDamage = 0;
	}

	void dump()
	{
		std::cout << rank << ", " << name << ", " << nationality << ", " << job << ", "
			<< kill << ", " << dead << ", " <<  contribution << ", "
			<< pcDamage << ", " << objectDamage << std::endl;
	}
};
