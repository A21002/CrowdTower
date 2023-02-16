//=============================================================================
//		�R�c�Q�[���v���O����
//		�b�R�c�`�����P�P�Q                             ver 3.1        2022.2.16
//
//		�J��������
//
//		(���_�ύX�����E��Q����������������Ă���)
//
//																Camera.cpp
//=============================================================================
#include  "Camera.h"
#include  "Playchar.h"
#include  "Enemy.h"
#include  "EnemyBoss.h"
#include  "Map.h"

//==========================================================================================================================
//
// �J�����v���V�[�W���̃R���X�g���N�^
//
//   �Ȃ��A�I�u�W�F�N�g��delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
//
// -------------------------------------------------------------------------------------------------------------------------
CCameraProc::CCameraProc(CGameMain* pGMain) : CBaseProc(pGMain)
{

	m_pObjArray.push_back(new CCameraObj(this));  // m_pObjArray�ɃI�u�W�F�N�g��o�^����

}

// --------------------------------------------------------------------------- 
//
// �J�����I�u�W�F�N�g�̃R���X�g���N�^
//
// ---------------------------------------------------------------------------
CCameraObj::CCameraObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	// �J��������t���O
	m_nCtrl = CAMERA_GAMEMAIN;      // �ŏ���TPS���_�̃J�����ʒu

	// ��_���[���h�}�g���b�N�X�̏����l
	m_mBaseWorld = XMMatrixTranslation(0, 0, 0);

	// ��_���[���h�}�g���b�N�X����̕ψʏ����l�̐ݒ�
	// �@ TPS���_�̂Ƃ��̊�_���[���h�}�g���b�N�X����̕ψʒl
	// �E�����_�́A�ψʋ�����m_vBaseLocalLookatTPS�ɐݒ肷��
	m_vBaseLocalLookatTPS = VECTOR3(0.0f, m_pGMain->m_pPcProc->GetPcObjPtr()->GetBBox()->m_vMax.y, 0.0f);

	// �E���_�́A�ψʊp�x�w�C�x��m_vBaseLocalEyeRotDistTPS.x m_vBaseLocalEyeRotDistTPS.y�ɁA�ψʒ���������m_vBaseLocalEyeRotDistTPS.z�ɐݒ肷��
	m_vBaseLocalEyeRotDistTPS.z = -10.0f;  // �ψʒ�������
	m_vBaseLocalEyeRotDistTPS.x = XMConvertToDegrees( atan2f(m_pGMain->m_pPcProc->GetPcObjPtr()->GetBBox()->m_vMax.y + 2.3f, -m_vBaseLocalEyeRotDistTPS.z)); // �ψʊp�x�w
	m_vBaseLocalEyeRotDistTPS.y = 0.0f;   // �ψʊp�x�x

	// �A FPS���_�̂Ƃ��̊�_���[���h�}�g���b�N�X����̕ψʒl
	// �E�����_�E���_���ɁA�ψʋ�����m_vBaseLocalEyeFPS�ɐݒ肷��
	m_vBaseLocalLookatFPS = VECTOR3(0.0f, 1.5f, 7.0f);
	m_vBaseLocalEyeFPS    = VECTOR3(0.0f, 1.5f, 0.3f);
	m_isLookStairs = false;
	m_isOnce = true;
	m_IsBossMovie = FALSE;
	m_IsMovePc = FALSE;
	m_bIsDeadBoss = FALSE;
	// �J�����ψʏ����l��ݒ肷��
	SetCamBase();
}
// ---------------------------------------------------------------------------
//
// �J�����I�u�W�F�N�g�̃f�X�g���N�^
//
// ---------------------------------------------------------------------------
CCameraObj::~CCameraObj()
{
	;
}
//-----------------------------------------------------------------------------
// �J�����I�u�W�F�N�g�̐���
//
//�@�o�b�̓����ɍ��킹�ăJ�����𐧌䂷��
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CCameraObj::Update()
{
	if (m_pGMain->m_pEnmProc->CountActiveEnm() == 0 && m_isOnce && m_pGMain->m_pMapProc->GetHierarchy() != 10) {
		m_isLookStairs = true;
		m_isOnce = false;
	}
	// �����_�E���_�̊�_�ƂȂ��_���[���h�}�g���b�N�X�̐ݒ�
	// (�����ł́A�o�b�̃��[�J�����}�g���b�N�X��ݒ肷��)
	if (m_IsBossMovie)
	{
		m_pGMain->m_pPcProc->GetPcObjPtr()->SetCanMove(FALSE);
		if (!m_IsMovePc)
		{
			m_mBaseWorld = m_pGMain->m_pEnmBossProc->GetEnmBossObjPtr()->GetWorld();
		}
		else {
			MoveBasePos();
		}
	}
	else {
		if (m_bIsDeadBoss) {
			m_mBaseWorld = m_pGMain->m_pEnmBossProc->GetEnmBossObjPtr()->GetWorld();
		}
		else {
			m_mBaseWorld = m_pGMain->m_pPcProc->GetPcObjPtr()->GetLocalMatrix();
		}
	}

	// �J�����I�u�W�F�N�g�̈ʒu���蓮����
	// �܂��A�J���������ύX�i�s�o�r���_���e�o�r���_���j
	//ControlCam();

	// �����_�̃x�N�g�����쐬���鏈��
	// (�����_�̃��[�J�����W�Ɗ�_���[���h�}�g���b�N�X���|�����킹�A�����_�}�g���b�N�X�𓾂�)
	MATRIX4X4 mLookat;
	if (!m_isLookStairs) {
		// PC������
		mLookat = XMMatrixTranslationFromVector(m_vLocalLookat) * m_mBaseWorld;
	}
	else {
		// �K�i������
		mLookat = XMMatrixTranslationFromVector(VECTOR3(0.0f, 0.0f, 22.0f));
	}

	if (m_nCtrl == CAMERA_GAMECLEAR || m_nCtrl == CAMERA_GAMEOVER) {
		mLookat = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	}
	m_pGMain->m_vLookatPt = GetPositionVector(mLookat);    // �����_�}�g���b�N�X���璍���_�ʒu�𓾂�

	// �J�����i���_�j�x�N�g�����쐬���鏈��
	// (�J�����i���_�j�̃��[�J�����W�Ɗ�_���[���h�}�g���b�N�X���|�����킹�A���_�}�g���b�N�X�𓾂�)
	MATRIX4X4 mEye = XMMatrixTranslationFromVector(m_vLocalEye) * GetPositionMatrix(m_mBaseWorld);
	m_pGMain->m_vEyePt = GetPositionVector( mEye );        // ���_�}�g���b�N�X���王�_�ʒu�𓾂�

	// TPS���_�̎��A��Q����������鏈��
	//if (m_nCtrl == 0 && !m_isLookStairs) EvasiveObstacle();

	// ���_�x�N�g���ƒ����_�x�N�g������J�����̃��[���h�}�g���b�N�X�𓾂�i���͎g�p���Ă��Ȃ��j
	m_mWorld = GetLookatMatrix(m_pGMain->m_vEyePt, m_pGMain->m_vLookatPt);

	// �r���[�}�g���b�N�X�̍쐬
	VECTOR3 vUpVec(0.0f, 1.0f, 0.0f);    // ����ʒu���΍��W�̏�����Ƃ���
	// �J����(���_)�̈ʒu�A������A����ђ����_���g�p���āA������W�n�̃r���[�s����쐬
	m_pGMain->m_mView = XMMatrixLookAtLH(m_pGMain->m_vEyePt, m_pGMain->m_vLookatPt, vUpVec);

	// �J�����̈ʒu�A������A����уJ�����̌������g�p���āA������W�n�̃r���[�s����쐬�i�g�p���Ă��Ȃ��j
	//VECTOR3 vEyeDir = XMVector3TransformCoord(VECTOR3(0, 0, 1), GetRotateMatrix(m_mWorld));
	//m_pGMain->m_mView = XMMatrixLookToLH(m_pGMain->m_vEyePt, vEyeDir, vUpVec);

	// ���C�g���_����̃r���[�g�����X�t�H�[���iShadowMap�p�j���C�g���_���o�b�̈ʒu�ɍ��킹��Ƃ��i�g�p���Ă��Ȃ��j
	//m_pGMain->m_vLightEye = m_pGMain->m_vLookatPt + m_pGMain->m_vLightDir * 100.0f;	        // �J�����i���_�j�ʒu�������̕���100���̈ʒu�ɂ���
	//m_pGMain->m_mLightView = XMMatrixLookAtLH(m_pGMain->m_vLightEye, m_pGMain->m_vLookatPt, vUpVec);

}

//----------------------------------------------------------------------------- 
// BaseWorld���v���C���[�ɋ߂Â�����
//----------------------------------------------------------------------------- 
void CCameraObj::MoveBasePos()
{
	VECTOR3 pcPos = GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());
	MATRIX4X4 world = m_mWorld;
	m_mWorld = m_mBaseWorld;
	const float zoomSpeed = 0.05f;
	const float speed = 0.25;

	if (!TargetMove(pcPos, speed, 180.0f))
	{
		MATRIX4X4 mTemp = XMMatrixRotationY(XMConvertToRadians(m_vRotUp.y));					// �ړI�n�iTarget�j�֌������p�x
		m_mBaseWorld = mTemp * m_mWorld;
		mTemp = XMMatrixTranslation(m_vPosUp.x, m_vPosUp.y + m_fJumpY, m_vPosUp.z);	// �ړI�n�iTarget�j�֌�����
		m_mBaseWorld = mTemp * m_mWorld;

		if (m_vLocalEyeRotDist.z < -15.0f)
		{
			SetLocalEyeTPS(0.0f, 0.0f, zoomSpeed);
		}
	}
	else {
		m_IsBossMovie = FALSE;
		m_pGMain->m_pPcProc->GetPcObjPtr()->SetCanMove(TRUE);
		m_pGMain->m_pEnmBossProc->GetEnmBossObjPtr()->SetMovieFinish(TRUE);
	}
	m_mWorld = world;
}

//----------------------------------------------------------------------------- 
// ��Q����������鏈��
//
// �E���_�ƒ����_�Ƃ̊Ԃɏ�Q�����������ꍇ�A���_����Q���̑O�Ɉړ����鏈��
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CCameraObj::EvasiveObstacle()
{
	VECTOR3 vHit;
	VECTOR3 vNorm;
	if (m_pGMain->m_pMapProc->Hitcheck(m_pGMain->m_vEyePt, m_pGMain->m_vLookatPt, &vHit, &vNorm))   // ��Q���Ƃ̐ڐG�`�F�b�N
	{
		MATRIX4X4 mTemp;
		mTemp = GetLookatMatrix(vHit, m_pGMain->m_vLookatPt);        // ��Q���Ƃ̐ڐG�_���璍���ʒu������}�g���b�N�X
		mTemp = XMMatrixTranslation(0.0f, 0.0f, 0.01f) * mTemp;      // ��Q���Ƃ̐ڐG�_����1cm���������Ɉړ��������_�ʒu�𓾂�
		m_pGMain->m_vEyePt = GetPositionVector(mTemp);
	}
}
//-----------------------------------------------------------------------------
// �J�����I�u�W�F�N�g�̎蓮���쐧��
//
//   �J�����I�u�W�F�N�g�̈ʒu���蓮�ő��삷��
//   �܂��A�J���������ύX�i�s�o�r���_���e�o�r���_���j
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CCameraObj::ControlCam()
{
	const float fRotUp  = 1.5f;     // �p�x�ύX�̑����l
	const float fDistUp = 0.05f;    // �����ύX�̑����l

	// �J��������̕ύX�i�s�o�r���_���e�o�r���_���j
	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_F)) // F�L�[
	{
		m_nCtrl = (m_nCtrl + 1) % 2;   // ����̕ύX�i0:TPS���_��1:FPS���_�j
		SetCamBase();   // �J�����ψʏ����l��ݒ肷��
	}

	// �J�������_�̑��Έʒu�̕ύX
	if (m_nCtrl == 0)   // TPS���_�̎�
	{

	/*
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD6)) // ��
		{
			SetLocalEyeTPS(fRotUp, 0, 0);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD4)) // ��
		{
			SetLocalEyeTPS(-fRotUp, 0, 0);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD8)) // ��
		{
			SetLocalEyeTPS(0, fRotUp, 0);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD2)) // ��
		{
			SetLocalEyeTPS(0, -fRotUp, 0);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD7)) // �߂Â���
		{
			SetLocalEyeTPS(0, 0, -fDistUp);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD9)) // ��������
		{
			SetLocalEyeTPS(0, 0, fDistUp);
		}
	*/
		
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_RBRACKET)) // ��  ]
		{
			//SetLocalEyeTPS(fRotUp, 0, 0);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_SEMICOLON)) // ��  ;
		{
			//SetLocalEyeTPS(-fRotUp, 0, 0);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_AT)) // ��  @
		{
			SetLocalEyeTPS(0, fRotUp, 0);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_COLON)) // ��  :
		{
			SetLocalEyeTPS(0, -fRotUp, 0);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_P)) // �߂Â���  P
		{
			SetLocalEyeTPS(0, 0, -fDistUp);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_LBRACKET)) // ��������  [
		{
			SetLocalEyeTPS(0, 0, fDistUp);
		}
		
	}
	else {    // FPS���_�̎�

	/*
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD6)) // ��
		{
			m_vLocalEye.x -= fDistUp/5;
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD4)) // ��
		{
			m_vLocalEye.x += fDistUp/5;
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD8)) // ��
		{
			m_vLocalLookat.y += fDistUp / 2;
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD2)) // ��
		{
			m_vLocalLookat.y -= fDistUp / 2;
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD7)) // �߂Â���
		{
			m_vLocalEye.z += fDistUp / 10;
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD9)) // ��������
		{
			m_vLocalEye.z -= fDistUp / 10;
		}
	*/

		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_RBRACKET)) // ��  ]
		{
			m_vLocalEye.x -= fDistUp / 5;
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_SEMICOLON)) // ��  ;
		{
			m_vLocalEye.x += fDistUp / 5;
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_AT)) // ��  @
		{
			m_vLocalLookat.y += fDistUp / 2;
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_COLON)) // ��  :
		{
			m_vLocalLookat.y -= fDistUp / 2;
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_P)) // �߂Â���  P
		{
			m_vLocalEye.z += fDistUp / 10;
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_LBRACKET)) // ��������  [
		{
			m_vLocalEye.z -= fDistUp / 10;
		}

	}

	// ��l�ɖ߂�����
	if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_HOME) || m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_H))
	{
		SetCamBase();    // �J�����ψʏ����l��ݒ肷��
	}
}

//----------------------------------------------------------------------------- // -- 2020.11.19
// �s�o�r�̂Ƃ��J�����ʒu�̐ݒ菈��
//
//   m_vLocalEyeRotDist.x��m_vLocalEyeRotDist.y�ɉ�]�p�x�Am_vLocalEyeRotDist.z��
//   �����_���王�_�܂ł̒��������������Ă���A
//   ���̒l�ɁA�����̉�]�Ƌ��������Z���A���̌��ʂ���ɁA�ψʒl���v�Z���A
//   ���̌��ʂ�m_vLocalEye�ɐݒ肷��
//
//   ����	float fRotY : �x��]
//			float fRotX : �w��]
//			float fDist : ����
//
//   �߂�l�@�Ȃ�
//-----------------------------------------------------------------------------
void	CCameraObj::SetLocalEyeTPS(float fRotY, float fRotX, float fDist)
{
	m_vLocalEyeRotDist.x += fRotX;
	m_vLocalEyeRotDist.y += fRotY;
	m_vLocalEyeRotDist.z += fDist;

	MATRIX4X4 mRotY = XMMatrixRotationY(XMConvertToRadians(m_vLocalEyeRotDist.y));
	MATRIX4X4 mRotX = XMMatrixRotationX(XMConvertToRadians(m_vLocalEyeRotDist.x));
	MATRIX4X4 mPos  = XMMatrixTranslation(0, 0, m_vLocalEyeRotDist.z);
	mPos = mPos * mRotX * mRotY;
	m_vLocalEye = GetPositionVector(mPos);
}
//----------------------------------------------------------------------------- // -- 2020.11.19
// �J�����ψʏ����l��ݒ肷�鏈��
//
// �@ TPS���_�̂Ƃ�
//    ���_�́A�ψʊp�x�ƕψʒ����������A��Um_vLocalEyeRotDist�ɐݒ肵�A
//    ���̌�SetLocalEyeTPS�֐��ɂ���āAm_vLocalEye�ɐݒ肷��
//    �����_�́A�ψʋ�����m_vLocalLookatTPS�ɐݒ肷��
//
// �A FPS���_�̂Ƃ�
//    ���_�́A�ψʋ�����m_vLocalEye�ɐݒ肷��
//    �����_�́A�ψʋ�����m_vLocalLookat�ɐݒ肷��
//
//   ����	�Ȃ�
//
//   �߂�l�@�Ȃ�
//-----------------------------------------------------------------------------
void	CCameraObj::SetCamBase()
{
	// �J�����ψʂ̏����l��ݒ肷��
	if (m_nCtrl == CAMERA_GAMEMAIN)
	{
		m_vLocalEyeRotDist = m_vBaseLocalEyeRotDistTPS;
		m_vLocalLookat = m_vBaseLocalLookatTPS;
		SetLocalEyeTPS(-45, 5, -5);
	}
	 else if(m_nCtrl == CAMERA_GAMEOVER || m_nCtrl == CAMERA_GAMECLEAR){
		// �Q�[���I�[�o�[�A�N���A���̃J�����p�x
		m_vLocalEyeRotDist = VECTOR3(0.0f, 0.0f, 0.0f);
		m_vLocalLookat = VECTOR3(0.0f, 0.0f, 0.0f);
		SetLocalEyeTPS(0, 0, -13);
		m_vLocalEye.y += 1;
	}
		/*
		if (m_pGMain->m_dwGameStatus == GAMEMAIN)
		{
			m_vLocalEyeRotDist = m_vBaseLocalEyeRotDistTPS;
			m_vLocalLookat = m_vBaseLocalLookatTPS;
			SetLocalEyeTPS(-45, 5, -5);
		}
		else {
			m_vLocalEyeRotDist = m_vBaseLocalEyeRotDistTPS;
			m_vLocalLookat = m_vBaseLocalLookatTPS;
			SetLocalEyeTPS(0, 30, -5);
		}
		*/
	
	else {
		m_vLocalEye    = m_vBaseLocalEyeFPS;
		m_vLocalLookat = m_vBaseLocalLookatFPS;
	}
}

void	CCameraObj::ResetMovieFlag()
{
	m_isOnce = true;
}

void CCameraObj::SetMovePc(bool flag)
{
	m_IsMovePc = flag;
}

void CCameraObj::SetBossMovie(bool flag)
{
	m_IsBossMovie = flag;
}

void CCameraObj::SetDeadBoss(bool flag)
{
	m_bIsDeadBoss = flag;
}

void	CCameraObj::SetCtrl(int num)
{
	m_nCtrl = num;
}