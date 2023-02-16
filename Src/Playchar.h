//=============================================================================
//		�v���C�L�����N�^�[�@�w�b�_�t�@�C��							Playchar.h
//=============================================================================
#pragma once
#include "GameMain.h"
#include "Weapon.h"
#include "Ability.h"

//======================================================================
// �v���C�L�����N�^�[�@�I�u�W�F�N�g�N���X
//======================================================================
class CPcObj : public CBaseObj
{
protected:
	int					m_nAllyNum;				// �����̐�
	int					m_nAttackKind;			// �s�����U���̎��
	int					m_nGunMagazine;			// �}�K�W���i��
	int					m_nCoolTime;			// �ߋ�������̃N�[���^�C��
	int					m_nShotSlashCoolTime;	// �a���N�[���^�C��
	float				m_fSpeed;				// �ړ����x      
	float				m_fRotSpeed;			// ��]���x
	float				m_fLocalRotY;			// ���[�J����(Y��) 
	float				m_fAngle;				// �ڕW��]�p�x

	BOOL				m_bIsKeyPush;			// �L�[����������
	BOOL				m_bIsCanMove;			// �A�j���[�V�����ɂ���Ĉړ��\��
	BOOL				m_bIsHaveAxe;			// ���̉����t���O
	BOOL				m_bIsToLateKeyPush;		// 2�ȏ�̃L�[��������Ă��邩

	// �Q�[���I�[�o�[���Ɏg�p
	int					m_nGameOverPosY;		// �w�i�̃X�v���C�g�ʒu

public:
	// GAMEMAIN����
	void Update() override;
	void UpdateNormal();								// �ʏ펞�̍X�V����
	void UpdateNormalMove();							// �ړ��Ɋւ���X�V����
	MATRIX4X4 UpdateNormalMoveKeystate(DWORD DIKey);	// ���͂��Ƃ̈ړ��Ɋւ���X�V
	void UpdateMoveRot();								// �ڕW��]�p�Ɍ������ĉ�]���鏈��
	void UpdateNormalAttack();							// �U���Ɋւ���X�V����
	void UpdateDamage();								// �_���[�W����
	void UpdateDead();									// ���S������
	
	void UpdateGameOver();								// �Q�[���I�[�o�[����
	void UpdateGameClear();								// �Q�[���N���A����

	void SetAllyObj();									// Map�J�ڎ��̖����I�u�W�F�N�g�̍Đ�������
	void SetRetryAllyNum();								// ���g���C���A���������I�u�W�F�N�g���̌v�Z
	void GetCursorRotate();								// �J�[�\���̈ʒu�Əd�Ȃ��Ă��郏�[���h���W������

		// ImGui�\������
	void ShowImgui(bool* p_open);

	// �A�N�Z�X�֐�
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

	CPcObj(CBaseProc* pProc);	// �R���X�g���N�^ 
	virtual	~CPcObj();
};

//======================================================================
// �v���C�L�����N�^�[�@�v���V�[�W���N���X
//======================================================================
class CPcProc : public CBaseProc
{
protected:
	CAbility* m_pAbility;
public:
	// �A�N�Z�X�֐�
	CAbility*	GetAbility();
	CPcObj*		GetPcObjPtr();

	// �R���X�g���N�^
	CPcProc(CGameMain* pGMain);
	virtual	~CPcProc();
};

//======================================================================
// �v���C�L�����N�^�[�@�萔�N���X
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