//=============================================================================
//		�G�L�����N�^�[�̏���
//																Enemy.cpp
//=============================================================================
#include  "Enemy.h"
#include  "Playchar.h"
#include  "Ally.h"
#include  "Weapon.h"
#include  "Map.h"
#include  "Effect.h"
#include  "BackFore.h"
#include  "SearchRoute.h"

//======================================================================
// �G�@�萔�N���X
//======================================================================
float const EnemyConstruct::SPEED = 0.08f;
float const EnemyConstruct::SWORD_RANGE = 1.5f;
VECTOR3 const EnemyConstruct::HITBOX_MIN = VECTOR3(-0.3f, 0.0f, -0.3f);
VECTOR3 const EnemyConstruct::HITBOX_MAX = VECTOR3(0.3f, 1.5f, 0.3f);

//============================================================================
// �G�v���V�[�W���N���X
//============================================================================
// ---------------------------------------------------------------------------
// �G�v���V�[�W���̃R���X�g���N�^
//
//   �Ȃ��A�I�u�W�F�N�g��delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
// ---------------------------------------------------------------------------
CEnmProc::CEnmProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_dwProcID = ENM_RW_ID;                  // �G�q�v��ID
	m_nNum = 0;

	m_bIsStartFlag = TRUE;

	// Fbx�X�L�����b�V���̐ݒ�
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Char/Enemy1/Enemy1_Skin.mesh"));
	m_pMesh->LoadAnimation(_T("Data/Char/Enemy1/Idle.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Enemy1/Run.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Enemy1/Sword.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Enemy1/Arrow.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Enemy1/Hit.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Enemy1/Anim_Dive.anmx"), eRootAnimXZ);
	m_pMesh->LoadAnimation(_T("Data/Char/Enemy1/Anim_LookAround.anmx"));
	m_pMesh->m_fHeightMax = 0.01f;
	m_pMesh->m_iMaxDevide = 8;
	m_HoldItem.m_nHoldObjNo = 9;	// ��Ɏ�����̃I�u�W�F�ԍ�
	m_HoldItem.m_nPosMesh = 0;		// �����ʒu�̃��b�V���ԍ�
	m_HoldItem.m_nPosBone = 15;	// �����ʒu�̃{�[���ԍ�
	m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.24f, 0.04f);  // ��Ɏ�����̈ʒu�I�t�Z�b�g
	m_HoldItem.m_nOffsetRot = VECTOR3(00.0f, 0.0f, 0.0f);  // ��Ɏ�����̊p�x�I�t�Z�b�g

	for (int i = 0; i < ENM_RW_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmObj(this));  // m_pObjArray�ɃI�u�W�F�N�g�𐶐�����
	}
}

// ---------------------------------------------------------------------------
// �G�v���V�[�W���̃f�X�g���N�^
// ---------------------------------------------------------------------------
CEnmProc::~CEnmProc()
{
}

// ---------------------------------------------------------------------------
// �G�̍ő�X�|�[�����̐ݒ�
// ---------------------------------------------------------------------------
void CEnmProc::SetMaxNum()
{
	m_nNum = m_pGMain->m_pMapProc->GetNavMap().size() * m_pGMain->m_pMapProc->GetHierarchy();
}

// ---------------------------------------------------------------------------
// �������������l�Ƀ��Z�b�g����
// �i�ő唭����(m_nMaxNum)�𔭐���m_nNum�ɐݒ肷��
//
// �߂�l  �Ȃ�
// ---------------------------------------------------------------------------
void CEnmProc::ResetEnmNum()
{
	m_nNum = m_pGMain->m_pMapProc->GetNavMap().size() * m_pGMain->m_pMapProc->GetHierarchy();
}

// ---------------------------------------------------------------------------
// ���݂̃A�N�e�B�u�̃I�u�W�F�N�g���v��
// ---------------------------------------------------------------------------
void CEnmProc::ResetStartFlag()
{
	m_bIsStartFlag = TRUE;
}

// ---------------------------------------------------------------------------
// ���݂̃A�N�e�B�u�̃I�u�W�F�N�g���v��
// ---------------------------------------------------------------------------
int CEnmProc::CountActiveEnm()
{
	return m_nNum;
}

// ---------------------------------------------------------------------------
// �G�v���V�[�W���̎蓮��������
// ---------------------------------------------------------------------------
BOOL CEnmProc::Start()
{
	for (DWORD i = 0; i < m_pObjArray.size(); i++) {
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start();
			return TRUE;
		}
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// �G�v���V�[�W���̎蓮��������
// �͈͓��Ƀ����_�����X�|�[��
// ---------------------------------------------------------------------------
BOOL CEnmProc::Start(VECTOR3 vMin, VECTOR3 vMax, int nNum)
{
	// �i�r�Q�[�V�����}�b�v�̐���
	NAVIGATIONMAP nav;
	nav.vMax = vMax;
	nav.vMin = vMin;

	int i = 0, j = 0;
	// �G�𐶐�
	for (; i < nNum; i++)
	{
		for (; j < m_pObjArray.size(); j++)
		{
			if (!m_pObjArray[j]->GetActive())
			{
				((CEnmObj*)m_pObjArray[j])->SetNav(nav);  // �I�΂ꂽ�i�r�}�b�v���I�u�W�F�N�g�ɓo�^����
				m_pObjArray[j]->Start();
				break;
			}
		}
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// �G�v���V�[�W���̎�����������
// ---------------------------------------------------------------------------
void CEnmProc::Update()
{
	if (m_bIsStartFlag) {
		// �܂��X�|�[�������Ă��Ȃ�
		m_bIsStartFlag = FALSE;
		int spownNum = m_pGMain->m_pMapProc->GetHierarchy();
		SetMaxNum();
		// �X�|�[���n�_���擾
		for (int i = 0; i < m_pGMain->m_pMapProc->GetNavMap().size(); i++) {
			for (int j = 0; j < spownNum; j++) {
				for (int k = 0; k < m_pObjArray.size(); k++) {
					if (!m_pObjArray[k]->GetActive())
					{
						((CEnmObj*)m_pObjArray[k])->SetNav(m_pGMain->m_pMapProc->GetNavMap()[i]);  // �I�΂ꂽ�i�r�}�b�v���I�u�W�F�N�g�ɓo�^����
						m_pObjArray[k]->Start();	// �G�𔭐�������
						break;
					}
				}
			}
		}
	}
}

// ---------------------------------------------------------------------------
// �G�v���V�[�W���̓����蔻�菈��
// ---------------------------------------------------------------------------
void CEnmProc::UpdateHitcheck()
{
	std::vector<CBaseObj*> stac;
	std::vector<CBaseObj*> hit;
	m_pGMain->m_pMapProc->GetPcSpPart()->AllHitcheck(0, m_pGMain->m_pMapProc->GetEnmSpPart()->GetSpArray(), stac, hit);

	int hitNum = hit.size() / 2;
	for (int i = 0; i < hitNum; i++) {
		hit[i]->Hitcheck(hit[i + 1], hit[i + 1]->GetBBox());
	}
}

//============================================================================
// �G�I�u�W�F�N�g�N���X
//============================================================================
// ---------------------------------------------------------------------------
// �G�I�u�W�F�N�g�̃R���X�g���N�^
//
//  ����
// ---------------------------------------------------------------------------
CEnmObj::CEnmObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nMaxHP = 100;      // �̗�
	const int nAtc = 1;      // �̂̍U����

	m_bActive = FALSE;
	m_dwStatus = NORMAL;

	m_vTarget = GetPositionVector(m_mWorld);

	// �o�E���f�B���O�{�b�N�X�̐ݒ�
	m_pBBox = new CBBox(m_pGMain->m_pShader, EnemyConstruct::HITBOX_MIN, EnemyConstruct::HITBOX_MAX);

	m_nHp = m_nMaxHp = nMaxHP;
	m_nAtc = nAtc;
	m_nTag = ENM;

	// �o�H�T���}�b�v�̐ݒ�
	m_pSearchRoute = new CSearchRoute(m_pGMain);

	m_HoldItem.m_nHoldObjNo = 0;	// ��Ɏ�����̃I�u�W�F�ԍ�
	m_HoldItem.m_nPosMesh = 0;		// �����ʒu�̃��b�V���ԍ�
	m_HoldItem.m_nPosBone = 15;	// �����ʒu�̃{�[���ԍ�
	m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.24f, 0.04f);  // ��Ɏ�����̈ʒu�I�t�Z�b�g
	m_HoldItem.m_nOffsetRot = VECTOR3(00.0f, 0.0f, 0.0f);  // ��Ɏ�����̊p�x�I�t�Z�b�g
}
// ---------------------------------------------------------------------------
// �G�I�u�W�F�N�g�̃f�X�g���N�^
//
//   �Ȃ��ACBBox��delete��CBaseObj�̃f�X�g���N�^�ōs�����ߕs�v
// ---------------------------------------------------------------------------
CEnmObj::~CEnmObj()
{
	SAFE_DELETE(m_pSearchRoute);
}

// ---------------------------------------------------------------------------
// �G�I�u�W�F�N�g�̔���
// ---------------------------------------------------------------------------
BOOL CEnmObj::Start()
{
	m_bActive = TRUE;	// �G�q�v�I�u�W�F�N�g
	m_dwStatus = NORMAL;
	m_bIsMove = true;
	// �i�r�Q�[�V�����}�b�v�͈͓̔��Ń����_���Ȉʒu�ɃX�|�[������
	m_mWorld = XMMatrixTranslation((float)Random(m_Nav.vMin.x, m_Nav.vMax.x),
									m_Nav.vMax.y, 
									(float)Random(m_Nav.vMin.z, m_Nav.vMax.z));
	// �����Ă�������𔽓]
	m_mWorld = XMMatrixRotationY(XMConvertToRadians(180.0f)) * m_mWorld;
	m_mWorldOld = m_mWorld;

	m_nHp = m_nMaxHp;
	m_nCnt2 = 60 * 2;

	// �o�H�T���z��𐶐�����
	m_pSearchRoute->MakeANode();
	m_vTarget = GetPositionVector(m_mWorld);
	// �o�H�T���z�񂩂�o�H�}�̍쐬����
	VECTOR3 vStart = VECTOR3(m_vTarget.x, 0, m_vTarget.z);   // �J�n�ʒu

	VECTOR3 vEnd = GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());    // �ړI�n

	// ������̏�����
	m_HoldItem.m_nHoldObjNo = 0;	// ��Ɏ�����̃I�u�W�F�ԍ�
	m_HoldItem.m_nPosMesh = 0;		// �����ʒu�̃��b�V���ԍ�
	m_HoldItem.m_nPosBone = 15;	// �����ʒu�̃{�[���ԍ�
	m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.24f, 0.04f);  // ��Ɏ�����̈ʒu�I�t�Z�b�g
	m_HoldItem.m_nOffsetRot = VECTOR3(00.0f, 0.0f, 0.0f);  // ��Ɏ�����̊p�x�I�t�Z�b�g

	m_pGMain->m_pBackForeProc->AddDeadEnemyNum(1);
	return TRUE;
}

//-----------------------------------------------------------------------------
// �G�I�u�W�F�N�g�̐���
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CEnmObj::Update()
{
	if (!m_bActive)
	{
		return;
	}

	// �ϐ��̏�����   ---------------------------------------
	m_vPosUp.x = m_vPosUp.y = m_vPosUp.z = 0.0f;
	m_vRotUp.x = m_vRotUp.y = m_vRotUp.z = 0.0f;
	m_mWorldOld = m_mWorld;  // ��O�̈ʒu

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

	// ���̓G�Əd�Ȃ�Ȃ��悤�ɂ��鏈��  --------------------
	m_pGMain->m_pMapProc->GetEnmSpPart()->SetKeepOffset(this, GetPositionVector(m_mWorld));

	// �}�b�v�R���W�����Ǝ��R����  --------------------------
	m_fJumpY += (0.5f * GRAVITY * 0.00005f) * m_fJumpTime;	// ���R����
	m_fJumpTime += 1.0f;
	if (m_pGMain->m_pMapProc->isCollisionMoveGravity(&m_mWorld, m_mWorldOld, m_pBBox->m_vMax.x, ENM) != 3)  // �}�b�v�R���W����
	{
		m_fJumpY = 0.0f;  // ���R�����̒�~
		m_fJumpTime = 0.0f;
	}
	else {
		m_dwStatus = DEAD;
	}


	// �I�u�W�F�N�g����ԕ����֓o�^
	VECTOR3 vMin, vMax;
	m_pGMain->m_pMapProc->GetEnmSpPart()->MakeAABB(
		GetPositionVector(m_mWorldOld), GetPositionVector(m_mWorld), 2.0f, vMin, vMax);
	m_pGMain->m_pMapProc->GetEnmSpPart()->SetObj(vMin, vMax, this);

	// �`�揈��  ---------------------------------------------
	// �X�L�����b�V�������_�����O 
	//GetMesh()->RenderToon(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, normalize(VECTOR3(0.8f, 1, -1)), m_pGMain->m_vEyePt);
	GetMesh()->Render(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
	//GetMesh()->RenderDisplace(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);

	// �o�E���f�B���O�{�b�N�X
	m_pBBox->m_mWorld = m_mWorld;
	//   �o�E���f�B���O�{�b�N�X�����_�����O
	//m_pBBox->Render(m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);

	MATRIX4X4 mHold;
	mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, m_HoldItem.m_nPosBone, m_HoldItem.m_nPosMesh);  // ��̈ʒu�̃{�[���}�g���b�N�X
	m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->Disp(mHold, m_HoldItem); // �A�C�e���\��

	// �G�̓���̃X�e�[�^�X�\��  -----------------------------
	VECTOR3 vEnmPos = GetPositionVector(m_mWorld);
	vEnmPos.y += 2.0f;
	float fHp = (float)m_nHp / m_nMaxHp;
	// �g�o�o�[�̕\��
	m_pGMain->m_pBackForeProc->GetSprite()->Draw3D(m_pGMain->m_pBackForeProc->GetImage(), vEnmPos, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vEyePt, VECTOR2(1.2f * fHp, 0.2f), VECTOR2(320, 96), VECTOR2(125 * fHp, 16));
	VECTOR3 vPc = GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());
	float dis = GetDistance(vPc);
	if (dis <= 20)
	{
		m_pGMain->m_pBackForeProc->DrawArrowAngle(m_mWorld);
	}
}

//-----------------------------------------------------------------------------
// �G�I�u�W�F�N�g�̐���
//
// �G���ʏ��Ԃ̂Ƃ��̏���
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CEnmObj::UpdateNormal()
{
	CPcObj* pPcObj = m_pGMain->m_pPcProc->GetPcObjPtr();
	MATRIX4X4 mTemp;
	BOOL isTargetAreaOut = FALSE;
	float fForward = EnemyConstruct::SPEED;

	// �G���_���[�W���畜�A�����Ƃ��̏���  ------------------------------
	if (m_dwStatus == FLASH)
	{
		m_nCnt1--;
		if (m_nCnt1 <= 0) m_dwStatus = NORMAL;
	}

	if (!TargetMove(m_vTarget, fForward, 180.0f))	// �ړI�n�iTarget�j�ɒB�������ǂ���
	{
		if (m_AnimStatus.animNum != EnemyConstruct::ANIM_WALK && m_AnimStatus.animNum != EnemyConstruct::ANIM_SWORD && m_AnimStatus.animNum != EnemyConstruct::ANIM_ARROW) {
			// �A�j���[�V�������ړ��A�U���ȊO�ł���Έړ��A�j���[�V�������Z�b�g
			m_AnimStatus.SetNum(EnemyConstruct::ANIM_WALK);
		}
		if (m_bIsMove) {
			mTemp = XMMatrixRotationY(XMConvertToRadians(m_vRotUp.y));					// �ړI�n�iTarget�j�֌������p�x
			m_mWorld = mTemp * m_mWorld;
			mTemp = XMMatrixTranslation(m_vPosUp.x, m_vPosUp.y + m_fJumpY, m_vPosUp.z);	// �ړI�n�iTarget�j�֌�����
			m_mWorld = mTemp * m_mWorld;
		}
	}
	if (m_AnimStatus.animNum != EnemyConstruct::ANIM_WALK && m_AnimStatus.animNum != EnemyConstruct::ANIM_SWORD && m_AnimStatus.animNum != EnemyConstruct::ANIM_ARROW) {
		m_AnimStatus.SetNum(EnemyConstruct::ANIM_IDLE);
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
}

//-----------------------------------------------------------------------------
// �G�I�u�W�F�N�g�̐���
//
// �U������̏���
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void CEnmObj::UpdateAttack()
{
	VECTOR3 pcPos = GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());
	VECTOR3 Hit = VECTOR3(0.0f, 0.0f, 0.0f);
	VECTOR3 Normal = VECTOR3(0.0f, 0.0f, 0.0f);

	// �|�U���ւ̑J�ڏ���
	if (m_nCnt2 <= 0 && m_AnimStatus.animNum != EnemyConstruct::ANIM_ARROW && !m_pGMain->m_pMapProc->Hitcheck(pcPos, GetPositionVector(m_mWorld), &Hit, &Normal))
	{
		// �v���C���[�̕�������
		float y = GetTargetRotateVector(m_mWorld, pcPos).y;
		// �����_���Ƀv���C���[�̕��p����u����
		int recoil = Random(-EnemyConstruct::RECOIL, EnemyConstruct::RECOIL);
		MATRIX4X4 mTemp = XMMatrixRotationY(XMConvertToRadians(y + recoil));
		m_mWorld = mTemp * m_mWorld;
		m_AnimStatus.SetNum(EnemyConstruct::ANIM_ARROW);
		SetHoldItem(ATTACK_ALLOW);
		m_bIsMove = false;
	}

	// ���U��
	if (m_nCnt2 <= 0 && m_AnimStatus.animNum != EnemyConstruct::ANIM_SWORD && GetDistance(pcPos) <= EnemyConstruct::SWORD_RANGE) {
		float y = GetTargetRotateVector(m_mWorld, pcPos).y;
		MATRIX4X4 mTemp = XMMatrixRotationY(XMConvertToRadians(y));
		m_mWorld = mTemp * m_mWorld;
		m_AnimStatus.SetNum(EnemyConstruct::ANIM_SWORD);
		SetHoldItem(ATTACK_SWORD);
		m_bIsMove = false;
	}

	// �|�A�j���[�V�����̏I�����A��𔭎�
	if (m_AnimStatus.isEnd(EnemyConstruct::ANIM_ARROW)) {
		m_pGMain->m_pWeaponProc->m_pWeaponArrowProc->Start(m_mWorld, ENM); // ���[�U�[����
		m_AnimStatus.SetNum(EnemyConstruct::ANIM_IDLE);
		SetHoldItem(ATTACK_NONE);
		m_nCnt2 = 60 * EnemyConstruct::COOLTIME;
		m_bIsMove = true;
	}
	
	// ���U��
	if (m_AnimStatus.animNum == EnemyConstruct::ANIM_SWORD)
	{
		MATRIX4X4 mSword, mHold;
		VECTOR3 vMin, vMax;

		mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, GetHoldItem().m_nPosBone, GetHoldItem().m_nPosMesh);  // ��̈ʒu�̃{�[���}�g���b�N�X
		m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->GetInfo(mHold, GetHoldItem(), mSword, vMin, vMax); // �{�[���}�g���b�N�X���w�肵�Ď�Ɏ��A�C�e�����𓾂�
		m_pGMain->m_pWeaponProc->m_pWeaponSwordProc->Start(mSword, vMin, vMax, ENM);  // ���U���̊J�n
		// ���A�j���[�V�����I�����A������Ԃɖ߂�
		if (m_AnimStatus.isEnd(2))
		{
			m_AnimStatus.SetNum(0);
			SetHoldItem(ATTACK_NONE);
			m_nCnt2 = 60 * EnemyConstruct::COOLTIME;
			m_bIsMove = TRUE;
		}
	}

	if (m_nCnt2 > 0) {
		m_nCnt2--;
	}
}

//-----------------------------------------------------------------------------
// �G�I�u�W�F�N�g�̐���
//
// �G���_���[�W��Ԃ̂Ƃ��̏���
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CEnmObj::UpdateDamage()
{
	MATRIX4X4 mTemp;
	const int nFlashTime = 5;  // ���G��Ԃ̎���

	if (m_pHitObj->GetTag() != PC && m_pHitObj->GetTag() != ALLY) {
		m_pGMain->m_pEffectProc->m_pBillProc->Start(GetPositionVector(m_mWorld) + VECTOR3(0.0f, 1.5f, 0.0f));
		mTemp = XMMatrixTranslation(0, 0, -0.5f);	// �o�b�N����
		m_mWorld = mTemp * m_mWorld;
		m_nHp -= m_pHitObj->GetAtc();	// �U�����󂯂��_���[�W
		if (m_nHp <= 0)
		{
			m_nHp = 0;
			m_dwStatus = DEAD;		// HP���O�Ȃ̂Ŏ��S��
		}
		else {
			m_nCnt1 = nFlashTime;   // ���G��Ԃ̎���
			m_dwStatus = FLASH;     // �_���[�W����̕��A�������s��
		}
	}
	else {
		m_dwStatus = NORMAL;
	}
}

//-----------------------------------------------------------------------------
// �G�I�u�W�F�N�g�̐���
//
// �G�����S��Ԃ̂Ƃ��̏���
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CEnmObj::UpdateDead()
{
	// �����I�u�W�F�N�g��100�̖����ł���΁A���ݒn�_�ɐ���
	if (m_pGMain->m_pPcProc->GetPcObjPtr()->GetAllyNum() < 100) {
		m_pGMain->m_pBackForeProc->AddAllAllyNum(1);
		m_pGMain->m_pAllyProc->Start(GetPositionVector(m_mWorld), FALSE);
		m_pGMain->m_pEffectProc->m_pParticleProc->Start(GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld()) + VECTOR3(0.0f, 1.5f, 0.0f), VECTOR3(0.0f, 1.0f, 0.0f));

	}
	// UI��U��
	m_pGMain->m_pBackForeProc->StartSwing(Random(0, 360));
	m_pGMain->m_pEnmProc->AddNum(-1);
	m_bActive = FALSE;
}

// ---------------------------------------------------------------------------
// �G�̕��펝���ւ��֐�
// ---------------------------------------------------------------------------
void CEnmObj::SetHoldItem(DWORD weaponId)
{
	// �e���킲�ƂɑJ��
	switch (weaponId) {
	case ATTACK_ALLOW:
		m_HoldItem.m_nHoldObjNo = 8;	// ��Ɏ�����̃I�u�W�F�ԍ�(�����Ȍ�)
		m_HoldItem.m_nPosMesh = 0;		// ���̍����ʒu�̃��b�V���ԍ�
		m_HoldItem.m_nPosBone = 17;	// ���̍����ʒu�̃{�[���ԍ�
		m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.18f, -0.03f);  // ��Ɏ�����̈ʒu�I�t�Z�b�g
		m_HoldItem.m_nOffsetRot = VECTOR3(68.0f, 230.0f, 0.0f);  // ��Ɏ�����̊p�x�I�t�Z�b�g
		break;

	case ATTACK_SWORD:
		m_HoldItem.m_nHoldObjNo = 9;	// ��Ɏ�����̃I�u�W�F�ԍ�
		m_HoldItem.m_nPosMesh = 0;		// �����ʒu�̃��b�V���ԍ�
		m_HoldItem.m_nPosBone = 15;	// �����ʒu�̃{�[���ԍ�
		m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.24f, 0.04f);  // ��Ɏ�����̈ʒu�I�t�Z�b�g
		m_HoldItem.m_nOffsetRot = VECTOR3(00.0f, 0.0f, 0.0f);  // ��Ɏ�����̊p�x�I�t�Z�b�g
		break;

	default:
		m_HoldItem.m_nHoldObjNo = 0;	// ��Ɏ�����̃I�u�W�F�ԍ�
		m_HoldItem.m_nPosMesh = 0;		// �����ʒu�̃��b�V���ԍ�
		m_HoldItem.m_nPosBone = 15;	// �����ʒu�̃{�[���ԍ�
		m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.24f, 0.04f);  // ��Ɏ�����̈ʒu�I�t�Z�b�g
		m_HoldItem.m_nOffsetRot = VECTOR3(00.0f, 0.0f, 0.0f);  // ��Ɏ�����̊p�x�I�t�Z�b�g
	}
}

//============================================================================
// �A�N�Z�X�֐�
//============================================================================
void CEnmObj::SetNav(NAVIGATIONMAP  Nav) 
{ 
	m_Nav = Nav; 
}