//=============================================================================
//		武器　　　ヘッダファイル								Weapon.h
//=============================================================================
#pragma once

#include "GameMain.h"
#include "Effect.h"

#define  ARROW_MAX          200
#define  ITEMBOX_MAX		1
#define  SWORD_MAX			200
#define  SWORDSHOT_MAX		200
#define  GUN_MAX			100
#define  AXE_MAX		    100
#define  AXESHOT_MAX		100
#define  DAMAGEAREA_MAX		10
#define  ATTACKBOX_MAX		10
#define  BALL_MAX           200
#define  SPRAY_MAX          300
#define  HOLDITEM_MAX       15

// --------------------------------------------------------------------
//武器　弓矢　オブジェクトクラス
// --------------------------------------------------------------------
class CWeaponArrowObj : public CBaseObj
{
protected:
	DWORD                    m_dwOwner;				// 武器を発射したキャラ区分（PC:PC　ENM:敵）
	int						 m_nReflectionCount;	// 反射可能回数
	int						 m_nHitCount;			// 貫通回数

public:
	BOOL Start(MATRIX4X4, DWORD dwOwner) override;
	void Update() override;
	void Render();

	// アクセス関数
	CWeaponArrowObj(CBaseProc* pProc);	// コンストラクタ        // -- 2022.2.16
	virtual	~CWeaponArrowObj();
};

// --------------------------------------------------------------------
//武器　弓矢　プロシージャクラス
// --------------------------------------------------------------------
class CWeaponArrowProc : public CBaseProc
{
protected:
	;
public:
	BOOL Start(MATRIX4X4, DWORD dwOwner);  //武器　レーザーの発射

	CWeaponArrowProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CWeaponArrowProc();
};

// --------------------------------------------------------------------
//武器　宝箱　オブジェクトクラス
// --------------------------------------------------------------------
class CWeaponItemBoxObj : public CBaseObj
{
protected:

public:
	BOOL Start(VECTOR3 vPos) override;
	void Update() override;
	void Render();

	CWeaponItemBoxObj(CBaseProc* pProc);	// コンストラクタ        // -- 2022.2.16
	virtual	~CWeaponItemBoxObj();
};

// --------------------------------------------------------------------
//武器　宝箱　プロシージャクラス
// --------------------------------------------------------------------
class CWeaponItemBoxProc : public CBaseProc
{
protected:
	;
public:
	BOOL Start(VECTOR3 vPos);

	CWeaponItemBoxProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CWeaponItemBoxProc();
};

// --------------------------------------------------------------------
//武器　剣　オブジェクトクラス
// --------------------------------------------------------------------
class CWeaponSwordObj : public CBaseObj
{
protected:
	DWORD           m_dwOwner;

public:
	BOOL Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner) override;
	void Update() override;

	CWeaponSwordObj(CBaseProc* pProc);	// コンストラクタ
	virtual	~CWeaponSwordObj();
};

// --------------------------------------------------------------------
// 武器　剣　プロシージャクラス
// --------------------------------------------------------------------
class CWeaponSwordProc : public CBaseProc
{
public:
	BOOL Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner);	// 剣の発射
	CWeaponSwordProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CWeaponSwordProc() { ; }
};

// --------------------------------------------------------------------
//武器　斬撃　オブジェクトクラス
// --------------------------------------------------------------------
class CWeaponShotSlashObj : public CBaseObj
{
protected:
	CBaseObj*				m_pEffect;
	float					m_fLength;

public:
	BOOL Start(MATRIX4X4, DWORD dwOwner) override;
	void Update() override;

	// アクセス関数
	CWeaponShotSlashObj(CBaseProc* pProc);	// コンストラクタ        // -- 2022.2.16
	virtual	~CWeaponShotSlashObj();
};

// --------------------------------------------------------------------
//武器　斬撃　プロシージャクラス
// --------------------------------------------------------------------
class CWeaponShotSlashProc : public CBaseProc
{
protected:
	;
public:
	BOOL Start(MATRIX4X4, DWORD dwOwner);  //武器　レーザーの発射
	//BOOL Start(MATRIX4X4 mGun, VECTOR3 vOffset, MATRIX4X4 mOwnerWorld, DWORD dwOwner);

	CWeaponShotSlashProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CWeaponShotSlashProc();
};

// --------------------------------------------------------------------
//武器　銃　オブジェクトクラス
// --------------------------------------------------------------------
class CWeaponGunObj : public CBaseObj
{
protected:
public:
	BOOL Start(MATRIX4X4 mWorld, DWORD dwOwner) override;
	void Update() override;
	void Render();

	// アクセス関数
	CWeaponGunObj(CBaseProc* pProc);	// コンストラクタ     
	virtual	~CWeaponGunObj();
};

// --------------------------------------------------------------------
//武器　銃　プロシージャクラス
// --------------------------------------------------------------------
class CWeaponGunProc : public CBaseProc
{
protected:
	;
public:
	BOOL Start(MATRIX4X4 mWorld, DWORD dwOwner);//武器　火炎の発射

	CWeaponGunProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CWeaponGunProc();
};

// --------------------------------------------------------------------
//武器　斧　オブジェクトクラス
// --------------------------------------------------------------------
class CWeaponAxeObj : public CBaseObj
{
protected:
public:
	BOOL Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner) override;
	void Update() override;

	// アクセス関数
	CWeaponAxeObj(CBaseProc* pProc);	// コンストラクタ
	virtual	~CWeaponAxeObj();
};

// --------------------------------------------------------------------
//武器　斧プロシージャクラス
// --------------------------------------------------------------------
class CWeaponAxeProc : public CBaseProc
{
protected:
	;
public:
	BOOL Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner);	// 剣の発射

	CWeaponAxeProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CWeaponAxeProc();
};

// --------------------------------------------------------------------
//武器　斧投げ　オブジェクトクラス
// --------------------------------------------------------------------
class CWeaponAxeShotObj : public CBaseObj
{
protected:
	DWORD               m_dwOwner;
	FLOAT				m_fLength;
	BOOL				m_bBackFlag;
	CBaseObj*			m_pEffect;
	float               m_fMoveRot;
	float               m_fAnimRot;

public:
	BOOL Start(MATRIX4X4 mStartWorld, DWORD dwLength, DWORD dwOwner) override;
	void Update() override;
	void Render();


	CWeaponAxeShotObj(CBaseProc* pProc);	// コンストラクタ      
	virtual	~CWeaponAxeShotObj();
};

// --------------------------------------------------------------------
//武器　斧投げプロシージャクラス
// --------------------------------------------------------------------
class CWeaponAxeShotProc : public CBaseProc
{
protected:
public:
	BOOL Start(MATRIX4X4 mGun, DWORD dwLength, DWORD dwOwner);  //武器　斧の発射

	CWeaponAxeShotProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CWeaponAxeShotProc();
};

// --------------------------------------------------------------------
//武器　ダメージエリア　オブジェクトクラス
// --------------------------------------------------------------------
class CWeaponDamageAreaObj : public CBaseObj
{
protected:

public:
	BOOL Start(VECTOR3 vStart, VECTOR3 vTarget, DWORD dwOwner) override;
	void Update() override;


	CWeaponDamageAreaObj(CBaseProc* pProc);	// コンストラクタ      
	virtual	~CWeaponDamageAreaObj();
};

// --------------------------------------------------------------------
//武器　ダメージエリア プロシージャクラス
// --------------------------------------------------------------------
class CWeaponDamageAreaProc : public CBaseProc
{
protected:
	DWORD		m_dwDamageTime;
	DWORD       m_dwKind;
	float		m_fAlfa;
	BOOL		m_bIsInObject;

public:
	BOOL Start(VECTOR3 min, VECTOR3 max, DWORD Kind);  //武器　斧の発射
	void Update() override;

	// アクセス関数
	void SetIsInObject(BOOL flag);

	CWeaponDamageAreaProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CWeaponDamageAreaProc();
};

// --------------------------------------------------------------------
//武器　攻撃ボックス　オブジェクトクラス
// --------------------------------------------------------------------
class CWeaponAttackBoxObj : public CBaseObj
{
protected:

public:
	BOOL Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner) override;
	void Render();


	CWeaponAttackBoxObj(CBaseProc* pProc);	// コンストラクタ      
	virtual	~CWeaponAttackBoxObj();
};

// --------------------------------------------------------------------
//武器　攻撃ボックス プロシージャクラス
// --------------------------------------------------------------------
class CWeaponAttackBoxProc : public CBaseProc
{
protected:

public:
	BOOL Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner);  //武器　斧の発射

	CWeaponAttackBoxProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CWeaponAttackBoxProc();
};


// --------------------------------------------------------------------
//武器　手に持つアイテム　オブジェクトクラス
// --------------------------------------------------------------------
class CWeaponHoldObj : public CBaseObj
{
protected:
	int                      m_nKindNo;			// 手に持つアイテムの種類(1:剣　2:銃)
	CFbxMesh*                m_pMesh;			// Fbxメッシュ(プロシージャではなくオブジェクトにメッシュを持たせる)
	VECTOR3                  m_vOffset;			// 銃口までのオフセット（銃のみ）
public:
	void Init();
	BOOL Disp(MATRIX4X4 mHold, BASEHOLDITEM HoldItem);
	void GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vOffset);
	void GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vMin, VECTOR3& vMax);
	CBBox* GetBBox(MATRIX4X4 mHold, BASEHOLDITEM HoldItem);

	void Update() override;
	void Render();

	int GetKind() { return m_nKindNo; }

	CWeaponHoldObj(CBaseProc* pProc);	// コンストラクタ        // -- 2022.2.16
	virtual	~CWeaponHoldObj();
};
// --------------------------------------------------------------------
//武器　手に持つアイテム　プロシージャクラス
// --------------------------------------------------------------------
class CWeaponHoldProc : public CBaseProc
{
protected:
	;
public:
	BOOL Disp(MATRIX4X4 mHold, BASEHOLDITEM HoldItem); // 武器　の表示
	void GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vOffset);
	void GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vMin, VECTOR3& vMax);
	CBBox* GetBBox(MATRIX4X4 mHold, BASEHOLDITEM HoldItem);
	int GetKind(BASEHOLDITEM HoldItem) { return ((CWeaponHoldObj*)m_pObjArray[HoldItem.m_nHoldObjNo])->GetKind(); }
	CWeaponHoldProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CWeaponHoldProc();
};

// ======================================================================
// 武器全体のメインプロシージャクラス
// ======================================================================
class CWeaponProc : public CBaseProc
{
public:
	CWeaponArrowProc*		m_pWeaponArrowProc;
	CWeaponItemBoxProc*		m_pWeaponItemBoxProc;
	CWeaponSwordProc*		m_pWeaponSwordProc;
	CWeaponShotSlashProc*	m_pWeaponShotSlashProc;
	CWeaponGunProc*			m_pWeaponGunProc;

	CWeaponAxeProc*			m_pWeaponAxeProc;
	CWeaponAxeShotProc*		m_pWeaponAxeShotProc;
	CWeaponDamageAreaProc*	m_pWeaponDamageAreaProc;
	CWeaponAttackBoxProc*	m_pWeaponAttackBoxProc;
	CWeaponHoldProc*		m_pWeaponHoldProc;

public:
	CWeaponProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CWeaponProc() { ; }
};

class WeaponConstruct
{
public:
	static const int		ARROW_ATTACK = 100;
	static const float		ARROW_SPEED;
	static const VECTOR3	ARROW_START_DIFF;
	static const VECTOR3	ARROW_HITBOX_MIN;
	static const VECTOR3	ARROW_HITBOX_MAX;

	static const VECTOR3	BOX_HITBOX_MIN;
	static const VECTOR3	BOX_HITBOX_MAX;

	static const int		SWORD_ATTACK = 100;

	static const int		SHOTSLASH_ATTACK = 100;
	static const int		SHOTSLASH_LENGTH = 2;
	static const float		SHOTSLASH_SPEED;
	static const VECTOR3	SHOTSLASH_HITBOX_MIN;
	static const VECTOR3	SHOTSLASH_HITBOX_MAX;

	static const int		GUN_ATTACK = 50;
	static const float		GUN_SPEED;
	static const VECTOR3	GUN_HITBOX_MIN;
	static const VECTOR3	GUN_HITBOX_MAX;

	static const int		AXE_ATTACK = 100;

	static const int		SHOTAXE_ATTACK = 100;
	static const float		SHOTAXE_SPEED;
	static const float		SHOTAXE_ROTSPEED;
	static const VECTOR3	SHOTAXE_HITBOX_MIN;
	static const VECTOR3	SHOTAXE_HITBOX_MAX;
};
