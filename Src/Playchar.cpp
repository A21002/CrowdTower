//=============================================================================
//		�v���C�L�����N�^�[�̐���
//																Playchar.cpp
//=============================================================================
#include  "Playchar.h"
#include  "Camera.h"
#include  "SearchRoute.h"
#include  "BackFore.h"
#include  "Weapon.h"
#include  "Map.h"
#include  "Effect.h"
#include  "Enemy.h"
#include  "EnemyBoss.h"
#include  "Ally.h"
#include  "Title.h"

//============================================================================
// PC�萔�N���X
//============================================================================
// ������
const float PcConstruct::SPEED = 0.08f;
const float PcConstruct::ROT_SPEED = 8.0f;
const float PcConstruct::WAITTIME_GUN = 0.2f;
const VECTOR3 PcConstruct::HITBOX_MIN = VECTOR3(-0.3f, -0.05f, -0.3f);
const VECTOR3 PcConstruct::HITBOX_MAX = VECTOR3(0.3f, 2.0f, 0.3f);

//============================================================================
// PC�v���V�[�W���N���X
//============================================================================
// ---------------------------------------------------------------------------
// �o�b�v���V�[�W���̃R���X�g���N�^
//
//   �Ȃ��A�I�u�W�F�N�g��delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
// ---------------------------------------------------------------------------
CPcProc::CPcProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// �L�����N�^�[�@�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Char/PC/PC_Skin.mesh"));    // �L�����N�^�[
	m_pMesh->LoadAnimation(_T("Data/Char/PC/Idle.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/PC/Run3.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/PC/Sword.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/PC/Arrow.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/PC/Axe.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/PC/Gun.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/PC/Dead.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/PC/Fall.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/PC/Victory.anmx"));
	m_pMesh->m_fHeightMax = 0.01f;
	m_pMesh->m_iMaxDevide = 16;
	m_HoldItem.m_nHoldObjNo = 0;	// ��Ɏ�����̃I�u�W�F�ԍ�
	m_HoldItem.m_nPosMesh = 0;		// �����ʒu�̃��b�V���ԍ�
	m_HoldItem.m_nPosBone = 0;	// �����ʒu�̃{�[���ԍ�
	m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.0f, 0.0f);  // ��Ɏ�����̈ʒu�I�t�Z�b�g
	m_HoldItem.m_nOffsetRot = VECTOR3(0.0f, 0.0f, 0.0f);  // ��Ɏ�����̊p�x�I�t�Z�b�g
	
	// -------------------------------------------------------------------------
	m_pObjArray.push_back(new CPcObj(this));	// m_pObjArray�ɃI�u�W�F�N�g��o�^����

	// ------------------------------------------------------------------------- 
	m_pAbility = new CAbility(pGMain);
}

// ---------------------------------------------------------------------------
// �o�b�v���V�[�W���̃f�X�g���N�^
// ---------------------------------------------------------------------------
CPcProc::~CPcProc()
{
	delete(m_pAbility);
}

//============================================================================
// �A�N�Z�X�֐�
//============================================================================
CAbility* CPcProc::GetAbility()
{
	return m_pAbility;
}

CPcObj* CPcProc::GetPcObjPtr()
{
	return (CPcObj*)GetObjArrayPtr()[0];
}


//============================================================================
// PC�I�u�W�F�N�g�N���X
//============================================================================
// ---------------------------------------------------------------------------
// �o�b�I�u�W�F�N�g�̃R���X�g���N�^
//
//  ����
//  CBaseProc* pProc		�F �e�v���V�[�W���ւ̃|�C���^
// ---------------------------------------------------------------------------
CPcObj::CPcObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	m_fLocalRotY = 0.0f;               

	m_fSpeed = PcConstruct::SPEED;						// �ړ����x      
	m_fRotSpeed = PcConstruct::ROT_SPEED;				// ��]���x      
	m_nAllyNum = PcConstruct::START_ALLY_NUM;			// �����̖����̐�

	// �o�E���f�B���O�{�b�N�X�̐ݒ�
	m_pBBox = new CBBox(m_pGMain->m_pShader, PcConstruct::HITBOX_MIN, PcConstruct::HITBOX_MAX);
	m_pBBox->m_mWorld = m_mWorld;                          

	m_nMaxHp = m_nAllyNum + 1;
	m_nAtc = 0;
	m_nTag = PC;

	// �o�b�I�u�W�F�N�g�̏���������
	m_bActive = TRUE;
	ResetStatus();

	// �A�j���[�V�����̏�����
	m_AnimStatus.playAnim = true;
	m_AnimStatus.SetNum(PcConstruct::ANIM_IDLE);
	m_AnimStatus.animFrame = 0;

	m_nAttackKind = ATTACK_NONE;
	m_bIsCanMove = TRUE;
	m_bIsHaveAxe = TRUE;
	m_bIsToLateKeyPush = FALSE;

	m_nHp = m_nAllyNum + 1;
	m_nGunMagazine = PcConstruct::BASE_MAGAZINE_NUM;
	m_nCnt2 = 0;
	m_nGameOverPosY = 0;
	m_nCoolTime = 0;
	m_nShotSlashCoolTime = 0;
	m_fAngle = 0.0f;
}

// ---------------------------------------------------------------------------
// �o�b�I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CPcObj::~CPcObj() 
{
}

// ---------------------------------------------------------------------------
// Map�J�ڌ�A�J�ڑO��AllyOBj�̐������Đ���
// ---------------------------------------------------------------------------
void CPcObj::SetAllyObj()
{
	for (int i = 0; i < m_nAllyNum; i++) {
		m_pGMain->m_pAllyProc->Start(GetPositionVector(m_mWorld), TRUE);
	}
}

// ---------------------------------------------------------------------------
// ���g���C��A�O��̍ŏI�K�w�����Q�Ƃ���AllyNum��ݒ�
// ---------------------------------------------------------------------------
void CPcObj::SetRetryAllyNum()
{
	int hierarchy = m_pGMain->m_pMapProc->GetHierarchy();
	m_nAllyNum = PcConstruct::START_ALLY_NUM + hierarchy / 4;
	m_pGMain->m_pBackForeProc->AddAllAllyNum(m_nAllyNum);
}

//-----------------------------------------------------------------------------
// �o�b�I�u�W�F�N�g�̍X�V
//
//�@�L�[�{�[�h�ło�b�𓮂���
//�@�o�b�̃A�j���[�V�����A�W�����v�A���R����
//�@���[���h���W�ԊҁA�}�b�v�Ƃ̐ڐG����
//�@�o�b�̍U���������s��
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CPcObj::Update()
{
	// ��O�̈ʒu��ۑ�
	m_mWorldOld = m_mWorld;

	// �U����ނ�������
	m_nAttackKind = ATTACK_NONE;

	// AllyNum��HP�𓯊�����
	m_nHp = m_nAllyNum + 1;
	// HP�Ȃ��A���S�A�j���[�V�����Ɉڍs���Ă��邩
	if (m_nHp <= 0 && m_AnimStatus.animNum != PcConstruct::ANIM_DEAD) {
		m_dwStatus = DEAD;
		m_AnimStatus.SetNum(PcConstruct::ANIM_DEAD);
		m_nCnt1 = 60 * PcConstruct::WAITTIME_DEAD;
	}

	// ���C�����ړ��������Ă����Ԃ̂�
	if (m_pGMain->m_isMove) {
		// �e��Ԃɉ���������
		switch (m_dwStatus) {
		case  FLASH:  // �o�b���_���[�W���畜�A�����Ƃ��̏���
		case  NORMAL: // �o�b���ʏ��Ԃ̂Ƃ��̏���
			UpdateNormal();  // �o�b�I�u�W�F�N�g�̍X�V �ʏ��ԁiNORMAL�j
			break;

		case  DAMAGE: // �o�b���_���[�W��Ԃ̂Ƃ��̏���
			UpdateDamage();  // �o�b�I�u�W�F�N�g�̍X�V �_���[�W��ԁiDAMAGE�j
			break;

		case  DEAD:	// �o�b�����S��Ԃ̂Ƃ��̏���
			UpdateDead();  // �o�b�I�u�W�F�N�g�̍X�V ���S��ԁiDEAD�j
			break;

		}
		// �{�X�Ƃ̏d�Ȃ菈��
		SetKeepOffset(m_pGMain->m_pEnmBossProc->GetEnmBossObjPtr());

		// �}�b�v�R���W�����Ǝ��R����
		m_fJumpY += (0.5f * GRAVITY * 0.00005f) * m_fJumpTime;	// ���R����
		m_fJumpTime += 1.0f;
		if (m_pGMain->m_pMapProc->isCollisionMoveGravityPc(&m_mWorld, m_mWorldOld) != 3)  // �}�b�v�R���W����
		{
			m_fJumpY = 0.0f;  // ���R�����̒�~
			m_fJumpTime = 0.0f;
		}

		// �I�u�W�F�N�g����ԕ����֓o�^ 
		// SetObj()���A�I�u�W�F�N�g����ԕ����֓o�^����֐��ł���
		VECTOR3 vMin, vMax;
		m_pGMain->m_pMapProc->GetPcSpPart()->MakeAABB(
			GetPositionVector(m_mWorldOld), GetPositionVector(m_mWorld), 2.0f, vMin, vMax);
		m_pGMain->m_pMapProc->GetPcSpPart()->SetObj(vMin, vMax, this);
	}
	else {
		// �ړ��s���́AIdle���[�V�����őҋ@
		m_AnimStatus.SetNum(PcConstruct::ANIM_IDLE);
		m_bIsKeyPush = FALSE;
	}
	
	// �J������PC�̈ʒu�Ń��C���΂��A�d�Ȃ����}�b�v���b�V���𓧉߂���
	m_pGMain->m_pMapProc->CheckCrossStageMesh(GetPositionVector(m_mWorld), GetPositionVector(m_pGMain->m_pCameraProc->GetCameraObjPtr()->GetWorld()));

	//�����_�����O
	GetMesh()->Render(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
	// �o�E���f�B���O�{�b�N�X
	m_pBBox->m_mWorld = m_mWorld;

	// ��Ɏ������A�C�e���̕\��������
	if (GetHoldItem().m_nHoldObjNo != 0) // ��Ɏ������A�C�e��������Ƃ�
	{
		MATRIX4X4 mHold;
		mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, GetHoldItem().m_nPosBone, GetHoldItem().m_nPosMesh);  // ��̈ʒu�̃{�[���}�g���b�N�X
		m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->Disp(mHold, GetHoldItem()); // �A�C�e���\��
	}
}

//-----------------------------------------------------------------------------
// �o�b�I�u�W�F�N�g�̍X�V �ʏ��ԁiNORMAL�j
//
//�@�L�[�{�[�h�ło�b�𓮂���
//�@�o�b�̃A�j���[�V�����A�W�����v�A���R����
//�@���[���h���W�ԊҁA�}�b�v�Ƃ̐ڐG����
//�@�o�b�̍U���������s��
//
//
//   �����@�Ȃ�
//-----------------------------------------------------------------------------
void	CPcObj::UpdateNormal()
{
	if (m_dwStatus == FLASH) // �o�b���_���[�W���畜�A�����Ƃ��̏���
	{
		m_nCnt1--;
		if (m_nCnt1 <= 0) m_dwStatus = NORMAL;
	}
	UpdateNormalMove();      // �o�b�I�u�W�F�N�g�̈ړ�����
	UpdateNormalAttack();    // �o�b�I�u�W�F�N�g�̍U������
}

//-----------------------------------------------------------------------------
// �o�b�I�u�W�F�N�g�̈ړ�����
//
//�@�L�[�{�[�h�^�}�E�X�ło�b�𓮂���
//�@�o�b�̃A�j���[�V�����A�W�����v�A���R����
//�@���[���h���W�ԊҁA�}�b�v�Ƃ̐ڐG����
//
//
//   �����@�Ȃ�
//-----------------------------------------------------------------------------
void	CPcObj::UpdateNormalMove()
{
	CDirectInput* pDI = m_pGMain->m_pDI;

	MATRIX4X4 mYaw;  // �o�b�x����]�}�g���b�N�X
	MATRIX4X4 mPos;  // �o�b�ړ��}�g���b�N�X
	m_bIsKeyPush = FALSE;// �L�[�����������ǂ����̃`�F�b�N
	m_bIsToLateKeyPush = FALSE;		// 2�ȏ�̃L�[��������Ă��邩

	// ���R�����̏����l
	mPos = XMMatrixTranslation(0.0f, m_fJumpY, 0.0f);

	// ImGui�̕\��
	static bool bImgOpen = false;
	if (pDI->CheckKey(KD_TRG, DIK_J))
		bImgOpen = true;
	if (bImgOpen) ShowImgui(&bImgOpen);

	// �o�b�̈ړ�����  ---------------------------------------------
	// �O�i����
	if ((pDI->CheckKey(KD_DAT, DIK_W) || pDI->CheckJoy(KD_DAT, DIJ_UP)) && m_bIsCanMove) //	���L�[
	{
		m_bIsKeyPush = TRUE;
		mPos = UpdateNormalMoveKeystate(DIK_W);
	}
	// ��ޏ���
	if ((pDI->CheckKey(KD_DAT, DIK_S) || pDI->CheckJoy(KD_DAT, DIJ_DOWN)) && m_bIsCanMove) //���L�[
	{
		m_bIsKeyPush = TRUE;
		mPos = UpdateNormalMoveKeystate(DIK_S);
	}
	// �E�ړ�����
	if ((pDI->CheckKey(KD_DAT, DIK_D) || pDI->CheckJoy(KD_DAT, DIJ_RIGHT)) && m_bIsCanMove)//���L�[
	{
		m_bIsKeyPush = TRUE;
		mPos = UpdateNormalMoveKeystate(DIK_D);
	}
	// ���ړ�����
	if ((pDI->CheckKey(KD_DAT, DIK_A) || pDI->CheckJoy(KD_DAT, DIJ_LEFT)) && m_bIsCanMove)//���L�[
	{
		m_bIsKeyPush = TRUE;
		mPos = UpdateNormalMoveKeystate(DIK_A);
	}

	// �ڕW�p�։�]
	UpdateMoveRot();

	if (!m_bIsKeyPush && m_AnimStatus.animNum < 2)  // �L�[�������Ă��Ȃ��Ƃ� && �U�����[�V�������ł͂Ȃ�
	{
		m_AnimStatus.SetNum(0);
	}

	// ���[�J��������Ƃ��āA�o�b���ړ������郏�[���h�}�g���b�N�X��ݒ�
	// // �U�����[�V�������ړ����s��Ȃ�
	// y����]�݂̂�K��
	if (m_bIsCanMove) {
		mYaw = XMMatrixRotationY(XMConvertToRadians(GetRotateVector(m_pGMain->m_mView).y - 90.0f));
		mYaw = XMMatrixRotationY(XMConvertToRadians(m_vRotUp.y)) * mYaw;
		m_mWorld = mPos * mYaw * GetLocalMatrix();
	}
}

//----------------------------------------------------------------------------- 
// �o�b�I�u�W�F�N�g�̈ړ������@�L�[���얈�̊e��ړ�����
//
//   �����@DWORD Key �L�[��ʁ@DirectInput�̃L�[�R�[�h
//
//   �߂�l  �ړ��}�g���b�N�X
//-----------------------------------------------------------------------------
MATRIX4X4 CPcObj::UpdateNormalMoveKeystate(DWORD DIKey)
{
	MATRIX4X4 mPos;

	if (m_AnimStatus.animNum == 0) m_AnimStatus.SetNum(1);

	// �i�s�����ɉ�]���Ă���O�i
		// ���[�g�{�[���A�j���[�V�������s�����ǂ������[�g�A�j���[�V�����^�C�v���m�F����
	if (GetMesh()->GetRootAnimType(m_AnimStatus.animNum) == eRootAnimNone)
	{
		// ���[�g�{�[���A�j���[�V�������s�킸�Œ�̑O�i�ړ��l
		mPos = XMMatrixTranslation(0.0f, m_fJumpY, m_fSpeed);
	}
	else {
		// ���[�g�{�[���A�j���[�V�����ł̑O�i�ړ��l
		mPos = XMMatrixTranslation(0.0f, m_fJumpY, 0.0f) * GetMesh()->GetRootAnimUpMatrices(m_AnimStatus);
	}

	// �i�s�����ɉ�]���鏈��
	// �E��]�p�x��m_vRotUp.y�ɂ���B-180�`180�x
	// �E�P��̉�]���x��m_fRotSpeed
	if (m_vRotUp.y > 180.0f)  m_vRotUp.y -= 360.0f;
	if (m_vRotUp.y < -180.0f) m_vRotUp.y += 360.0f;

	if (!m_bIsToLateKeyPush)
	{
		switch (DIKey)
		{
		case DIK_W:    // �O�i
			m_fAngle = 0.0f;
			break;
		case DIK_S:    // ���
			if (m_vRotUp.y >= 0)  // �ŏ���]�ɂȂ�悤�ɕ��������킹��
			{
				m_fAngle = 180.0f;
			}
			else {
				m_fAngle = -180.0f;
			}
			break;
		case DIK_D:    // �E
			m_fAngle = 90.0f;
			if (m_vRotUp.y == -180.0f) m_vRotUp.y = 180.0f;  // �ŏ���]�ɂȂ�悤�ɕ��������킹��
			break;
		case DIK_A:    // ��
			m_fAngle = -90.0f;
			if (m_vRotUp.y == 180.0f) m_vRotUp.y = -180.0f;  // �ŏ���]�ɂȂ�悤�ɕ��������킹��
			break;
		}
		m_bIsToLateKeyPush = TRUE;
	}
	else {
		switch (DIKey)
		{
		case DIK_S:    // ���
			// �O�i�ƌ�ނ̓�������
			m_fAngle = 0.0f;
			mPos = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
			break;

		case DIK_D:    // �E
			if (m_fAngle == 0.0f) {
				// �O�i�ƉE
				m_fAngle += 45.0f;
			}
			else {
				// ��ނƉE
				m_fAngle -= 45.0f;
			}
			break;

		case DIK_A:    // ��
			if (m_fAngle == 0.0f) {
				// �O�i�ƍ�
				m_fAngle -= 45.0f;
			}
			else {
				// ��ނƉE
				m_fAngle += 45.0f;
			}
			break;
		}
	}
	return mPos;
}

//-----------------------------------------------------------------------------
// �ڕW��]�p�Ɍ������ĉ�]���鏈��
//-----------------------------------------------------------------------------
void CPcObj::UpdateMoveRot()
{
	if (m_vRotUp.y > m_fAngle)  // ����]
	{
		m_vRotUp.y -= m_fRotSpeed;
		if (m_vRotUp.y < m_fAngle) m_vRotUp.y = m_fAngle;
	}
	if (m_vRotUp.y < m_fAngle)  // �E��]
	{
		m_vRotUp.y += m_fRotSpeed;
		if (m_vRotUp.y > m_fAngle) m_vRotUp.y = m_fAngle;
	}
}

//-----------------------------------------------------------------------------
// �o�b�I�u�W�F�N�g�̍U������
//
//�@�L�[�{�[�h�^�}�E�X�ło�b�̍U���������s��
//
//
//   �����@�Ȃ�
//-----------------------------------------------------------------------------
void	CPcObj::UpdateNormalAttack()
{
	CDirectInput* pDI = m_pGMain->m_pDI;
	CBaseProc* pProc = m_pGMain->m_pPcProc;
	CAbility* ability = m_pGMain->m_pPcProc->GetAbility();

	// ------------------------------------------------------------- 
	// ���u����
	// ���݂̏������Ă��镐����Q��
	switch (ability->GetLongWeapon())
	{
	case ARROW:
		if (pDI->CheckJoy(KD_DAT, DIJ_A) || pDI->CheckMouse(KD_TRG, DIM_LBUTTON) && m_AnimStatus.animNum != PcConstruct::ANIM_ARROW && m_bIsCanMove) // X�L�[
		{
			m_nAttackKind = ATTACK_ALLOW;
			// �J�[�\���̈ʒu������
			GetCursorRotate();
			// �|������
			pProc->SetHoldItem(ATTACK_ALLOW);
			// �A�j���[�V�����ݒ�
			m_AnimStatus.SetNum(PcConstruct::ANIM_ARROW);
			m_AnimStatus.animSpeed = ability->GetAbilityLevel(0) * 0.3f + 1;
			// �A�j���[�V�������ړ��s��
			m_bIsCanMove = FALSE;
			m_pGMain->m_pArrow->Play();
		}
		break;

	case GUN:
		// �N���b�N�J�n���A�e�A�j���[�V������Ԃł̒�����������
		if ((pDI->CheckJoy(KD_DAT, DIJ_A) || pDI->CheckMouse(KD_DAT, DIM_LBUTTON) && m_AnimStatus.animNum == PcConstruct::ANIM_GUN) ||
			pDI->CheckMouse(KD_TRG, DIM_LBUTTON) && m_bIsCanMove)
		{
			if (m_nCnt2 <= 0) {
				// �����[�h���A�N�[���^�C���ł͂Ȃ�
				m_bIsCanMove = FALSE;
				if (m_nGunMagazine > 0) {
					// �}�K�W���c�e����
					if (m_AnimStatus.animNum != PcConstruct::ANIM_GUN)
					{
						// �A�j���[�V�����A�����Ă���A�C�e���̐ݒ�
						m_AnimStatus.SetNum(PcConstruct::ANIM_GUN);
						pProc->SetHoldItem(ATTACK_GUN);
					}
					m_nGunMagazine--;
					GetCursorRotate();
					m_pGMain->m_pGun->Play();
					m_nAttackKind = ATTACK_GUN;
					m_pGMain->m_pWeaponProc->m_pWeaponGunProc->Start(XMMatrixTranslation(0.0f, 0.7f, 0.5f) * m_mWorld, PC);
					if (m_nGunMagazine <= 0) {
						// �����[�h
						m_nCnt2 = 60 * PcConstruct::WAITTIME_RELOAD - 20 * ability->GetAbilityLevel(1);
					}
					else {
						// ���ˊԊu
						m_nCnt2 = 60 * PcConstruct::WAITTIME_GUN;
					}
				}
			}
		}
		else {
			if (m_AnimStatus.animNum == PcConstruct::ANIM_GUN) {
				// �e�������~�߂�����A�A�j���[�V�����ƕ��������
				m_AnimStatus.SetNum(PcConstruct::ANIM_IDLE);
				pProc->ResetHoldItem();
				m_AnimStatus.animSpeed = 1;
				m_bIsCanMove = TRUE;
			}
		}

		if (pDI->CheckJoy(KD_DAT, DIJ_A) || pDI->CheckKey(KD_TRG, DIK_R) && m_bIsCanMove) // R�L�[
		{
			// �����[�h
			m_nCnt2 = 60 * PcConstruct::WAITTIME_RELOAD - 20 * ability->GetAbilityLevel(1);
			m_nGunMagazine = 0;

		}

		// �����[�h����
		if (m_nGunMagazine <= 0 && m_nCnt2 <= 0) {
			m_nGunMagazine = PcConstruct::BASE_MAGAZINE_NUM + 5 * ability->GetAbilityLevel(7);
		}
		break;

	default:
		break;
	}

	if (m_AnimStatus.isEnd(PcConstruct::ANIM_ARROW))
	{
		// �|��A�j���[�V�����I����
		m_pGMain->m_pWeaponProc->m_pWeaponArrowProc->Start(m_mWorld, PC); // �|���
		m_AnimStatus.SetNum(0);
		pProc->ResetHoldItem();
		m_AnimStatus.animSpeed = 1;
		m_bIsCanMove = TRUE;
	}

	// ------------------------------------------------------------- 
	// �ߐڍU��
	switch (ability->GetShortWeapon()) {
	case SWORD:
		if (pDI->CheckJoy(KD_TRG, DIJ_C) || pDI->CheckMouse(KD_TRG, DIM_RBUTTON) && m_AnimStatus.animNum != PcConstruct::ANIM_SWORD && m_nCoolTime <= 0 && m_bIsCanMove) // Z�L�[
		{
			m_nAttackKind = ATTACK_SWORD;
			// �J�[�\���̕���������
			GetCursorRotate();
			// ��������
			pProc->SetHoldItem(ATTACK_SWORD);
			if (ability->GetAbilityLevel(4) > 0) {
				m_nAttackKind = ATTACK_SHOTSWORD;
				m_pGMain->m_pWeaponProc->m_pWeaponShotSlashProc->Start(m_mWorld, PC);	// �a��
			}
			m_pGMain->m_pEffectProc->m_pBillSlashProc->Start(m_mWorld);	// ���̃G�t�F�N�g
			// �A�j���[�V�����ݒ�
			m_AnimStatus.SetNum(PcConstruct::ANIM_SWORD);
			m_AnimStatus.animSpeed = ability->GetAbilityLevel(3) * 0.3f + 1;
			// �U�����[�V�������ړ��s��
			m_bIsCanMove = FALSE;
			m_pGMain->m_pSlash->Play();
			m_nCoolTime = 60 * PcConstruct::WAITTIME_MELEE_WEAPON * (1 - 0.3 * ability->GetAbilityLevel(3));
		}
		break;

	case AXE:
		if ((pDI->CheckJoy(KD_TRG, DIJ_C) || pDI->CheckMouse(KD_TRG, DIM_RBUTTON)) && m_AnimStatus.animNum != PcConstruct::ANIM_AXE && m_nCoolTime <= 0 && m_bIsCanMove) // Z�L�[
		{
			m_nAttackKind = ATTACK_AXE;
			// �J�[�\���̕���������
			GetCursorRotate();
			if (ability->GetAbilityLevel(4) > 0) {
				// ����
				if (m_bIsHaveAxe)
				{
					// �܂������Ă��Ȃ�
					m_pGMain->m_pWeaponProc->m_pWeaponAxeShotProc->Start(m_mWorld* XMMatrixTranslation(0.0f, 1.0f, 1.0f), 5 + 2 * ability->GetAbilityLevel(4), PC);
					m_bIsHaveAxe = FALSE;
					m_nAttackKind = ATTACK_AXESHOT;
				}
			}
			else {
				// �ʏ�
				m_pGMain->m_pEffectProc->m_pBillSlashProc->Start(m_mWorld);	// �G�t�F�N�g
				pProc->SetHoldItem(ATTACK_AXE);
				m_nCoolTime = 60 * PcConstruct::WAITTIME_MELEE_WEAPON * (1 - ability->GetAbilityLevel(3) * 0.3);
			}
			// �A�j���[�V�����ݒ�
			m_AnimStatus.SetNum(PcConstruct::ANIM_AXE);
			m_AnimStatus.animSpeed = ability->GetAbilityLevel(3) * 0.3f + 1;
			// �U�����[�V�������ړ��s��
			m_bIsCanMove = FALSE;
			m_pGMain->m_pSlash->Play();
		}
		break;

	default:
		break;
	}

	// �ߐڍU���̔���
	if (m_AnimStatus.animNum == PcConstruct::ANIM_SWORD || m_AnimStatus.animNum == PcConstruct::ANIM_AXE)
	{
		MATRIX4X4 mSword, mHold;
		VECTOR3 vMin, vMax;
		if (GetHoldItem().m_nHoldObjNo != 0)
		{
			mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, GetHoldItem().m_nPosBone, GetHoldItem().m_nPosMesh);  // ��̈ʒu�̃{�[���}�g���b�N�X
			m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->GetInfo(mHold, GetHoldItem(), mSword, vMin, vMax); // �{�[���}�g���b�N�X���w�肵�Ď�Ɏ��A�C�e�����𓾂�
			if (m_AnimStatus.animNum == PcConstruct::ANIM_AXE && ability->GetAbilityLevel(10) == 0) {
				// �������������̕��̍U������
				m_pGMain->m_pWeaponProc->m_pWeaponAxeProc->Start(mSword, vMin, vMax, PC);
			}
			if (m_AnimStatus.animNum == PcConstruct::ANIM_SWORD) {
				// ���U���̔���
				m_pGMain->m_pWeaponProc->m_pWeaponSwordProc->Start(mSword, vMin, vMax, PC);  
			}
		}

		if (m_AnimStatus.isEnd(PcConstruct::ANIM_SWORD))
		{
			// ���U���̏I������
			m_AnimStatus.SetNum(PcConstruct::ANIM_IDLE);
			m_AnimStatus.animSpeed = 1;
			pProc->ResetHoldItem();
			m_bIsCanMove = TRUE;
		}

		if (m_AnimStatus.isEnd(PcConstruct::ANIM_AXE))
		{
			// ���U���̏I������
			m_AnimStatus.SetNum(PcConstruct::ANIM_IDLE);
			pProc->ResetHoldItem();
			m_AnimStatus.animSpeed = 1;
			m_bIsCanMove = TRUE;
		}
	}

	// �J�E���g�_�E��
	if (m_nCnt2 > 0) {
		m_nCnt2--;
	}

	if (m_nCoolTime > 0) {
		m_nCoolTime--;
	}
}

//-----------------------------------------------------------------------------
// �o�b�I�u�W�F�N�g�̍X�V �_���[�W��ԁiDAMAGE�j
//
//�@�_���[�W���󂯂��Ƃ��̏���
//
//
//   �����@�Ȃ�
//-----------------------------------------------------------------------------
void	CPcObj::UpdateDamage()
{
	MATRIX4X4 mTemp;

	mTemp = XMMatrixTranslation(0, 0, -0.25f);	// �o�b�N����
	m_mWorld = mTemp * m_mWorld;

	// �U�����󂯂��_���[�W
	if (m_pHitObj->GetAtc() > 0) {
		// Ally�I�u�W�F�N�g���폜
		m_pGMain->m_pAllyProc->DeleteObj(1);
	}
	if (m_nHp <= 0)		// HP���O�Ȃ̂Ŏ��S��
	{
		m_nHp = 0;
		m_dwStatus = DEAD;		// HP���O�Ȃ̂Ŏ��S��
		m_AnimStatus.SetNum(PcConstruct::ANIM_DEAD);	// ���S���[�V�����ɂ���
		m_nCnt1 = PcConstruct::WAITTIME_DEAD;			// ���S���Ԃ̐ݒ�
	}
	else {
		m_nCnt1 = PcConstruct::WAITTIME_FLASH;   // ���G��Ԃ̎���
		m_dwStatus = FLASH;     // �_���[�W����̕��A�������s��
	}
}
//----------------------------------------------------------------------------- 
// �o�b�I�u�W�F�N�g�̍X�V ���S��ԁiDEAD�j
//
//�@���S��Ԃ̂Ƃ��̏���
//
//
//   �����@�Ȃ�
//-----------------------------------------------------------------------------
void	CPcObj::UpdateDead()
{
	m_pGMain->m_pBackForeProc->GetSprite()->DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(255, 0, 0), 0.5f); // ��ʂ�Ԃ�����
	m_bIsCanMove = TRUE;
	m_pGMain->m_pPcProc->ResetHoldItem();
	m_pGMain->m_pAllyProc->ResetHoldItem();

	if (m_AnimStatus.isEnd(PcConstruct::ANIM_DEAD))  // ���S���[�V�������I������Ƃ�
	{
		m_AnimStatus.playAnim = false;	// �A�j���[�V�������~�߂�
		m_AnimStatus.animFrame = m_AnimStatus.endFrame - 1; // �t���[���G���h�̈��O�ɂ���
	}
	if (--m_nCnt1 <= 0) // ���S���Ԃ��I������Ƃ�
	{
		m_pGMain->m_pBackForeProc->SetBlackOut(TRUE);
		if (m_pGMain->m_pBackForeProc->GetWhiteOut()) {
			m_pGMain->m_pBackForeProc->AddDeadNum(1);
			m_pGMain->m_dwGameStatus = GAMEOVER;	// �Q�[���I�[�o�[��
			m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetCtrl(CAMERA_GAMEOVER);	// �J�����̃��[�h���Q�[���I�[�o�[��
			m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetCamBase();				// �J�����̈ʒu��ύX
			m_pGMain->m_pBgm1->Stop();
			m_pGMain->m_pBossBgm->Stop();
			//m_pGMain->m_pGameOver->Play(AUDIO_LOOP);
		}
	}
}

//----------------------------------------------------------------------------- 
// �Q�[���I�[�o�[���̏���
//
//   �����@�Ȃ�
//-----------------------------------------------------------------------------
void CPcObj::UpdateGameOver()
{
	m_mWorld = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	m_mWorld = XMMatrixRotationY(XMConvertToRadians(90.0f));

	// �����A�j���[�V�����ɂ���
	m_AnimStatus.SetNum(PcConstruct::ANIM_FALL);
	m_AnimStatus.playAnim = true;

	// VECTOR3�ɕϊ�
	VECTOR3 pos = GetPositionVector(m_mWorld);
	// �v���C���[�̈ʒu��������
	m_mWorld = XMMatrixTranslation(0.0f, -1.0f, 0.0f) * m_mWorld;
	m_mWorld = XMMatrixRotationX(XMConvertToRadians(20)) * m_mWorld;

	// z����2�i�߂�
	pos.z += 2;
	// Sprite��`��
	m_nGameOverPosY += 5;
	if (m_nGameOverPosY >= WINDOW_HEIGHT)
	{
		m_nGameOverPosY = 0;
	}
	m_pGMain->m_pBackForeProc->GetSprite()->Draw3D(m_pGMain->m_pTitleProc->GetImageOver(), pos, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vEyePt, VECTOR2(WINDOW_WIDTH / 60, WINDOW_HEIGHT / 60), VECTOR2(0, m_nGameOverPosY), VECTOR2(WINDOW_WIDTH, WINDOW_HEIGHT));

	// PC��`��
	GetMesh()->Render(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
}

//----------------------------------------------------------------------------- 
// �Q�[���N���A���̏���
//
//   �����@�Ȃ�
//-----------------------------------------------------------------------------
void CPcObj::UpdateGameClear()
{
	m_mWorld = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	m_mWorld = XMMatrixRotationY(XMConvertToRadians(180.0f));
	m_mWorld = XMMatrixRotationX(XMConvertToRadians(-45.0f)) * m_mWorld;

	// �����A�j���[�V�����ɂ���
	m_AnimStatus.SetNum(PcConstruct::ANIM_VICTORY);
	m_AnimStatus.playAnim = true;

	// VECTOR3�ɕϊ�
	VECTOR3 pos = GetPositionVector(m_mWorld);
	// z����2�i�߂�
	pos.z += 2;
	// �v���C���[�̈ʒu��������
	m_mWorld = XMMatrixTranslation(0.0f, -1.5f, 0.0f) * m_mWorld;
	m_mWorld = XMMatrixRotationX(XMConvertToRadians(30)) * m_mWorld;
	// Sprite��`��
	m_pGMain->m_pBackForeProc->GetSprite()->Draw3D(m_pGMain->m_pTitleProc->GetImageClear(), pos, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vEyePt, VECTOR2(WINDOW_WIDTH / 60, WINDOW_HEIGHT / 60), VECTOR2(0, 0), VECTOR2(WINDOW_WIDTH, WINDOW_HEIGHT));
	//m_mWorld = XMMatrixTranslation(0.0f, -1.0f, 0.0f) * m_mWorld;
	// PC��`��
	GetMesh()->Render(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
}

//----------------------------------------------------------------------------------------------------
//���݂̃L�[�̕����������p�x��Ԃ�
//----------------------------------------------------------------------------------------------------
void CPcObj::GetCursorRotate()
{
	// �J�[�\���̍��W
	VECTOR2 cursorPos;
	cursorPos.x= m_pGMain->m_pDI->GetMousePos().x;
	cursorPos.y = m_pGMain->m_pDI->GetMousePos().y;
	
	VECTOR3 nearpos, farpos, hitPos, normal;
	nearpos = XMVector3Unproject(VECTOR3(cursorPos.x, cursorPos.y, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, 1.0f,
		m_pGMain->m_mProj, m_pGMain->m_mView, XMMatrixIdentity());
	farpos = XMVector3Unproject(VECTOR3(cursorPos.x, cursorPos.y, 1.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, 1.0f,
		m_pGMain->m_mProj, m_pGMain->m_mView, XMMatrixIdentity());

	m_pGMain->m_pMapProc->Hitcheck(nearpos, farpos, &hitPos, &normal);
	float rot = GetTargetRotateVector(m_mWorld, hitPos).y;

	m_mWorld = XMMatrixRotationY(XMConvertToRadians(rot)) * m_mWorld;
}

//============================================================================
// �A�N�Z�X�֐�
//============================================================================
MATRIX4X4 CPcObj::GetLocalMatrix()
{
	MATRIX4X4 mPos = GetPositionMatrix(m_mWorld);
	MATRIX4X4 mRotY;

	mRotY = XMMatrixRotationY(XMConvertToRadians(m_fLocalRotY));
	mRotY = mRotY * mPos;
	return mRotY;
}

void CPcObj::SetLocalRotY(float fRotY) 
{ 
	m_fLocalRotY = fRotY; 
}

void CPcObj::AddAllyNum(int num)
{ 
	m_nAllyNum += num; 
}

BOOL CPcObj::GetKeyPush() 
{ 
	return m_bIsKeyPush; 
}

int CPcObj::GetAttackKind() 
{ 
	return m_nAttackKind; 
}

int CPcObj::GetAllyNum() 
{ 
	return m_nAllyNum; 
}

int CPcObj::GetGunMagazine() 
{ 
	return m_nGunMagazine; 
}

BOOL CPcObj::GetIsHaveAxe()
{
	return m_bIsHaveAxe;
}

void CPcObj::SetIsHaveAxe(BOOL flag)
{
	m_bIsHaveAxe = flag;
	m_nCoolTime = 60 * PcConstruct::WAITTIME_MELEE_WEAPON * (1 - m_pGMain->m_pPcProc->GetAbility()->GetAbilityLevel(3) * 0.3);
}

int CPcObj::GetCoolTime()
{
	return m_nCoolTime;
}

void CPcObj::SetCanMove(BOOL flag)
{
	m_bIsCanMove = flag;
}

// ImGui�\������
void CPcObj::ShowImgui(bool* p_open)
{
	// �E�B���h�E�ʒu�ƃT�C�Y��ݒ肵�܂��BImGuiCond_Once �ɂ��A����̂ݐݒ肳��܂��B
	ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 420), ImGuiCond_Once);
	// �e�t���[���̘g��
	ImGui::GetStyle().FrameBorderSize = 1.0f; // �g���̑���
	// �E�B���h�E��`�̐ݒ�
	// �EBegin�ŃE�C���h�E�̖��O�ݒ�B�Ȃ��A���O�̓E�C���h�E���ƂɃ��j�[�N�ɂȂ�悤�ɂ��邱��
	// �Ep_open�́A����{�^�����������Ƃ�false���Ԃ�
	if (ImGui::Begin(u8"ImGui �f�o�b�N�E�C���h�E", p_open))
	{
		if (ImGui::Button(u8"�o�H�e�[�u������", ImVec2(300, 20)))
		{
			CSearchRoute* pSearch = new CSearchRoute(m_pGMain);
			pSearch->MakeANode();

			pSearch->MakeRouteTable();
			SAFE_DELETE(pSearch);

		}

		ImGui::Separator();
		if (ImGui::Button(u8"�G��S�ł�����", ImVec2(150, 20)))
		{
			m_pGMain->m_pEnmProc->SetNonActive();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"�����̒ǉ�", ImVec2(60, 20)))
		{
			m_pGMain->m_pAllyProc->Start(GetPositionVector(m_mWorld), TRUE);
			m_nAllyNum++;
		}

		ImGui::Separator();
		if (ImGui::Button(u8"�K�w������", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->SetHierarchy(m_pGMain->m_pMapProc->GetHierarchy() + 1);
		}

		ImGui::Separator();
		if (ImGui::Button(u8"�{�X�X�|�[��", ImVec2(60, 20)))
		{
			m_pGMain->m_pEnmBossProc->Start(VECTOR3(0.0f, 0.0f, 0.0f));
		}

		ImGui::Separator();
		if (ImGui::Button(u8"�\�͋���", ImVec2(60, 20)))
		{
			m_pGMain->m_pPcProc->GetAbility()->AddAbilityLevel(9);
			m_pGMain->m_pPcProc->GetAbility()->AddAbilityLevel(10);
			m_pGMain->m_pPcProc->GetAbility()->AddAbilityLevel(11);
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map2", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap2();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map3", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap3();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map4", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap4();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map5", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap5();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map6", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap6();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map7", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap7();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map8", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap8();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map9", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap9();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map10", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap10();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map11", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap11();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map12", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap12();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map13", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap13();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map14", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap14();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map15", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap15();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map16", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap16();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map17", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap17();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map18", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap18();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map19", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap19();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map20", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap20();
		}

		ImGui::Separator();
		// �{�^������
		ImGui::Text(" ");
		ImGui::SameLine();
		if (ImGui::Button(u8"�I��", ImVec2(60, 20)))
		{
			// �{�^�����������Ƃ��A*p_open��false��Ԃ�
			*p_open = false;
		}
	}
	ImGui::End();
}