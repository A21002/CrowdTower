//=============================================================================
//		敵キャラクター　ヘッダファイル							Enemy.h
//=============================================================================
#pragma once
#include "GameMain.h"
#include "Map.h"

#define  ENM_RW_ID           4
#define  ENM_RW_MAX          100

class CEnmProc;
//======================================================================
// 敵　オブジェクトクラス
//======================================================================
class CEnmObj : public CBaseObj
{
protected:
	VECTOR3				m_vTarget;			// ターゲット
	NAVIGATIONMAP		m_Nav;				// ナビゲーションマップ
	CSearchRoute*		m_pSearchRoute;		// 経路探索
	BOOL				m_bIsMove;
	BASEHOLDITEM		m_HoldItem;			// 手に持つアイテム 

public:
	BOOL Start() override;
	void Update() override;
	void UpdateNormal();
	void UpdateAttack();
	void UpdateDamage();
	void UpdateDead();

	void SetHoldItem(DWORD weaponId);

	// アクセス関数
	void SetNav(NAVIGATIONMAP  Nav);

	CEnmObj(CBaseProc* pProc);	// コンストラクタ
	virtual	~CEnmObj();	// デストラクタ
};

//======================================================================
// 敵　プロシージャクラス
//======================================================================
class CEnmProc : public CBaseProc
{
protected:
	BOOL				m_bIsStartFlag;	// スポーン済みか
public:
	BOOL Start();
	BOOL Start(VECTOR3 vMin, VECTOR3 vMax, int nNum);
	void Update() override;
	void UpdateHitcheck();
	void ResetEnmNum();
	int  CountActiveEnm();
	void ResetStartFlag();
	void SetMaxNum() override;
	CEnmProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CEnmProc();	// デストラクタ

};

//======================================================================
// 敵　定数クラス
//======================================================================
class EnemyConstruct
{
public:
	static const float	SPEED;
	static const float	SWORD_RANGE;
	static const VECTOR3 HITBOX_MIN;
	static const VECTOR3 HITBOX_MAX;

	static const int	RECOIL = 30;
	static const int	COOLTIME = 1;

	static const int	ANIM_IDLE = 0;
	static const int	ANIM_WALK = 1;
	static const int	ANIM_SWORD = 2;
	static const int	ANIM_ARROW = 3;
	static const int	ANIM_HIT = 4;
	static const int	ANIM_DIVE = 5;
	static const int	ANIM_LOOKAROUND = 6;
};