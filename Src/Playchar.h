//=============================================================================
//		プレイキャラクター　ヘッダファイル							Playchar.h
//=============================================================================
#pragma once
#include "GameMain.h"
#include "Weapon.h"
#include "Ability.h"

//======================================================================
// プレイキャラクター　オブジェクトクラス
//======================================================================
class CPcObj : public CBaseObj
{
protected:
	int					m_nAllyNum;				// 味方の数
	int					m_nAttackKind;			// 行った攻撃の種類
	int					m_nGunMagazine;			// マガジン段数
	int					m_nCoolTime;			// 近距離武器のクールタイム
	int					m_nShotSlashCoolTime;	// 斬撃クールタイム
	float				m_fSpeed;				// 移動速度      
	float				m_fRotSpeed;			// 回転速度
	float				m_fLocalRotY;			// ローカル軸(Y軸) 
	float				m_fAngle;				// 目標回転角度

	BOOL				m_bIsKeyPush;			// キーを押したか
	BOOL				m_bIsCanMove;			// アニメーションによって移動可能か
	BOOL				m_bIsHaveAxe;			// 斧の遠投フラグ
	BOOL				m_bIsToLateKeyPush;		// 2つ以上のキーが押されているか

	// ゲームオーバー時に使用
	int					m_nGameOverPosY;		// 背景のスプライト位置

public:
	// GAMEMAIN処理
	void Update() override;
	void UpdateNormal();								// 通常時の更新処理
	void UpdateNormalMove();							// 移動に関する更新処理
	MATRIX4X4 UpdateNormalMoveKeystate(DWORD DIKey);	// 入力ごとの移動に関する更新
	void UpdateMoveRot();								// 目標回転角に向かって回転する処理
	void UpdateNormalAttack();							// 攻撃に関する更新処理
	void UpdateDamage();								// ダメージ処理
	void UpdateDead();									// 死亡時処理
	
	void UpdateGameOver();								// ゲームオーバー処理
	void UpdateGameClear();								// ゲームクリア処理

	void SetAllyObj();									// Map遷移時の味方オブジェクトの再生成処理
	void SetRetryAllyNum();								// リトライ時、初期味方オブジェクト数の計算
	void GetCursorRotate();								// カーソルの位置と重なっているワールド座標を向く

		// ImGui表示処理
	void ShowImgui(bool* p_open);

	// アクセス関数
	MATRIX4X4  GetLocalMatrix();
	void SetLocalRotY(float fRotY);
	void AddAllyNum(int num);
	BOOL GetKeyPush();
	int  GetAttackKind();
	int  GetAllyNum();
	int  GetGunMagazine();
	BOOL GetIsHaveAxe();
	void SetIsHaveAxe(BOOL flag);
	int  GetCoolTime();
	void SetCanMove(BOOL flag);

	CPcObj(CBaseProc* pProc);	// コンストラクタ 
	virtual	~CPcObj();
};

//======================================================================
// プレイキャラクター　プロシージャクラス
//======================================================================
class CPcProc : public CBaseProc
{
protected:
	CAbility* m_pAbility;
public:
	// アクセス関数
	CAbility*	GetAbility();
	CPcObj*		GetPcObjPtr();

	// コンストラクタ
	CPcProc(CGameMain* pGMain);
	virtual	~CPcProc();
};

//======================================================================
// プレイキャラクター　定数クラス
//======================================================================
class PcConstruct
{
public:
	static const float	SPEED;
	static const float	ROT_SPEED;

	static const VECTOR3 HITBOX_MIN;
	static const VECTOR3 HITBOX_MAX;

	static const int	START_ALLY_NUM = 4;
	static const int	BASE_MAGAZINE_NUM = 10;

	static const int	WAITTIME_DEAD = 4;
	static const float	WAITTIME_GUN;
	static const int	WAITTIME_RELOAD = 2;
	static const int	WAITTIME_FLASH = 60;
	static const int	WAITTIME_MELEE_WEAPON = 1;
	static const int	WAITTIME_SHOTSLASH = 3;

	static const int	ANIM_IDLE = 0;
	static const int	ANIM_WALK = 1;
	static const int	ANIM_SWORD = 2;
	static const int	ANIM_ARROW = 3;
	static const int	ANIM_AXE = 4;
	static const int	ANIM_GUN = 5;
	static const int	ANIM_DEAD = 6;
	static const int	ANIM_FALL = 7;
	static const int	ANIM_VICTORY = 8;
};