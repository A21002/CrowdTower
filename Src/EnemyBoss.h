#pragma once
//=============================================================================
//		敵ボスキャラクター　ヘッダファイル							EnemyBoss.h
//=============================================================================
#pragma once
#include "GameMain.h"
#include "Map.h"

#define  ENM_BOSS_MAX          1

class CEnmBossProc;
//======================================================================
// 敵ボス　オブジェクトクラス
//======================================================================
class CEnmBossObj : public CBaseObj
{
protected:
	VECTOR3				m_vTarget;			// ターゲット
	CSearchRoute*		m_pSearchRoute;		// 経路探索
	BOOL				m_bIsMove;
	BOOL				m_bIsMovieFinish;
	BOOL				m_bIsBossMovie;
	int					m_nRoarCoolTime;
	int					m_nOldAttackKind;

public:
	BOOL Start(VECTOR3 vPos) override;
	void Update() override;
	void UpdateGame();
	void UpdateMovie();
	void UpdateNormal();
	void UpdateAttack();
	void UpdateDamage();
	void UpdateDead();

	// アクセス関数
	void SetMovieFinish(bool flag);
	BOOL GetMovieFinish();

	CEnmBossObj(CBaseProc* pProc);	// コンストラクタ
	virtual	~CEnmBossObj();	// デストラクタ
};

//======================================================================
// 敵ボス　プロシージャクラス
//======================================================================
class CEnmBossProc : public CBaseProc
{
protected:

public:
	BOOL Start(VECTOR3 vPos);
	void ResetEnmNum();
	int  CountActiveEnm();
	void SetMaxNum() override;
	void SetHoldItem(DWORD weaponId) override;
	CEnmBossObj* GetEnmBossObjPtr();


	CEnmBossProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CEnmBossProc();	// デストラクタ

};

//======================================================================
// 敵　定数クラス
//======================================================================
class EnemyBossConstruct
{
public:
	static const int	ANIM_IDLE		= 0;
	static const int	ANIM_WALK		= 1;
	static const int	ANIM_KICK		= 2;
	static const int	ANIM_KICK_LEFT	= 3;
	static const int	ANIM_STOMP		= 4;
	static const int	ANIM_STOMP_LEFT = 5;
	static const int	ANIM_BODYSTOMP	= 6;
	static const int	ANIM_STANDUP	= 7;
	static const int	ANIM_ROAR		= 8;
	static const int	ANIM_DIE		= 9;

	static const int	MELEE_RANGE		= 10;
	static const int	ATTACK_KICK_RIGHT	= 1;
	static const int	ATTACK_KICK_LEFT	= 2;
	static const int	ATTACK_STOMP_RIGHT	= 3;
	static const int	ATTACK_STOMP_LEFT	= 4;
	static const int	ATTACK_BODYSTOMP	= 5;

	static const int	ROAR_COOLTIME	= 8;
	static const int	ROAR_SPOWN_NUM	= 5;

	static const int	HP = 10000;

	static const float	SPEED;
};