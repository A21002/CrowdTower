//=============================================================================
//		����̏���
//=============================================================================
#include  "Enemy.h"
#include  "Playchar.h"
#include  "Ally.h"
#include  "Weapon.h"
#include  "Map.h"
#include  "Effect.h"
#include  "BackFore.h"

//=============================================================================================================================
// ���탁�C���v���V�[�W���̃R���X�g���N�^
//
//   �Ȃ��A�v���V�[�W����delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
// ----------------------------------------------------------------------------------------------------------------------------
CWeaponProc::CWeaponProc(CGameMain* pGMain) : CBaseProc(pGMain)
{

	m_pWeaponArrowProc = new CWeaponArrowProc(pGMain);		// ����E�|��v���V�[�W��
	m_pProcArray.push_back(m_pWeaponArrowProc);				// �v���V�[�W�����v���V�[�W���z��ɓo�^����

	m_pWeaponItemBoxProc = new CWeaponItemBoxProc(pGMain);	// �A�C�e���E�󔠃v���V�[�W��
	m_pProcArray.push_back(m_pWeaponItemBoxProc);				// �v���V�[�W�����v���V�[�W���z��ɓo�^����

	m_pWeaponSwordProc = new CWeaponSwordProc(pGMain);		// ����E���v���V�[�W��
	m_pProcArray.push_back(m_pWeaponSwordProc);				// �v���V�[�W�����v���V�[�W���z��ɓo�^����

	m_pWeaponShotSlashProc = new CWeaponShotSlashProc(pGMain);
	m_pProcArray.push_back(m_pWeaponShotSlashProc);

	m_pWeaponGunProc = new CWeaponGunProc(pGMain);					// ����E�e�v���V�[�W��
	m_pProcArray.push_back(m_pWeaponGunProc);						// �v���V�[�W�����v���V�[�W���z��ɓo�^����

	m_pWeaponAxeProc = new CWeaponAxeProc(pGMain);					// ����E���v���V�[�W��
	m_pProcArray.push_back(m_pWeaponAxeProc);						// �v���V�[�W�����v���V�[�W���z��ɓo�^����

	m_pWeaponAxeShotProc = new CWeaponAxeShotProc(pGMain);			// ����E�����v���V�[�W��
	m_pProcArray.push_back(m_pWeaponAxeShotProc);					// �v���V�[�W�����v���V�[�W���z��ɓo�^����

	m_pWeaponDamageAreaProc = new CWeaponDamageAreaProc(pGMain);	// ����E�_���[�W�G���A�v���V�[�W��
	m_pProcArray.push_back(m_pWeaponDamageAreaProc);				// �v���V�[�W�����v���V�[�W���z��ɓo�^����

	m_pWeaponAttackBoxProc = new CWeaponAttackBoxProc(pGMain);		// ����E�U���͈̓v���V�[�W��
	m_pProcArray.push_back(m_pWeaponAttackBoxProc);					// �v���V�[�W�����v���V�[�W���z��ɓo�^����

	m_pWeaponHoldProc = new CWeaponHoldProc(pGMain);				// ����E��Ɏ��A�C�e���̃v���V�[�W��
	m_pProcArray.push_back(m_pWeaponHoldProc);						// �v���V�[�W�����v���V�[�W���z��ɓo�^����
}

// ================================================================================================================================
// �萔��`
// ================================================================================================================================
const float WeaponConstruct::ARROW_SPEED = 0.5f;
const VECTOR3 WeaponConstruct::ARROW_START_DIFF = VECTOR3(0.0f, 0.3f, 1.0f);
const VECTOR3 WeaponConstruct::ARROW_HITBOX_MIN = VECTOR3(-0.1f, -0.1f, -0.4f);
const VECTOR3 WeaponConstruct::ARROW_HITBOX_MAX = VECTOR3(0.1f, 0.1f, 0.4f);

const VECTOR3 WeaponConstruct::BOX_HITBOX_MIN = VECTOR3(-1.7f, 0.0f, 18.9f);
const VECTOR3 WeaponConstruct::BOX_HITBOX_MAX = VECTOR3(1.7f, 2.2f, 21.1f);

const VECTOR3 WeaponConstruct::SHOTSLASH_HITBOX_MIN = VECTOR3(-0.5f, 0.0f, -0.1f);
const VECTOR3 WeaponConstruct::SHOTSLASH_HITBOX_MAX = VECTOR3(0.5f, 0.5f, 0.1f);
const float WeaponConstruct::SHOTSLASH_SPEED = 0.4f;

const float WeaponConstruct::GUN_SPEED = 0.5f;
const VECTOR3 WeaponConstruct::GUN_HITBOX_MIN = VECTOR3(-0.05f, -0.05f, -0.05f);
const VECTOR3 WeaponConstruct::GUN_HITBOX_MAX = VECTOR3(0.05f, 0.05f, 0.05f);

const VECTOR3 WeaponConstruct::SHOTAXE_HITBOX_MIN = VECTOR3(-0.4f, -0.6f, -0.2f);
const VECTOR3 WeaponConstruct::SHOTAXE_HITBOX_MAX = VECTOR3(0.4f, 0.6f, 0.2f);

const float WeaponConstruct::SHOTAXE_SPEED = 0.3f;
const float WeaponConstruct::SHOTAXE_ROTSPEED = 3.0f;

// ================================================================================================================================
// �e����v���V�[�W�����̏���
// ================================================================================================================================
// ========================================================================================
// �|��v���V�[�W���N���X
// ========================================================================================
// ---------------------------------------------------------------------------
// ����E�|��v���V�[�W���̃R���X�g���N�^
//
//   �Ȃ��A�I�u�W�F�N�g��delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
// ---------------------------------------------------------------------------
CWeaponArrowProc::CWeaponArrowProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// �|��b�V���̐ݒ�
	m_pMesh = new CFbxMesh( m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Arrow/Allow_Test.mesh"));

	for (int i = 0; i < ARROW_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponArrowObj(this));	// m_pObjArray�ɕ���E�|��I�u�W�F�N�g�𐶐�����
	}
}

// ---------------------------------------------------------------------------
// ����E�|��v���V�[�W���̃f�X�g���N�^
// ---------------------------------------------------------------------------
CWeaponArrowProc::~CWeaponArrowProc()
{
	;
}

// ---------------------------------------------------------------------------
// ����E�|��v���V�[�W���̔���
//
//  MATRIX4X4  mWorld       �F�|��̔��ˈʒu�̃}�g���b�N�X
//  DWORD       dwOwner      �F����𔭎˂����L�����敪�iPC:PC(�ȗ��l)�@ENM:�G�j
// ---------------------------------------------------------------------------
BOOL CWeaponArrowProc::Start(MATRIX4X4 mWorld, DWORD dwOwner)
{
	for (int i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(mWorld, dwOwner);	       // ����E�|��I�u�W�F�N�g�̔���
			return TRUE;
		}
	}
	return FALSE;
}

// ========================================================================================
// �|��I�u�W�F�N�g�N���X
// ========================================================================================
// ---------------------------------------------------------------------------
// ����E�|��I�u�W�F�N�g�̃R���X�g���N�^
// ---------------------------------------------------------------------------
CWeaponArrowObj::CWeaponArrowObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	m_bActive = FALSE;
	m_dwStatus = NORMAL;

	m_nAtc = WeaponConstruct::ARROW_ATTACK;
	m_dwOwner = 0;
	m_nReflectionCount = 0;
	m_nHitCount = 0;

	// �����蔻��
	m_pBBox = new CBBox(m_pGMain->m_pShader, WeaponConstruct::ARROW_HITBOX_MIN, WeaponConstruct::ARROW_HITBOX_MAX);
}

// ---------------------------------------------------------------------------
// ����E�|��I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CWeaponArrowObj::~CWeaponArrowObj()
{
}

// ---------------------------------------------------------------------------
// ����E�|��I�u�W�F�N�g�̔���
//
//     MATRIX4X4 mStartWorld     �|��̔��ˈʒu�̃}�g���b�N�X
//     DWORD      dwOwner         ����𔭎˂����L�����敪�iPC:PC(�ȗ��l)�@ENM:�G�j
//
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
BOOL CWeaponArrowObj::Start(MATRIX4X4 mStartWorld, DWORD dwOwner)
{
	if (m_bActive) return FALSE;

	m_bActive = TRUE;
	ResetStatus();
	m_mWorld = XMMatrixTranslationFromVector(WeaponConstruct::ARROW_START_DIFF) * mStartWorld;
	m_vPosUp = VECTOR3(0, 0, WeaponConstruct::ARROW_SPEED);
	m_nTag = WEAPON;
	m_dwOwner = dwOwner;

	// �\�͂��Q��
	if (dwOwner == PC)
	{
		m_nReflectionCount = m_pGMain->m_pPcProc->GetAbility()->GetAbilityLevel(1);
		m_nHitCount = m_pGMain->m_pPcProc->GetAbility()->GetAbilityLevel(2);
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
// ����E�|��I�u�W�F�N�g�̍X�V
//
//     �����@�@�@�Ȃ�
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
void CWeaponArrowObj::Update()
{
	if (m_bActive)
	{
		// �ړ�����
		m_mWorldOld = m_mWorld;
		m_mWorld = XMMatrixTranslationFromVector(m_vPosUp) * m_mWorld;
		m_pBBox->m_mWorld = m_mWorld;

		// �}�b�v�Ƃ̂����蔻��
		VECTOR3 vHit, vNormal;
		if (m_pGMain->m_pMapProc->Hitcheck(GetPositionVector(m_mWorld), GetPositionVector(m_mWorldOld), &vHit, &vNormal))
		{
			if (m_nReflectionCount > 0) {
				// ���˂���
				m_nReflectionCount--;
				if (vNormal.x == 1.0f) {
					// |  �̕ǂ̏ꍇ
					float y = GetRotateVector(m_mWorld).y;
					if (y < 0) {
						y *= -1;
					}
					float temp = y * 2.0f;
					MATRIX4X4 rot = XMMatrixRotationY(XMConvertToRadians(temp));
					m_mWorld = rot * m_mWorld;
				}

				else if (vNormal.z == -1.0f) {
					// - �̕ǂ̏ꍇ
					float y = GetRotateVector(m_mWorld).y;
					float temp = 180.0f - y * 2.0f;
					MATRIX4X4 rot = XMMatrixRotationY(XMConvertToRadians(temp));
					m_mWorld = rot * m_mWorld;
				}

				else if (vNormal.x == -1.0f) {
					//  |�@�̕ǂ̏ꍇ
					float y = GetRotateVector(m_mWorld).y;
					if (y < 0) {
						y *= -1;
					}
					float temp = y * 2.0f * -1.0f;
					MATRIX4X4 rot = XMMatrixRotationY(XMConvertToRadians(temp));
					m_mWorld = rot * m_mWorld;
				}

				else if (vNormal.z == 1.0f) {
					// _ �̕ǂ̏ꍇ
					float y = GetRotateVector(m_mWorld).y;
					float temp = 180.0f - y * 2.0f;
					MATRIX4X4 rot = XMMatrixRotationY(XMConvertToRadians(temp));
					m_mWorld = rot * m_mWorld;
				}
				else {
					m_bActive = FALSE;	// ����
				}
					
			}
			else {
				// ���˂����ɍ폜����
				m_bActive = FALSE;	// ����
			}
				
		}

		// �}�b�v�͈͊O�ɂ���I�u�W�F�N�g�͓����蔻����s��Ȃ�
		VECTOR3 pos = GetPositionVector(m_mWorld);
		if ((pos.x > -30.0f && pos.x < 30.0f) && (pos.z > -30.0f && pos.z < 30.0f)) {
			// �G��o�b�Ƃ̂����蔻��
			if ((m_dwOwner != ENM && (m_pGMain->m_pMapProc->GetEnmSpPart()->HitcheckSpPart(this, GetPositionVector(m_mWorld))))
				|| (m_dwOwner != PC && (m_pGMain->m_pMapProc->GetPcSpPart()->HitcheckSpPart(this, GetPositionVector(m_mWorld)))))
			{
				if (m_nHitCount <= 0) {
					m_bActive = FALSE;
				}
				else {
					m_dwStatus = NORMAL;
					m_nHitCount--;
				}
			}
		}
		// �����_�����O
		Render();
	}
}

// ---------------------------------------------------------------------------
// ����E�|��I�u�W�F�N�g�̃����_�����O
//
//   �A�e�����Ȃ������_�����O�@���̕�����(0,0,0)
//
//     �����@�@�@�Ȃ�
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
void CWeaponArrowObj::Render()
{
	GetMesh()->Render(m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, VECTOR3(0, 0, 0), m_pGMain->m_vEyePt);
}

// ========================================================================================
// �󔠃v���V�[�W���N���X
// ========================================================================================
//------------------------------------------------------------------------
// �A�C�e���E�󔠃v���V�[�W���̃R���X�g���N�^
//
//   �Ȃ��A�I�u�W�F�N�g��delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
// ---------------------------------------------------------------------------
CWeaponItemBoxProc::CWeaponItemBoxProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// �󔠃��b�V���̐ݒ�
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Map/MapItem/box/box.mesh"));

	for (int i = 0; i < ITEMBOX_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponItemBoxObj(this));	// m_pObjArray�ɃA�C�e���E�󔠃I�u�W�F�N�g�𐶐�����
	}
}

//------------------------------------------------------------------------
// �A�C�e���E�󔠃v���V�[�W���̃f�X�g���N�^
// ---------------------------------------------------------------------------
CWeaponItemBoxProc::~CWeaponItemBoxProc()
{
	;
}

// ---------------------------------------------------------------------------
// �A�C�e���E�󔠃v���V�[�W���̔���
//
//  VECTOR3 vPos	       �F�����ʒu
// ---------------------------------------------------------------------------
BOOL CWeaponItemBoxProc::Start(VECTOR3 vPos)
{
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(vPos);	// �A�C�e���E�󔠃I�u�W�F�N�g
			return TRUE;
		}
	}
	return FALSE;
}

// ========================================================================================
// �󔠃I�u�W�F�N�g�N���X
// ========================================================================================
//------------------------------------------------------------------------
// �A�C�e���E�󔠃I�u�W�F�N�g�̃R���X�g���N�^
// ---------------------------------------------------------------------------
CWeaponItemBoxObj::CWeaponItemBoxObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	// �����蔻��
	m_pBBox = new CBBox(m_pGMain->m_pShader, WeaponConstruct::BOX_HITBOX_MIN, WeaponConstruct::BOX_HITBOX_MAX);

	m_bActive = FALSE;
	m_dwStatus = NORMAL;

	m_nAtc = 0;
}

// ---------------------------------------------------------------------------
// �A�C�e���E�󔠃I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CWeaponItemBoxObj::~CWeaponItemBoxObj()
{
	;
}

// ---------------------------------------------------------------------------
// �A�C�e���E�󔠃I�u�W�F�N�g�̔���
//
//     VECTOR3 vPos		       �F�����ʒu
//
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
BOOL CWeaponItemBoxObj::Start(VECTOR3 vPos)
{
	m_mWorld = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
	m_mWorldOld = m_mWorld;
	m_bActive = TRUE;
	ResetStatus();
	return TRUE;
}

// ---------------------------------------------------------------------------
// �A�C�e���E�󔠃I�u�W�F�N�g�̍X�V
//
//     �����@�@�@�Ȃ�
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
void CWeaponItemBoxObj::Update()
{
	if (m_bActive)
	{
		// �G��o�b�Ƃ̂����蔻��
		if (m_pGMain->m_pPcProc->Hitcheck(this, m_pBBox))
		{
			// AbilityUI�̕\��
			if (m_pGMain->m_pMapProc->GetHierarchy() == 20) {
				m_pGMain->m_dwGameStatus = GAMECLEAR;
			}
			else {
				m_pGMain->m_pBackForeProc->m_nDialogKind = DIALOG_CHECK;
				m_pGMain->m_pBackForeProc->SetSelectAbility();
				m_pGMain->m_isMove = false;
				m_bActive = FALSE;
			}
			
		}
		// �����_�����O
		Render();
	}
}

// ---------------------------------------------------------------------------
// �A�C�e���E�󔠃I�u�W�F�N�g�̃����_�����O
//
//   �A�e�����Ȃ������_�����O�@���̕�����(0,0,0)
//
//     �����@�@�@�Ȃ�
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
void CWeaponItemBoxObj::Render()
{
	GetMesh()->Render(m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, VECTOR3(0, 0, 0), m_pGMain->m_vEyePt);        // -- 2022.2.16
}

// ========================================================================================
// ���v���V�[�W���N���X
// ========================================================================================
// ---------------------------------------------------------------------------
// ����E���v���V�[�W���̃R���X�g���N�^
//
//   �Ȃ��A�I�u�W�F�N�g��delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
// ---------------------------------------------------------------------------
CWeaponSwordProc::CWeaponSwordProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (int i = 0; i < SWORD_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponSwordObj(this));	// m_pObjArray�ɕ���E���I�u�W�F�N�g�𐶐�����
	}
}

// ---------------------------------------------------------------------------
// ���팕�E�v���V�[�W���̔���
// ---------------------------------------------------------------------------
BOOL CWeaponSwordProc::Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner)
{
	for (int i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(mMat, vMin, vMax, dwOwner);	       // ����E���I�u�W�F�N�g�̔���
			return TRUE;
		}
	}
	return FALSE;
}

// ========================================================================================
// ���I�u�W�F�N�g�N���X
// ========================================================================================
// ---------------------------------------------------------------------------
// ����E���I�u�W�F�N�g�̃R���X�g���N�^
// ---------------------------------------------------------------------------
CWeaponSwordObj::CWeaponSwordObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nAtc = WeaponConstruct::SWORD_ATTACK;    // ���̍U����

	m_bActive = FALSE;
	m_dwStatus = NORMAL;

	// BBox�̐ݒ�(�傫���́A���ɐݒ�)
	m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.1f, -0.1f, -0.1f), VECTOR3(0.1f, 0.1f, 0.1f));
	m_nAtc = nAtc;
	m_dwOwner = 0;
}

// ---------------------------------------------------------------------------
// ����E���I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CWeaponSwordObj::~CWeaponSwordObj()
{
	;
}

// ---------------------------------------------------------------------------
// ����E���I�u�W�F�N�g�̔����Ə����A�`��
//
//     MATRIX4X4  mMat        �F���̈ʒu
//     VECTOR3 vMin        �F�o�E���f�B���O�{�b�N�X�̍ŏ��l
//     VECTOR3 vMax        �F�o�E���f�B���O�{�b�N�X�̍ő�l
//     DWORD       dwOwner     �F����𔭎˂����L�����敪�iPC:PC(�ȗ��l)�@ENM:�G�j
//
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
BOOL CWeaponSwordObj::Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner)
{
	m_bActive = TRUE;  // �X�^�[�g���ɕ\���J�n
	ResetStatus();
	// �o�E���f�B���O�{�b�N�X�̃T�C�Y���ς���Ă�������ւ���
	if (m_pBBox->m_vMin.x != vMin.x || m_pBBox->m_vMin.y != vMin.y || m_pBBox->m_vMin.z != vMin.z
		||
		m_pBBox->m_vMax.x != vMax.x || m_pBBox->m_vMax.y != vMax.y || m_pBBox->m_vMax.z != vMax.z)
	{
		m_pBBox->InitBBox(vMin, vMax);   // �o�E���f�B���O�{�b�N�X�̑傫���ύX
	}
	m_pBBox->m_mWorld = mMat;   // �o�E���f�B���O�{�b�N�X�̃��[���h�}�g���b�N�X���Z�b�g

	m_nTag = WEAPON;
	m_dwOwner = dwOwner;

	return TRUE;
}
// ---------------------------------------------------------------------------
// ����E���I�u�W�F�N�g�̍X�V
//
//     �����@�@�@�Ȃ�
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
void CWeaponSwordObj::Update()
{
	if (m_bActive) {
		// �G��o�b�Ƃ̂����蔻��
		if ((m_dwOwner != ENM && (m_pGMain->m_pMapProc->GetEnmSpPart()->HitcheckSpPart(this, GetPositionVector(m_pBBox->m_mWorld))))
			|| (m_dwOwner != PC && (m_pGMain->m_pMapProc->GetPcSpPart()->HitcheckSpPart(this, GetPositionVector(m_pBBox->m_mWorld))))) {
			m_bActive = FALSE;
		}
		m_bActive = FALSE;  // �P�񏈗�������A����
	}
}

// ========================================================================================
// �a���v���V�[�W���N���X
// ========================================================================================
// ---------------------------------------------------------------------------
// ����E�a���v���V�[�W���̃R���X�g���N�^
//
//   �Ȃ��A�I�u�W�F�N�g��delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
// ---------------------------------------------------------------------------
CWeaponShotSlashProc::CWeaponShotSlashProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (int i = 0; i < SWORDSHOT_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponShotSlashObj(this));	// m_pObjArray�ɕ���E�a���I�u�W�F�N�g�𐶐�����
	}
}

// ---------------------------------------------------------------------------
// �a���v���V�[�W���̃f�X�g���N�^
// ---------------------------------------------------------------------------
CWeaponShotSlashProc::~CWeaponShotSlashProc()
{
	;
}

// ---------------------------------------------------------------------------
// ����E�a���v���V�[�W���̔���
//
//  MATRIX4X4  mWorld       �F���[�U�[�̔��ˈʒu�̃}�g���b�N�X
//  DWORD       dwOwner      �F����𔭎˂����L�����敪�iPC:PC(�ȗ��l)�@ENM:�G�j
// ---------------------------------------------------------------------------
BOOL CWeaponShotSlashProc::Start(MATRIX4X4 mWorld, DWORD dwOwner)
{
	for (int i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(mWorld, dwOwner);	       // ����E�a���I�u�W�F�N�g�̔���
			return TRUE;
		}
	}
	return FALSE;
}

// ========================================================================================
// �a���I�u�W�F�N�g�N���X
// ========================================================================================
// ---------------------------------------------------------------------------
// ����E�a���I�u�W�F�N�g�̃R���X�g���N�^
// ---------------------------------------------------------------------------
CWeaponShotSlashObj::CWeaponShotSlashObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nAtc = WeaponConstruct::SHOTSLASH_ATTACK;				// ���[�U�[�̍U����

	m_bActive = FALSE;
	m_dwStatus = NORMAL;

	// BBox�̐ݒ�(�傫���́A���ɐݒ�)
	m_pBBox = new CBBox(m_pGMain->m_pShader, WeaponConstruct::SHOTSLASH_HITBOX_MIN, WeaponConstruct::SHOTSLASH_HITBOX_MAX);
	m_pEffect = NULL;
	m_fLength = 0.0f;

	m_nAtc = nAtc;
}

// ---------------------------------------------------------------------------
// ����E�a���I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CWeaponShotSlashObj::~CWeaponShotSlashObj()
{
	;
}

// ---------------------------------------------------------------------------
// ����E�a���I�u�W�F�N�g�̔���
//
//     MATRIX4X4 mStartWorld     ���[�U�[�̔��ˈʒu�̃}�g���b�N�X
//     DWORD      dwOwner         ����𔭎˂����L�����敪�iPC:PC(�ȗ��l)�@ENM:�G�j
//
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
BOOL CWeaponShotSlashObj::Start(MATRIX4X4 mStartWorld, DWORD dwOwner)
{
	m_fLength = WeaponConstruct::SHOTSLASH_LENGTH * m_pGMain->m_pPcProc->GetAbility()->GetAbilityLevel(4);
	if (m_bActive) return FALSE;

	m_bActive = TRUE;
	ResetStatus();
	m_mWorld = XMMatrixTranslation(0.0f, 0.3f, 1.0f) * mStartWorld;
	m_vPosUp = VECTOR3(0, 0, WeaponConstruct::SHOTSLASH_SPEED);
	m_pEffect = m_pGMain->m_pEffectProc->m_pBillSlashShotProc->Start(m_mWorld);
	m_nTag = WEAPON;

	return TRUE;
}

// ---------------------------------------------------------------------------
// ����E�a���I�u�W�F�N�g�̍X�V
//
//     �����@�@�@�Ȃ�
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
void CWeaponShotSlashObj::Update()
{
	if (m_bActive)
	{
		m_bActive = m_pEffect->GetActive();
		m_mWorldOld = m_mWorld;
		m_mWorld = XMMatrixTranslationFromVector(m_vPosUp) * m_mWorld;
		m_pBBox->m_mWorld = m_mWorld;

		m_fLength -= m_vPosUp.z;
		if (m_fLength <= 0.0f) {
			m_dwStatus = DAMAGE;
		}

		switch (m_dwStatus)
		{
		case DAMAGE:
			m_bActive = FALSE;
			m_pEffect->SetActive(FALSE);
			return;
		}
		
		// �G�Ƃ̂����蔻��
		if (m_pGMain->m_pMapProc->GetEnmSpPart()->HitcheckSpPart(this, GetPositionVector(m_mWorld)))
		{
			m_bActive = FALSE;	// ����
			m_pEffect->SetActive(FALSE);
		}
		
		// �}�b�v�Ƃ̂����蔻��
		VECTOR3 vHit, vNormal;
		if (m_pGMain->m_pMapProc->Hitcheck(GetPositionVector(m_mWorld), GetPositionVector(m_mWorldOld), &vHit, &vNormal))
		{
			m_bActive = FALSE;	// ����
			m_pEffect->SetActive(FALSE);
		}
		
		m_pEffect->SetWorld(m_mWorld);
	}
}

// ========================================================================================
// �e�v���V�[�W���N���X
// ========================================================================================
// ---------------------------------------------------------------------------
// ����E�e�v���V�[�W���̃R���X�g���N�^
//
//   �Ȃ��A�I�u�W�F�N�g��delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
// ---------------------------------------------------------------------------
CWeaponGunProc::CWeaponGunProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// �e�ۃ��b�V���̐ݒ�
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Mark.mesh"));

	for (int i = 0; i < GUN_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponGunObj(this));	// m_pObjArray�ɕ���E�e�I�u�W�F�N�g�𐶐�����
	}
}

//============================================================================
// ����E�e�v���V�[�W���̃f�X�g���N�^
// ---------------------------------------------------------------------------
CWeaponGunProc::~CWeaponGunProc()
{
	;
}

// ---------------------------------------------------------------------------
// ����E�e�v���V�[�W���̔���
//
//  VECTOR3 vStart       �F���ˈʒu
//  DWORD       dwOwner      �F����𔭎˂����L�����敪�iPC:PC(�ȗ��l)�@ENM:�G�j
// ---------------------------------------------------------------------------
BOOL CWeaponGunProc::Start(MATRIX4X4 mWorld, DWORD dwOwner)
{
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(mWorld, dwOwner);	// ����E�e�I�u�W�F�N�g
			return TRUE;
		}
	}
	return FALSE;
}

// ========================================================================================
// �e�I�u�W�F�N�g�N���X
// ========================================================================================
// ---------------------------------------------------------------------------
// ����E�e�I�u�W�F�N�g�̃R���X�g���N�^
// ---------------------------------------------------------------------------
CWeaponGunObj::CWeaponGunObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nAtc = WeaponConstruct::GUN_ATTACK;  // �U����

	m_bActive = FALSE;
	m_dwStatus = NORMAL;
	m_nAtc = nAtc;

	// Box�̐ݒ�
	m_pBBox = new CBBox(m_pGMain->m_pShader, WeaponConstruct::GUN_HITBOX_MIN, WeaponConstruct::GUN_HITBOX_MAX);
}

// ---------------------------------------------------------------------------
// ����E�e�I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CWeaponGunObj::~CWeaponGunObj()
{
	;
}

// ---------------------------------------------------------------------------
// ����E�e�I�u�W�F�N�g�̔���
//
//     MATRIX4X4 mWorld     �F�e�ۂ̔��ˈʒu
//     DWORD   dwOwner      �F����𔭎˂����L�����敪�iPC:PC(�ȗ��l)�@ENM:�G�j
//
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
BOOL CWeaponGunObj::Start(MATRIX4X4 mWorld, DWORD dwOwner)
{
	m_bActive = TRUE;
	m_mWorld = mWorld;
	m_mWorldOld = m_mWorld;
	m_nAtc = 50 + m_pGMain->m_pPcProc->GetAbility()->GetAbilityLevel(0) * 20;

	m_nTag = WEAPON;
	m_vPosUp = VECTOR3(0, 0, 0.5f);

	return TRUE;
}

// ---------------------------------------------------------------------------
// ����E�e�I�u�W�F�N�g�̍X�V
//
//     �����@�@�@�Ȃ�
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
void CWeaponGunObj::Update()
{

	if (m_bActive)
	{
		m_mWorldOld = m_mWorld;
		m_pBBox->m_mWorld = m_mWorld;

		MATRIX4X4 mTran;
		mTran = XMMatrixTranslation(m_vPosUp.x, m_vPosUp.y, m_vPosUp.z);
		m_mWorld = mTran * m_mWorld;

		// �G��o�b�Ƃ̂����蔻��
		if (m_pGMain->m_pMapProc->GetEnmSpPart()->HitcheckSpPart(this, GetPositionVector(m_mWorld)))
		{
			m_bActive = FALSE;		
		}
		// �}�b�v�Ƃ̂����蔻��
		VECTOR3 vHit, vNormal;
		if (m_pGMain->m_pMapProc->Hitcheck(GetPositionVector(m_mWorld), GetPositionVector(m_mWorldOld), &vHit, &vNormal))
		{
			m_bActive = FALSE;
		}
		// �����_�����O
		Render();
	}
}

// ---------------------------------------------------------------------------
// ����E�e�I�u�W�F�N�g�̃����_�����O
//
//   �A�e�����Ȃ������_�����O�@���̕�����(0,0,0)
//
//     �����@�@�@�Ȃ�
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
void CWeaponGunObj::Render()
{
	// �e�̃����_�����O
	GetMesh()->Render(m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, VECTOR3(0, 0, 0), m_pGMain->m_vEyePt);   
}

// ========================================================================================
// ���v���V�[�W���N���X
// ========================================================================================
// ---------------------------------------------------------------------------
// ����E���v���V�[�W���̃R���X�g���N�^
//
//   �Ȃ��A�I�u�W�F�N�g��delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
// ---------------------------------------------------------------------------
CWeaponAxeProc::CWeaponAxeProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (int i = 0; i < AXE_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponAxeObj(this));	// m_pObjArray�ɕ���E���I�u�W�F�N�g�𐶐�����
	}
}

// ---------------------------------------------------------------------------
// ����E���v���V�[�W���̃f�X�g���N�^
// ---------------------------------------------------------------------------
CWeaponAxeProc::~CWeaponAxeProc()
{
	;
}

// ---------------------------------------------------------------------------
// ����E���v���V�[�W���̔���
//
//  MATRIX4X4  mGun         �F�e�̈ʒu�}�g���b�N�X
//  VECTOR3    vOffset      �F�ʒu�̔�����
//  MATRIX4X4  mOwnerWorld  �F���˂���L�����̃}�g���b�N�X
//  DWORD      dwOwner      �F����𔭎˂����L�����敪�iPC:PC(�ȗ��l)�@ENM:�G�j
// ---------------------------------------------------------------------------
BOOL CWeaponAxeProc::Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner)
{
	for (int i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(mMat, vMin, vMax, dwOwner);	       // ����E���I�u�W�F�N�g�̔���
			return TRUE;
		}
	}
	return FALSE;
}

// ========================================================================================
// ���I�u�W�F�N�g�N���X
// ========================================================================================
// ---------------------------------------------------------------------------
// ����E���I�u�W�F�N�g�̃R���X�g���N�^
// ---------------------------------------------------------------------------
CWeaponAxeObj::CWeaponAxeObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nAtc = WeaponConstruct::AXE_ATTACK;    // ���̍U����

	m_bActive = FALSE;
	m_dwStatus = NORMAL;

	// BBox�̐ݒ�(�傫���́A���ɐݒ�)
	m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.1f, -0.1f, -0.1f), VECTOR3(0.1f, 0.1f, 0.1f));
	m_nAtc = nAtc;
}

// ---------------------------------------------------------------------------
// ����E���I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CWeaponAxeObj::~CWeaponAxeObj()
{
	;
}

// ---------------------------------------------------------------------------
// ����E���I�u�W�F�N�g�̔���
//
//     MATRIX4X4 mStartWorld     ���̔��ˈʒu�̃}�g���b�N�X
//     DWORD      dwOwner        ����𔭎˂����L�����敪�iPC:PC(�ȗ��l)�@ENM:�G�j
//
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
BOOL CWeaponAxeObj::Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner)
{
	m_bActive = TRUE;  // �X�^�[�g���ɕ\���J�n
	ResetStatus();
	// �\�͒l�ɉ����ē����蔻����g��
	m_pBBox->InitBBox(vMin, vMax + VECTOR3(0.0f, 0.0f, m_pGMain->m_pPcProc->GetAbility()->GetAbilityLevel(5)));
	m_pBBox->m_mWorld = mMat;   // �o�E���f�B���O�{�b�N�X�̃��[���h�}�g���b�N�X���Z�b�g

	m_nTag = WEAPON;

	return TRUE;
}

// ---------------------------------------------------------------------------
// ����E���I�u�W�F�N�g�̍X�V
//
//     �����@�@�@�Ȃ�
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
void CWeaponAxeObj::Update()
{

	if (m_bActive) {
		// �G��o�b�Ƃ̂����蔻��
		if (m_pGMain->m_pMapProc->GetEnmSpPart()->HitcheckSpPart(this, GetPositionVector(m_pBBox->m_mWorld)))
		{
			m_bActive = FALSE;
		}
		m_bActive = FALSE;  // �P�񏈗�������A����
	}
}

// ========================================================================================
// �������v���V�[�W���N���X
// ========================================================================================
// ---------------------------------------------------------------------------
// ����E�������v���V�[�W���̃R���X�g���N�^
//
//   �Ȃ��A�I�u�W�F�N�g��delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
// ---------------------------------------------------------------------------
CWeaponAxeShotProc::CWeaponAxeShotProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Axe/Axe.mesh"));

	for (int i = 0; i < AXESHOT_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponAxeShotObj(this));	// m_pObjArray�ɕ���E�������I�u�W�F�N�g�𐶐�����
	}
}

// ---------------------------------------------------------------------------
// ����E�������v���V�[�W���̃f�X�g���N�^
// ---------------------------------------------------------------------------
CWeaponAxeShotProc::~CWeaponAxeShotProc()
{
}

// ---------------------------------------------------------------------------
// ����E�������v���V�[�W���̔���
//
//  MATRIX4X4  mGun         �F�e�̈ʒu�}�g���b�N�X�B�}�g���b�N�X�̈ʒu�̂ݎg�p����
//  VECTOR3    vOffset      �F�ʒu�̔�����
//  MATRIX4X4  mOwnerWorld  �F���˂���L�����̃}�g���b�N�X�B�}�g���b�N�X�̊p�x�̂ݎg�p����
//  DWORD      dwOwner      �F����𔭎˂����L�����敪�iPC:PC(�ȗ��l)�@ENM:�G�j
// ---------------------------------------------------------------------------
BOOL CWeaponAxeShotProc::Start(MATRIX4X4 mGun, DWORD dwLength, DWORD dwOwner)
{
	for (int i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(mGun, dwLength, dwOwner);	       // ����E�X�v���[�I�u�W�F�N�g�̔���
			break;
		}
	}
	return FALSE;
}

// ========================================================================================
// �������I�u�W�F�N�g�N���X
// ========================================================================================
// ---------------------------------------------------------------------------
// ����E�������I�u�W�F�N�g�̃R���X�g���N�^
// ---------------------------------------------------------------------------
CWeaponAxeShotObj::CWeaponAxeShotObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nAtc = WeaponConstruct::SHOTAXE_ATTACK;  // �U����

	m_pEffect = NULL;

	m_bActive = FALSE;
	m_bBackFlag = FALSE;

	m_dwStatus = NORMAL;
	m_nAtc = nAtc;
	m_dwOwner = 0;
	m_fMoveRot = 0.0f;
	m_fLength = 0.0f;
	m_fAnimRot = 0.0f;

	m_pBBox = new CBBox(m_pGMain->m_pShader, WeaponConstruct::SHOTAXE_HITBOX_MIN, WeaponConstruct::SHOTAXE_HITBOX_MAX);
}

// ---------------------------------------------------------------------------
// ����E�������I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CWeaponAxeShotObj::~CWeaponAxeShotObj()
{
}

// ---------------------------------------------------------------------------
// ����E�������I�u�W�F�N�g�̔���
//
//     MATRIX4X4 mStartWorld     �X�v���[�̔��ˈʒu�̃}�g���b�N�X
//     DWORD      dwOwner        ����𔭎˂����L�����敪�iPC:PC(�ȗ��l)�@ENM:�G�j
//
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
BOOL CWeaponAxeShotObj::Start(MATRIX4X4 mStartWorld, DWORD dwLength, DWORD dwOwner)
{
	m_bActive = TRUE;
	m_mWorld = mStartWorld;
	m_mWorldOld = mStartWorld;
	m_fLength = dwLength;
	m_fMoveRot = GetRotateVector(m_mWorld).y;
	m_fAnimRot = 0.0f;

	// �\�͒l�ɉ����ē����蔻����g��
	m_pBBox->InitBBox(WeaponConstruct::SHOTAXE_HITBOX_MIN, WeaponConstruct::SHOTAXE_HITBOX_MAX + VECTOR3(0.0f, 0.0f, m_pGMain->m_pPcProc->GetAbility()->GetAbilityLevel(5)));

	m_vPosUp = VECTOR3(0, 0, WeaponConstruct::SHOTAXE_SPEED + WeaponConstruct::SHOTAXE_SPEED * (0.3 * m_pGMain->m_pPcProc->GetAbility()->GetAbilityLevel(3)));
	m_vRotUp = VECTOR3(0.0f, WeaponConstruct::SHOTAXE_ROTSPEED, 0.0f);
	m_dwOwner = dwOwner;
	m_pEffect = m_pGMain->m_pEffectProc->m_pBillAxeShotProc->Start(m_mWorld);
	m_bBackFlag = FALSE;
	m_dwStatus = NORMAL;
	m_nTag = WEAPON;
	return TRUE;
}

// ---------------------------------------------------------------------------
//
// ����E�������I�u�W�F�N�g�̍X�V
//
//     �����@�@�@�Ȃ�
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
void CWeaponAxeShotObj::Update()
{
	if (m_bActive)
	{
		m_mWorldOld = m_mWorld;
		MATRIX4X4 mTran;
		VECTOR3 vPos = GetPositionVector(m_mWorld);
		m_mWorld = XMMatrixRotationY(XMConvertToRadians(m_fMoveRot)) * XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
		if (!m_bBackFlag)
		{
			mTran = XMMatrixTranslation(m_vPosUp.x, m_vPosUp.y, m_vPosUp.z);
			m_fLength = m_fLength - m_vPosUp.z;
			if (m_fLength <= 0) {
				m_bBackFlag = TRUE;
			}
			m_mWorld = mTran * m_mWorld;
		}
		else {
			// �茳�ɖ߂��Ă���
			m_vPosUp.z = WeaponConstruct::SHOTAXE_SPEED;
			VECTOR3 m_vTarget = GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());
			if (!TargetMove(m_vTarget, m_vPosUp.z, 180.0f))	// �ړI�n�iTarget�j�ɒB�������ǂ���
			{
				MATRIX4X4 mTemp = XMMatrixRotationY(XMConvertToRadians(m_vRotUp.y));					// �ړI�n�iTarget�j�֌������p�x
				m_mWorld = mTemp * m_mWorld;
				mTemp = XMMatrixTranslation(m_vPosUp.x, m_vPosUp.y + m_fJumpY, m_vPosUp.z);	// �ړI�n�iTarget�j�֌�����
				m_mWorld = mTemp * m_mWorld;
			}
			else {
				// �߂����ꍇ
				m_pEffect->SetActive(FALSE);
				m_bActive = FALSE;
				m_pGMain->m_pPcProc->GetPcObjPtr()->SetIsHaveAxe(TRUE);
			}
		}
		m_pBBox->m_mWorld = m_mWorld;
		m_pEffect->SetWorld(m_mWorld);

		// �}�b�v�Ƃ̂����蔻��
		VECTOR3 vHit, vNormal;
		if (m_pGMain->m_pMapProc->Hitcheck(GetPositionVector(m_mWorld), GetPositionVector(m_mWorldOld), &vHit, &vNormal))
		{
			m_bBackFlag = TRUE;
		}

		// �G��PC�Ƃ̓����蔻��
		if ((m_dwOwner != ENM && (m_pGMain->m_pMapProc->GetEnmSpPart()->HitcheckSpPart(this, GetPositionVector(m_pBBox->m_mWorld))))
			|| (m_dwOwner != PC && (m_pGMain->m_pMapProc->GetPcSpPart()->HitcheckSpPart(this, GetPositionVector(m_pBBox->m_mWorld)))))
		{
			// �q�b�g�����Ƃ�
			m_dwStatus = NORMAL;
		}

		// ��]�A�j���[�V����
		m_mWorld = XMMatrixRotationX(XMConvertToRadians(90.0f)) * m_mWorld;
		m_mWorld = XMMatrixRotationZ(XMConvertToRadians(m_fAnimRot)) * m_mWorld;
		m_fAnimRot += 60.0f;
		if (360.0f <= m_fAnimRot)
		{
			m_fAnimRot = 0.0f;
		}

		// �����_�����O
		Render();
	}
}

// ---------------------------------------------------------------------------
//
// ����E�������I�u�W�F�N�g�̃����_�����O
//
//
//     �����@�@�@�Ȃ�
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
void CWeaponAxeShotObj::Render()
{
	GetMesh()->Render(m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, VECTOR3(0, 0, 0), m_pGMain->m_vEyePt);
}

// ========================================================================================
// �_���[�W�G���A�v���V�[�W���N���X
// ========================================================================================
// ---------------------------------------------------------------------------
// ����E�_���[�W�G���A�v���V�[�W���̃R���X�g���N�^
//
//   �Ȃ��A�I�u�W�F�N�g��delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
// ---------------------------------------------------------------------------
CWeaponDamageAreaProc::CWeaponDamageAreaProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_dwDamageTime = 60;
	m_dwKind = 0;
	m_fAlfa = 0.0f;
	m_bIsInObject = FALSE;
	for (int i = 0; i < DAMAGEAREA_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponDamageAreaObj(this));	// m_pObjArray�ɕ���E�_���[�W�G���A�I�u�W�F�N�g�𐶐�����
	}
}

// ---------------------------------------------------------------------------
// ����E�_���[�W�G���A�v���V�[�W���̃f�X�g���N�^
// ---------------------------------------------------------------------------
CWeaponDamageAreaProc::~CWeaponDamageAreaProc()
{
}

// ---------------------------------------------------------------------------
// ����E�_���[�W�G���A�v���V�[�W���̔���
//
//  VECTOR3	   min          �FBox�R���W�����̍ŏ��ʒu
//  VECTOR3    max          �FBox�R���W�����̍ő�ʒu
//  DWORD      kind         �F�_���[�W�G���A�̎��(POISION, RAVA�Ȃ�)
// ---------------------------------------------------------------------------
BOOL CWeaponDamageAreaProc::Start(VECTOR3 min, VECTOR3 max, DWORD Kind)
{
	m_dwKind = Kind;
	for (int i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(min, max, Kind);	       // ����E�_���[�W�G���A�I�u�W�F�N�g�̊J�n
			break;
		}
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// ����E�_���[�W�G���A�v���V�[�W���̍X�V����
// ---------------------------------------------------------------------------
void CWeaponDamageAreaProc::Update()
{
	if (m_bIsInObject)
	{
		m_dwDamageTime--;
		if (m_dwDamageTime <= 0) {
			m_pGMain->m_pAllyProc->DeleteObj(1);
			m_dwDamageTime = 60;
		}

		if (m_fAlfa < 0.5f)
		{
			m_fAlfa += 0.008f;
		}
	}
	else {
		if (m_fAlfa <= 0.0f)
		{
			m_fAlfa = 0.0f;
		}
		else {
			m_fAlfa -= 0.016f;
		}
	}

	if (m_fAlfa >= 0.0f)
	{
		// ��ʂɐF���d�˂�
		if (m_dwKind == POISON)
		{
			// ��
			m_pGMain->m_pBackForeProc->GetSprite()->DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(218, 66, 220), m_fAlfa);
		}
		else if (m_dwKind == LAVA)
		{
			m_pGMain->m_pBackForeProc->GetSprite()->DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(255, 0, 0), m_fAlfa);
		}
	}
	m_bIsInObject = FALSE;
}

// ========================================================================================
// �A�N�Z�X�֐�
// ========================================================================================
void CWeaponDamageAreaProc::SetIsInObject(BOOL flag)
{
	m_bIsInObject = flag;
}

// ========================================================================================
// �_���[�W�G���A�I�u�W�F�N�g�N���X
// ========================================================================================
// ---------------------------------------------------------------------------
// ����E�_���[�W�G���A�I�u�W�F�N�g�̃R���X�g���N�^
// ---------------------------------------------------------------------------
CWeaponDamageAreaObj::CWeaponDamageAreaObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nAtc = 0;  // �U����
	m_nAtc = nAtc;
	m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-1, -1, -1), VECTOR3(1, 1, 1));
}

// ---------------------------------------------------------------------------
// ����E�_���[�W�G���A�I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CWeaponDamageAreaObj::~CWeaponDamageAreaObj()
{
}

// ---------------------------------------------------------------------------
// ����E�_���[�W�G���A�I�u�W�F�N�g�̔���
//
//     MATRIX4X4 mStartWorld     �X�v���[�̔��ˈʒu�̃}�g���b�N�X
//     DWORD      dwOwner        ����𔭎˂����L�����敪�iPC:PC(�ȗ��l)�@ENM:�G�j
//
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
BOOL CWeaponDamageAreaObj::Start(VECTOR3 vStart, VECTOR3 vTarget, DWORD dwOwner)
{
	m_bActive = TRUE;
	m_pBBox->InitBBox(vStart, vTarget);
	m_dwStatus = NORMAL;
	return TRUE;
}

// ---------------------------------------------------------------------------
//
// ����E�_���[�W�G���A�I�u�W�F�N�g�̍X�V
//
//     �����@�@�@�Ȃ�
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
void CWeaponDamageAreaObj::Update()
{
	if (m_bActive)
	{
		m_dwStatus = NORMAL;
		VECTOR3 vHit = VECTOR3(0.0f, 0.0f, 0.0f), vNrm = VECTOR3(0.0f, 1.0f, 0.0f);
		if (m_pBBox->OBBCollisionDetection(m_pGMain->m_pPcProc->GetPcObjPtr()->GetBBox(), &vHit, &vNrm) &&
			m_pGMain->m_pPcProc->GetPcObjPtr()->GetStatus() == NORMAL)
		{
			m_pGMain->m_pWeaponProc->m_pWeaponDamageAreaProc->SetIsInObject(TRUE);

		}
	}
}

// ========================================================================================
// �U���͈̓v���V�[�W���N���X
// ========================================================================================
// ---------------------------------------------------------------------------
// ����E�U���͈̓v���V�[�W���̃R���X�g���N�^
//
//   �Ȃ��A�I�u�W�F�N�g��delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
// ---------------------------------------------------------------------------
CWeaponAttackBoxProc::CWeaponAttackBoxProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (int i = 0; i < ATTACKBOX_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponAttackBoxObj(this));	// m_pObjArray�ɕ���E�U���͈̓I�u�W�F�N�g�𐶐�����
	}
}

// ---------------------------------------------------------------------------
// ����E�U���͈̓v���V�[�W���̃f�X�g���N�^
// ---------------------------------------------------------------------------
CWeaponAttackBoxProc::~CWeaponAttackBoxProc()
{
}

// ---------------------------------------------------------------------------
// ����E�U���͈̓v���V�[�W���̔���
//
//  VECTOR3	   min          �FBox�R���W�����̍ŏ��ʒu
//  VECTOR3    max          �FBox�R���W�����̍ő�ʒu
//  DWORD      kind         �F�_���[�W�G���A�̎��(POISION, RAVA�Ȃ�)
// ---------------------------------------------------------------------------
BOOL CWeaponAttackBoxProc::Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner)
{
	for (int i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(mMat, vMin, vMax, dwOwner);	       // ����E�U���͈̓I�u�W�F�N�g�̊J�n
			break;
		}
	}
	return FALSE;
}

// ========================================================================================
// �U���͈̓I�u�W�F�N�g�N���X
// ========================================================================================
// ---------------------------------------------------------------------------
// ����E�U���͈̓I�u�W�F�N�g�̃R���X�g���N�^
// ---------------------------------------------------------------------------
CWeaponAttackBoxObj::CWeaponAttackBoxObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nAtc = 100;  // �U����
	m_nAtc = nAtc;
	m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-1, -1, -1), VECTOR3(1, 1, 1));
}

// ---------------------------------------------------------------------------
// ����E�U���͈̓I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CWeaponAttackBoxObj::~CWeaponAttackBoxObj()
{
}

// ---------------------------------------------------------------------------
// ����E�U���͈̓I�u�W�F�N�g�̔���
//
//     MATRIX4X4 mStartWorld     �X�v���[�̔��ˈʒu�̃}�g���b�N�X
//     DWORD      dwOwner        ����𔭎˂����L�����敪�iPC:PC(�ȗ��l)�@ENM:�G�j
//
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
BOOL CWeaponAttackBoxObj::Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner)
{
	m_bActive = TRUE;
	m_pBBox->m_mWorld = mMat;
	m_pBBox->InitBBox(vMin, vMax);
	m_dwStatus = NORMAL;
	m_pGMain->m_pPcProc->Hitcheck(this, m_pBBox);
	m_pGMain->m_pAllyProc->Hitcheck(this, m_pBBox);

	Render();
	m_bActive = FALSE;
	return TRUE;
}

// ---------------------------------------------------------------------------
//
// ����E�U���͈̓I�u�W�F�N�g�̃����_�����O
//
//
//     �����@�@�@�Ȃ�
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
void CWeaponAttackBoxObj::Render()
{
	// m_pBBox->Render(m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
}


// ============================================================================================================================
//============================================================================
//
// ����E��Ɏ��A�C�e���v���V�[�W���̃R���X�g���N�^
//
// �E���ڍU�����镐��ł͂Ȃ��A�����\��������A�ʒu����Ԃ����肷��
//
//   ���b�V���̓v���V�[�W���ł͂Ȃ��A�I�u�W�F�N�g�œǂݍ���
//   �Ȃ��A�I�u�W�F�N�g��delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
//
// ---------------------------------------------------------------------------
CWeaponHoldProc::CWeaponHoldProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (int i = 0; i < HOLDITEM_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponHoldObj(this));	// m_pObjArray�ɃI�u�W�F�N�g�𐶐�����
		((CWeaponHoldObj*)m_pObjArray.back())->Init();   // �I�u�W�F�N�g�̏�����
	}
}
// ---------------------------------------------------------------------------
//
// ����E��Ɏ��A�C�e���v���V�[�W���̃f�X�g���N�^
//
// ---------------------------------------------------------------------------
CWeaponHoldProc::~CWeaponHoldProc()
{
	;
}

// ---------------------------------------------------------------------------
//
// ����E��Ɏ��A�C�e���v���V�[�W���̏��𓾂�
//
//     MATRIX4X4  mHold       �F���̈ʒu
//     BASEHOLDITEM HoldItem  :��Ɏ�����̏��
//     MATRIX4X4& mMat        :��Ɏ�����̃}�g���b�N�X(Out)
//     VECTOR3& vMin          :�o�E���f�B���O�{�b�N�X��Min(Out)
//     VECTOR3& vMax          :�o�E���f�B���O�{�b�N�X��Max(Out)
//
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
void  CWeaponHoldProc::GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vMin, VECTOR3& vMax)
{
	((CWeaponHoldObj*)m_pObjArray[HoldItem.m_nHoldObjNo])->GetInfo(mHold, HoldItem, mMat, vMin, vMax);	// ����E���I�u�W�F�N�g

	return;
}
// ---------------------------------------------------------------------------
//
// ����E��Ɏ��A�C�e���v���V�[�W���̏��𓾂�
//
//     MATRIX4X4  mHold       �F���̈ʒu
//     BASEHOLDITEM HoldItem  :��Ɏ�����̏��
//     MATRIX4X4& mMat        :��Ɏ�����̃}�g���b�N�X(Out)
//     VECTOR3& vOffset       :�e���܂ł̃I�t�Z�b�g(Out)
//
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
void  CWeaponHoldProc::GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vOffset)
{
	((CWeaponHoldObj*)m_pObjArray[HoldItem.m_nHoldObjNo])->GetInfo(mHold, HoldItem, mMat, vOffset);	// ����E�e�I�u�W�F�N�g

	return;
}

// ---------------------------------------------------------------------------
//
// ����E��Ɏ��A�C�e���v���V�[�W���̃o�E���f�B���O�{�b�N�X�𓾂�
//
//     MATRIX4X4  mHold       �F���̈ʒu
//     BASEHOLDITEM HoldItem  :��Ɏ�����̏��
//
//     �߂�l�@�@CBBox* �o�E���f�B���O�{�b�N�X
// ---------------------------------------------------------------------------
CBBox*  CWeaponHoldProc::GetBBox(MATRIX4X4 mHold, BASEHOLDITEM HoldItem)
{
	return ((CWeaponHoldObj*)m_pObjArray[HoldItem.m_nHoldObjNo])->GetBBox(mHold, HoldItem);	// ����E���I�u�W�F�N�g
}

// ---------------------------------------------------------------------------
//
// ����E��Ɏ��A�C�e���v���V�[�W���̕\��
//
//     MATRIX4X4  mHold       �F���̈ʒu
//     BASEHOLDITEM HoldItem  :��Ɏ�����̏��
//
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
BOOL CWeaponHoldProc::Disp(MATRIX4X4 mHold, BASEHOLDITEM HoldItem)
{

	((CWeaponHoldObj*)m_pObjArray[HoldItem.m_nHoldObjNo])->Disp(mHold, HoldItem);	// ����E���I�u�W�F�N�g

	return TRUE;
}
//============================================================================
//
// ����E��Ɏ��A�C�e���I�u�W�F�N�g�̃R���X�g���N�^
//
// �E���ڍU�����镐��ł͂Ȃ��A�����\��������A�ʒu����Ԃ����肷��
//
// ---------------------------------------------------------------------------
CWeaponHoldObj::CWeaponHoldObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	m_nKindNo = 0;
	m_pMesh = NULL;
	m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // �I�t�Z�b�g�̐ݒ�
}
//============================================================================
//
// ����E��Ɏ��A�C�e���I�u�W�F�N�g�̏�����
//
// �E���b�V���́A�v���V�[�W���ł͂Ȃ��I�u�W�F�N�g�̒��ɐݒ肷��
//
// ---------------------------------------------------------------------------
void CWeaponHoldObj::Init()
{

	switch (m_dwObjNo)
	{
	case 0:     // ���ݒ�
		m_nKindNo = 0;
		m_pMesh = NULL; // �I�u�W�F�N�g�͖���
		m_pBBox = NULL; // BBox�̐ݒ�
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // �I�t�Z�b�g�Ȃ�
		break;

	case 1:     // �����Ȍ��̐ݒ�
		m_nKindNo = 1;  // 1:��ʁ@��
		m_pMesh = NULL; // �I�u�W�F�N�g�͖���
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.15f, -0.15f, -0.15f), VECTOR3(0.15f, 1.8f, 0.15f));// BBox�̐ݒ�
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // �e���܂ł̃I�t�Z�b�g�̐ݒ�
		break;

	case 2:     // ���i���{���j���b�V���̐ݒ�
		m_nKindNo = 1;  // 1:��ʁ@��
		m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Sword.mesh"));
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.15f, 0.1f, -0.15f), VECTOR3(0.15f, 1.8f, 0.15f));// BBox�̐ݒ�
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // �e���܂ł̃I�t�Z�b�g�̐ݒ�
		break;

	case 3:     // ���i�\�[�h�j���b�V���̐ݒ�
		m_nKindNo = 1;  // 1:��ʁ@��
		m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Sword2.mesh"));
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.15f, 0.1f, -0.15f), VECTOR3(0.15f, 1.2f, 0.15f));// BBox�̐ݒ�
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // �e���܂ł̃I�t�Z�b�g�̐ݒ�
		break;

	case 4:     // �����ȏe�̐ݒ�
		m_nKindNo = 2;  // 2:��ʁ@�e
		m_pMesh = NULL; // �I�u�W�F�N�g�͖���
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.51f);   // �e���܂ł̃I�t�Z�b�g�̐ݒ�
		break;

	case 5:     // �e�i���C�t���j���b�V���̐ݒ�
		m_nKindNo = 2;  // 2:��ʁ@�e
		m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Gun.mesh"));
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.51f);   // �e���܂ł̃I�t�Z�b�g�̐ݒ�
		break;

	case 6:     // �e�i�s�X�g���j���b�V���̐ݒ�
		m_nKindNo = 2;  // 2:��ʁ@�e
		m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Pistol.mesh"));
		m_vOffset = VECTOR3(0.0f, 0.07f, 0.17f);   // �e���܂ł̃I�t�Z�b�g�̐ݒ�
		break;

	case 7:     // �����ȏ��̐ݒ�
		m_nKindNo = 1;  // 1:��ʁ@��
		m_pMesh = NULL; // �I�u�W�F�N�g�͖���
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-1.0f, -1.5f, -0.3f), VECTOR3(1.0f, 1.5f, 0.3f));// BBox�̐ݒ�
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // �e���܂ł̃I�t�Z�b�g�̐ݒ�
		break;

	case 8:		// �|��
		m_nKindNo = 2;  // 1:��ʁ@��
		m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Arrow/Arrow.mesh"));
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.15f, 0.1f, -0.15f), VECTOR3(0.15f, 1.0f, 0.15f));// BBox�̐ݒ�
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // �e���܂ł̃I�t�Z�b�g�̐ݒ�
		break;

	case 9:		// ��
		m_nKindNo = 1;
		m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Sword/Sword.mesh"));
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.17f, 0.0f, -0.09f), VECTOR3(0.17f, 1.1f, 0.09f));// BBox�̐ݒ�
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // �e���܂ł̃I�t�Z�b�g�̐ݒ�
		break;

	case 10:	// �e
		m_nKindNo = 2;
		m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Gun/Gun.mesh"));
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.15f, 0.1f, -0.15f), VECTOR3(0.15f, 1.0f, 0.15f));// BBox�̐ݒ�
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // �e���܂ł̃I�t�Z�b�g�̐ݒ�
		break;

	case 11:	// ��
		m_nKindNo = 1;
		m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Axe/Axe.mesh"));
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.15f, 0.1f, -0.15f), VECTOR3(0.15f, 1.0f, 0.15f));// BBox�̐ݒ�
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // �e���܂ł̃I�t�Z�b�g�̐ݒ�
		break;

	case 12:
		m_nKindNo = 0;
		m_pMesh = NULL; // �I�u�W�F�N�g�͖���
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.9f, -0.9f, -0.9f), VECTOR3(0.9f, 2.0f, 0.9f));// BBox�̐ݒ�
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // �e���܂ł̃I�t�Z�b�g�̐ݒ�
		break;

	case 13:
		m_nKindNo = 0;
		m_pMesh = NULL; // �I�u�W�F�N�g�͖���
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-1.0f, -0.8f, -1.0f), VECTOR3(1.0f, 2.0f, 1.0f));// BBox�̐ݒ�
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // �e���܂ł̃I�t�Z�b�g�̐ݒ�
		break;

	case 14:
		m_nKindNo = 0;
		m_pMesh = NULL; // �I�u�W�F�N�g�͖���
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-3.0f, -4.0f, -1.0f), VECTOR3(3.0f, 9.0f, 1.0f));// BBox�̐ݒ�
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // �e���܂ł̃I�t�Z�b�g�̐ݒ�
		break;

	default:
		m_nKindNo = 0;
		m_pMesh = NULL; // �I�u�W�F�N�g�͖���
		m_pBBox = NULL; // BBox�̐ݒ�
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // �I�t�Z�b�g�Ȃ�
		break;
	}

	m_bActive = FALSE;
	m_dwStatus = NORMAL;

}
// ---------------------------------------------------------------------------
//
// ����E��Ɏ��A�C�e���I�u�W�F�N�g�̃f�X�g���N�^
//
// ---------------------------------------------------------------------------
CWeaponHoldObj::~CWeaponHoldObj()
{
	SAFE_DELETE(m_pMesh);
}

// ---------------------------------------------------------------------------
//
// ����E��Ɏ��A�C�e���I�u�W�F�N�g�̏��𓾂�
//
//     MATRIX4X4  mHold       �F���̈ʒu
//     BASEHOLDITEM HoldItem  :��Ɏ�����̏��
//     MATRIX4X4& mMat        :��Ɏ�����̃}�g���b�N�X(Out)
//     VECTOR3& vMin          :�o�E���f�B���O�{�b�N�X��Min(Out)
//     VECTOR3& vMax          :�o�E���f�B���O�{�b�N�X��Max(Out)
//
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
void  CWeaponHoldObj::GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vMin, VECTOR3& vMax)
{
	// �I�t�Z�b�g�}�g���b�N�X�̌v�Z
	MATRIX4X4 mPos = XMMatrixTranslation(HoldItem.m_nOffsetPos.x, HoldItem.m_nOffsetPos.y, HoldItem.m_nOffsetPos.z);

	MATRIX4X4 mRotX = XMMatrixRotationX(XMConvertToRadians(HoldItem.m_nOffsetRot.x));
	MATRIX4X4 mRotY = XMMatrixRotationY(XMConvertToRadians(HoldItem.m_nOffsetRot.y));
	MATRIX4X4 mRotZ = XMMatrixRotationZ(XMConvertToRadians(HoldItem.m_nOffsetRot.z));

	MATRIX4X4 mOffset = mRotY * mRotX * mRotZ;
	mOffset = mOffset * mPos;

	// 
	// ���[���h�}�g���b�N�X�̌v�Z
	// 
	// �A���A���̂Ƃ�m_mWorld��m_pBBox->m_mWorld�̐ݒ�͍s���Ă��Ȃ�!!
	// 
	mMat = mOffset * mHold;     // ���̈ʒu�̃��[���h�}�g���b�N�X

	vMin = m_pBBox->m_vMin;
	vMax = m_pBBox->m_vMax;

	return;
}

// ---------------------------------------------------------------------------
//
// ����E��Ɏ��A�C�e���I�u�W�F�N�g�̏��𓾂�
//
//     MATRIX4X4  mHold       �F�e�̈ʒu
//     BASEHOLDITEM HoldItem  :��Ɏ�����̏��
//     MATRIX4X4& mMat        :��Ɏ�����̃}�g���b�N�X(Out)
//     VECTOR3& vOffset       :�e���܂ł̃I�t�Z�b�g(Out)
//
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
void  CWeaponHoldObj::GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vOffset)
{
	// �I�t�Z�b�g�}�g���b�N�X�̌v�Z
	MATRIX4X4 mPos = XMMatrixTranslation(HoldItem.m_nOffsetPos.x, HoldItem.m_nOffsetPos.y, HoldItem.m_nOffsetPos.z);

	MATRIX4X4 mRotX = XMMatrixRotationX(XMConvertToRadians(HoldItem.m_nOffsetRot.x));
	MATRIX4X4 mRotY = XMMatrixRotationY(XMConvertToRadians(HoldItem.m_nOffsetRot.y));
	MATRIX4X4 mRotZ = XMMatrixRotationZ(XMConvertToRadians(HoldItem.m_nOffsetRot.z));

	MATRIX4X4 mOffset = mRotY * mRotX * mRotZ;
	mOffset = mOffset * mPos;

	// ���[���h�}�g���b�N�X�̌v�Z
	// 
	// �A���A���̂Ƃ�m_mWorld�̐ݒ�͍s���Ă��Ȃ�!!
	// 
	mMat = mOffset * mHold;     // �e�̈ʒu�̃��[���h�}�g���b�N�X

	vOffset = m_vOffset;

	return;
}

// ---------------------------------------------------------------------------
//
// ����E��Ɏ��A�C�e���I�u�W�F�N�g�̃o�E���f�B���O�{�b�N�X�𓾂�
//
//     MATRIX4X4  mHold       �F���̈ʒu
//     BASEHOLDITEM HoldItem  :��Ɏ�����̏��
//
//     �߂�l�@�@CBBox* �o�E���f�B���O�{�b�N�X
// ---------------------------------------------------------------------------
CBBox* CWeaponHoldObj::GetBBox(MATRIX4X4 mHold, BASEHOLDITEM HoldItem)
{

	// �I�t�Z�b�g�}�g���b�N�X�̌v�Z
	MATRIX4X4 mPos = XMMatrixTranslation(HoldItem.m_nOffsetPos.x, HoldItem.m_nOffsetPos.y, HoldItem.m_nOffsetPos.z);

	MATRIX4X4 mRotX = XMMatrixRotationX(XMConvertToRadians(HoldItem.m_nOffsetRot.x));
	MATRIX4X4 mRotY = XMMatrixRotationY(XMConvertToRadians(HoldItem.m_nOffsetRot.y));
	MATRIX4X4 mRotZ = XMMatrixRotationZ(XMConvertToRadians(HoldItem.m_nOffsetRot.z));

	MATRIX4X4 mOffset = mRotY * mRotX * mRotZ;
	mOffset = mOffset * mPos;

	// ���[���h�}�g���b�N�X�̌v�Z
	// 
	// �A���A���̂Ƃ�m_mWorld��m_pBBox->m_mWorld�̐ݒ���s���Ă���!!
	// 
	m_mWorld = mOffset * mHold;     // ���̈ʒu�̃��[���h�}�g���b�N�X
	m_pBBox->m_mWorld = m_mWorld;   // �o�E���f�B���O�{�b�N�X�̃��[���h�}�g���b�N�X���Z�b�g

	return m_pBBox;

}


// ---------------------------------------------------------------------------
//
// ����E��Ɏ��A�C�e���I�u�W�F�N�g�̕`��
//
//     MATRIX4X4  mHold        �F�A�C�e���̈ʒu
//     VECTOR3    vPos         :�ʒu�̃I�t�Z�b�g
//     VECTOR3    vRot         :�p�x�̃I�t�Z�b�g
//
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
BOOL CWeaponHoldObj::Disp(MATRIX4X4 mHold, BASEHOLDITEM HoldItem)
{
	m_bActive = TRUE;  // �X�^�[�g���ɕ\���J�n

	// �I�t�Z�b�g�}�g���b�N�X�̌v�Z
	MATRIX4X4 mPos = XMMatrixTranslation(HoldItem.m_nOffsetPos.x, HoldItem.m_nOffsetPos.y, HoldItem.m_nOffsetPos.z);

	MATRIX4X4 mRotX = XMMatrixRotationX(XMConvertToRadians(HoldItem.m_nOffsetRot.x));
	MATRIX4X4 mRotY = XMMatrixRotationY(XMConvertToRadians(HoldItem.m_nOffsetRot.y));
	MATRIX4X4 mRotZ = XMMatrixRotationZ(XMConvertToRadians(HoldItem.m_nOffsetRot.z));

	MATRIX4X4 mOffset = mRotY * mRotX * mRotZ;
	mOffset = mOffset * mPos;

	// ���[���h�}�g���b�N�X�̌v�Z
	m_mWorld = mOffset * mHold;     // ���̈ʒu�̃��[���h�}�g���b�N�X
	if (m_pBBox) m_pBBox->m_mWorld = m_mWorld;   // �o�E���f�B���O�{�b�N�X�̃��[���h�}�g���b�N�X���Z�b�g

	// �����_�����O
	Render();

	m_bActive = FALSE;  // �P�񏈗�������A����

	return TRUE;
}

// ---------------------------------------------------------------------------
//
// ����E��Ɏ��A�C�e���I�u�W�F�N�g�̍X�V
//
//     �����@�@�@�Ȃ�
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
void CWeaponHoldObj::Update()
{
	;
}
// ---------------------------------------------------------------------------
//
// ����E��Ɏ��A�C�e���I�u�W�F�N�g�̕`��
//
//     �����@�@�@�Ȃ�
//     �߂�l�@�@�Ȃ�
// ---------------------------------------------------------------------------
void CWeaponHoldObj::Render()
{

	// ��Ɏ��A�C�e���̃����_�����O
	if (m_pMesh) m_pMesh->RenderToon(m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, normalize(VECTOR3(0.8f, 1, -1)), m_pGMain->m_vEyePt);

	// �o�E���f�B���O�{�b�N�X�����_�����O
	//if (m_pBBox) m_pBBox->Render(m_pGMain->m_mView, m_pGMain->m_mProj, VECTOR3(0, 0, 0), m_pGMain->m_vEyePt);
}
