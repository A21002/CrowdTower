//=============================================================================
//		�����L�����N�^�[�@�w�b�_�t�@�C��							     Ally.h
//=============================================================================

#pragma once
#include "GameMain.h"

#define  ALLY_MAX 100

//======================================================================
// �����L�����N�^�[�@�I�u�W�F�N�g�N���X
//======================================================================
class CAllyObj : public CBaseObj
{
protected:
	VECTOR3				m_vTarget;	// �^�[�Q�b�g
	CSearchRoute*		m_pSearchRoute;		// �o�H�T��
	int					m_nOldAttackKind;
	BOOL				m_IsCanMove;

public:
	BOOL Start(VECTOR3 vPos, BOOL flag) override;
	void Update() override;
	void UpdateNormal();
	void UpdateAttack();
	void UpdateMoveSearch(float fForward);
	void UpdateMoveBoid(float fForward);

	CAllyObj(CBaseProc* pProc);	// �R���X�g���N�^
	virtual	~CAllyObj();
};

//======================================================================
// �����L�����N�^�[�@�v���V�[�W���N���X
//======================================================================
class CAllyProc : public CBaseProc
{
protected:
	float m_fAveTran;
	float m_fAveRot;
	float m_fOldAveTran;
	float m_fOldAveRot;
	
public:
	
	BOOL Start(VECTOR3 vPos, BOOL flag);
	void Update() override;
	void DeleteObj(int num);
	void SetHoldItem(DWORD weaponId) override;

	// �A�N�Z�X�֐�
	float GetAveTran();
	float GetAveRot();
	void AddAveTran(float tran);
	void AddAveRot(float rot);

	// �R���X�g���N�^
	CAllyProc(CGameMain* pGMain);
	virtual	~CAllyProc();
};

//======================================================================
// �����L�����N�^�[�@�萔�N���X
//======================================================================
class AllyConstruct
{
public:
	static const float	START_INVIN_TIME;

	static const VECTOR3 HITBOX_MIN;
	static const VECTOR3 HITBOX_MAX;

	static const float	SPEED;

	static const int	ANIM_IDLE = 0;
	static const int	ANIM_WALK = 1;
	static const int	ANIM_SWORD = 2;
	static const int	ANIM_ARROW = 3;
	static const int	ANIM_AXE = 4;
	static const int	ANIM_GUN = 5;
	static const int	ANIM_HIT = 6;
};