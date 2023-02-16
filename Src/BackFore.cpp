#include  "BackFore.h"
#include  "Ability.h"
#include  "Playchar.h"
#include  "Enemy.h"
#include  "EnemyBoss.h"
#include  "Ally.h"
#include  "Map.h"


// ---------------------------------------------------------------------------
// �O�i�E�w�i�v���V�[�W���̃R���X�g���N�^
// ---------------------------------------------------------------------------
CBackForeProc::CBackForeProc(CGameMain* pGMain)
{
	m_pGMain		= pGMain;
	m_pImageFore = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/foregrd3.png"));
	m_pImageUI = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/UI.png")); // �X�v���C�g�C���[�W�̃��[�h
	m_pImageAbilityDialog = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/AbilityDialog.png"));
	m_pImageFade = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/Fade.png"));
	m_pImagePcIcon = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/PcIcon.png"));
	m_pImageTutorial = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/tutorial.png"));
	m_pImageArrow = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/ArrowImage.png"));
	m_pSprite       = new CSprite(m_pGMain->m_pShader); // �X�v���C�g�̐���
	m_nDialogKind = DIALOG_NONE;
	m_nLogNum = 0;
	for (int i = 0; i < 36; i++) {
		m_nArrowDrawTime[i] = 0;
	}
	m_nFadePosY = WINDOW_HEIGHT + 1;
	m_nFadeDrawPosY = m_pImageFade->m_dwImageHeight - WINDOW_HEIGHT;
	m_nAnimMove = 0;
	m_nCnt = 60;
	m_fBlackOutAlpha = 0;
	m_nOpenAllyNum = 1;
	m_ntest = 0;
	m_nDeadNum = 0;
	m_nDeadAllyNum = 0;
	m_nDeadEnemyNum = 0;
	m_nAllAllyNum = PcConstruct::START_ALLY_NUM;
	m_fSwingAngle = 0.0f;
	m_fSwingWidth = 0.0f;
	m_vSwingWidth = VECTOR2(0.0f, 0.0f);
	m_bIsAnim = FALSE;
	m_bIsFade = FALSE;
	m_bIsFadeInFinish = FALSE;
	m_bIsFadeOutFinish = FALSE;
	m_bIsBlackOut = FALSE;
	m_bIsWhiteOut = FALSE;
	m_bIsSwing = FALSE;
	m_bIsWalkAudio = TRUE;
	m_bIsFirstAxe = FALSE;
	m_bIsFirstGun = FALSE;
}

// ---------------------------------------------------------------------------
// �O�i�E�w�i�v���V�[�W���̃f�X�g���N�^
// ---------------------------------------------------------------------------
CBackForeProc::~CBackForeProc()
{
	SAFE_DELETE(m_pImageArrow);
	SAFE_DELETE(m_pImageTutorial);
	SAFE_DELETE(m_pImagePcIcon);
	SAFE_DELETE(m_pImageFade);
	SAFE_DELETE(m_pImageAbilityDialog);
	SAFE_DELETE(m_pImageUI);
	SAFE_DELETE(m_pImageFore);
	SAFE_DELETE(m_pSprite);

}

//------------------------------------------------------------------------
//	�O�i�E�w�i�v���V�[�W���̍X�V	
//------------------------------------------------------------------------
void CBackForeProc::Update()
{
	TCHAR str[256] = { 0 };
	CAbility* ability = m_pGMain->m_pPcProc->GetAbility();

	if (m_pGMain->m_pEnmBossProc->GetEnmBossObjPtr()->GetMovieFinish())
	{ 

		// �U������
		if (m_bIsSwing)
		{
			// �U�����̌v�Z
			m_vSwingWidth.x = cos(XMConvertToRadians(m_fSwingAngle)) * m_fSwingWidth;
			m_vSwingWidth.y = sin(XMConvertToRadians(m_fSwingAngle)) * m_fSwingWidth;

			// �U���p�x�𔽓]
			m_fSwingAngle += 180.0f;
			if (m_fSwingAngle >= 360.0f)
			{
				m_fSwingAngle -= 360.0f;
			}
			// �U�ꕝ���k��
			m_fSwingWidth *= 0.8f;

			// �U�ꕝ��1�ȉ��ɂȂ�����U����~
			if (m_fSwingWidth <= 1.0f)
			{
				m_bIsSwing = FALSE;
			}
		}
		else {
			// 0�Ƀ��Z�b�g����
			m_vSwingWidth = VECTOR2(0.0f, 0.0f);
		}

		// UI�\��
		int   DestX = 0, DestY = 0;
		m_pSprite->Draw(m_pImageUI, DestX + m_vSwingWidth.x, DestY + m_vSwingWidth.y, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		// �K�w���\��
		DestX = 1260;
		DestY = 50;
		_stprintf_s(str, _T("%d"), m_pGMain->m_pMapProc->GetHierarchy());
		m_pGMain->m_pFont->Draw(DestX + m_vSwingWidth.x, DestY + m_vSwingWidth.y, str, 48, RGB(41, 228, 98), 1.0f, _T("GN-�L���SU����NA"));

		// HP�\��
		DestX = 125;
		DestY = 670;
		_stprintf_s(str, _T("%d"), m_pGMain->m_pPcProc->GetPcObjPtr()->GetHp());
		m_pGMain->m_pFont->Draw(DestX + m_vSwingWidth.x, DestY + m_vSwingWidth.y, str, 60, RGB(253, 220, 0), 1.0f, _T("GN-�L���SU����NA"));

		// �c�e�\������
		if (ability->GetLongWeapon() == GUN)
		{
			// �����[�h�A�C�R��
			m_pSprite->Draw(m_pImageFore, 41 + m_vSwingWidth.x, 500 + m_vSwingWidth.y, 300, 200, 80, 26);

			int num = m_pGMain->m_pPcProc->GetPcObjPtr()->GetGunMagazine();
			m_pSprite->Draw(m_pImageFore, 41 + m_vSwingWidth.x, 552 + m_vSwingWidth.y, 0, 410, 23 * num, 30);
		}

		// �{�X��HP�o�[�̕\��
		if (m_pGMain->m_bIsBoss)
		{
			VECTOR2 BarPos = VECTOR2(144, 594);
			VECTOR2 BarSize = VECTOR2(1078, 46);
			VECTOR2 Bar2Size = VECTOR2(1044, 14);
			VECTOR2 BarDiff = VECTOR2(16, 16);

			float ratio = (float)m_pGMain->m_pEnmBossProc->GetEnmBossObjPtr()->GetHp() / (float)m_pGMain->m_pEnmBossProc->GetEnmBossObjPtr()->GetMaxHp();

			m_pSprite->Draw(m_pImageFore, BarPos.x, BarPos.y, 0, 500, BarSize.x, BarSize.y);
			m_pSprite->Draw(m_pImageFore, BarPos.x + BarDiff.x, BarPos.y + BarDiff.y, 0, 600, Bar2Size.x * ratio, Bar2Size.y);
		}

		// �\�͂̉�@�E������Ԃ̕\��
		int abilityLevel[2][3];

		// �z��Ɋi�[
		// ���u
		abilityLevel[0][0] = ability->GetAbilityLevel(0);
		abilityLevel[0][1] = ability->GetAbilityLevel(1);
		abilityLevel[0][2] = ability->GetAbilityLevel(2);

		// �ߐ�
		abilityLevel[1][0] = ability->GetAbilityLevel(3);
		abilityLevel[1][1] = ability->GetAbilityLevel(4);
		abilityLevel[1][2] = ability->GetAbilityLevel(5);

		int maxCoolTime = 60 * PcConstruct::WAITTIME_MELEE_WEAPON * (1 - 0.3 * abilityLevel[1][0]);
		int nowCoolTime = m_pGMain->m_pPcProc->GetPcObjPtr()->GetCoolTime();

		float ratio = (float)nowCoolTime / (float)maxCoolTime;

		// �������Ă��镐��̃A�C�R����\��
		m_pSprite->Draw(m_pImageAbilityDialog, 26 + m_vSwingWidth.x, 20 + m_vSwingWidth.y, 1000 + 87 * ability->GetLongWeapon(), 500, 87, 87);
		m_pSprite->Draw(m_pImageAbilityDialog, 26 + m_vSwingWidth.x, 20 + 116 + m_vSwingWidth.y, 1000 + 87 * ability->GetShortWeapon(), 500, 87, 87);
		m_pSprite->Draw(m_pImageAbilityDialog, 26 + m_vSwingWidth.x, 20 + 116 + m_vSwingWidth.y, 1400 + 87 * ability->GetShortWeapon(), 500, 87, 87 * ratio);

		int abiNum = 0;
		for (int i = 0; i < 2; i++) {
			int weaponId;
			if (i == 0) {
				weaponId = ability->GetLongWeapon();
			}
			else {
				weaponId = ability->GetShortWeapon();
			}
			for (int j = 0; j < 3; j++) {
				if (abilityLevel[i][j] != 0) {
					// �A�r���e�B�A�C�R���̕`��
					int iconX = 130, iconY = 88;
					int posX = 113 + 12 + abiNum * (iconX + 12);
					int posY = 20 + i * (iconY + 29);
					m_pSprite->Draw(m_pImageAbilityDialog, posX + m_vSwingWidth.x, posY + m_vSwingWidth.y, 600 + 130 * j, 500 + 100 * weaponId, iconX, iconY);

					// ���̕`��
					for (int k = 0; k < abilityLevel[i][j]; k++) {
						int starX = 17, starY = 19;
						int starPosX = posX + 86;
						int startPosY = posY + 17 + k * 21;
						m_pSprite->Draw(m_pImageAbilityDialog, starPosX + m_vSwingWidth.x, startPosY + m_vSwingWidth.y, 1000, 600, starX, starY);
					}
					abiNum++;
				}
			}
			abiNum = 0;
		}

	}


	// �A�b�v�O���[�hUI�̕\��
	switch (m_nDialogKind) {
	case DIALOG_NONE:
		// UI��\�����Ȃ�
		break;

	case DIALOG_CHECK:
		// �󔠂��J���邩�m�FUI��\��
		if (UpdateCheckDialog())
		{
			switch (m_nSelectNum)
			{
			case 1:
				// �J����
				if (m_pGMain->m_pPcProc->GetPcObjPtr()->GetHp() - 1 >= m_nOpenAllyNum) {
					// ������10�l�ȏ�
					m_pGMain->m_pAllyProc->DeleteObj(m_nOpenAllyNum);
					m_nOpenAllyNum += 1;
					m_nDialogKind = DIALOG_ABILITY;
				}
				else {
					m_nDialogKind = DIALOG_ERROR;
				}
				break;

			case 2:
				// �J���Ȃ�
				m_nDialogKind = DIALOG_NONE;
				m_pGMain->m_isMove = TRUE;
				break;
			}
		}
		break;

	case DIALOG_ABILITY:
		// �A�b�v�O���[�hUI�̕\��
		if (UpdateDialog()) {
			// �I�������̂ōēx��\��
			m_nDialogKind = DIALOG_NONE;
			m_pGMain->m_isMove = TRUE;
			if (m_nSelectNum != 4)
			{
				// �\�͎擾�Ȃ��łȂ��ꍇ
				ability->AddAbilityLevel(m_nSelectAbility[m_nSelectNum]);
			}
		}
		break;

	case DIALOG_ERROR:
		if (UpdateErrorDialog()) {
			m_nDialogKind = DIALOG_NONE;
			m_pGMain->m_isMove = TRUE;
		}
		break;

	case DIALOG_TUTORIAL1:
		if (UpdateTutorialDialog(DIALOG_TUTORIAL1, false, true, false))
		{
			m_nDialogKind = DIALOG_TUTORIAL2;
		}
		break;

	case DIALOG_TUTORIAL2:
		if (UpdateTutorialDialog(DIALOG_TUTORIAL2, true, true, false))
		{
			switch (m_nSelectNum)
			{
			case BUTTON_LEFT:
				m_nDialogKind = DIALOG_TUTORIAL1;
				break;

			case BUTTON_RIGHT:
				m_nDialogKind = DIALOG_TUTORIAL_SWORD;
				break;
			}
		}
		break;

	case DIALOG_TUTORIAL3:
		if (UpdateTutorialDialog(DIALOG_TUTORIAL3, false, false, true))
		{
			m_nDialogKind = DIALOG_NONE;
			m_pGMain->m_isMove = TRUE;
		}
		break;

	case DIALOG_TUTORIAL_SWORD:
		if (UpdateTutorialDialog(DIALOG_TUTORIAL_SWORD, true, true, false))
		{
			switch (m_nSelectNum)
			{
			case BUTTON_LEFT:
				m_nDialogKind = DIALOG_TUTORIAL2;
				break;

			case BUTTON_RIGHT:
				m_nDialogKind = DIALOG_TUTORIAL_ARROW;
				break;
			}
		}
		break;

	case DIALOG_TUTORIAL_ARROW:
		if (UpdateTutorialDialog(DIALOG_TUTORIAL_ARROW, true, false, true))
		{
			switch (m_nSelectNum)
			{
			case BUTTON_LEFT:
				m_nDialogKind = DIALOG_TUTORIAL_SWORD;
				break;

			case BUTTON_FINISH:
				m_nDialogKind = DIALOG_NONE;
				m_pGMain->m_isMove = TRUE;
				m_pGMain->m_pMapProc->SetTutorial(FALSE);
				break;
			}
		}
		break;

	case DIALOG_TUTORIAL_AXE:
		if (UpdateTutorialDialog(DIALOG_TUTORIAL_AXE, false, false, true))
		{
			m_nDialogKind = DIALOG_NONE;
			m_pGMain->m_isMove = TRUE;
			m_pGMain->m_pPcProc->GetPcObjPtr()->SetCanMove(TRUE);
		}
		break;

	case DIALOG_TUTORIAL_GUN:
		if (UpdateTutorialDialog(DIALOG_TUTORIAL_GUN, false, false, true))
		{
			m_nDialogKind = DIALOG_NONE;
			m_pGMain->m_isMove = TRUE;
			m_pGMain->m_pPcProc->GetPcObjPtr()->SetCanMove(TRUE);
		}
		break;
	}

	// ���̕`��
	VECTOR2 vArrowSize = VECTOR2(161, 161);
	int slope = 392;
	m_ntest++;
	if (m_ntest >= 360) {
		m_ntest = 0;
	}

	
	for (int i = 0; i < 36; i++) {
		if (m_nArrowDrawTime[i] > 0) {
			m_nArrowDrawTime[i]--;

			VECTOR2 vPos = VECTOR2(sin(XMConvertToRadians(i * 10)) * slope, cos(XMConvertToRadians(i * 10)) * slope);

			// ��ʊO�ɔ�яo��̂�h�~
			if (vPos.y < 0) {
				// ��ʉ��ɍs���̂�h�~
				vPos.y += vArrowSize.y;
			}
			if (vPos.x < 0) {
				vPos.x -= vArrowSize.x;
			}

			m_pSprite->Draw(m_pImageArrow, vPos.x + WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - vPos.y, vArrowSize.x* (i % 9), vArrowSize.y* (i / 9), vArrowSize.x, vArrowSize.y);

		}
	}
	
	// �t�F�[�h����
	if (m_bIsAnim) {
		UpdateAnim();
	}
	else {
		UpdateFade();
	}
	UpdateBlackOut();
}

// ---------------------------------------------------------------------------
// Upgrade�_�C�A���O�̕\��
// TRUE : �I������  FALSE : �I�����Ă��Ȃ�
// ---------------------------------------------------------------------------
BOOL CBackForeProc::UpdateCheckDialog()
{
	BOOL bRet = FALSE;

	VECTOR2 DialogPos = VECTOR2(328, 229);
	VECTOR2 OkPos = VECTOR2(380, 552);
	VECTOR2 NoPos = VECTOR2(710, 552);

	VECTOR2 DialogSize = VECTOR2(711, 360);
	VECTOR2 ButtonSize = VECTOR2(276, 75);

	VECTOR2 NumPos = VECTOR2(510, 415);
	const int OverMouse = 7;

	// �`��
	// �_�C�A���O
	m_pSprite->Draw(m_pImageAbilityDialog, DialogPos.x, DialogPos.y, 2049, 683, DialogSize.x, DialogSize.y);
	TCHAR str[256];
	_stprintf_s(str, _T("%d"), m_nOpenAllyNum);
	m_pGMain->m_pFont->Draw(NumPos.x, NumPos.y, str, 38, RGB(230, 0, 19), 1.0f, _T("GN-�L���SU����NA"));

	// �{�^��
	// Yes
	if (CheckMousePos(OkPos.x, OkPos.y, ButtonSize.x, ButtonSize.y))
	{
		m_pSprite->Draw(m_pImageAbilityDialog, OkPos.x, OkPos.y + OverMouse, 1676, 782, ButtonSize.x, ButtonSize.y);
		m_nSelectNum = 1;     // �}�E�X�J�[�\�����Q�[�����[�h�X�v���C�g���ɂ���
		if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
		{
			m_pGMain->m_pClick_Button->Play();
			bRet = TRUE;  // �I��
		}
	}
	else {
		m_pSprite->Draw(m_pImageAbilityDialog, OkPos.x, OkPos.y, 1400, 782, ButtonSize.x, ButtonSize.y + OverMouse);
	}

	if (CheckMousePos(NoPos.x, NoPos.y, ButtonSize.x, ButtonSize.y))
	{
		m_pSprite->Draw(m_pImageAbilityDialog, NoPos.x, NoPos.y + OverMouse, 1676, 700, ButtonSize.x, ButtonSize.y);
		m_nSelectNum = 2;     // �}�E�X�J�[�\�����Q�[�����[�h�X�v���C�g���ɂ���
		if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
		{
			m_pGMain->m_pClick_Button->Play();
			bRet = TRUE;  // �I��
		}
	}
	else {
		m_pSprite->Draw(m_pImageAbilityDialog, NoPos.x, NoPos.y, 1400, 700, ButtonSize.x, ButtonSize.y + OverMouse);
	}

	return bRet;
}

// ---------------------------------------------------------------------------
// Error�_�C�A���O�̕\��
// TRUE : �I������  FALSE : �I�����Ă��Ȃ�
// ---------------------------------------------------------------------------
BOOL CBackForeProc::UpdateErrorDialog()
{
	BOOL bRet = FALSE;
	VECTOR2 DialogPos = VECTOR2(328, 229);
	VECTOR2 ButtonPos = VECTOR2(545, 552);

	VECTOR2 DialogSize = VECTOR2(711, 360);
	VECTOR2 ButtonSize = VECTOR2(276, 75);
	const int OverMouse = 7;

	// �`��
	// �_�C�A���O
	m_pSprite->Draw(m_pImageAbilityDialog, DialogPos.x, DialogPos.y, 2784, 683, DialogSize.x, DialogSize.y);

	// �{�^��
	if (CheckMousePos(ButtonPos.x, ButtonPos.y, ButtonSize.x, ButtonSize.y))
	{
		m_pSprite->Draw(m_pImageAbilityDialog, ButtonPos.x, ButtonPos.y + OverMouse, 1676, 782, ButtonSize.x, ButtonSize.y);
		m_nSelectNum = 1;     // �}�E�X�J�[�\�����Q�[�����[�h�X�v���C�g���ɂ���
		if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
		{
			m_pGMain->m_pClick_Button->Play();
			bRet = TRUE;  // �I��
		}
	}
	else {
		m_pSprite->Draw(m_pImageAbilityDialog, ButtonPos.x, ButtonPos.y, 1400, 782, ButtonSize.x, ButtonSize.y + OverMouse);
	}

	return bRet;
}

// ---------------------------------------------------------------------------
// Upgrade�_�C�A���O�̕\��
// TRUE : �I������  FALSE : �I�����Ă��Ȃ�
// ---------------------------------------------------------------------------
BOOL CBackForeProc::UpdateDialog()
{
	BOOL bRet = FALSE;
	// �}�b�v�摜�̉��ɕ\�����镶��

	const int nStartX = 450, nStartY = 146;
	const int nWidth = 466, nHeight = 156;
	const int nSrcX = 600, nSrcY = 0;

	// �w�i�̕\��
	m_pSprite->Draw(m_pImageAbilityDialog, 431, 74, 0, 0, 504, 663);

	// �Q�[�����[�h�Z���N�g�\��
	for (int i = 0; i < 3; i++)
	{
		int x = m_nSelectAbility[i] / 3, y = m_nSelectAbility[i] % 3;
		// �I�����Ă��Ȃ����[�h
		m_pSprite->Draw(m_pImageAbilityDialog, nStartX, nStartY + (nHeight + 19) * i, nSrcX + x * nWidth, nSrcY + y * nHeight, nWidth, nHeight);
	}

	// �}�E�X�ɂ��Q�[�����[�h�Z���N�g(1�`4)
	for (int i = 0; i < 3; i++)
	{
		if (CheckMousePos(nStartX, nStartY + (nHeight + 19) * i, nWidth, nHeight))
		{
			m_nSelectNum = i;     // �}�E�X�J�[�\�����Q�[�����[�h�X�v���C�g���ɂ���
		}
	}

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
	{
		m_pGMain->m_pClick_Ability->Play();
		bRet = TRUE;  // �I��
	}

	// �I���Ȃ��̃{�^��
	VECTOR2 exitPos = VECTOR2(514, 674);
	VECTOR2 exitSize = VECTOR2(334, 61);
	m_pSprite->Draw(m_pImageAbilityDialog, exitPos.x, exitPos.y, 0, 700, exitSize.x, exitSize.y);
	if (CheckMousePos(exitPos.x, exitPos.y, exitSize.x, exitSize.y))
	{
		m_nSelectNum = 4;
		if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
		{
			m_pGMain->m_pClick_Button->Play();
			bRet = TRUE;  // �I��
		}

	}
	return bRet;
}

// ---------------------------------------------------------------------------
// Tutorial�̃_�C�A���O�̕\��
// TRUE : �I������  FALSE : �I�����Ă��Ȃ�
// ---------------------------------------------------------------------------
BOOL CBackForeProc::UpdateTutorialDialog(int page, bool left, bool right, bool finish)
{
	BOOL bRet = FALSE;
	VECTOR2 vDialogSize = VECTOR2(641, 453);
	VECTOR2 vDialogPos = VECTOR2(362, 230);

	VECTOR2 vButton1Size = VECTOR2(41, 160);
	VECTOR2 vButton1PosLeft = VECTOR2(342, 377);
	VECTOR2 vButton1PosRight = VECTOR2(983, 377);

	VECTOR2 vButton2Size = VECTOR2(246, 46);
	VECTOR2 vButton2Pos = VECTOR2(560, 651);

	int nOverMouse = 7;

	// �_�C�A���O�̕\��
	m_pSprite->Draw(m_pImageTutorial, vDialogPos.x, vDialogPos.y, vDialogSize.x * (page - DIALOG_TUTORIAL1), 0, vDialogSize.x, vDialogSize.y);

	if (left) {
		// ���{�^��
		if (CheckMousePos(vButton1PosLeft.x, vButton1PosLeft.y, vButton1Size.x, vButton1Size.y))
		{
			m_pSprite->Draw(m_pImageTutorial, vButton1PosLeft.x, vButton1PosLeft.y + nOverMouse, 0, 453 + vButton1Size.y + nOverMouse, vButton1Size.x, vButton1Size.y);
			m_nSelectNum = BUTTON_LEFT;     // �}�E�X�J�[�\�����Q�[�����[�h�X�v���C�g���ɂ���
			if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
			{
				m_pGMain->m_pClick_Button->Play();
				bRet = TRUE;  // �I��
			}
		}
		else {
			m_pSprite->Draw(m_pImageTutorial, vButton1PosLeft.x, vButton1PosLeft.y, 0, 453, vButton1Size.x, vButton1Size.y + nOverMouse);
		}
	}

	if (right) {
		// �E�{�^��
		if (CheckMousePos(vButton1PosRight.x, vButton1PosRight.y, vButton1Size.x, vButton1Size.y))
		{
			m_pSprite->Draw(m_pImageTutorial, vButton1PosRight.x, vButton1PosRight.y + nOverMouse, vButton1Size.x, 453 + vButton1Size.y + nOverMouse, vButton1Size.x, vButton1Size.y);
			m_nSelectNum = BUTTON_RIGHT;     // �}�E�X�J�[�\�����Q�[�����[�h�X�v���C�g���ɂ���
			if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
			{
				m_pGMain->m_pClick_Button->Play();
				bRet = TRUE;  // �I��
			}
		}
		else {
			m_pSprite->Draw(m_pImageTutorial, vButton1PosRight.x, vButton1PosRight.y, vButton1Size.x, 453, vButton1Size.x, vButton1Size.y + nOverMouse);
		}
	}

	if (finish) {
		// �I���{�^��
		if (CheckMousePos(vButton2Pos.x, vButton2Pos.y, vButton2Size.x, vButton2Size.y))
		{
			m_pSprite->Draw(m_pImageTutorial, vButton2Pos.x, vButton2Pos.y + nOverMouse, vButton1Size.x * 2, 453 + vButton2Size.y + nOverMouse, vButton2Size.x, vButton2Size.y);
			m_nSelectNum = BUTTON_FINISH;     // �}�E�X�J�[�\�����Q�[�����[�h�X�v���C�g���ɂ���
			if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
			{
				m_pGMain->m_pClick_Button->Play();
				bRet = TRUE;  // �I��
			}
		}
		else {
			m_pSprite->Draw(m_pImageTutorial, vButton2Pos.x, vButton2Pos.y, vButton1Size.x * 2, 453, vButton2Size.x, vButton2Size.y + nOverMouse);
		}
	}
	return bRet;
}

// ---------------------------------------------------------------------------
// Upgrade�_�C�A���O�ɕ\������I�����̑I��
// ---------------------------------------------------------------------------
void CBackForeProc::SetSelectAbility()
{
	// ������
	for (int i = 0; i < 3; i++) {
		m_nSelectAbility[i] = -1;
	}

	for (int i = 0; i < 3; i++) {
		int num = Random(0, WEAPON_KIND * ABILITY_KIND - 1);

		// ���肷��܂Ŗ������[�v
		while (1) {
			// ���ɑI�΂ꂽ�l��
			for (int j = 0; j < 3 && j < i; j++) {
				if (num == m_nSelectAbility[j])
				{
					// �I�΂�Ă����ꍇ
					num++;
					if (num == WEAPON_KIND * ABILITY_KIND)
					{
						num = 0;
					}
					j = -1;
					continue;
				}
			}

			// �ő勭���ς݂�
			if (!m_pGMain->m_pPcProc->GetAbility()->CheckAbilityLevel(num))
			{
				// �ő勭���ς�
				num++;
				if (num == WEAPON_KIND * ABILITY_KIND)
				{
					num = 0;
				}
				continue;
			}
			break;
		}
		m_nSelectAbility[i] = num;
	}
}

// ---------------------------------------------------------------------------
// UI�̐U���X�^�[�g����
// ---------------------------------------------------------------------------
void CBackForeProc::StartSwing(float angle)
{
	if (!m_bIsSwing) {
		// �h��Ă��Ȃ����
		m_fSwingAngle = angle;		// �h�炷�p�x
		m_fSwingWidth = 20.0f;		// �h�炷��
		m_bIsSwing = TRUE;			// Update�����ŗh�炷�t���O
	}
}

//------------------------------------------------------------------------
// �G����ʊO����U�������ۂɍU�������������w�����̕`��J�n����
//------------------------------------------------------------------------
void CBackForeProc::DrawArrowAngle(MATRIX4X4 mWorld)
{
	// ���[���h���W����X�N���[�����W�ɕϊ�
	VECTOR3 vScreenPos = XMVector3Project(GetPositionVector(mWorld), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
										 0.0f, 1.0f, m_pGMain->m_mProj, m_pGMain->m_mView, XMMatrixIdentity());

	// ��ʊO�ɂ��邩�̃`�F�b�N
	if ((0 <= vScreenPos.x && vScreenPos.x <= WINDOW_WIDTH) &&
		(0 <= vScreenPos.y && vScreenPos.y <= WINDOW_HEIGHT)) {
	}else
	{
		// ��ʂ̒��S�ƃX�N���[�����W�̊p�x
		VECTOR2 vCenterPos = VECTOR2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
		float fAngle = XMConvertToDegrees(atan2(vScreenPos.x - vCenterPos.x, vCenterPos.y - vScreenPos.y));

		// �Ή�����v�f���v�Z
		int index = (int)fAngle / 10;
		if (index < 0)
		{
			return;
		}
		// ���ɕ`�悪�J�n����Ă��邩
		if (m_nArrowDrawTime[index] <= 0) {
			// �`�悪�J�n����Ă��Ȃ�
			m_nArrowDrawTime[index] = 1;
		}
	}
}

//------------------------------------------------------------------------
// �}�E�X�J�[�\�����w�肵����`���ɂ��邩�ǂ����𔻒肷��
//
// ����  int nPosX,  int nPosY     ��`������W
//       int nWidth, int nHeight   ��`�̕��ƍ���
//
// �߂�l  BOOL  TRUE:��`���ɂ���    FALSE:��`���ɖ���
//------------------------------------------------------------------------
BOOL CBackForeProc::CheckMousePos(int nPosX, int nPosY, int nWidth, int nHeight)
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

// ---------------------------------------------------------------------------
// �}�b�v�ړ����̃t�F�[�h�C���A�t�F�[�h�A�E�g���鏈��
// ---------------------------------------------------------------------------
void CBackForeProc::UpdateFade()
{
	VECTOR2 size = VECTOR2(1366.0f, 1152.0f);
	const int fadeSpeed = 24;
	int i;
	if (m_bIsFade) {
		// �t�F�[�h�C��
		m_pGMain->m_pPcProc->GetPcObjPtr()->SetCanMove(FALSE);
		if (m_nFadePosY > -(WINDOW_HEIGHT / 2) + 200) {
			// �t�F�[�h�C����
			m_nFadePosY -= fadeSpeed;
		}
		else {
			// �t�F�[�h�C������
			m_bIsFadeInFinish = TRUE;			// �t�F�[�h�C�������ʒm
			m_bIsFadeOutFinish = FALSE;			// �t�F�[�h�A�E�g�����ʒm���Z�b�g
			m_bIsAnim = TRUE;					// �A�j���[�V�����X�^�[�g
		}

		// ���ߕ��̕`��
		for (i = 0; i < 100; i++) {
			// ���X�ɕs�����ɂ��ăO���f�[�V�������o��
			m_pSprite->Draw(m_pImageFade, 0, m_nFadePosY + i * 2, 0, m_nFadeDrawPosY - 100 * 2 + i * 2, size.x, 2, 0.01 * i);
		}
		// �s����������`��
		m_pSprite->Draw(m_pImageFade, 0, m_nFadePosY + i * 2, 0, m_nFadeDrawPosY, WINDOW_WIDTH, WINDOW_HEIGHT);
	}

	else {
		// �t�F�[�h�A�E�g
		if (m_nFadePosY <= WINDOW_HEIGHT) {
			// �t�F�[�h�A�E�g��
			m_nFadePosY += fadeSpeed;
		}
		else {
			// �t�F�[�h�A�E�g�I��
			m_bIsFadeOutFinish = TRUE;			// �t�F�[�h�A�E�g�����ʒm
			m_bIsFadeInFinish = FALSE;			// �t�F�[�h�C�������ʒm
		}

		// ���ߕ��̕`��
		for (i = 0; i < 100; i++) {
			// ���X�ɕs�����ɂ��ăO���f�[�V�������o��
			m_pSprite->Draw(m_pImageFade, 0, m_nFadePosY + i * 2, 0, m_nFadeDrawPosY - 100 * 2 + i * 2, size.x, 2, 0.01 * i);
		}
		// �s����������`��
		m_pSprite->Draw(m_pImageFade, 0, m_nFadePosY + i * 2, 0, m_nFadeDrawPosY, WINDOW_WIDTH, WINDOW_HEIGHT);
	}
}

// ---------------------------------------------------------------------------
// �}�b�v�ړ����̃A�j���[�V��������
// ---------------------------------------------------------------------------
void CBackForeProc::UpdateAnim()
{
	const int moveNum = 187;		// �e�K�w�ǂ����̕�
	const int moveSpeed = 2;		// �A�C�R���ړ����x

	// ���Đ�
	if (m_bIsWalkAudio)
	{
		m_pGMain->m_pAnimWalk->Play(AUDIO_LOOP);
		m_bIsWalkAudio = FALSE;
	}

	if (moveNum > m_nAnimMove) {
		m_nAnimMove += moveSpeed;
	}
	if (m_nAnimMove > 188) {
		m_nAnimMove = 0;
	}

	// �`��
	// ���̃C���X�g
	m_pSprite->Draw(m_pImageFade, 0, 0, 0, m_nFadeDrawPosY - m_nAnimMove, WINDOW_WIDTH, WINDOW_HEIGHT);
	// �v���C���[�A�C�R��
	m_pSprite->Draw(m_pImagePcIcon, 995, 335, 0, 0, m_pImagePcIcon->m_dwImageWidth, m_pImagePcIcon->m_dwImageHeight);

	// ���K�w�ɓ��B���Ă���̑ҋ@����
	if (moveNum <= m_nAnimMove) {
		m_pGMain->m_pAnimWalk->Stop();
		// �ҋ@
		if (m_nCnt > 0) {
			m_nCnt--;
		}

		// �A�j���[�V���������I��
		else {
			m_nCnt = 60;						// �ҋ@���ԃ��Z�b�g
			m_bIsAnim = FALSE;					// �A�j���[�V�����t���O
			m_bIsWalkAudio = TRUE;
			m_nFadeDrawPosY -= m_nAnimMove;		// ���̃C���X�g�̕`��ʒu
			m_nAnimMove = 0;					// ����A�j���[�V�����p�Ɍ��݂̍��v�ړ��ʂ����Z�b�g
			m_nDialogKind = DIALOG_NONE;		// �t�F�[�h�C���ȑO�Ƀ_�C�A���O���\������Ă���ꍇ�̑΍�
			m_pGMain->m_pPcProc->GetPcObjPtr()->SetCanMove(TRUE);
		}
	}
}

// ---------------------------------------------------------------------------
// �u���b�N�A�E�g����
// ---------------------------------------------------------------------------
void CBackForeProc::UpdateBlackOut()
{
	// �u���b�N�A�E�g����������
	if (m_bIsBlackOut)
	{
		// ���̒����`���w��̓��ߒl�ŕ`��
		m_pSprite->DrawRect(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0, m_fBlackOutAlpha);

		if (!m_bIsWhiteOut) {
			// ���X�ɓ��ߒl�������Ă���
			m_fBlackOutAlpha += 0.04f;
			if (m_fBlackOutAlpha > 1.00f)
			{
				// ���S�ɍ��ɂȂ����瓧�ߒl���グ�Ă����悤�Ƀt���O�𗧂Ă�
				m_bIsWhiteOut = TRUE;
			}
		}
		else {
			// ���X�ɓ��ߒl���グ�Ă���
			m_fBlackOutAlpha -= 0.04f;
			if (m_fBlackOutAlpha <= 0.00f)
			{
				// �����ɂȂ����珈�����I������
				m_bIsBlackOut = FALSE;
				m_bIsWhiteOut = FALSE;		// �t���O�����Z�b�g
			}
		}
	}
}

// ===================================================================
// �A�N�Z�X�֐�
// ===================================================================
CSpriteImage* CBackForeProc::GetImage() 
{ 
	return m_pImageFore; 
}

CSprite* CBackForeProc::GetSprite() 
{ 
	return m_pSprite; 
}

void CBackForeProc::SetFade(BOOL fade) 
{ 
	m_bIsFade = fade; 
}

BOOL CBackForeProc::GetFadeInFinish() 
{ 
	return m_bIsFadeInFinish; 
}

BOOL CBackForeProc::GetFadeOutFinish()
{ 
	return m_bIsFadeOutFinish; 
}

BOOL CBackForeProc::GetBlackOut() 
{ 
	return m_bIsBlackOut; 
}

BOOL CBackForeProc::GetWhiteOut() 
{ 
	return m_bIsWhiteOut;
}

void CBackForeProc::SetBlackOut(BOOL flag) 
{ 
	m_bIsBlackOut = flag; 
}

CSpriteImage* CBackForeProc::GetImageFade() 
{ 
	return m_pImageFade; 
}

CSpriteImage* CBackForeProc::GetPcIcon() 
{ 
	return m_pImagePcIcon; 
}

CSpriteImage* CBackForeProc::GetImageAbilityDialog()
{
	return m_pImageAbilityDialog;
}

void CBackForeProc::SetBackImageMovePos(int pos) 
{ 
	m_nFadeDrawPosY = pos; 
}

void CBackForeProc::ResetOpenAllyNum()
{
	m_nOpenAllyNum = 1;
}

int CBackForeProc::GetSelectNum()
{
	return m_nSelectNum;
}

void CBackForeProc::SetSelectNum(int id)
{
	m_nSelectNum = id;
}

void CBackForeProc::SetFirstAxe(bool flag)
{
	m_bIsFirstAxe = flag;
}

BOOL CBackForeProc::GetFirstAxe()
{
	return m_bIsFirstAxe;
}

void CBackForeProc::SetFirstGun(bool flag)
{
	m_bIsFirstGun = flag;
}

BOOL CBackForeProc::GetFirstGun()
{
	return m_bIsFirstGun;
}

void CBackForeProc::AddDeadNum(int num)
{
	m_nDeadNum += num;
}

void CBackForeProc::AddDeadAllyNum(int num)
{
	m_nDeadAllyNum += num;
}

void CBackForeProc::AddDeadEnemyNum(int num)
{
	m_nDeadEnemyNum += num;
}

void CBackForeProc::AddAllAllyNum(int num)
{
	m_nAllAllyNum += num;
}

void CBackForeProc::SetDeadNum(int num)
{
	m_nDeadNum = num;
}

void CBackForeProc::SetDeadAllyNum(int num)
{
	m_nDeadAllyNum = num;
}

void CBackForeProc::SetDeadEnemyNum(int num)
{
	m_nDeadEnemyNum = num;
}

void CBackForeProc::SetAllAllyNum(int num)
{
	m_nAllAllyNum = num;
}

int CBackForeProc::GetDeadNum()
{
	return m_nDeadNum;
}

int CBackForeProc::GetDeadAllyNum()
{
	return m_nDeadAllyNum;
}

int CBackForeProc::GetDeadEnemyNum()
{
	return m_nDeadEnemyNum;
}

int CBackForeProc::GetAllAllyNum()
{
	return m_nAllAllyNum;
}
