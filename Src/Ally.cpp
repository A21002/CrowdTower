//=============================================================================
//		�����L�����N�^�[�̐���
//																Ally.cpp
//=============================================================================
#include  "Ally.h"
#include  "Weapon.h"
#include  "Map.h"
#include  "Effect.h"
#include  "Playchar.h"
#include  "EnemyBoss.h"
#include  "BackFore.h"
#include  "SearchRoute.h"

//======================================================================
// �����L�����N�^�[�@�萔�N���X
//======================================================================
const float AllyConstruct::SPEED = 0.08f;
const float AllyConstruct::START_INVIN_TIME = 0.5f;
const VECTOR3 AllyConstruct::HITBOX_MIN = VECTOR3(-0.7f, -0.05f, -0.3f);
const VECTOR3 AllyConstruct::HITBOX_MAX = VECTOR3(0.7f, 2.0f, 0.3f);


//============================================================================
// �����v���V�[�W���N���X
//============================================================================
// ---------------------------------------------------------------------------
// �����v���V�[�W���̃R���X�g���N�^
//
//   �Ȃ��A�I�u�W�F�N�g��delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
// ---------------------------------------------------------------------------
CAllyProc::CAllyProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// �L�����N�^�[�@�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Char/Ally/Ally_Skin.mesh"));    // �L�����N�^�[
	m_pMesh->LoadAnimation(_T("Data/Char/Ally/Idle.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Ally/Run2.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Ally/Sword.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Ally/Arrow.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Ally/Axe.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Ally/Gun.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Ally/Hit.anmx"));
	m_pMesh->m_fHeightMax = 0.01f;
	m_pMesh->m_iMaxDevide = 16;
	ResetHoldItem();
	m_fAveTran = 0.0f;
	m_fAveRot = 0.0f;
	m_fOldAveTran = 0.0f;
	m_fOldAveRot = 0.0f;
	// -------------------------------------------------------------------------
	for (int i = 0; i < ALLY_MAX; i++)
	{
		m_pObjArray.push_back(new CAllyObj(this));  // m_pObjArray�ɃI�u�W�F�N�g�𐶐�����
	}
}

// ---------------------------------------------------------------------------
// �����v���V�[�W���̃f�X�g���N�^
// ---------------------------------------------------------------------------
CAllyProc::~CAllyProc()
{
}

// ---------------------------------------------------------------------------
// �����̕��펝���ւ��֐�
// ---------------------------------------------------------------------------
void CAllyProc::SetHoldItem(DWORD weaponId)
{
	// �e���킲�ƂɑJ��
	switch (weaponId) {
	case ATTACK_ALLOW:
		m_HoldItem.m_nHoldObjNo = 8;	// ��Ɏ�����̃I�u�W�F�ԍ�
		m_HoldItem.m_nPosMesh = 0;		// �����ʒu�̃��b�V���ԍ�
		m_HoldItem.m_nPosBone = 16;	// �����ʒu�̃{�[���ԍ�
		m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.18f, -0.03f);  // ��Ɏ�����̈ʒu�I�t�Z�b�g
		m_HoldItem.m_nOffsetRot = VECTOR3(68.0f, 230.0f, 0.0f);  // ��Ɏ�����̊p�x�I�t�Z�b�g
		break;

	case ATTACK_SWORD:
		m_HoldItem.m_nHoldObjNo = 9;
		m_HoldItem.m_nPosMesh = 0;
		m_HoldItem.m_nPosBone = 14;	
		m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.24f, -0.07f);
		m_HoldItem.m_nOffsetRot = VECTOR3(0.0f, 0.0f, 0.0f); 
		break;

	case ATTACK_GUN:
		m_HoldItem.m_nHoldObjNo = 10;
		m_HoldItem.m_nPosMesh = 0;
		m_HoldItem.m_nPosBone = 14;
		m_HoldItem.m_nOffsetPos = VECTOR3(-0.1f, 0.25f, 0.06f);
		m_HoldItem.m_nOffsetRot = VECTOR3(90.0f, 1.0f, 108.0f);
		break;

	case ATTACK_AXE:
		m_HoldItem.m_nHoldObjNo = 11;
		m_HoldItem.m_nPosMesh = 0;
		m_HoldItem.m_nPosBone = 14;	
		m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.14f, 0.04f); 
		m_HoldItem.m_nOffsetRot = VECTOR3(0.0f, -90.0f, 0.0f);
		break;

	default:
		break;
	}
}

// ---------------------------------------------------------------------------
// �����v���V�[�W���̔�������
// ---------------------------------------------------------------------------
BOOL CAllyProc::Start(VECTOR3 vPos, BOOL flag)
{
	for (DWORD i = 0; i < m_pObjArray.size(); i++) {
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(vPos, flag);
			return TRUE;
		}
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// �����v���V�[�W���̍X�V����
// ---------------------------------------------------------------------------
void CAllyProc::Update()
{
	// �O��̑S�I�u�W�F�N�g�̈ړ��A��]�ʂ�ۑ�
	m_fOldAveTran = m_fAveTran;
	m_fOldAveRot = m_fAveRot;
	// ����̈ړ��A��]�ʂ̕ۑ��Ɍ����ă��Z�b�g
	m_fAveTran = 0.0f;
	m_fAveRot = 0.0f;
}

// ---------------------------------------------------------------------------
//
// �����v���V�[�W���̍폜����
//
// ---------------------------------------------------------------------------
void CAllyProc::DeleteObj(int num)
{
	m_pGMain->m_pPcProc->GetPcObjPtr()->AddAllyNum(-1 * num);
	for (int i = 0; i < num; i++) {
		for (int j = 0; j < m_pObjArray.size(); j++) {
			if (m_pObjArray[j]->GetActive()) {
				m_pObjArray[j]->SetActive(false);
				break;
			}
		}
	}
}

//============================================================================
// �A�N�Z�X�֐�
//============================================================================
float CAllyProc::GetAveTran()
{
	return m_fOldAveTran;
}

float CAllyProc::GetAveRot()
{
	return m_fOldAveRot;
}

void CAllyProc::AddAveTran(float tran)
{
	m_fAveTran += tran;
}

void CAllyProc::AddAveRot(float rot)
{
	m_fAveRot += rot;
}

//============================================================================
// �����I�u�W�F�N�g�N���X
//============================================================================
// ---------------------------------------------------------------------------
// �����I�u�W�F�N�g�̃R���X�g���N�^
//
//  ����
//  CBaseProc* pProc		�F �e�v���V�[�W���ւ̃|�C���^
// ---------------------------------------------------------------------------
CAllyObj::CAllyObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nHP = 1;		// �̗�
	const int nAtc = 0;		// �U����

	m_bActive = FALSE;
	m_dwStatus = NORMAL;

	m_vTarget = GetPositionVector(m_mWorld);

	// �o�E���f�B���O�{�b�N�X�̐ݒ�
	m_pBBox = new CBBox(m_pGMain->m_pShader, AllyConstruct::HITBOX_MIN, AllyConstruct::HITBOX_MAX);
	m_pBBox->m_mWorld = m_mWorld; 
	m_nMaxHp = nHP;
	m_nAtc = nAtc;
	m_nTag = ALLY;

	// �o�H�T���}�b�v�̐ݒ�
	m_pSearchRoute = new CSearchRoute(m_pGMain);
	m_nCnt2 = 0;
}

// ---------------------------------------------------------------------------
// �����I�u�W�F�N�g�̃f�X�g���N�^
//
//   �Ȃ��ACBBox��delete��CBaseObj�̃f�X�g���N�^�ōs�����ߕs�v
// ---------------------------------------------------------------------------
CAllyObj::~CAllyObj()
{
	SAFE_DELETE(m_pSearchRoute);
}

// ---------------------------------------------------------------------------
// �����I�u�W�F�N�g�̔���
// ---------------------------------------------------------------------------
BOOL CAllyObj::Start(VECTOR3 vPos, BOOL flag)
{
	m_bActive = TRUE;
	ResetStatus();
	m_dwStatus = FLASH;
	m_nCnt1 = 60 * AllyConstruct::START_INVIN_TIME;

	// �����X�|�[���̏ꍇ�A�͈͓��Ń����_���X�|�[������
	if (flag) {
		vPos.x = Random(-5, 5);
		vPos.z = Random(-25, -20);
	}
	m_mWorld = XMMatrixTranslationFromVector(vPos);
	m_mWorldOld = m_mWorld;

	// �ړI�n��ݒ�
	MATRIX4X4 m_mPcWorld = XMMatrixTranslation(0.0f, -1.0f, 0.0f) * m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld();
	m_vTarget = GetPositionVector(m_mPcWorld);

	// ������
	m_nOldAttackKind = ATTACK_NONE;
	m_nHp = m_nMaxHp;
	m_IsCanMove = TRUE;
	// �o�H�T���z�������������
	m_pSearchRoute->MakeANode();

	m_bGroup = flag;
	if (!flag) {
		m_pGMain->m_pSpown->Play();
		m_pGMain->m_pPcProc->GetPcObjPtr()->AddAllyNum(1);
	}
	
	return TRUE;
}

//-----------------------------------------------------------------------------
// �����I�u�W�F�N�g�̐���
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CAllyObj::Update()
{
	if (!m_bActive) return;  // ��\���̂Ƃ��͏��������Ȃ�

	// �ϐ��̏�����   ---------------------------------------
	m_vPosUp.x = m_vPosUp.y = m_vPosUp.z = 0.0f;
	m_vRotUp.x = m_vRotUp.y = m_vRotUp.z = 0.0f;
	m_mWorldOld = m_mWorld;  // ��O�̈ʒu

	// �X�e�[�^�X���Ƃ̏���  --------------------------------
	switch (m_dwStatus)
	{
	case  FLASH:	// ���G���
		if (--m_nCnt1 <= 0)
		{
			m_dwStatus = NORMAL;
		}

	case  NORMAL:	// �������ʏ��Ԃ̂Ƃ��̏���
		UpdateNormal();
		break;

	case  DAMAGE:	// �������_���[�W��Ԃ̂Ƃ��̏���
		if (m_AnimStatus.animNum != AllyConstruct::ANIM_HIT)
		{
			m_AnimStatus.SetNum(AllyConstruct::ANIM_HIT);
			m_pGMain->m_pEffectProc->m_pMinusParticleProc->Start(GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld()) + VECTOR3(0.0f, 1.5f, 0.0f), VECTOR3(0.0f, 1.0f, 0.0f));
			m_pGMain->m_pSeDead->Play();
		}
		if (m_AnimStatus.isEnd(AllyConstruct::ANIM_HIT))
		{
			m_dwStatus = DEAD;
		}
		break;

	case  DEAD:	// ���������S��Ԃ̂Ƃ��̏���
		m_pGMain->m_pPcProc->GetPcObjPtr()->AddAllyNum(-1);
		m_pGMain->m_pBackForeProc->AddDeadAllyNum(1);
		m_bActive = false;
		return;
		break;
	}
	// �{�X�Ƃ̏d�Ȃ蔻��
	SetKeepOffset(m_pGMain->m_pEnmBossProc->GetEnmBossObjPtr());

	// �}�b�v�R���W�����Ǝ��R����  --------------------------
	m_fJumpY += (0.5f * GRAVITY * 0.00005f) * m_fJumpTime;	// ���R����
	m_fJumpTime += 1.0f;
	if (m_pGMain->m_pMapProc->isCollisionMoveGravity(&m_mWorld, m_mWorldOld, m_pBBox->m_vMax.x) != 3)  // �}�b�v�R���W����
	{
		m_fJumpY = 0.0f;  // ���R�����̒�~
		m_fJumpTime = 0.0f;
	}

	// ���S�A�_���[�W���͔���z��ɉ����Ȃ�
	if (m_dwStatus == NORMAL) {
		// PC�A�����Ƃ̏Փˉ��
		m_pGMain->m_pMapProc->GetPcSpPart()->SetKeepOffset(this, GetPositionVector(m_mWorld));
		// ��ԕ����֓o�^
		VECTOR3 vMin, vMax;
		m_pGMain->m_pMapProc->GetPcSpPart()->MakeAABB(
			GetPositionVector(m_mWorldOld), GetPositionVector(m_mWorld), 2.0f, vMin, vMax);
		m_pGMain->m_pMapProc->GetPcSpPart()->SetObj(vMin, vMax, this);
	}

	// �}�b�v�R���W�����Ǝ��R����  --------------------------
	m_fJumpY += (0.5f * GRAVITY * 0.00005f) * m_fJumpTime;	// ���R����
	m_fJumpTime += 1.0f;
	if (m_pGMain->m_pMapProc->isCollisionMoveGravity(&m_mWorld, m_mWorldOld, m_pBBox->m_vMax.x) != 3)  // �}�b�v�R���W����
	{
		m_fJumpY = 0.0f;  // ���R�����̒�~
		m_fJumpTime = 0.0f;
	}

	// �`�揈��  ---------------------------------------------
	// �X�L�����b�V�������_�����O 
	GetMesh()->Render(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);

	if (GetHoldItem().m_nHoldObjNo != 0 && m_bGroup) // ��Ɏ������A�C�e��������Ƃ�
	{
		MATRIX4X4 mHold;
		mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, GetHoldItem().m_nPosBone, GetHoldItem().m_nPosMesh);  // ��̈ʒu�̃{�[���}�g���b�N�X
		m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->Disp(mHold, GetHoldItem()); // �A�C�e���\��
	}

	// �o�E���f�B���O�{�b�N�X
	m_pBBox->m_mWorld = m_mWorld;
}

//-----------------------------------------------------------------------------
// �����I�u�W�F�N�g�̐���
//
// �������ʏ��Ԃ̂Ƃ��̏���
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CAllyObj::UpdateNormal()
{
	if (m_IsCanMove) {
		// �U���A�j���[�V�����ɂ���Ĉړ�����������Ă��Ȃ�
		if (m_bGroup)
		{
			// �O���[�v�ɑ����Ă���
			UpdateMoveBoid(AllyConstruct::SPEED);
		}
		else {
			// �O���[�v�ɑ����Ă��Ȃ�
			UpdateMoveSearch(AllyConstruct::SPEED);
		}
	}
	// �O���[�v�ɍ������Ă���΍U������
	if (m_bGroup)
	{
		UpdateAttack();
	}
}

//-----------------------------------------------------------------------------
// �����I�u�W�F�N�g�̍U������
//
//�@PC�̏�����ɍU������
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void CAllyObj::UpdateAttack()
{
	// PC�̍U�����͏��
	int attackKind = m_pGMain->m_pPcProc->GetPcObjPtr()->GetAttackKind();
	CBaseProc* pProc = m_pGMain->m_pAllyProc;
	CAbility* ability = m_pGMain->m_pPcProc->GetAbility();

	switch (attackKind)
	{
	case ATTACK_NONE:
		// �U�����s���Ă��Ȃ�
		if (m_AnimStatus.isEnd(AllyConstruct::ANIM_GUN)) {
			// �e�U���A�j���[�V�����ŁA���ꂪ�I�������ꍇ
			m_AnimStatus.SetNum(AllyConstruct::ANIM_IDLE);
			m_IsCanMove = TRUE;
			m_AnimStatus.animSpeed = 1;
			pProc->ResetHoldItem();
		}
		break;

	case ATTACK_SHOTSWORD:
		// ���U��(�a������)
		// PC�Ɠ�������������
		m_mWorld = GetRotateMatrixY(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld()) * GetPositionMatrix(m_mWorld);
		// �U�����[�V�������s���s�\
		m_IsCanMove = FALSE;
		// ��������
		pProc->SetHoldItem(ATTACK_SWORD);
		// �G�t�F�N�g����
		m_pGMain->m_pEffectProc->m_pBillSlashProc->Start(m_mWorld);

		if (ability->GetAbilityLevel(4) > 0) {
			// �a���\�͎擾�ς�
			m_pGMain->m_pWeaponProc->m_pWeaponShotSlashProc->Start(m_mWorld, PC);
		}
		// ���U���A�j���[�V�����ݒ�
		m_AnimStatus.SetNum(AllyConstruct::ANIM_SWORD);
		m_AnimStatus.animSpeed = ability->GetAbilityLevel(3) * 0.3f + 1;
		break;

	case ATTACK_SWORD:
		// ���U��(�a���Ȃ�)
				// PC�Ɠ�������������
		m_mWorld = GetRotateMatrixY(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld()) * GetPositionMatrix(m_mWorld);
		// �U�����[�V�������s���s�\
		m_IsCanMove = FALSE;
		// ��������
		pProc->SetHoldItem(ATTACK_SWORD);
		// �G�t�F�N�g����
		m_pGMain->m_pEffectProc->m_pBillSlashProc->Start(m_mWorld);

		// ���U���A�j���[�V�����ݒ�
		m_AnimStatus.SetNum(AllyConstruct::ANIM_SWORD);
		m_AnimStatus.animSpeed = ability->GetAbilityLevel(3) * 0.3f + 1;
		break;

	case ATTACK_ALLOW:
		// �|��U��
		// PC�Ɠ�������������
		m_mWorld = GetRotateMatrixY(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld()) * GetPositionMatrix(m_mWorld);
		// �U�����[�V�������s���s�\
		m_IsCanMove = FALSE;
		// �|������
		pProc->SetHoldItem(ATTACK_ALLOW);

		// �|��U���A�j���[�V�����ݒ�
		m_AnimStatus.SetNum(AllyConstruct::ANIM_ARROW);
		m_AnimStatus.animSpeed = ability->GetAbilityLevel(0) * 0.3f + 1;
		break;

	case ATTACK_AXE:
		// ���U��
		// PC�Ɠ�������������
		m_mWorld = GetRotateMatrixY(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld()) * GetPositionMatrix(m_mWorld);
		// �U�����[�V�������s���s�\
		m_IsCanMove = FALSE;
		// ��������
		pProc->SetHoldItem(ATTACK_AXE);
		m_pGMain->m_pEffectProc->m_pBillSlashProc->Start(m_mWorld);	// �G�t�F�N�g

		// ���U���A�j���[�V�����ݒ�
		m_AnimStatus.SetNum(AllyConstruct::ANIM_AXE);
		m_AnimStatus.animSpeed = ability->GetAbilityLevel(3) * 0.3f + 1;
		break;

	case ATTACK_AXESHOT:
		// ���̉����U��
		// PC�Ɠ�������������
		m_mWorld = GetRotateMatrixY(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld()) * GetPositionMatrix(m_mWorld);
		// �U�����[�V�������s���s�\
		m_IsCanMove = FALSE;

		// ���U���A�j���[�V�����ݒ�
		m_AnimStatus.SetNum(AllyConstruct::ANIM_AXE);
		m_AnimStatus.animSpeed = ability->GetAbilityLevel(3) * 0.3f + 1;

		m_pGMain->m_pWeaponProc->m_pWeaponAxeShotProc->Start(m_mWorld * XMMatrixTranslation(0.0f, 1.0f, 1.0f), 5 + 2 * ability->GetAbilityLevel(4), PC);

		break;

	case ATTACK_GUN:
		// �e�U��
		// PC�Ɠ�������������
		m_mWorld = GetRotateMatrixY(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld()) * GetPositionMatrix(m_mWorld);
		// ���U���A�j���[�V�����ݒ�
		if (m_AnimStatus.animNum != AllyConstruct::ANIM_GUN)
		{
			m_IsCanMove = FALSE;
			m_AnimStatus.SetNum(AllyConstruct::ANIM_GUN);
			// �e������
			pProc->SetHoldItem(ATTACK_GUN);
		}

		// ����
		m_pGMain->m_pWeaponProc->m_pWeaponGunProc->Start(XMMatrixTranslation(0.0f, 0.7f, 0.5f) * m_mWorld, PC);
		
		break;
	}
	m_nOldAttackKind = attackKind;

	// �|�U��
	if (m_AnimStatus.isEnd(AllyConstruct::ANIM_ARROW))
	{
		m_pGMain->m_pWeaponProc->m_pWeaponArrowProc->Start(m_mWorld, PC); // ���
		m_AnimStatus.SetNum(AllyConstruct::ANIM_IDLE);
		m_AnimStatus.animSpeed = 1;
		pProc->ResetHoldItem();
		m_IsCanMove = TRUE;
	}

	// ���U��
	if (m_AnimStatus.animNum == AllyConstruct::ANIM_SWORD)
	{
		if (m_AnimStatus.isEnd(AllyConstruct::ANIM_SWORD))
		{
			m_AnimStatus.SetNum(AllyConstruct::ANIM_IDLE);
			m_AnimStatus.animSpeed = 1;
			pProc->ResetHoldItem();
			m_IsCanMove = TRUE;
		}
		else {
			MATRIX4X4 mSword, mHold;
			VECTOR3 vMin, vMax;

			mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, GetHoldItem().m_nPosBone, GetHoldItem().m_nPosMesh);  // ��̈ʒu�̃{�[���}�g���b�N�X
			m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->GetInfo(mHold, GetHoldItem(), mSword, vMin, vMax); // �{�[���}�g���b�N�X���w�肵�Ď�Ɏ��A�C�e�����𓾂�
			m_pGMain->m_pWeaponProc->m_pWeaponSwordProc->Start(mSword, vMin, vMax, PC);  // ���U���̊J�n

		}
	}

	// ���U��
	if (m_AnimStatus.animNum == AllyConstruct::ANIM_AXE) {
		if (m_AnimStatus.isEnd(AllyConstruct::ANIM_AXE))
		{
			m_AnimStatus.SetNum(AllyConstruct::ANIM_IDLE);
			m_AnimStatus.animSpeed = 1;
			pProc->ResetHoldItem();
			m_IsCanMove = TRUE;
		}
		else if(GetHoldItem().m_nHoldObjNo != 0){
			MATRIX4X4 mSword, mHold;
			VECTOR3 vMin, vMax;
			mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, GetHoldItem().m_nPosBone, GetHoldItem().m_nPosMesh);  // ��̈ʒu�̃{�[���}�g���b�N�X
			m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->GetInfo(mHold, GetHoldItem(), mSword, vMin, vMax); // �{�[���}�g���b�N�X���w�肵�Ď�Ɏ��A�C�e�����𓾂�
			m_pGMain->m_pWeaponProc->m_pWeaponAxeProc->Start(mSword, vMin, vMax, PC);  // ���U���̊J�n
		}
	}
}

//-----------------------------------------------------------------------------
// �����I�u�W�F�N�g�̃O���[�v�ɑ����Ă��Ȃ��Ƃ��̈ړ�
//
// �o�H�T�������ƂɈړ�����
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void CAllyObj::UpdateMoveSearch(float fForward)
{
	MATRIX4X4 mTemp;
	if (!TargetMove(m_vTarget, fForward, 180.0f))	// �ړI�n�iTarget�j�ɒB�������ǂ���
	{
		// �ړI�n�ֈړ�
		if (m_AnimStatus.animNum != AllyConstruct::ANIM_WALK) {
			// ���s�A�j���[�V�����֑J��
			m_AnimStatus.SetNum(AllyConstruct::ANIM_WALK);
		}
		mTemp = XMMatrixRotationY(XMConvertToRadians(m_vRotUp.y));					// �ړI�n�iTarget�j�֌������p�x
		m_mWorld = mTemp * m_mWorld;
		mTemp = XMMatrixTranslation(m_vPosUp.x, m_vPosUp.y + m_fJumpY, m_vPosUp.z);	// �ړI�n�iTarget�j�֌�����
		m_mWorld = mTemp * m_mWorld;
	}
	else {
		m_bGroup = TRUE;
	}

	// �o�H�T���̎��̈ʒu���Z�b�g����
	m_vTarget = GetPositionVector(m_mWorld);
	VECTOR3 vStart = VECTOR3(m_vTarget.x, 0, m_vTarget.z);   // �J�n�ʒu�i���݈ʒu�j

	VECTOR3 vEnd = GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());
	BOOL isTargetAreaOut = TRUE;
	m_vTarget = m_pSearchRoute->GetNextPos(vStart, vEnd, &isTargetAreaOut);
	
}

//-----------------------------------------------------------------------------
// �����I�u�W�F�N�g�̃O���[�v�ɑ����Ă���Ƃ��̈ړ�
//
// �Q�O�A���S���Y�������ƂɈړ�����
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void CAllyObj::UpdateMoveBoid(float fForward)
{
	VECTOR3 pos = GetPositionVector(m_mWorld);
	float rot = GetRotateVector(m_mWorld).y;
	m_mWorld = XMMatrixTranslation(pos.x, pos.y, pos.z);
	// �@�Q��̒��S�Ɍ�����
	VECTOR3 vTarget = GetPositionVector(XMMatrixTranslation(0.0f, 0.0f, -1.0f) * m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());
	float tran1 = GetDistance(vTarget);
	float rot1 = GetTargetRotateVector(m_mWorld, vTarget).y;

	// �A�������x�A�����ɂ���
	float aveTran = m_pGMain->m_pAllyProc->GetAveTran();
	float aveRot = m_pGMain->m_pAllyProc->GetAveRot();
	int num = m_pGMain->m_pPcProc->GetPcObjPtr()->GetHp() - 1;
	if (num >= 2) {
		aveTran /= num;
		aveRot /= num;
	}
	
	// �ړ�
	float movePos = 0.0f;
	float moveRot = 0.0f;
	if (m_pGMain->m_pPcProc->GetPcObjPtr()->GetKeyPush()) {
		movePos = tran1 * 1 + aveTran * 0.1;
		moveRot = rot1 * 1 + aveRot * 0.1;
		if (movePos > fForward) {
			movePos = fForward;
		}

		m_mWorld = XMMatrixTranslation(0.0f, 0.0f, movePos) * XMMatrixRotationY(XMConvertToRadians(moveRot)) * m_mWorld;
		
		if (m_AnimStatus.animNum < AllyConstruct::ANIM_SWORD) {
			m_AnimStatus.SetNum(AllyConstruct::ANIM_WALK);
		}
	}
	else {
		m_mWorld = XMMatrixRotationY(XMConvertToRadians(rot)) * m_mWorld;
		movePos = 0.0f;
		if (m_AnimStatus.animNum != AllyConstruct::ANIM_IDLE) {
			m_AnimStatus.SetNum(AllyConstruct::ANIM_IDLE);
		}
	}
	m_pGMain->m_pAllyProc->AddAveTran(movePos * 0.2);
	m_pGMain->m_pAllyProc->AddAveRot(moveRot * 0.2);
}