#pragma once
#include "GameMain.h"
// 能力を管理するクラス
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
	//		  弓矢の能力
	// 1：攻撃力↑
	// 2：反射
	// 3：貫通
	//-----------------------------
	int longAbility[ABILITY_KIND];
	int longWeapon;

	//-----------------------------
	//		  　剣の能力
	// 1：攻撃力↑
	// 2：斬撃
	// 3：剣のサイズ
	//-----------------------------
	int shortAbility[ABILITY_KIND];
	int shortWeapon;

	CGameMain* m_pGMain;				// ゲームメインクラス

public:
	// 最大強化済みか
	bool CheckAbilityLevel(int num);

	// アクセス関数
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

	// コンストラクタ
	CAbility(CGameMain* pGMain);
};