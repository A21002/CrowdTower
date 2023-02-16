//=============================================================================
//		����@�@�@�w�b�_�t�@�C��								Weapon.h
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
//����@�|��@�I�u�W�F�N�g�N���X
// --------------------------------------------------------------------
class CWeaponArrowObj : public CBaseObj
{
protected:
	DWORD                    m_dwOwner;				// ����𔭎˂����L�����敪�iPC:PC�@ENM:�G�j
	int						 m_nReflectionCount;	// ���ˉ\��
	int						 m_nHitCount;			// �ђʉ�

public:
	BOOL Start(MATRIX4X4, DWORD dwOwner) override;
	void Update() override;
	void Render();

	// �A�N�Z�X�֐�
	CWeaponArrowObj(CBaseProc* pProc);	// �R���X�g���N�^        // -- 2022.2.16
	virtual	~CWeaponArrowObj();
};

// --------------------------------------------------------------------
//����@�|��@�v���V�[�W���N���X
// --------------------------------------------------------------------
class CWeaponArrowProc : public CBaseProc
{
protected:
	;
public:
	BOOL Start(MATRIX4X4, DWORD dwOwner);  //����@���[�U�[�̔���

	CWeaponArrowProc(CGameMain* pGMain);	// �R���X�g���N�^
	virtual	~CWeaponArrowProc();
};

// --------------------------------------------------------------------
//����@�󔠁@�I�u�W�F�N�g�N���X
// --------------------------------------------------------------------
class CWeaponItemBoxObj : public CBaseObj
{
protected:

public:
	BOOL Start(VECTOR3 vPos) override;
	void Update() override;
	void Render();

	CWeaponItemBoxObj(CBaseProc* pProc);	// �R���X�g���N�^        // -- 2022.2.16
	virtual	~CWeaponItemBoxObj();
};

// --------------------------------------------------------------------
//����@�󔠁@�v���V�[�W���N���X
// --------------------------------------------------------------------
class CWeaponItemBoxProc : public CBaseProc
{
protected:
	;
public:
	BOOL Start(VECTOR3 vPos);

	CWeaponItemBoxProc(CGameMain* pGMain);	// �R���X�g���N�^
	virtual	~CWeaponItemBoxProc();
};

// --------------------------------------------------------------------
//����@���@�I�u�W�F�N�g�N���X
// --------------------------------------------------------------------
class CWeaponSwordObj : public CBaseObj
{
protected:
	DWORD           m_dwOwner;

public:
	BOOL Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner) override;
	void Update() override;

	CWeaponSwordObj(CBaseProc* pProc);	// �R���X�g���N�^
	virtual	~CWeaponSwordObj();
};

// --------------------------------------------------------------------
// ����@���@�v���V�[�W���N���X
// --------------------------------------------------------------------
class CWeaponSwordProc : public CBaseProc
{
public:
	BOOL Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner);	// ���̔���
	CWeaponSwordProc(CGameMain* pGMain);	// �R���X�g���N�^
	virtual	~CWeaponSwordProc() { ; }
};

// --------------------------------------------------------------------
//����@�a���@�I�u�W�F�N�g�N���X
// --------------------------------------------------------------------
class CWeaponShotSlashObj : public CBaseObj
{
protected:
	CBaseObj*				m_pEffect;
	float					m_fLength;

public:
	BOOL Start(MATRIX4X4, DWORD dwOwner) override;
	void Update() override;

	// �A�N�Z�X�֐�
	CWeaponShotSlashObj(CBaseProc* pProc);	// �R���X�g���N�^        // -- 2022.2.16
	virtual	~CWeaponShotSlashObj();
};

// --------------------------------------------------------------------
//����@�a���@�v���V�[�W���N���X
// --------------------------------------------------------------------
class CWeaponShotSlashProc : public CBaseProc
{
protected:
	;
public:
	BOOL Start(MATRIX4X4, DWORD dwOwner);  //����@���[�U�[�̔���
	//BOOL Start(MATRIX4X4 mGun, VECTOR3 vOffset, MATRIX4X4 mOwnerWorld, DWORD dwOwner);

	CWeaponShotSlashProc(CGameMain* pGMain);	// �R���X�g���N�^
	virtual	~CWeaponShotSlashProc();
};

// --------------------------------------------------------------------
//����@�e�@�I�u�W�F�N�g�N���X
// --------------------------------------------------------------------
class CWeaponGunObj : public CBaseObj
{
protected:
public:
	BOOL Start(MATRIX4X4 mWorld, DWORD dwOwner) override;
	void Update() override;
	void Render();

	// �A�N�Z�X�֐�
	CWeaponGunObj(CBaseProc* pProc);	// �R���X�g���N�^     
	virtual	~CWeaponGunObj();
};

// --------------------------------------------------------------------
//����@�e�@�v���V�[�W���N���X
// --------------------------------------------------------------------
class CWeaponGunProc : public CBaseProc
{
protected:
	;
public:
	BOOL Start(MATRIX4X4 mWorld, DWORD dwOwner);//����@�Ή��̔���

	CWeaponGunProc(CGameMain* pGMain);	// �R���X�g���N�^
	virtual	~CWeaponGunProc();
};

// --------------------------------------------------------------------
//����@���@�I�u�W�F�N�g�N���X
// --------------------------------------------------------------------
class CWeaponAxeObj : public CBaseObj
{
protected:
public:
	BOOL Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner) override;
	void Update() override;

	// �A�N�Z�X�֐�
	CWeaponAxeObj(CBaseProc* pProc);	// �R���X�g���N�^
	virtual	~CWeaponAxeObj();
};

// --------------------------------------------------------------------
//����@���v���V�[�W���N���X
// --------------------------------------------------------------------
class CWeaponAxeProc : public CBaseProc
{
protected:
	;
public:
	BOOL Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner);	// ���̔���

	CWeaponAxeProc(CGameMain* pGMain);	// �R���X�g���N�^
	virtual	~CWeaponAxeProc();
};

// --------------------------------------------------------------------
//����@�������@�I�u�W�F�N�g�N���X
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


	CWeaponAxeShotObj(CBaseProc* pProc);	// �R���X�g���N�^      
	virtual	~CWeaponAxeShotObj();
};

// --------------------------------------------------------------------
//����@�������v���V�[�W���N���X
// --------------------------------------------------------------------
class CWeaponAxeShotProc : public CBaseProc
{
protected:
public:
	BOOL Start(MATRIX4X4 mGun, DWORD dwLength, DWORD dwOwner);  //����@���̔���

	CWeaponAxeShotProc(CGameMain* pGMain);	// �R���X�g���N�^
	virtual	~CWeaponAxeShotProc();
};

// --------------------------------------------------------------------
//����@�_���[�W�G���A�@�I�u�W�F�N�g�N���X
// --------------------------------------------------------------------
class CWeaponDamageAreaObj : public CBaseObj
{
protected:

public:
	BOOL Start(VECTOR3 vStart, VECTOR3 vTarget, DWORD dwOwner) override;
	void Update() override;


	CWeaponDamageAreaObj(CBaseProc* pProc);	// �R���X�g���N�^      
	virtual	~CWeaponDamageAreaObj();
};

// --------------------------------------------------------------------
//����@�_���[�W�G���A �v���V�[�W���N���X
// --------------------------------------------------------------------
class CWeaponDamageAreaProc : public CBaseProc
{
protected:
	DWORD		m_dwDamageTime;
	DWORD       m_dwKind;
	float		m_fAlfa;
	BOOL		m_bIsInObject;

public:
	BOOL Start(VECTOR3 min, VECTOR3 max, DWORD Kind);  //����@���̔���
	void Update() override;

	// �A�N�Z�X�֐�
	void SetIsInObject(BOOL flag);

	CWeaponDamageAreaProc(CGameMain* pGMain);	// �R���X�g���N�^
	virtual	~CWeaponDamageAreaProc();
};

// --------------------------------------------------------------------
//����@�U���{�b�N�X�@�I�u�W�F�N�g�N���X
// --------------------------------------------------------------------
class CWeaponAttackBoxObj : public CBaseObj
{
protected:

public:
	BOOL Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner) override;
	void Render();


	CWeaponAttackBoxObj(CBaseProc* pProc);	// �R���X�g���N�^      
	virtual	~CWeaponAttackBoxObj();
};

// --------------------------------------------------------------------
//����@�U���{�b�N�X �v���V�[�W���N���X
// --------------------------------------------------------------------
class CWeaponAttackBoxProc : public CBaseProc
{
protected:

public:
	BOOL Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner);  //����@���̔���

	CWeaponAttackBoxProc(CGameMain* pGMain);	// �R���X�g���N�^
	virtual	~CWeaponAttackBoxProc();
};


// --------------------------------------------------------------------
//����@��Ɏ��A�C�e���@�I�u�W�F�N�g�N���X
// --------------------------------------------------------------------
class CWeaponHoldObj : public CBaseObj
{
protected:
	int                      m_nKindNo;			// ��Ɏ��A�C�e���̎��(1:���@2:�e)
	CFbxMesh*                m_pMesh;			// Fbx���b�V��(�v���V�[�W���ł͂Ȃ��I�u�W�F�N�g�Ƀ��b�V������������)
	VECTOR3                  m_vOffset;			// �e���܂ł̃I�t�Z�b�g�i�e�̂݁j
public:
	void Init();
	BOOL Disp(MATRIX4X4 mHold, BASEHOLDITEM HoldItem);
	void GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vOffset);
	void GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vMin, VECTOR3& vMax);
	CBBox* GetBBox(MATRIX4X4 mHold, BASEHOLDITEM HoldItem);

	void Update() override;
	void Render();

	int GetKind() { return m_nKindNo; }

	CWeaponHoldObj(CBaseProc* pProc);	// �R���X�g���N�^        // -- 2022.2.16
	virtual	~CWeaponHoldObj();
};
// --------------------------------------------------------------------
//����@��Ɏ��A�C�e���@�v���V�[�W���N���X
// --------------------------------------------------------------------
class CWeaponHoldProc : public CBaseProc
{
protected:
	;
public:
	BOOL Disp(MATRIX4X4 mHold, BASEHOLDITEM HoldItem); // ����@�̕\��
	void GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vOffset);
	void GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vMin, VECTOR3& vMax);
	CBBox* GetBBox(MATRIX4X4 mHold, BASEHOLDITEM HoldItem);
	int GetKind(BASEHOLDITEM HoldItem) { return ((CWeaponHoldObj*)m_pObjArray[HoldItem.m_nHoldObjNo])->GetKind(); }
	CWeaponHoldProc(CGameMain* pGMain);	// �R���X�g���N�^
	virtual	~CWeaponHoldProc();
};

// ======================================================================
// ����S�̂̃��C���v���V�[�W���N���X
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
	CWeaponProc(CGameMain* pGMain);	// �R���X�g���N�^
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
