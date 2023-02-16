//=============================================================================
//		�G�L�����N�^�[�̏���
//																EnemyBoss.cpp
//=============================================================================
#include  "EnemyBoss.h"
#include  "Enemy.h"
#include  "Playchar.h"
#include  "Ally.h"
#include  "Weapon.h"
#include  "Map.h"
#include  "Effect.h"
#include  "BackFore.h"
#include  "SearchRoute.h"
#include  "Camera.h"

//======================================================================
// �G�{�X�@�萔�N���X
//======================================================================
float const EnemyBossConstruct::SPEED = 0.07f;

//============================================================================
// �G�{�X�v���V�[�W���N���X
//============================================================================
// ---------------------------------------------------------------------------
// �G�{�X�v���V�[�W���̃R���X�g���N�^
//
//   �Ȃ��A�I�u�W�F�N�g��delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
// ---------------------------------------------------------------------------
CEnmBossProc::CEnmBossProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_nNum = 0;

	// Fbx�X�L�����b�V���̐ݒ�
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Char/EnemyBoss/EnemyBoss.mesh"));
	m_pMesh->LoadAnimation(_T("Data/Char/EnemyBoss/Idle.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/EnemyBoss/run.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/EnemyBoss/kick.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/EnemyBoss/kick_Left.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/EnemyBoss/stomp.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/EnemyBoss/stomp_Left.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/EnemyBoss/bodyStomp.anmx"), eRootAnimXZ);
	m_pMesh->LoadAnimation(_T("Data/Char/EnemyBoss/standUp.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/EnemyBoss/roar.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/EnemyBoss/die.anmx"));
	m_pMesh->m_fHeightMax = 0.01f;
	m_pMesh->m_iMaxDevide = 8;
	m_HoldItem.m_nHoldObjNo = 12;	// ��Ɏ�����̃I�u�W�F�ԍ�
	m_HoldItem.m_nPosMesh = 0;		// �����ʒu�̃��b�V���ԍ�
	m_HoldItem.m_nPosBone = 2;	// �����ʒu�̃{�[���ԍ�
	m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.0f, 0.0f);  // ��Ɏ�����̈ʒu�I�t�Z�b�g
	m_HoldItem.m_nOffsetRot = VECTOR3(0.0f, 0.0f, 0.0f);  // ��Ɏ�����̊p�x�I�t�Z�b�g

	for (int i = 0; i < ENM_BOSS_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmBossObj(this));  // m_pObjArray�ɃI�u�W�F�N�g�𐶐�����
	}
}

// ---------------------------------------------------------------------------
// �G�{�X�v���V�[�W���̃f�X�g���N�^
// ---------------------------------------------------------------------------
CEnmBossProc::~CEnmBossProc()
{
}

// ---------------------------------------------------------------------------
// �G�̍ő�X�|�[�����̐ݒ�
// ---------------------------------------------------------------------------
void CEnmBossProc::SetMaxNum()
{
	m_nNum = m_pGMain->m_pMapProc->GetNavMap().size() * m_pGMain->m_pMapProc->GetHierarchy();
}

// ---------------------------------------------------------------------------
// ����̕ύX
// ---------------------------------------------------------------------------
void CEnmBossProc::SetHoldItem(DWORD weaponId)
{
	switch (weaponId)
	{
	case EnemyBossConstruct::ATTACK_KICK_RIGHT:
		m_HoldItem.m_nHoldObjNo = 12;	// ��Ɏ�����̃I�u�W�F�ԍ�
		m_HoldItem.m_nPosBone = 2;	// �����ʒu�̃{�[���ԍ�
		break;

	case EnemyBossConstruct::ATTACK_KICK_LEFT:
		m_HoldItem.m_nHoldObjNo = 12;	// ��Ɏ�����̃I�u�W�F�ԍ�
		m_HoldItem.m_nPosBone = 5;	// �����ʒu�̃{�[���ԍ�
		break;

	case EnemyBossConstruct::ATTACK_STOMP_RIGHT:
		m_HoldItem.m_nHoldObjNo = 13;	// ��Ɏ�����̃I�u�W�F�ԍ�
		m_HoldItem.m_nPosBone = 0;	// �����ʒu�̃{�[���ԍ�
		break;

	case EnemyBossConstruct::ATTACK_STOMP_LEFT:
		m_HoldItem.m_nHoldObjNo = 13;	// ��Ɏ�����̃I�u�W�F�ԍ�
		m_HoldItem.m_nPosBone = 3;	// �����ʒu�̃{�[���ԍ�
		break;

	case EnemyBossConstruct::ATTACK_BODYSTOMP:
		m_HoldItem.m_nHoldObjNo = 14;	// ��Ɏ�����̃I�u�W�F�ԍ�
		m_HoldItem.m_nPosBone = 8;	// �����ʒu�̃{�[���ԍ�


	default:
		break;
	}
}

CEnmBossObj* CEnmBossProc::GetEnmBossObjPtr()
{
	return (CEnmBossObj*)GetObjArrayPtr()[0];
}

// ---------------------------------------------------------------------------
// �������������l�Ƀ��Z�b�g����
// �i�ő唭����(m_nMaxNum)�𔭐���m_nNum�ɐݒ肷��
//
// �߂�l  �Ȃ�
// ---------------------------------------------------------------------------
void CEnmBossProc::ResetEnmNum()
{
	m_nNum = m_pGMain->m_pMapProc->GetNavMap().size() * m_pGMain->m_pMapProc->GetHierarchy();
}

// ---------------------------------------------------------------------------
// ���݂̃A�N�e�B�u�̃I�u�W�F�N�g���v��
// ---------------------------------------------------------------------------
int CEnmBossProc::CountActiveEnm()
{
	return m_nNum;
}

// ---------------------------------------------------------------------------
// �G�{�X�v���V�[�W���̎蓮��������
// ---------------------------------------------------------------------------
BOOL CEnmBossProc::Start(VECTOR3 vPos)
{
	for (DWORD i = 0; i < m_pObjArray.size(); i++) {
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(vPos);
			return TRUE;
		}
	}
	return FALSE;
}

//============================================================================
// �G�{�X�I�u�W�F�N�g�N���X
//============================================================================
// ---------------------------------------------------------------------------
// �G�{�X�I�u�W�F�N�g�̃R���X�g���N�^
//
//  ����
// ---------------------------------------------------------------------------
CEnmBossObj::CEnmBossObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nMaxHP = EnemyBossConstruct::HP;      // �̗�
	const int nAtc = 1;      // �̂̍U����

	m_bActive = FALSE;
	m_bIsMovieFinish = TRUE;
	m_bIsBossMovie = FALSE;
	m_dwStatus = NORMAL;

	m_vTarget = GetPositionVector(m_mWorld);

	// �o�E���f�B���O�{�b�N�X�̐ݒ�
	VECTOR3 vMin(-2.5f, 0.0f, -2.5f);
	VECTOR3 vMax(2.5f, 12.4f, 2.5f);
	m_pBBox = new CBBox(m_pGMain->m_pShader, vMin, vMax);

	m_nHp = m_nMaxHp = nMaxHP;
	m_nAtc = nAtc;
	m_nTag = ENM;
	m_nRoarCoolTime = 60 * EnemyBossConstruct::ROAR_COOLTIME;

	// �o�H�T���}�b�v�̐ݒ�
	m_pSearchRoute = new CSearchRoute(m_pGMain);

}
// ---------------------------------------------------------------------------
// �G�{�X�I�u�W�F�N�g�̃f�X�g���N�^
//
//   �Ȃ��ACBBox��delete��CBaseObj�̃f�X�g���N�^�ōs�����ߕs�v
// ---------------------------------------------------------------------------
CEnmBossObj::~CEnmBossObj()
{
	SAFE_DELETE(m_pSearchRoute);
}

// ---------------------------------------------------------------------------
// �G�{�X�I�u�W�F�N�g�̔���
// ---------------------------------------------------------------------------
BOOL CEnmBossObj::Start(VECTOR3 vPos)
{
	m_bActive = TRUE;
	m_dwStatus = NORMAL;
	m_nOldAttackKind = ATTACK_NONE;
	m_bIsMove = true;
	if (!m_bIsBossMovie) {
		m_bIsBossMovie = TRUE;
		m_bIsMovieFinish = FALSE;
		m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetBossMovie(TRUE);
		m_nCnt2 = 60 * 2;
		m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetLocalEyeTPS(0.0f, 0.0f, -10.0f);
	}
	m_mWorld = XMMatrixTranslationFromVector(vPos);
	m_mWorld = XMMatrixRotationY(XMConvertToRadians(180.0f)) * m_mWorld;
	m_mWorldOld = m_mWorld;

	m_nHp = m_nMaxHp;
	// �o�H�T���z��𐶐�����
	m_pSearchRoute->MakeANode();
	m_vTarget = GetPositionVector(m_mWorld);
	// �o�H�T���z�񂩂�o�H�}�̍쐬����
	VECTOR3 vStart = VECTOR3(m_vTarget.x, 0, m_vTarget.z);   // �J�n�ʒu

	VECTOR3 vEnd = GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());

	m_pGMain->m_bIsBoss = TRUE;
	m_pGMain->m_pBgm1->Stop();
	m_pGMain->m_pBossBgm->Play(AUDIO_LOOP);
	return TRUE;
}

//-----------------------------------------------------------------------------
// �G�{�X�I�u�W�F�N�g�̐���
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CEnmBossObj::Update()
{
	if (!m_bActive) return;  // ��\���̂Ƃ��͏��������Ȃ�

	// �ϐ��̏�����   ---------------------------------------
	m_vPosUp.x = m_vPosUp.y = m_vPosUp.z = 0.0f;
	m_vRotUp.x = m_vRotUp.y = m_vRotUp.z = 0.0f;
	m_mWorldOld = m_mWorld;  // ��O�̈ʒu

	if (m_bIsMovieFinish)
	{
		UpdateGame();
	}
	else
	{
		UpdateMovie();
	}

	// �}�b�v�R���W�����Ǝ��R����  --------------------------
	m_fJumpY += (0.5f * GRAVITY * 0.00005f) * m_fJumpTime;	// ���R����
	m_fJumpTime += 1.0f;
	if (m_pGMain->m_pMapProc->isCollisionMoveGravity(&m_mWorld, m_mWorldOld, m_pBBox->m_vMax.x, ENM) != 3)  // �}�b�v�R���W����
	{
		m_fJumpY = 0.0f;  // ���R�����̒�~
		m_fJumpTime = 0.0f;
	}

	// �I�u�W�F�N�g����ԕ����֓o�^
	VECTOR3 vMin, vMax;
	m_pGMain->m_pMapProc->GetEnmSpPart()->MakeAABB(
		GetPositionVector(m_mWorldOld), GetPositionVector(m_mWorld), 2.0f, vMin, vMax);
	m_pGMain->m_pMapProc->GetEnmSpPart()->SetObj(vMin, vMax, this);

	// �J�����ƃv���C���[�̒�����ɏd�Ȃ邩
	VECTOR3 vHit = VECTOR3(0.0f, 0.0f, 0.0f);
	VECTOR3 vNrm = VECTOR3(0.0f, 0.0f, 0.0f);
	VECTOR3 vNow = GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());
	VECTOR3 vOld = GetPositionVector(m_pGMain->m_pCameraProc->GetCameraObjPtr()->GetWorld());

	if (m_pBBox->OBBCollisionLay(vNow, vOld, &vHit, &vNrm))
	{
		GetMesh()->m_vDiffuse.w = 0.3f;
	}
	else {
		GetMesh()->m_vDiffuse.w = 1.0f;
	}

	// �`�揈��  ---------------------------------------------
	// �X�L�����b�V�������_�����O 
	GetMesh()->RenderToon(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, normalize(VECTOR3(0.8f, 1, -1)), m_pGMain->m_vEyePt);

	// �o�E���f�B���O�{�b�N�X
	m_pBBox->m_mWorld = m_mWorld;
}

//-----------------------------------------------------------------------------
// �G�{�X�I�u�W�F�N�g�̃Q�[�����̐���
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void CEnmBossObj::UpdateGame()
{
	// �X�e�[�^�X���Ƃ̏���  --------------------------------
	switch (m_dwStatus)
	{
	case  FLASH: // �G���_���[�W���畜�A�����Ƃ��̏����Bbreak�������Ɏ���NORMAL���s���B
	case  NORMAL: // �G���ʏ��Ԃ̂Ƃ��̏���
		if (m_pGMain->m_pBackForeProc->GetFadeOutFinish()) {
			UpdateNormal();
		}
		break;

	case  DAMAGE: // �G���_���[�W��Ԃ̂Ƃ��̏���
		UpdateDamage();
		break;

	case  DEAD:	// �G�����S��Ԃ̂Ƃ��̏���
		UpdateDead();
		return;
		break;
	}
}

//-----------------------------------------------------------------------------
// �G�{�X�I�u�W�F�N�g�̃��[�r�[����
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void CEnmBossObj::UpdateMovie()
{
	if (m_nCnt2 > 0) {
		// �t�F�[�h�A�E�g�ҋ@
		m_nCnt2--;
		if (m_nCnt2 == 0)
		{
			// ���[�r�[�A�j���[�V�����J�n
			m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_ROAR);
		}
	}
	else {
		if (m_AnimStatus.animFrame >= 150)
		{
			// ���K�I����
			m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetMovePc(TRUE);
		}
	}
}

//-----------------------------------------------------------------------------
// �G�{�X�I�u�W�F�N�g�̐���
//
// �G���ʏ��Ԃ̂Ƃ��̏���
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CEnmBossObj::UpdateNormal()
{
	CPcObj* pPcObj = m_pGMain->m_pPcProc->GetPcObjPtr();
	MATRIX4X4 mTemp;
	BOOL isTargetAreaOut = FALSE;
	float fForward = EnemyBossConstruct::SPEED;

	// �G���_���[�W���畜�A�����Ƃ��̏���  ------------------------------
	if (m_dwStatus == FLASH)
	{
		m_nCnt1--;
		if (m_nCnt1 <= 0) m_dwStatus = NORMAL;
	}
	if (m_bIsMove) {

		if (!TargetMove(m_vTarget, fForward))	// �ړI�n�iTarget�j�ɒB�������ǂ���
		{
			if (m_AnimStatus.animNum < EnemyBossConstruct::ANIM_WALK) {
				m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_WALK);
			}
			mTemp = XMMatrixRotationY(XMConvertToRadians(m_vRotUp.y));					// �ړI�n�iTarget�j�֌������p�x
			m_mWorld = mTemp * m_mWorld;
			mTemp = XMMatrixTranslation(m_vPosUp.x, m_vPosUp.y + m_fJumpY, m_vPosUp.z);	// �ړI�n�iTarget�j�֌�����
			m_mWorld = mTemp * m_mWorld;
		}
		else
		{
			m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_IDLE);
		}
	}

	// �o�H�T���̎��̈ʒu���Z�b�g����
	m_vTarget = GetPositionVector(m_mWorld);
	VECTOR3 vStart = VECTOR3(m_vTarget.x, 0, m_vTarget.z);   // �J�n�ʒu�i���݈ʒu�j

	VECTOR3 vEnd = GetPositionVector(pPcObj->GetWorld());
	m_vTarget = m_pSearchRoute->GetNextPos(vStart, vEnd, &isTargetAreaOut);

	if (!isTargetAreaOut)
	{
		UpdateAttack();
	}
	else {
		m_bIsMove = TRUE;
	}
}

//-----------------------------------------------------------------------------
// �G�{�X�I�u�W�F�N�g�̐���
//
// �U������̏���
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void CEnmBossObj::UpdateAttack()
{
	VECTOR3 pcPos = GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());

	// �ߋ����U�����s��������
	if (GetDistance(pcPos) <= EnemyBossConstruct::MELEE_RANGE && m_bIsMove) {
		float rot = GetTargetRotateVector(m_mWorld, pcPos).y;
		if (rot >= 180) rot -= 360;
		if (rot <= -180) rot += 360;

		// �w��ɂ���ꍇ�A�{�f�B�v���X�Ɉڍs����
		if (rot > 90.0f || rot < -90.0f)
		{
			m_bIsMove = FALSE;
			m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_BODYSTOMP);
			m_pGMain->m_pEnmBossProc->SetHoldItem(EnemyBossConstruct::ATTACK_BODYSTOMP);
		}else
			if (m_nOldAttackKind == ATTACK_NONE)
			{
				m_mWorld = XMMatrixRotationY(XMConvertToRadians(GetTargetRotateVector(m_mWorld, pcPos).y)) * m_mWorld;
				if (Random(0, 1) == 0)
				{
					// �L�b�N�U���֑J��
					if (m_AnimStatus.animNum != EnemyBossConstruct::ANIM_KICK && m_AnimStatus.animNum != EnemyBossConstruct::ANIM_KICK_LEFT)
					{
						m_bIsMove = FALSE;
						m_nOldAttackKind = EnemyBossConstruct::ATTACK_KICK_RIGHT;
						if (Random(0, 1) == 0) {
							m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_KICK);
							m_pGMain->m_pEnmBossProc->SetHoldItem(EnemyBossConstruct::ATTACK_KICK_RIGHT);
						}
						else {
							m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_KICK_LEFT);
							m_pGMain->m_pEnmBossProc->SetHoldItem(EnemyBossConstruct::ATTACK_KICK_LEFT);
						}
					}
				}
				else {
					// ���݂��U���֑J��
					if (m_AnimStatus.animNum != EnemyBossConstruct::ANIM_STOMP && m_AnimStatus.animNum != EnemyBossConstruct::ANIM_STOMP_LEFT)
					{
						m_bIsMove = FALSE;
						m_nOldAttackKind = EnemyBossConstruct::ATTACK_STOMP_RIGHT;
						if (Random(0, 1)) {
							m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_STOMP);
							m_pGMain->m_pEnmBossProc->SetHoldItem(EnemyBossConstruct::ATTACK_STOMP_RIGHT);
						}
						else {
							m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_STOMP_LEFT);
							m_pGMain->m_pEnmBossProc->SetHoldItem(EnemyBossConstruct::ATTACK_STOMP_LEFT);
						}
					}
				}
			}
			else {
				m_mWorld = XMMatrixRotationY(XMConvertToRadians(GetTargetRotateVector(m_mWorld, pcPos).y)) * m_mWorld;
				// �L�b�N�U���֑J��
				if (m_AnimStatus.animNum != EnemyBossConstruct::ANIM_KICK && m_nOldAttackKind != EnemyBossConstruct::ATTACK_KICK_RIGHT)
				{
					m_bIsMove = FALSE;
					m_nOldAttackKind = ATTACK_NONE;
					if (Random(0, 1) == 0) {
						m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_KICK);
						m_pGMain->m_pEnmBossProc->SetHoldItem(EnemyBossConstruct::ATTACK_KICK_RIGHT);
					}
					else {
						m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_KICK_LEFT);
						m_pGMain->m_pEnmBossProc->SetHoldItem(EnemyBossConstruct::ATTACK_KICK_LEFT);
					}
				}
				else {
					// ���݂��U���֑J��
					if (m_AnimStatus.animNum != EnemyBossConstruct::ANIM_STOMP && m_nOldAttackKind != EnemyBossConstruct::ATTACK_STOMP_RIGHT)
					{
						m_bIsMove = FALSE;
						m_nOldAttackKind = ATTACK_NONE;
						if (Random(0, 1)) {
							m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_STOMP);
							m_pGMain->m_pEnmBossProc->SetHoldItem(EnemyBossConstruct::ATTACK_STOMP_RIGHT);
						}
						else {
							m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_STOMP_LEFT);
							m_pGMain->m_pEnmBossProc->SetHoldItem(EnemyBossConstruct::ATTACK_STOMP_LEFT);
						}
					}
				}
			}

		// �{�f�B�v���X�U���֑J��
		if (m_AnimStatus.animNum != EnemyBossConstruct::ANIM_BODYSTOMP && m_bIsMove)
		{
		}
	}
	else {
		// ���K�A�j���[�V�����ւ̑J��
		if (m_bIsMove && m_nRoarCoolTime <= 0 && m_AnimStatus.animNum != EnemyBossConstruct::ANIM_ROAR)
		{
			m_bIsMove = FALSE;
			m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_ROAR);
			VECTOR3 vMin = GetPositionVector(m_mWorld) - VECTOR3(m_pBBox->m_fLengthX, 0.0f, m_pBBox->m_fLengthZ);
			VECTOR3 vMax = GetPositionVector(m_mWorld) + VECTOR3(m_pBBox->m_fLengthX, 0.0f, m_pBBox->m_fLengthZ);
			m_pGMain->m_pEnmProc->Start(vMin, vMax, EnemyBossConstruct::ROAR_SPOWN_NUM);
		}

	}

	// �N�[���^�C���̃J�E���g�_�E��
	if (m_nRoarCoolTime > 0)
	{
		m_nRoarCoolTime--;
	}

	// �L�b�N�U���̔���
	if (m_AnimStatus.animNum == EnemyBossConstruct::ANIM_KICK || m_AnimStatus.animNum == EnemyBossConstruct::ANIM_KICK_LEFT)
	{
		MATRIX4X4 mSword, mHold;
		VECTOR3 vMin, vMax;

		mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, GetHoldItem().m_nPosBone, GetHoldItem().m_nPosMesh);  // ��̈ʒu�̃{�[���}�g���b�N�X
		m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->GetInfo(mHold, GetHoldItem(), mSword, vMin, vMax); // �{�[���}�g���b�N�X���w�肵�Ď�Ɏ��A�C�e�����𓾂�
		m_pGMain->m_pWeaponProc->m_pWeaponAttackBoxProc->Start(mSword, vMin, vMax, ENM);
	}
	
	// ���݂��U���̔���
	if ((m_AnimStatus.animNum == EnemyBossConstruct::ANIM_STOMP || m_AnimStatus.animNum == EnemyBossConstruct::ANIM_STOMP_LEFT) && m_AnimStatus.animFrame >= 50) {
		MATRIX4X4 mSword, mHold;
		VECTOR3 vMin, vMax;

		mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, GetHoldItem().m_nPosBone, GetHoldItem().m_nPosMesh);  // ��̈ʒu�̃{�[���}�g���b�N�X
		m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->GetInfo(mHold, GetHoldItem(), mSword, vMin, vMax); // �{�[���}�g���b�N�X���w�肵�Ď�Ɏ��A�C�e�����𓾂�
		m_pGMain->m_pWeaponProc->m_pWeaponAttackBoxProc->Start(mSword, vMin, vMax, ENM);
	}

	// �{�f�B�v���X�U���̔���
	if (m_AnimStatus.animNum == EnemyBossConstruct::ANIM_BODYSTOMP)
	{
		MATRIX4X4 mPos = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		// �ړ�����
		if (GetMesh()->GetRootAnimType(m_AnimStatus.animNum) == eRootAnimXZ)
		{
			// ���[�g�{�[���A�j���[�V�����ł̑O�i�ړ��l
			mPos = GetMesh()->GetRootAnimUpMatrices(m_AnimStatus);
			
		}
		m_mWorld = mPos * m_mWorld;

		// �����蔻��
		if (m_AnimStatus.animFrame >= 80)
		{
			MATRIX4X4 mSword, mHold;
			VECTOR3 vMin, vMax;

			mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, GetHoldItem().m_nPosBone, GetHoldItem().m_nPosMesh);  // ��̈ʒu�̃{�[���}�g���b�N�X
			m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->GetInfo(mHold, GetHoldItem(), mSword, vMin, vMax); // �{�[���}�g���b�N�X���w�肵�Ď�Ɏ��A�C�e�����𓾂�
			m_pGMain->m_pWeaponProc->m_pWeaponAttackBoxProc->Start(mSword, vMin, vMax, ENM);

		}
	}

	// ���K����UI�U������
	if (m_AnimStatus.animNum == EnemyBossConstruct::ANIM_ROAR && m_AnimStatus.animFrame >= 100 && m_AnimStatus.animFrame <= 250)
	{
		m_pGMain->m_pBackForeProc->StartSwing(Random(0, 360));
	}

	// �I������
	if (m_AnimStatus.isEnd(EnemyBossConstruct::ANIM_KICK) || m_AnimStatus.isEnd(EnemyBossConstruct::ANIM_KICK_LEFT) ||
		m_AnimStatus.isEnd(EnemyBossConstruct::ANIM_STOMP) || m_AnimStatus.isEnd(EnemyBossConstruct::ANIM_STOMP_LEFT))
	{
		m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_IDLE);
		m_bIsMove = TRUE;
	}
	if (m_AnimStatus.isEnd(EnemyBossConstruct::ANIM_ROAR))
	{
		m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_IDLE);
		m_bIsMove = TRUE;
		m_nRoarCoolTime = 60 * EnemyBossConstruct::ROAR_COOLTIME;

	}

	// �{�f�B�v���X�̏I������
	if (m_AnimStatus.isEnd(EnemyBossConstruct::ANIM_BODYSTOMP))
	{
		m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_STANDUP);
	}

	// �����オ��̏I������
	if (m_AnimStatus.isEnd(EnemyBossConstruct::ANIM_STANDUP))
	{
		m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_IDLE);
		m_bIsMove = TRUE;
	}
}

//-----------------------------------------------------------------------------
// �G�{�X�I�u�W�F�N�g�̐���
//
// �G���_���[�W��Ԃ̂Ƃ��̏���
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CEnmBossObj::UpdateDamage()
{
	const int nFlashTime = 5;  // ���G��Ԃ̎���

	if (m_pHitObj->GetTag() != PC) {
		m_pGMain->m_pEffectProc->m_pBillProc->Start(GetPositionVector(m_mWorld) + VECTOR3(0.0f, 1.5f, 0.0f));
		m_nHp -= m_pHitObj->GetAtc();	// �U�����󂯂��_���[�W
		if (m_nHp <= 0)
		{
			m_nHp = 0;
			m_pGMain->m_pBackForeProc->StartSwing(Random(0, 360));
			m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_DIE);
			m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetDeadBoss(TRUE);
			m_dwStatus = DEAD;		// HP���O�Ȃ̂Ŏ��S��
		}
		else {
			m_nCnt1 = nFlashTime;   // ���G��Ԃ̎���
			m_dwStatus = FLASH;     // �_���[�W����̕��A�������s��
			UpdateAttack();
		}
	}
	else {
		m_dwStatus = NORMAL;
	}
}

//-----------------------------------------------------------------------------
// �G�{�X�I�u�W�F�N�g�̐���
//
// �G�����S��Ԃ̂Ƃ��̏���
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CEnmBossObj::UpdateDead()
{
	if (m_AnimStatus.isEnd(EnemyBossConstruct::ANIM_DIE))
	{
		m_pGMain->m_pBackForeProc->SetBlackOut(TRUE);
		m_AnimStatus.playAnim = false;
		m_AnimStatus.animFrame = m_AnimStatus.endFrame - 1; // �t���[���G���h�̈��O�ɂ���

	}
	
	if (m_pGMain->m_pBackForeProc->GetWhiteOut()) {
		// �N���A��ʂֈڍs
		m_pGMain->m_dwGameStatus = GAMECLEAR;
		m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetDeadBoss(FALSE);
		m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetCtrl(CAMERA_GAMECLEAR);
		m_pGMain->m_pBossBgm->Stop();
		m_pGMain->m_pGameClear->Play(AUDIO_LOOP);
		m_bActive = FALSE;
	}
}

//============================================================================
// �A�N�Z�X�֐�
//============================================================================
void CEnmBossObj::SetMovieFinish(bool flag)
{
	m_bIsMovieFinish = flag;
}

BOOL CEnmBossObj::GetMovieFinish()
{
	return m_bIsMovieFinish;
}
