//=============================================================================
//		�R�c�Q�[���v���O����
//		�b�R�c�`�����P�P�Q�@�@�@�@�@�@                 ver 3.1        2022.2.16
//
//		�Q�[���̃��C������
//																GameMain.cpp
//=============================================================================
#include "GameMain.h"
#include "Playchar.h"
#include "Ally.h"
#include "Camera.h"
#include "Enemy.h"
#include "EnemyBoss.h"
#include "Map.h"
#include "Weapon.h"
#include "Effect.h"
#include "BackFore.h"
#include "Title.h"

// ============================================================================================
//
// CGameMain �Q�[�����C���N���X�̏���
//
// ============================================================================================
//------------------------------------------------------------------------
//
//	�Q�[�����C���N���X�̃R���X�g���N�^	
//
//  �����@�Ȃ�
//
//------------------------------------------------------------------------
CGameMain::CGameMain(CMain*	pMain)
{
	ZeroMemory(this, sizeof(CGameMain));
	m_pMain = pMain;
}
//------------------------------------------------------------------------
//
//	�Q�[�����C���N���X�̃f�X�g���N�^	
//
//------------------------------------------------------------------------
CGameMain::~CGameMain()
{
	MyImgui::ImguiQuit();          // -- 2020.11.15    // MyImgui�̏I������

	SAFE_DELETE(m_pGameOver);
	SAFE_DELETE(m_pGameClear);
	SAFE_DELETE(m_pBossBgm);
	SAFE_DELETE(m_pSeDead);
	SAFE_DELETE(m_pSeNitro);
	SAFE_DELETE(m_pBgm1);
	SAFE_DELETE(m_pSpown);
	SAFE_DELETE(m_pSlash);
	SAFE_DELETE(m_pArrow);
	SAFE_DELETE(m_pGun);
	SAFE_DELETE(m_pClick_Ability);
	SAFE_DELETE(m_pClick_Button);
	SAFE_DELETE(m_pAnimWalk);
	SAFE_DELETE(m_pDownCage);
	SAFE_DELETE(m_pShotSlashCharge);

	SAFE_DELETE(m_pEnmProc);
	SAFE_DELETE(m_pEnmBossProc);
	SAFE_DELETE(m_pPcProc);
	SAFE_DELETE(m_pAllyProc);
	SAFE_DELETE(m_pCameraProc);
	SAFE_DELETE(m_pMapProc);
	SAFE_DELETE(m_pWeaponProc);
	SAFE_DELETE(m_pEffectProc);
	SAFE_DELETE(m_pBackForeProc);
	SAFE_DELETE(m_pTitleProc);

	SAFE_DELETE(m_pFbxMeshCtrl); 
	SAFE_DELETE(m_pShader);
	SAFE_DELETE(m_pFont);
	SAFE_DELETE(m_pDI);
	SAFE_DELETE(m_pXAudio);
	SAFE_DELETE(m_pD3D);

	CoUninitialize();   // COM�̉��
}

//------------------------------------------------------------------------
//
//	�A�v���P�[�V�����̏���������	
//
//  �����@�Ȃ�
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CGameMain::Init()
{
	// �b�n�l�̏�����
	//CoInitializeEx(NULL, COINIT_MULTITHREADED);
	CoInitialize( NULL );

	// Direct3D�̏�����
	m_pD3D = new CDirect3D;
	if (FAILED(m_pD3D->InitD3D(m_pMain->m_hWnd, WINDOW_WIDTH, WINDOW_HEIGHT))) {
		MessageBox(0, _T("Direct3D���������o���܂���"), NULL, MB_OK);
		return E_FAIL;
	}

	// �V�F�[�_�[�̏�����
	m_pShader = new CShader(m_pD3D);
	if (FAILED(m_pShader->InitShader())) {
		MessageBox(0, _T("Shader�𐶐��E�������o���܂���"), NULL, MB_OK);
		return E_FAIL;
	}

	// XAudio2�̏�����
	m_pXAudio = new CXAudio;
	if (FAILED(m_pXAudio->InitAudio(m_pMain->m_hWnd))) {
		MessageBox(0, _T("XAudio2���������o���܂���"), NULL, MB_OK);
		return E_FAIL;
	}

	// DirectInput�̏�����
	m_pDI = new CDirectInput;
	if (!(m_pDI->StartDirectInput(m_pMain->m_hInstance, m_pMain->m_hWnd, 
                    INIT_KEYBOARD | INIT_MOUSE | INIT_JOYSTICK, WINDOW_WIDTH, WINDOW_HEIGHT))) {
		MessageBox(0, _T("DirectInput���������o���܂���"), NULL, MB_OK);
		return E_FAIL;
	}

	// �t�H���g�e�N�X�`���[�̏�����
	m_pFont = new CFontTexture(m_pShader);

	// MyImgui�̏�����
	MyImgui::ImguiInit(m_pMain->m_hWnd, m_pD3D, WINDOW_WIDTH, WINDOW_HEIGHT);           // -- 2020.11.15

	// Fbx���b�V���R���g���[���N���X
	m_pFbxMeshCtrl = new CFbxMeshCtrl(m_pShader);                       // -- 2021.2.4

	if (FAILED(m_pD3D->CreateShaderResourceViewFromFile(_T("Data/Image/ToonPaint.png"), &m_pD3D->m_pToonTexture, 3)))
	{
		MessageBox(NULL, _T("Data/Image/ToonPaint.png"), _T("�g�D�[���e�N�X�`���t�@�C��������܂���"), MB_OK);
		return E_FAIL;
	}

	// ----------------------------------------------------------------------------------------
	// �ϐ��̏�����
	m_dwGameStatus = TITLE;
	m_vLightDir = normalize( VECTOR3(0.8f, 1, -1) );  // �������W�̏����l�B���K������
	m_isMove = TRUE;
	m_bIsTutorial = FALSE;
	m_bIsBoss = FALSE;
	m_bIsBoxTutorial = FALSE;

	// �v���W�F�N�V�����g�����X�t�H�[���i�ˉe�ϊ��j�̏����l
	//m_mProj = XMMatrixPerspectiveFovLH((FLOAT)(XM_PI / 4), (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 1000.0);
	m_mProj = XMMatrixPerspectiveFovLH(XMConvertToRadians(38.0f), (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 1000.0);

	// �r���[�g�����X�t�H�[���i���_���W�ϊ��j�̏����l
	VECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//����ʒu
	m_vEyePt = VECTOR3(0.0f, 1.0f, 0.0f);
	m_vLookatPt = VECTOR3(0.0f, 1.0f, 1.0f);
	m_mView = XMMatrixLookAtLH(m_vEyePt, m_vLookatPt, vUpVec);

	// �e�v���V�[�W���̏�����
	m_pPcProc = new CPcProc(this);		// �o�b�v���V�[�W��

	m_pAllyProc = new CAllyProc(this);	// �����v���V�[�W��

	m_pCameraProc = new CCameraProc(this);	// �J�����v���V�[�W��

	m_pEnmProc = new CEnmProc(this);	// �G�v���V�[�W��

	m_pEnmBossProc = new CEnmBossProc(this);	// �G�{�X�v���V�[�W��

	m_pEffectProc = new CEffectProc(this);  // �R�c����

	m_pWeaponProc = new CWeaponProc(this);	// Weapon�v���V�[�W��

	m_pMapProc = new CMapProc(this);	// Map�v���V�[�W��

	m_pBackForeProc = new CBackForeProc(this);  // �O�i�E�w�i

	m_pTitleProc = new CTitleProc(this);  // �^�C�g���B�Q�[���N�����[�B�Q�[���I�[�o�[
	m_pTitleProc->CheckSaveData();

	// �T�E���h
	m_pGameClear = new CXAudioSource( m_pXAudio, _T("Data/Sound/GameClear.mp3"));
	m_pSeDead = new CXAudioSource( m_pXAudio, _T("Data/Sound/Dead.wav"), 10);
	m_pBossBgm = new CXAudioSource( m_pXAudio, _T("Data/Sound/MainBGM.mp3"));
	m_pBgm1 = new CXAudioSource( m_pXAudio, _T("Data/Sound/Title.mp3"));
	m_pGameOver = new CXAudioSource(m_pXAudio, _T("Data/Sound/GameOver.mp3"));
	m_pSpown = new CXAudioSource(m_pXAudio, _T("Data/Sound/Spown.wav"), 10);
	m_pSlash = new CXAudioSource(m_pXAudio, _T("Data/Sound/Slash.wav"), 10);
	m_pArrow = new CXAudioSource(m_pXAudio, _T("Data/Sound/Arrow.wav"), 10);
	m_pGun = new CXAudioSource(m_pXAudio, _T("Data/Sound/Gun.wav"), 10);
	m_pClick_Button = new CXAudioSource(m_pXAudio, _T("Data/Sound/Click_Button.wav"));
	m_pClick_Ability = new CXAudioSource(m_pXAudio, _T("Data/Sound/Click_Ability.wav"));
	m_pAnimWalk = new CXAudioSource(m_pXAudio, _T("Data/Sound/AnimWalk.wav"));
	m_pDownCage = new CXAudioSource(m_pXAudio, _T("Data/Sound/DownCage.wav"));
	m_pShotSlashCharge = new CXAudioSource(m_pXAudio, _T("Data/Sound/ShotSlashCharge.wav"));
	//m_pBgm1->Play(AUDIO_LOOP);	
	return S_OK;
}

//------------------------------------------------------------------------
//
//	�Q�[���̃��[�v����	
//
//  �����@�Ȃ�
//
//	�߂�l �Ȃ�
//
//------------------------------------------------------------------------
void CGameMain::Loop()
{

	m_pDI->GetInput();			// ���͏��̎擾
	m_pFont->Refresh();         // �t�H���g���̃��t���b�V��
	MyImgui::ImguiNewFrame();   // MyImgui�`��O����

		//��ʃN���A�i���ۂ͒P�F�ŉ�ʂ�h��Ԃ������j
	float ClearColor[4] = { 0,0,0,1 };// �N���A�F�쐬�@RGBA�̏�
	const int firstFloorPosY = 3826;

	m_pD3D->ClearRenderTarget(ClearColor); // ��ʃN���A

	if (m_pDI->CheckKey(KD_TRG, DIK_F4))  ChangeScreenMode();   // �t���X�N���[���̐؂�ւ�

	switch (m_dwGameStatus)
	{
	case TITLE:				// �Q�[���J�n�^�C�g��
		if (m_pTitleProc->Title())
		{
			switch (m_pTitleProc->m_nSelectNum) {
			case 0:
				// ���̂܂܃X�^�[�g
				m_dwGameStatus = TITLEANIM;
				m_pMapProc->SetTutorial(TRUE);
				m_pAnimWalk->Play(AUDIO_LOOP);
				break;

			case 1:
				// ��������X�^�[�g
				m_pCameraProc->GetCameraObjPtr()->SetCtrl(CAMERA_GAMEOVER);
				m_pCameraProc->GetCameraObjPtr()->SetCamBase();
				Load();
				m_dwGameStatus = GAMERETRY;
				break;

			case 2:
				// �`���[�g���A��
				m_bIsTutorial = TRUE;
				m_pBackForeProc->m_nDialogKind = DIALOG_TUTORIAL1;
				break;
			}
		}

		// �`���[�g���A���̕\��
		if (m_bIsTutorial)
		{
			switch (m_pBackForeProc->m_nDialogKind)
			{
			case DIALOG_TUTORIAL1:
				if (m_pBackForeProc->UpdateTutorialDialog(DIALOG_TUTORIAL1, false, true, false))
				{
					m_pBackForeProc->m_nDialogKind = DIALOG_TUTORIAL2;
				}
				break;

			case DIALOG_TUTORIAL2:
				if (m_pBackForeProc->UpdateTutorialDialog(DIALOG_TUTORIAL2, true, false, true))
				{
					switch (m_pBackForeProc->GetSelectNum())
					{
					case BUTTON_LEFT:
						m_pBackForeProc->m_nDialogKind = DIALOG_TUTORIAL1;
						break;

					case BUTTON_FINISH:
						m_pBackForeProc->m_nDialogKind = DIALOG_NONE;
						m_bIsTutorial = FALSE;
					default:
						break;
					}
				}
				break;

			}
		}
		break;

	case TITLEANIM:
		m_pTitleProc->TitleAnim();
		break;

	case GAMEMAIN:			// �Q�[���又��
		GameMain();
		break;

	case GAMECLEAR:			// �Q�[���N�����[����
		m_pTitleProc->GameClear();
		m_pBackForeProc->UpdateBlackOut();
		break;

	case GAMEOVER:			// �Q�[���I�[�o�[����
		if (m_pTitleProc->GameOver()) {
			switch (m_pTitleProc->m_nSelectNum) {
			case 0:
				// RETRY�{�^��
				m_dwGameStatus = GAMERETRY;
				m_bIsBoss = FALSE;
				break;

			case 1:
				// EXIT�{�^��
				m_dwGameStatus = GAMESAVE;
				break;
			}
		}
		// �Ó]����
		m_pBackForeProc->UpdateBlackOut();
		break;

	case GAMERETRY:
		if (m_pTitleProc->SelectWeapon()) {
			switch (m_pTitleProc->m_nSelectNum % 2) {
			case 0:			// ���u��������Z�b�g
				m_pPcProc->GetAbility()->ResetLLongAbility();
				break;

			case 1:			// �ߐڕ�������Z�b�g
				m_pPcProc->GetAbility()->ResetShortAbility();
				break;
			}
			m_pBackForeProc->SetBlackOut(TRUE);
		}
		if (m_pBackForeProc->GetWhiteOut())
		{
			m_dwGameStatus = GAMEMAIN;
			m_pGameOver->Stop();
			m_pBgm1->Play(AUDIO_LOOP);
			m_pPcProc->GetPcObjPtr()->SetRetryAllyNum();
			m_pMapProc->SetHierarchy(1);
			m_pMapProc->LoadMap1();
			m_pCameraProc->GetCameraObjPtr()->SetCtrl(CAMERA_GAMEMAIN);
			m_pCameraProc->GetCameraObjPtr()->SetCamBase();
			m_pBackForeProc->SetBackImageMovePos(firstFloorPosY);
			m_pBackForeProc->ResetOpenAllyNum();
		}
		m_pBackForeProc->UpdateBlackOut();
		break;

	case GAMESAVE:
		if (m_pTitleProc->Save()) {
			switch (m_pTitleProc->m_nSelectNum) {
			case 0:
				// No
				Save(0);
				break;

			case 1:
				// Yes
				Save(1);
				break;
			}
			m_dwGameStatus = GAMEEND;
		}
		break;


	case GAMEEND:			// �Q�[���I��
		PostQuitMessage(0);
		break;

	}

	MyImgui::ImguiRender();      // MyImgui���`�揈��    // -- 2020.11.15

	//��ʍX�V�i�o�b�N�o�b�t�@���t�����g�o�b�t�@�Ɂj
	//m_pD3D->m_pSwapChain->Present(1, 0);                   // 60fps Vsync
	m_pD3D->m_pSwapChain->Present(0, 0);                   // Vsync�Ȃ�



}

//------------------------------------------------------------------------
//
//	�Q�[���̃��C������	
//
//  �����@�Ȃ�
//
//	�߂�l �Ȃ�
//
//------------------------------------------------------------------------
void CGameMain::GameMain()
{

	//��ʃN���A�i���ۂ͒P�F�ŉ�ʂ�h��Ԃ������j
	float ClearColor[4] = { 0,0,0,1 };// �N���A�F�쐬�@RGBA�̏�
	m_pD3D->ClearRenderTarget(ClearColor); // ��ʃN���A

	// Map�̍X�V
	m_pMapProc->Update();

	// �o�b�̍X�V
	m_pPcProc->UpdateAll();

	// �����̍X�V
	m_pAllyProc->UpdateAll();

	// �G�̍X�V
	m_pEnmProc->UpdateAll();
	m_pEnmProc->UpdateHitcheck();

	// �G�{�X�̍X�V
	m_pEnmBossProc->UpdateAll();

	// ����̍X�V
	m_pWeaponProc->UpdateAll();

	// ��ԕ����̃��X�g�𖈉񂷂ׂăN�����[����   
	// �i�G���邢�͕���̒��ŋ�ԕ������g�p����Ƃ��́A�����ŏ����N�����[���s���j
	m_pMapProc->GetPcSpPart()->ClearList();
	m_pMapProc->GetEnmSpPart()->ClearList();
	// �R�c���ʂ̍X�V
	m_pEffectProc->UpdateAll();

	// Map�̕`��2
	// �������̃}�b�v�I�u�W�F�N�g��`�悷�邽�߁A�僋�[�v�̍Ō�Ɏ��s
	m_pMapProc->Render2();

	// �O�i�E�w�i�̍X�V
	m_pBackForeProc->Update();
	m_nScore = 0;

	// �J�����̍X�V
	m_pCameraProc->UpdateAll();

}

//------------------------------------------------------------------------
//
//	�A�v���P�[�V�����̏I������	
//
//  �����@�Ȃ�
//
//	�߂�l �Ȃ�
//
//------------------------------------------------------------------------
void CGameMain::Quit()
{
	;
}

void CGameMain::Save(int mode)
{
	const int BUFSIZE = 512;
	FILE* fp;
	CAbility* ab = m_pPcProc->GetAbility();

	fp = _tfopen(_T("Data/SaveData.csv"), _T("w"));
	if (fp) {
		// �Z�[�u�f�[�^����ɐݒ�
		fprintf(fp, "%d\n", Cipher(mode));
		// �K�w��
		fprintf(fp, "%d\n", Cipher(m_pMapProc->GetHierarchy()));
		// �ߐڕ���
		fprintf(fp, "%d,%d,%d,%d\n", Cipher(ab->GetShortWeapon()), Cipher(ab->GetAbilityLevel(0)), Cipher(ab->GetAbilityLevel(1)), Cipher(ab->GetAbilityLevel(2)));
		// ���u����
		fprintf(fp, "%d,%d,%d,%d\n", Cipher(ab->GetLongWeapon()), Cipher(ab->GetAbilityLevel(3)), Cipher(ab->GetAbilityLevel(4)), Cipher(ab->GetAbilityLevel(5)));
		// �e���U���g�f�[�^
		fprintf(fp, "%d,%d,%d,%d\n", Cipher(m_pBackForeProc->GetAllAllyNum()), Cipher(m_pBackForeProc->GetDeadAllyNum()), Cipher(m_pBackForeProc->GetDeadEnemyNum()), Cipher(m_pBackForeProc->GetDeadNum()));
		// �`���[�g���A���A���[�r�[�̃t���O
		fprintf(fp, "%d,%d,%d\n", Cipher(m_bIsBoxTutorial), Cipher(m_pBackForeProc->GetFirstAxe()), Cipher(m_pBackForeProc->GetFirstGun()));
		fclose(fp);
	}
}

BOOL CGameMain::Load()
{
	const int BUFSIZE = 512;
	TCHAR str[BUFSIZ];
	FILE* fp;
	CAbility* ab = m_pPcProc->GetAbility();
	int hierarchry = 0;

	fp = _tfopen(_T("Data/SaveData.csv"), _T("r"));

	if (fp) {
		int tmp = 0;
		_fgetts(str, BUFSIZE, fp);
		_stscanf_s(str, _T("%d"), &tmp);  // �Z�[�u�f�[�^�̗L��
		if (Cipher(tmp) == 0) {
			// �Z�[�u�f�[�^�Ȃ�
			return FALSE;
		}
		// �Z�[�u�f�[�^����
		_fgetts(str, BUFSIZE, fp);
		_stscanf_s(str, _T("%d"), &hierarchry);  // �O��K�w��

		_fgetts(str, BUFSIZE, fp);
		int tmp1 = 0, tmp2 = 0, tmp3 = 0;
		_stscanf_s(str, _T("%d,%d,%d,%d"), &tmp, &tmp1, &tmp2, &tmp3);  // �ߐڕ���
		ab->SetShortWeapon(Cipher(tmp));
		ab->SetShortWeaponAbility(Cipher(tmp1), Cipher(tmp2), Cipher(tmp3));

		_fgetts(str, BUFSIZE, fp);
		_stscanf_s(str, _T("%d,%d,%d,%d"), &tmp, &tmp1, &tmp2, &tmp3);  // ���u
		ab->SetLongWeapon(Cipher(tmp));
		ab->SetLongWeaponAbility(Cipher(tmp1), Cipher(tmp2), Cipher(tmp3));

		_fgetts(str, BUFSIZE, fp);
		_stscanf_s(str, _T("%d,%d,%d,%d"), &tmp, &tmp1, &tmp2, &tmp3);  // ���U���g�f�[�^
		m_pBackForeProc->SetAllAllyNum(Cipher(tmp));
		m_pBackForeProc->SetDeadAllyNum(Cipher(tmp1));
		m_pBackForeProc->SetDeadEnemyNum(Cipher(tmp2));
		m_pBackForeProc->SetDeadNum(Cipher(tmp3));

		_fgetts(str, BUFSIZE, fp);
		_stscanf_s(str, _T("%d,%d,%d"), &tmp, &tmp1, &tmp2);  // �`���[�g���A���\���t���O
		m_bIsBoxTutorial = Cipher(tmp);
		m_pBackForeProc->SetFirstAxe(Cipher(tmp1));
		m_pBackForeProc->SetFirstGun(Cipher(tmp2));
		fclose(fp);
	}
}

int CGameMain::Cipher(int num)
{
	int key = 0x45af6e5d; //�C�ӂ̈Í����L�[
	num = num ^ key;
	return num;
}

//------------------------------------------------------------------------   
//
//	�E�B���h�E���[�h�ƃt���X�N���[�����[�h��؂�ւ��鏈��	
//
//  �����@���[�h  0:�E�C���h�E���[�h�@1:�t���X�N���[���@-1:���[�h�؂�ւ��i�ȗ��l�j
//
//	�߂�l �Ȃ�
//
//------------------------------------------------------------------------
HRESULT CGameMain::ChangeScreenMode(int nMode)
{
	HRESULT Ret = S_OK;
	BOOL bFullScreen;  // �t���X�N���[�����[�h���H

	//	���݂̃X�N���[�����[�h�𓾂�
	m_pD3D->m_pSwapChain->GetFullscreenState(&bFullScreen, NULL);

	switch (nMode)
	{
	case 0:    // �E�C���h�E���[�h�ɂ���
		if( bFullScreen ) Ret = m_pD3D->m_pSwapChain->SetFullscreenState(false, NULL);
		break;
	case 1:    // �t���X�N���[�����[�h�ɂ���
		if( !bFullScreen ) Ret = m_pD3D->m_pSwapChain->SetFullscreenState(true, NULL);
		break;
	default:   // ���݂̃��[�h��؂�ւ���
		if (bFullScreen)
		{
			Ret = m_pD3D->m_pSwapChain->SetFullscreenState(false, NULL);
		}
		else {
			Ret = m_pD3D->m_pSwapChain->SetFullscreenState(true, NULL);
		}
	}

	return Ret;
}

