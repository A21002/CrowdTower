#include "Ability.h"
#include "GameMain.h"
#include "BackFore.h"
#include "Playchar.h"
#include <tchar.h>

// ---------------------------------------------------------------------------
// コンストラクタ
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
// 引数の能力が最大強化済みか
// TRUE: レベル3未満
// FALSE: 最大強化済み
// ---------------------------------------------------------------------------
bool CAbility::CheckAbilityLevel(int num)
{
	int weaponKind = num / 3;
	int abilityKind = num % 3;
	int level = 0;
	
	// 未所持の武器の能力であればTRUEを返す
	if (weaponKind != longWeapon && weaponKind != shortWeapon)
	{
		return true;
	}

	// numが近接が遠隔か
	if (weaponKind % 2 == 0) {
		// 遠隔
		level = longAbility[abilityKind];
	}
	else {
		// 近接
		level = shortAbility[abilityKind];
	}

	// 最大レベルの場合FALSEを返す
	if (level == 3) {
		return false;
	}
	else {
		return true;
	}
}

// ---------------------------------------------------------------------------
// 引数の能力のレベル値を返す
// ---------------------------------------------------------------------------
int CAbility::GetAbilityLevel(int num) {
	int weaponKind = num / 3;
	int abilityKind = num % 3;
	int level = 0;

	// numが近接が遠隔か
	if (weaponKind % 2 == 0) {
		// 遠隔
		level = longAbility[abilityKind];
	}
	else {
		// 近接
		level = shortAbility[abilityKind];
	}

	return level;
}

// ---------------------------------------------------------------------------
// 引数の能力のレベルを1上げる
// ---------------------------------------------------------------------------
void CAbility::AddAbilityLevel(int num) {
	int weaponKind = num / 3;
	int abilityKind = num % 3;
	int level = 0;

	// numが近接が遠隔か
	if (weaponKind % 2 == 0) {
		// 遠隔
		if (longWeapon != weaponKind) {
			// 現在持っている武器と違う武器を選択した場合
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
		// 近接
		if (shortWeapon != weaponKind) {
			// 現在持っている武器と違う武器を選択した場合
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
// 遠距離武器の取得済みの能力の数を返す
// ---------------------------------------------------------------------------
int CAbility::GetLongAbilityNum()
{
	int num = 0;
	for (int i = 0; i < 3; i++) {
		if (longAbility[i] != 0) {
			// 能力を取得済み
			num++;
		}
	}
	return num;
}

// ---------------------------------------------------------------------------
// 近距離武器の取得済みの能力の数を返す
// ---------------------------------------------------------------------------
int CAbility::GetShortAbilityNum()
{
	int num = 0;
	for (int i = 0; i < 3; i++)
	{
		if (shortAbility[i] != 0)
		{
			// 能力を取得済み
			num++;
		}
	}
	return num;
}

// ---------------------------------------------------------------------------
// 近距離武器の能力をリセットする
// ---------------------------------------------------------------------------
void CAbility::ResetShortAbility()
{
	for (int i = 0; i < ABILITY_KIND; i++)
	{
		shortAbility[i] = 0;
	}
}

// ---------------------------------------------------------------------------
// 遠距離武器の能力をリセットする
// ---------------------------------------------------------------------------
void CAbility::ResetLLongAbility()
{
	for (int i = 0; i < ABILITY_KIND; i++)
	{
		longAbility[i] = 0;
	}
}

//============================================================================
// アクセス関数
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