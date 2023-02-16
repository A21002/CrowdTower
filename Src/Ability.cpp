#include "Ability.h"
#include "GameMain.h"
#include "BackFore.h"
#include "Playchar.h"
#include <tchar.h>

// ---------------------------------------------------------------------------
// �R���X�g���N�^
// ---------------------------------------------------------------------------
CAbility::CAbility(CGameMain* pGMain)
{
	for (int i = 0; i < ABILITY_KIND; i++)
	{
		shortAbility[i] = 0;
		longAbility[i] = 0;
	}
	m_pGMain = pGMain;
	shortWeapon = SWORD;
	longWeapon = ARROW;
}

// ---------------------------------------------------------------------------
// �����̔\�͂��ő勭���ς݂�
// TRUE: ���x��3����
// FALSE: �ő勭���ς�
// ---------------------------------------------------------------------------
bool CAbility::CheckAbilityLevel(int num)
{
	int weaponKind = num / 3;
	int abilityKind = num % 3;
	int level = 0;
	
	// �������̕���̔\�͂ł����TRUE��Ԃ�
	if (weaponKind != longWeapon && weaponKind != shortWeapon)
	{
		return true;
	}

	// num���ߐڂ����u��
	if (weaponKind % 2 == 0) {
		// ���u
		level = longAbility[abilityKind];
	}
	else {
		// �ߐ�
		level = shortAbility[abilityKind];
	}

	// �ő僌�x���̏ꍇFALSE��Ԃ�
	if (level == 3) {
		return false;
	}
	else {
		return true;
	}
}

// ---------------------------------------------------------------------------
// �����̔\�͂̃��x���l��Ԃ�
// ---------------------------------------------------------------------------
int CAbility::GetAbilityLevel(int num) {
	int weaponKind = num / 3;
	int abilityKind = num % 3;
	int level = 0;

	// num���ߐڂ����u��
	if (weaponKind % 2 == 0) {
		// ���u
		level = longAbility[abilityKind];
	}
	else {
		// �ߐ�
		level = shortAbility[abilityKind];
	}

	return level;
}

// ---------------------------------------------------------------------------
// �����̔\�͂̃��x����1�グ��
// ---------------------------------------------------------------------------
void CAbility::AddAbilityLevel(int num) {
	int weaponKind = num / 3;
	int abilityKind = num % 3;
	int level = 0;

	// num���ߐڂ����u��
	if (weaponKind % 2 == 0) {
		// ���u
		if (longWeapon != weaponKind) {
			// ���ݎ����Ă��镐��ƈႤ�����I�������ꍇ
			if (!m_pGMain->m_pBackForeProc->GetFirstGun()) {
				m_pGMain->m_pBackForeProc->m_nDialogKind = DIALOG_TUTORIAL_GUN;
				m_pGMain->m_pBackForeProc->SetFirstGun(true);
				m_pGMain->m_pPcProc->GetPcObjPtr()->SetCanMove(FALSE);
			}
			longWeapon = weaponKind;
			ResetLLongAbility();
		}
		longAbility[abilityKind]++;
	}
	else {
		// �ߐ�
		if (shortWeapon != weaponKind) {
			// ���ݎ����Ă��镐��ƈႤ�����I�������ꍇ
			if (!m_pGMain->m_pBackForeProc->GetFirstAxe()) {
				m_pGMain->m_pBackForeProc->m_nDialogKind = DIALOG_TUTORIAL_AXE;
				m_pGMain->m_pBackForeProc->SetFirstAxe(true);
				m_pGMain->m_pPcProc->GetPcObjPtr()->SetCanMove(FALSE);
			}
			shortWeapon = weaponKind;
			ResetShortAbility();
		}
		shortAbility[abilityKind]++;
	}
}

// ---------------------------------------------------------------------------
// ����������̎擾�ς݂̔\�͂̐���Ԃ�
// ---------------------------------------------------------------------------
int CAbility::GetLongAbilityNum()
{
	int num = 0;
	for (int i = 0; i < 3; i++) {
		if (longAbility[i] != 0) {
			// �\�͂��擾�ς�
			num++;
		}
	}
	return num;
}

// ---------------------------------------------------------------------------
// �ߋ�������̎擾�ς݂̔\�͂̐���Ԃ�
// ---------------------------------------------------------------------------
int CAbility::GetShortAbilityNum()
{
	int num = 0;
	for (int i = 0; i < 3; i++)
	{
		if (shortAbility[i] != 0)
		{
			// �\�͂��擾�ς�
			num++;
		}
	}
	return num;
}

// ---------------------------------------------------------------------------
// �ߋ�������̔\�͂����Z�b�g����
// ---------------------------------------------------------------------------
void CAbility::ResetShortAbility()
{
	for (int i = 0; i < ABILITY_KIND; i++)
	{
		shortAbility[i] = 0;
	}
}

// ---------------------------------------------------------------------------
// ����������̔\�͂����Z�b�g����
// ---------------------------------------------------------------------------
void CAbility::ResetLLongAbility()
{
	for (int i = 0; i < ABILITY_KIND; i++)
	{
		longAbility[i] = 0;
	}
}

//============================================================================
// �A�N�Z�X�֐�
//============================================================================
void CAbility::SetShortWeapon(int num)
{
	shortWeapon = num;
}

void CAbility::SetLongWeapon(int num)
{
	longWeapon = num;
}

void CAbility::SetShortWeaponAbility(int num1, int num2, int num3)
{
	shortAbility[0] = num1;
	shortAbility[1] = num2;
	shortAbility[2] = num3;
}

void CAbility::SetLongWeaponAbility(int num1, int num2, int num3)
{
	longAbility[0] = num1;
	longAbility[1] = num2;
	longAbility[2] = num3;
}

int CAbility::GetShortWeapon()
{
	return shortWeapon;
}

int CAbility::GetLongWeapon()
{
	return longWeapon;
}