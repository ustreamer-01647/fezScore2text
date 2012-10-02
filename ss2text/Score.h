#pragma once

#include <iostream>
#include <string>
#include <sstream>

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

	std::string getNation()
	{
		switch ( nationality )
		{
		case NNetzawar:
			return "�l�c�@����";
		case NIelsord:
			return "�G���\�[�h";
		case NHordaine:
			return "�z���f�C��";
		case NGeburand:
			return "�Q�u�����h";
		case NCesedria:
			return "�J�Z�h���A";
		case NUnknown:
		default:
			return "�s��";
		}
	}

	std::string getJob()
	{
		switch ( job )
		{
		case Job::JWarrior:
			return "�E�H���A�[";
		case Job::JScout:
			return "�X�J�E�g";
		case Job::JSorcerer:
			return "�\�[�T���[";
		case Job::JFencer:
			return "�t�F���T�[";
		case Job::JCestas:
			return "�Z�X�^�X";
		case Job::JUnknown:
		default:
			return "�s��";
		}
	}

	std::string toString()
	{
		std::string n = getNation();
		std::string j = getJob();

		std::stringstream ss;
		ss << rank << ", " << name << ", " << n<< ", " << j << ", "
			<< kill << ", " << dead << ", " <<  contribution << ", "
			<< pcDamage << ", " << objectDamage;

		return ss.str();

		//return rank + ", " + name + ", " + n + ", " + j + ", " + kill + ", " + dead + ", " + contribution
		//	+ ", " + pcDamage + ", " + objectDamage;
	}
};
