#pragma once
#include "GameMain.h"
// �\�͂��Ǘ�����N���X
#define ARROW 0
#define SWORD 1
#define GUN 2
#define AXE 3

#define WEAPON_KIND 4
#define ABILITY_KIND 3

class CAbility
{
private:
	//-----------------------------
	//		  �|��̔\��
	// 1�F�U���́�
	// 2�F����
	// 3�F�ђ�
	//-----------------------------
	int longAbility[ABILITY_KIND];
	int longWeapon;

	//-----------------------------
	//		  �@���̔\��
	// 1�F�U���́�
	// 2�F�a��
	// 3�F���̃T�C�Y
	//-----------------------------
	int shortAbility[ABILITY_KIND];
	int shortWeapon;

	CGameMain* m_pGMain;				// �Q�[�����C���N���X

public:
	// �ő勭���ς݂�
	bool CheckAbilityLevel(int num);

	// �A�N�Z�X�֐�
	int GetShortWeapon();
	int GetLongWeapon();

	int GetAbilityLevel(int num);
	void AddAbilityLevel(int num);

	int GetLongAbilityNum();
	int GetShortAbilityNum();
	
	void ResetShortAbility();
	void ResetLLongAbility();

	void SetShortWeapon(int num);
	void SetLongWeapon(int num);
	void SetShortWeaponAbility(int num1, int num2, int num3);
	void SetLongWeaponAbility(int num1, int num2, int num3);

	// �R���X�g���N�^
	CAbility(CGameMain* pGMain);
};