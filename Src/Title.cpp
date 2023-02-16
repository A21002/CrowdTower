//=============================================================================
//		�^�C�g���E�Q�[���N�����[�E�Q�[���I�[�o�[�̏���
//																Title.cpp
//=============================================================================
#include  "Title.h"
#include  "Map.h"
#include  "Playchar.h"
#include  "BackFore.h"
#include  "Camera.h"
#include  "Ability.h"

// ---------------------------------------------------------------------------
// �^�C�g���v���V�[�W���̃R���X�g���N�^
// ---------------------------------------------------------------------------
CTitleProc::CTitleProc(CGameMain* pGMain)
{
	m_pGMain = pGMain;

	m_pImageTitle = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/TITLE.png")); // �C���[�W�̃��[�h
	m_pImageClear = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/gameClearBack.png")); // �C���[�W�̃��[�h
	m_pImageClearScore = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/ClearScore.png")); // �C���[�W�̃��[�h
	m_pImageOver  = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/OVER.png")); // �C���[�W�̃��[�h
	m_pImageButton = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/Button.png"));
	m_pImageClearFore = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/gameClearFore.png"));
	m_pSprite = new CSprite(m_pGMain->m_pShader); // �X�v���C�g�̐���

	m_dwCntTime = 0;
	m_nSelectNum = 0;
	m_nBackImageMoveNum = 0;
	m_nIconMoveNum = 0;
	m_nCnt = 1 * 60;
	m_bIsSaveData = FALSE;
}

// ---------------------------------------------------------------------------
// �^�C�g���^�C�g���v���V�[�W���̃f�X�g���N�^
// ---------------------------------------------------------------------------
CTitleProc::~CTitleProc()
{
	SAFE_DELETE(m_pImageTitle);
	SAFE_DELETE(m_pImageClear);
	SAFE_DELETE(m_pImageClearScore);
	SAFE_DELETE(m_pImageClearFore);
	SAFE_DELETE(m_pImageOver);
	SAFE_DELETE(m_pImageButton);
	SAFE_DELETE(m_pSprite);
}

//------------------------------------------------------------------------
//	�^�C�g���̕\��	
//------------------------------------------------------------------------
BOOL CTitleProc::Title()
{
	BOOL bRet = FALSE;
	// �\��
	m_pSprite->Draw(m_pImageTitle, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);

	// �{�^���̕`��
	int bottonWidth = 331;
	int bottonHeight = 79;
	VECTOR2 button1Pos = VECTOR2(517, 410);
	VECTOR2 button2Pos = VECTOR2(517, 520);
	VECTOR2 button3Pos = VECTOR2(517, 630);
	int mouseOver = 8;

	// �ŏ�����X�^�[�g�{�^��
	if (CheckMousePos(button1Pos.x, button1Pos.y, bottonWidth, bottonHeight) && !m_pGMain->m_bIsTutorial)
	{
		m_pSprite->Draw(m_pImageButton, button1Pos.x, button1Pos.y + mouseOver, 1201, 0, bottonWidth, bottonHeight - mouseOver);
		m_nSelectNum = 0;     // �}�E�X�J�[�\�����Q�[�����[�h�X�v���C�g���ɂ���
		if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
		{
			m_pGMain->m_pClick_Button->Play();
			bRet = TRUE;  // �I��
		}
	}
	else {
		m_pSprite->Draw(m_pImageButton, button1Pos.x, button1Pos.y, 1533, 0, bottonWidth, bottonHeight);
	}

	if (m_bIsSaveData) {
		// ��������X�^�[�g
		if (CheckMousePos(button2Pos.x, button2Pos.y, bottonWidth, bottonHeight) && !m_pGMain->m_bIsTutorial)
		{
			m_pSprite->Draw(m_pImageButton, button2Pos.x, button2Pos.y + mouseOver, 1200, bottonHeight - 8, bottonWidth + 1, bottonHeight - mouseOver);
			m_nSelectNum = 1;     // �}�E�X�J�[�\�����Q�[�����[�h�X�v���C�g���ɂ���
			if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
			{
				m_pGMain->m_pClick_Button->Play();
				bRet = TRUE;  // �I��
			}
		}
		else {
			m_pSprite->Draw(m_pImageButton, button2Pos.x, button2Pos.y, 1533, bottonHeight, bottonWidth, bottonHeight);
		}
	}

	// �`���[�g���A���̕\���{�^��
	if (CheckMousePos(button3Pos.x, button3Pos.y, bottonWidth, bottonHeight) && !m_pGMain->m_bIsTutorial)
	{
		m_pSprite->Draw(m_pImageButton, button3Pos.x, button3Pos.y + mouseOver, 1200, (bottonHeight - mouseOver) * 2, bottonWidth + 1, bottonHeight - mouseOver);
		m_nSelectNum = 2;     // �}�E�X�J�[�\�����Q�[�����[�h�X�v���C�g���ɂ���
		if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
		{
			m_pGMain->m_pClick_Button->Play();
			bRet = TRUE;  // �I��
		}
	}
	else {
		m_pSprite->Draw(m_pImageButton, button3Pos.x, button3Pos.y, 1533, bottonHeight * 2, bottonWidth, bottonHeight);
	}

	return bRet;
}

//------------------------------------------------------------------------
//	�Q�[���X�^�[�g���̃A�j���[�V����	
//------------------------------------------------------------------------
void CTitleProc::TitleAnim()
{
	CSpriteImage* pPcIcon = m_pGMain->m_pBackForeProc->GetPcIcon();
	CSpriteImage* backImage = m_pGMain->m_pBackForeProc->GetImageFade();
	VECTOR2 iconPos = VECTOR2(927, 669);
	int backImageMoveY = 133;
	int iconMoveY = 334;

	if (backImageMoveY > m_nBackImageMoveNum) {
		m_nBackImageMoveNum++;
	}
	else if (iconMoveY > m_nIconMoveNum) {
		m_nIconMoveNum++;
	}
	if (iconMoveY > m_nIconMoveNum) {
		m_nIconMoveNum++;
	}

	// �`��
	m_pSprite->Draw(backImage, 0, 0, 0, backImage->m_dwImageHeight - WINDOW_HEIGHT - m_nBackImageMoveNum, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_pSprite->Draw(pPcIcon, iconPos.x, iconPos.y - m_nIconMoveNum, 0, 0, pPcIcon->m_dwImageWidth, pPcIcon->m_dwImageHeight);

	if (backImageMoveY <= m_nBackImageMoveNum && iconMoveY <= m_nIconMoveNum)
	{
		m_pGMain->m_pAnimWalk->Stop();
		if (m_nCnt > 0) {
			m_nCnt--;
		}
		else {
			if (!m_pGMain->m_pBackForeProc->GetBlackOut()) {
				m_pGMain->m_pBackForeProc->SetBlackOut(TRUE);
			}
			m_pGMain->m_pBackForeProc->UpdateBlackOut();
			if (m_pGMain->m_pBackForeProc->GetWhiteOut())
			{
				m_pGMain->m_dwGameStatus = GAMEMAIN;
				// �ŏ��̓}�b�v�P�̃��[�h
				m_pGMain->m_pMapProc->SetHierarchy(1);
				m_pGMain->m_pMapProc->LoadMap1();
				//m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetCamBase();
				m_pGMain->m_pBackForeProc->SetBackImageMovePos(backImage->m_dwImageHeight - WINDOW_HEIGHT - m_nBackImageMoveNum);
			}
		}
	}
}

//------------------------------------------------------------------------
//	�Q�[���N�����[�̕\��	
//------------------------------------------------------------------------
void CTitleProc::GameClear()
{
	// PC��`��
	m_pGMain->m_pPcProc->GetPcObjPtr()->UpdateGameClear();
	// �J������`��
	m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetCamBase();
	m_pGMain->m_pCameraProc->UpdateAll();

	// �\��
	m_pSprite->Draw(m_pImageClearFore, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);

	int spownAlly = m_pGMain->m_pBackForeProc->GetAllAllyNum();
	int deadAlly = m_pGMain->m_pBackForeProc->GetDeadAllyNum();
	int spownEnemy = m_pGMain->m_pBackForeProc->GetDeadEnemyNum();
	int retry = m_pGMain->m_pBackForeProc->GetDeadNum();

	VECTOR2 numPos = VECTOR2(902, 484);
	int numDiff = 72;
	TCHAR str[256] = _T("");

	_stprintf_s(str, _T("%d"), spownAlly);
	m_pGMain->m_pFont->Draw(numPos.x, numPos.y, str, 48, RGB(230, 31, 31), 1.0f, _T("GN-�L���SU����NA"));

	_stprintf_s(str, _T("%d"), deadAlly);
	m_pGMain->m_pFont->Draw(numPos.x, numPos.y + numDiff, str, 48, RGB(230, 31, 31), 1.0f, _T("GN-�L���SU����NA"));

	_stprintf_s(str, _T("%d"), spownEnemy);
	m_pGMain->m_pFont->Draw(numPos.x, numPos.y + numDiff * 2, str, 48, RGB(230, 31, 31), 1.0f, _T("GN-�L���SU����NA"));

	_stprintf_s(str, _T("%d"), retry);
	m_pGMain->m_pFont->Draw(numPos.x, numPos.y + numDiff * 3, str, 48, RGB(230, 31, 31), 1.0f, _T("GN-�L���SU����NA"));

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON) ||  m_dwCntTime > 600 )	//
	{
		m_pGMain->m_pBgm1->Stop();

		m_pGMain->m_dwGameStatus = GAMEEND;

		m_dwCntTime = 0;
	}
}

//------------------------------------------------------------------------
//	�Q�[���I�[�o�[�̕\��	
//------------------------------------------------------------------------
BOOL CTitleProc::GameOver()
{
	// PC��`��
	m_pGMain->m_pPcProc->GetPcObjPtr()->UpdateGameOver();
	// �J������`��
	//m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetCamBase();
	m_pGMain->m_pCameraProc->UpdateAll();

	// �\��
	m_pSprite->Draw(m_pImageButton, 420, 46, 0, 400, 493, 280);

	BOOL bRet = FALSE;
	// �}�b�v�摜�̉��ɕ\�����镶��

	const int nWidth = 400, nHeight = 100;
	const int nStartX = 483, nStartY = 500;
	const int nSrcX = 0, nSrcY = 0;
	const int mouseOver = 8;

	// ���g���C�{�^��
	if (CheckMousePos(nStartX, nStartY, nWidth, nHeight + mouseOver))
	{
		m_pSprite->Draw(m_pImageButton, nStartX, nStartY + mouseOver, 400, 0, nWidth, nHeight);
		m_nSelectNum = 0;     // �}�E�X�J�[�\�����Q�[�����[�h�X�v���C�g���ɂ���
		if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
		{
			m_pGMain->m_pClick_Button->Play();
			bRet = TRUE;  // �I��
		}
	}
	else {
		m_pSprite->Draw(m_pImageButton, nStartX, nStartY, 800, 0, nWidth, nHeight + mouseOver);
	}

	// EXIT�{�^��
	if (CheckMousePos(nStartX, nStartY + nHeight + 50, nWidth, nHeight + mouseOver))
	{
		m_pSprite->Draw(m_pImageButton, nStartX, nStartY + nHeight + 50 + mouseOver, 400, 101, nWidth, nHeight);
		m_nSelectNum = 1;     // �}�E�X�J�[�\�����Q�[�����[�h�X�v���C�g���ɂ���
		if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
		{
			m_pGMain->m_pClick_Button->Play();
			bRet = TRUE;  // �I��
		}
	}
	else {
		m_pSprite->Draw(m_pImageButton, nStartX, nStartY + nHeight + 50, 800, 108, nWidth, nHeight + mouseOver);
	}
	
	return bRet;
}

//------------------------------------------------------------------------
// ���g���C���A�����p�������I��
//------------------------------------------------------------------------
BOOL CTitleProc::SelectWeapon()
{
	BOOL bRet = FALSE;
	CAbility* ability = m_pGMain->m_pPcProc->GetAbility();
	VECTOR2 Button1 = VECTOR2(303, 428);
	VECTOR2 Button2 = VECTOR2(863, 428);
	VECTOR2 ButtonSize = VECTOR2(200, 200);

	// PC��`��
	m_pGMain->m_pPcProc->GetPcObjPtr()->UpdateGameOver();
	// �J������`��
	m_pGMain->m_pCameraProc->UpdateAll();

	// �u�����p�������I���v�̕\��
	m_pSprite->Draw(m_pImageButton, 174, 232, 500, 400, 1000, 100);

	int shortWeapon = ability->GetShortWeapon();
	int longWeapon = ability->GetLongWeapon();
	// ����A�C�R���{�^���̕\��
	m_pSprite->Draw(m_pImageButton, Button1.x, Button1.y, 200 * longWeapon, 200, ButtonSize.x, ButtonSize.y);
	m_pSprite->Draw(m_pImageButton, Button2.x, Button2.y, 200 * shortWeapon, 200, ButtonSize.x, ButtonSize.y);

	// ����������̔\�͂̕`��J�n�ʒu�̒���
	int abilityNum = ability->GetLongAbilityNum();
	VECTOR2 abilitySize = VECTOR2(130, 88);
	VECTOR2 abilityPos = VECTOR2(Button1.x + ButtonSize.x / 2 - abilitySize.x / 2 * abilityNum, Button1.y - 100);
	if (CheckMousePos(Button1.x, Button1.y, ButtonSize.x, ButtonSize.y)) {
		m_nSelectNum = longWeapon + 1;
		// �擾�ςݔ\�͂���ɕ\��
		for (int i = 0; i < ABILITY_KIND; i++)
		{
			if (ability->GetAbilityLevel(i) != 0)
			{
				m_pSprite->Draw(m_pGMain->m_pBackForeProc->GetImageAbilityDialog(), abilityPos.x, abilityPos.y, 600 + 130 * i, 500 + 100 * longWeapon, abilitySize.x, abilitySize.y);
				// ���̕`��
				for (int j = 0; j < ability->GetAbilityLevel(i); j++)
				{
					int starX = 17, starY = 19;
					int starPosX = abilityPos.x + 86;
					int startPosY = abilityPos.y + 17 + j * 21;
					m_pSprite->Draw(m_pGMain->m_pBackForeProc->GetImageAbilityDialog(), starPosX, startPosY, 1000, 600, starX, starY);
				}
				abilityPos += VECTOR2(abilitySize.x, 0);
			}
		}
		if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
		{
			m_pGMain->m_pClick_Button->Play();
			bRet = TRUE;  // �I��
		}
	}

	abilityNum = ability->GetShortAbilityNum();
	abilitySize = VECTOR2(130, 88);
	abilityPos = VECTOR2(Button2.x + ButtonSize.x / 2 - abilitySize.x / 2 * abilityNum, Button2.y - 100);
	if (CheckMousePos(Button2.x, Button2.y, ButtonSize.x, ButtonSize.y)) {
		m_nSelectNum = shortWeapon + 1;
		// �擾�ςݔ\�͂���ɕ\��
		for (int i = 0; i < ABILITY_KIND; i++)
		{
			if (ability->GetAbilityLevel(i + 3) != 0)
			{
				m_pSprite->Draw(m_pGMain->m_pBackForeProc->GetImageAbilityDialog(), abilityPos.x, abilityPos.y, 600 + 130 * i, 500 + 100 * shortWeapon, abilitySize.x, abilitySize.y);
				// ���̕`��
				for (int j = 0; j < ability->GetAbilityLevel(i + 3); j++)
				{
					int starX = 17, starY = 19;
					int starPosX = abilityPos.x + 86;
					int startPosY = abilityPos.y + 17 + j * 21;
					m_pSprite->Draw(m_pGMain->m_pBackForeProc->GetImageAbilityDialog(), starPosX, startPosY, 1000, 600, starX, starY);
				}
				abilityPos += VECTOR2(abilitySize.x, 0);
			}
		}
		if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
		{
			m_pGMain->m_pClick_Button->Play();
			bRet = TRUE;  // �I��
		}
	}

	return bRet;
}

//------------------------------------------------------------------------
//	�Q�[���I�����A�Z�[�u���邩�̑I���̕\��
//------------------------------------------------------------------------
BOOL CTitleProc::Save()
{
	BOOL bRet = FALSE;

	// PC��`��
	m_pGMain->m_pPcProc->GetPcObjPtr()->UpdateGameOver();
	// �J������`��
	m_pGMain->m_pCameraProc->UpdateAll();

	m_pSprite->Draw(m_pImageButton, 328, 293, 0, 683, 711, 358);

	// Yes
	if (CheckMousePos(389, 608, 277, 75)) {
		m_pSprite->Draw(m_pImageButton, 389, 608 + 8, 1076, 782, 276, 75);
		m_nSelectNum = 1;
		if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
		{
			m_pGMain->m_pClick_Button->Play();
			bRet = TRUE;  // �I��
		}
	}
	else {
		m_pSprite->Draw(m_pImageButton, 389, 608, 800, 782, 276, 83);
	}

	// No
	if (CheckMousePos(701, 608, 276, 75)) {
		m_pSprite->Draw(m_pImageButton, 701, 608 + 7, 1076, 700, 276, 75);
		m_nSelectNum = 0;
		if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
		{
			m_pGMain->m_pClick_Button->Play();
			bRet = TRUE;  // �I��
		}
	}
	else {
		m_pSprite->Draw(m_pImageButton, 701, 608, 800, 701, 275, 82);
	}


	return bRet;
}

//------------------------------------------------------------------------
// �}�E�X�J�[�\�����w�肵����`���ɂ��邩�ǂ����𔻒肷��
//
// ����  int nPosX,  int nPosY     ��`������W
//       int nWidth, int nHeight   ��`�̕��ƍ���
//
// �߂�l  BOOL  TRUE:��`���ɂ���    FALSE:��`���ɖ���
//------------------------------------------------------------------------
BOOL CTitleProc::CheckMousePos(int nPosX, int nPosY, int nWidth, int nHeight)
{
	POINT MPos = m_pGMain->m_pDI->GetMousePos();
	if (MPos.x >= nPosX && MPos.x <= nPosX + nWidth &&
		MPos.y >= nPosY && MPos.y <= nPosY + nHeight)
	{
		return TRUE;  // ��`���ɂ���
	}
	else {
		return FALSE;  // ��`���ɖ���
	}
}

//------------------------------------------------------------------------
//	�Z�[�u�f�[�^�����݂��邩�m�F
//------------------------------------------------------------------------
void CTitleProc::CheckSaveData()
{
	const int BUFSIZE = 512;
	TCHAR str[BUFSIZ];
	FILE* fp;

	fp = _tfopen(_T("Data/SaveData.csv"), _T("r"));

	int tmp = 0;
	_fgetts(str, BUFSIZE, fp);
	_stscanf_s(str, _T("%d"), &tmp);  // �Z�[�u�f�[�^�̗L��
	if (m_pGMain->Cipher(tmp) == 0) {
		// �Z�[�u�f�[�^�Ȃ�
		m_bIsSaveData = FALSE;
	}
	else {
		m_bIsSaveData = TRUE;
	}
}
