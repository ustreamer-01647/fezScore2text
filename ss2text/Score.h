#pragma once

#include <iostream>
#include <string>


// ����
enum Nationality { NUnknown, NNetzawar, NIelsord, NHordaine, NGeburand, NCesedria };
// �N���X�Dclass���\���Ȃ̂� job �Ƃ���
enum Job { JUnknown, JWarrior, JScout, JSorcerer, JFencer, JCestas };

// �X�R�A
struct Score
{
	// ����
	int rank;
	static const int RankOffset = 0;
	static const int RankWidth = 20;
	// �L�����N�^��
	std::string name;
	static const int NameOffset = 29;
	static const int NameWidth = 114;
	// ������
	enum Nationality nationality;
	static const int NationalityOffset = 180;
	static const int NationalityWidth = 24;
	// �N���X
	enum Job job;
	static const int JobOffset = 225;
	static const int JobWidth = 84;
	// �L����
	int kill;
	static const int KillOffset = 323;
	static const int KillWidth = 46;
	// �f�b�h��
	int dead;
	static const int DeadOffset = 386;
	static const int DeadWidth = 40;
	// �v���x
	int contribution;
	static const int ContributionOffset = 451;
	static const int ContributionWidth = 50;
	// PC�^�_���[�W
	int pcDamage;
	static const int PcDamageOffset = 523;
	static const int PcDamageWidth = 88;
	// �����^�_���[�W
	int objectDamage;
	static const int ObjectDamageOffset = 629;
	//static const int ObjectDamageWidth; // �摜�E�[

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
