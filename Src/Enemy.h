//=============================================================================
//		�G�L�����N�^�[�@�w�b�_�t�@�C��							Enemy.h
//=============================================================================
#pragma once
#include "GameMain.h"
#include "Map.h"

#define  ENM_RW_ID           4
#define  ENM_RW_MAX          100

class CEnmProc;
//======================================================================
// �G�@�I�u�W�F�N�g�N���X
//======================================================================
class CEnmObj : public CBaseObj
{
protected:
	VECTOR3				m_vTarget;			// �^�[�Q�b�g
	NAVIGATIONMAP		m_Nav;				// �i�r�Q�[�V�����}�b�v
	CSearchRoute*		m_pSearchRoute;		// �o�H�T��
	BOOL				m_bIsMove;
	BASEHOLDITEM		m_HoldItem;			// ��Ɏ��A�C�e�� 

public:
	BOOL Start() override;
	void Update() override;
	void UpdateNormal();
	void UpdateAttack();
	void UpdateDamage();
	void UpdateDead();

	void SetHoldItem(DWORD weaponId);

	// �A�N�Z�X�֐�
	void SetNav(NAVIGATIONMAP  Nav);

	CEnmObj(CBaseProc* pProc);	// �R���X�g���N�^
	virtual	~CEnmObj();	// �f�X�g���N�^
};

//======================================================================
// �G�@�v���V�[�W���N���X
//======================================================================
class CEnmProc : public CBaseProc
{
protected:
	BOOL				m_bIsStartFlag;	// �X�|�[���ς݂�
public:
	BOOL Start();
	BOOL Start(VECTOR3 vMin, VECTOR3 vMax, int nNum);
	void Update() override;
	void UpdateHitcheck();
	void ResetEnmNum();
	int  CountActiveEnm();
	void ResetStartFlag();
	void SetMaxNum() override;
	CEnmProc(CGameMain* pGMain);	// �R���X�g���N�^
	virtual	~CEnmProc();	// �f�X�g���N�^

};

//======================================================================
// �G�@�萔�N���X
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