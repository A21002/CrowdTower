#include  "BackFore.h"
#include  "Ability.h"
#include  "Playchar.h"
#include  "Enemy.h"
#include  "EnemyBoss.h"
#include  "Ally.h"
#include  "Map.h"


// ---------------------------------------------------------------------------
// 前景・背景プロシージャのコンストラクタ
// ---------------------------------------------------------------------------
CBackForeProc::CBackForeProc(CGameMain* pGMain)
{
	m_pGMain		= pGMain;
	m_pImageFore = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/foregrd3.png"));
	m_pImageUI = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/UI.png")); // スプライトイメージのロード
	m_pImageAbilityDialog = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/AbilityDialog.png"));
	m_pImageFade = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/Fade.png"));
	m_pImagePcIcon = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/PcIcon.png"));
	m_pImageTutorial = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/tutorial.png"));
	m_pImageArrow = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/ArrowImage.png"));
	m_pSprite       = new CSprite(m_pGMain->m_pShader); // スプライトの生成
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
// 前景・背景プロシージャのデストラクタ
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
//	前景・背景プロシージャの更新	
//------------------------------------------------------------------------
void CBackForeProc::Update()
{
	TCHAR str[256] = { 0 };
	CAbility* ability = m_pGMain->m_pPcProc->GetAbility();

	if (m_pGMain->m_pEnmBossProc->GetEnmBossObjPtr()->GetMovieFinish())
	{ 

		// 振動処理
		if (m_bIsSwing)
		{
			// 振動幅の計算
			m_vSwingWidth.x = cos(XMConvertToRadians(m_fSwingAngle)) * m_fSwingWidth;
			m_vSwingWidth.y = sin(XMConvertToRadians(m_fSwingAngle)) * m_fSwingWidth;

			// 振動角度を反転
			m_fSwingAngle += 180.0f;
			if (m_fSwingAngle >= 360.0f)
			{
				m_fSwingAngle -= 360.0f;
			}
			// 振れ幅を縮小
			m_fSwingWidth *= 0.8f;

			// 振れ幅が1以下になったら振動停止
			if (m_fSwingWidth <= 1.0f)
			{
				m_bIsSwing = FALSE;
			}
		}
		else {
			// 0にリセットする
			m_vSwingWidth = VECTOR2(0.0f, 0.0f);
		}

		// UI表示
		int   DestX = 0, DestY = 0;
		m_pSprite->Draw(m_pImageUI, DestX + m_vSwingWidth.x, DestY + m_vSwingWidth.y, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		// 階層数表示
		DestX = 1260;
		DestY = 50;
		_stprintf_s(str, _T("%d"), m_pGMain->m_pMapProc->GetHierarchy());
		m_pGMain->m_pFont->Draw(DestX + m_vSwingWidth.x, DestY + m_vSwingWidth.y, str, 48, RGB(41, 228, 98), 1.0f, _T("GN-キルゴUかなNA"));

		// HP表示
		DestX = 125;
		DestY = 670;
		_stprintf_s(str, _T("%d"), m_pGMain->m_pPcProc->GetPcObjPtr()->GetHp());
		m_pGMain->m_pFont->Draw(DestX + m_vSwingWidth.x, DestY + m_vSwingWidth.y, str, 60, RGB(253, 220, 0), 1.0f, _T("GN-キルゴUかなNA"));

		// 残弾表示処理
		if (ability->GetLongWeapon() == GUN)
		{
			// リロードアイコン
			m_pSprite->Draw(m_pImageFore, 41 + m_vSwingWidth.x, 500 + m_vSwingWidth.y, 300, 200, 80, 26);

			int num = m_pGMain->m_pPcProc->GetPcObjPtr()->GetGunMagazine();
			m_pSprite->Draw(m_pImageFore, 41 + m_vSwingWidth.x, 552 + m_vSwingWidth.y, 0, 410, 23 * num, 30);
		}

		// ボスのHPバーの表示
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

		// 能力の解法・強化状態の表示
		int abilityLevel[2][3];

		// 配列に格納
		// 遠隔
		abilityLevel[0][0] = ability->GetAbilityLevel(0);
		abilityLevel[0][1] = ability->GetAbilityLevel(1);
		abilityLevel[0][2] = ability->GetAbilityLevel(2);

		// 近接
		abilityLevel[1][0] = ability->GetAbilityLevel(3);
		abilityLevel[1][1] = ability->GetAbilityLevel(4);
		abilityLevel[1][2] = ability->GetAbilityLevel(5);

		int maxCoolTime = 60 * PcConstruct::WAITTIME_MELEE_WEAPON * (1 - 0.3 * abilityLevel[1][0]);
		int nowCoolTime = m_pGMain->m_pPcProc->GetPcObjPtr()->GetCoolTime();

		float ratio = (float)nowCoolTime / (float)maxCoolTime;

		// 所持している武器のアイコンを表示
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
					// アビリティアイコンの描画
					int iconX = 130, iconY = 88;
					int posX = 113 + 12 + abiNum * (iconX + 12);
					int posY = 20 + i * (iconY + 29);
					m_pSprite->Draw(m_pImageAbilityDialog, posX + m_vSwingWidth.x, posY + m_vSwingWidth.y, 600 + 130 * j, 500 + 100 * weaponId, iconX, iconY);

					// 星の描画
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


	// アップグレードUIの表示
	switch (m_nDialogKind) {
	case DIALOG_NONE:
		// UIを表示しない
		break;

	case DIALOG_CHECK:
		// 宝箱を開けるか確認UIを表示
		if (UpdateCheckDialog())
		{
			switch (m_nSelectNum)
			{
			case 1:
				// 開ける
				if (m_pGMain->m_pPcProc->GetPcObjPtr()->GetHp() - 1 >= m_nOpenAllyNum) {
					// 味方が10人以上
					m_pGMain->m_pAllyProc->DeleteObj(m_nOpenAllyNum);
					m_nOpenAllyNum += 1;
					m_nDialogKind = DIALOG_ABILITY;
				}
				else {
					m_nDialogKind = DIALOG_ERROR;
				}
				break;

			case 2:
				// 開けない
				m_nDialogKind = DIALOG_NONE;
				m_pGMain->m_isMove = TRUE;
				break;
			}
		}
		break;

	case DIALOG_ABILITY:
		// アップグレードUIの表示
		if (UpdateDialog()) {
			// 選択したので再度非表示
			m_nDialogKind = DIALOG_NONE;
			m_pGMain->m_isMove = TRUE;
			if (m_nSelectNum != 4)
			{
				// 能力取得なしでない場合
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

	// 矢印の描画
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

			// 画面外に飛び出るのを防止
			if (vPos.y < 0) {
				// 画面下に行くのを防止
				vPos.y += vArrowSize.y;
			}
			if (vPos.x < 0) {
				vPos.x -= vArrowSize.x;
			}

			m_pSprite->Draw(m_pImageArrow, vPos.x + WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - vPos.y, vArrowSize.x* (i % 9), vArrowSize.y* (i / 9), vArrowSize.x, vArrowSize.y);

		}
	}
	
	// フェード制御
	if (m_bIsAnim) {
		UpdateAnim();
	}
	else {
		UpdateFade();
	}
	UpdateBlackOut();
}

// ---------------------------------------------------------------------------
// Upgradeダイアログの表示
// TRUE : 選択した  FALSE : 選択していない
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

	// 描画
	// ダイアログ
	m_pSprite->Draw(m_pImageAbilityDialog, DialogPos.x, DialogPos.y, 2049, 683, DialogSize.x, DialogSize.y);
	TCHAR str[256];
	_stprintf_s(str, _T("%d"), m_nOpenAllyNum);
	m_pGMain->m_pFont->Draw(NumPos.x, NumPos.y, str, 38, RGB(230, 0, 19), 1.0f, _T("GN-キルゴUかなNA"));

	// ボタン
	// Yes
	if (CheckMousePos(OkPos.x, OkPos.y, ButtonSize.x, ButtonSize.y))
	{
		m_pSprite->Draw(m_pImageAbilityDialog, OkPos.x, OkPos.y + OverMouse, 1676, 782, ButtonSize.x, ButtonSize.y);
		m_nSelectNum = 1;     // マウスカーソルがゲームモードスプライト内にある
		if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
		{
			m_pGMain->m_pClick_Button->Play();
			bRet = TRUE;  // 終了
		}
	}
	else {
		m_pSprite->Draw(m_pImageAbilityDialog, OkPos.x, OkPos.y, 1400, 782, ButtonSize.x, ButtonSize.y + OverMouse);
	}

	if (CheckMousePos(NoPos.x, NoPos.y, ButtonSize.x, ButtonSize.y))
	{
		m_pSprite->Draw(m_pImageAbilityDialog, NoPos.x, NoPos.y + OverMouse, 1676, 700, ButtonSize.x, ButtonSize.y);
		m_nSelectNum = 2;     // マウスカーソルがゲームモードスプライト内にある
		if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
		{
			m_pGMain->m_pClick_Button->Play();
			bRet = TRUE;  // 終了
		}
	}
	else {
		m_pSprite->Draw(m_pImageAbilityDialog, NoPos.x, NoPos.y, 1400, 700, ButtonSize.x, ButtonSize.y + OverMouse);
	}

	return bRet;
}

// ---------------------------------------------------------------------------
// Errorダイアログの表示
// TRUE : 選択した  FALSE : 選択していない
// ---------------------------------------------------------------------------
BOOL CBackForeProc::UpdateErrorDialog()
{
	BOOL bRet = FALSE;
	VECTOR2 DialogPos = VECTOR2(328, 229);
	VECTOR2 ButtonPos = VECTOR2(545, 552);

	VECTOR2 DialogSize = VECTOR2(711, 360);
	VECTOR2 ButtonSize = VECTOR2(276, 75);
	const int OverMouse = 7;

	// 描画
	// ダイアログ
	m_pSprite->Draw(m_pImageAbilityDialog, DialogPos.x, DialogPos.y, 2784, 683, DialogSize.x, DialogSize.y);

	// ボタン
	if (CheckMousePos(ButtonPos.x, ButtonPos.y, ButtonSize.x, ButtonSize.y))
	{
		m_pSprite->Draw(m_pImageAbilityDialog, ButtonPos.x, ButtonPos.y + OverMouse, 1676, 782, ButtonSize.x, ButtonSize.y);
		m_nSelectNum = 1;     // マウスカーソルがゲームモードスプライト内にある
		if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
		{
			m_pGMain->m_pClick_Button->Play();
			bRet = TRUE;  // 終了
		}
	}
	else {
		m_pSprite->Draw(m_pImageAbilityDialog, ButtonPos.x, ButtonPos.y, 1400, 782, ButtonSize.x, ButtonSize.y + OverMouse);
	}

	return bRet;
}

// ---------------------------------------------------------------------------
// Upgradeダイアログの表示
// TRUE : 選択した  FALSE : 選択していない
// ---------------------------------------------------------------------------
BOOL CBackForeProc::UpdateDialog()
{
	BOOL bRet = FALSE;
	// マップ画像の下に表示する文字

	const int nStartX = 450, nStartY = 146;
	const int nWidth = 466, nHeight = 156;
	const int nSrcX = 600, nSrcY = 0;

	// 背景の表示
	m_pSprite->Draw(m_pImageAbilityDialog, 431, 74, 0, 0, 504, 663);

	// ゲームモードセレクト表示
	for (int i = 0; i < 3; i++)
	{
		int x = m_nSelectAbility[i] / 3, y = m_nSelectAbility[i] % 3;
		// 選択していないモード
		m_pSprite->Draw(m_pImageAbilityDialog, nStartX, nStartY + (nHeight + 19) * i, nSrcX + x * nWidth, nSrcY + y * nHeight, nWidth, nHeight);
	}

	// マウスによるゲームモードセレクト(1～4)
	for (int i = 0; i < 3; i++)
	{
		if (CheckMousePos(nStartX, nStartY + (nHeight + 19) * i, nWidth, nHeight))
		{
			m_nSelectNum = i;     // マウスカーソルがゲームモードスプライト内にある
		}
	}

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
	{
		m_pGMain->m_pClick_Ability->Play();
		bRet = TRUE;  // 終了
	}

	// 選択なしのボタン
	VECTOR2 exitPos = VECTOR2(514, 674);
	VECTOR2 exitSize = VECTOR2(334, 61);
	m_pSprite->Draw(m_pImageAbilityDialog, exitPos.x, exitPos.y, 0, 700, exitSize.x, exitSize.y);
	if (CheckMousePos(exitPos.x, exitPos.y, exitSize.x, exitSize.y))
	{
		m_nSelectNum = 4;
		if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
		{
			m_pGMain->m_pClick_Button->Play();
			bRet = TRUE;  // 終了
		}

	}
	return bRet;
}

// ---------------------------------------------------------------------------
// Tutorialのダイアログの表示
// TRUE : 選択した  FALSE : 選択していない
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

	// ダイアログの表示
	m_pSprite->Draw(m_pImageTutorial, vDialogPos.x, vDialogPos.y, vDialogSize.x * (page - DIALOG_TUTORIAL1), 0, vDialogSize.x, vDialogSize.y);

	if (left) {
		// 左ボタン
		if (CheckMousePos(vButton1PosLeft.x, vButton1PosLeft.y, vButton1Size.x, vButton1Size.y))
		{
			m_pSprite->Draw(m_pImageTutorial, vButton1PosLeft.x, vButton1PosLeft.y + nOverMouse, 0, 453 + vButton1Size.y + nOverMouse, vButton1Size.x, vButton1Size.y);
			m_nSelectNum = BUTTON_LEFT;     // マウスカーソルがゲームモードスプライト内にある
			if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
			{
				m_pGMain->m_pClick_Button->Play();
				bRet = TRUE;  // 終了
			}
		}
		else {
			m_pSprite->Draw(m_pImageTutorial, vButton1PosLeft.x, vButton1PosLeft.y, 0, 453, vButton1Size.x, vButton1Size.y + nOverMouse);
		}
	}

	if (right) {
		// 右ボタン
		if (CheckMousePos(vButton1PosRight.x, vButton1PosRight.y, vButton1Size.x, vButton1Size.y))
		{
			m_pSprite->Draw(m_pImageTutorial, vButton1PosRight.x, vButton1PosRight.y + nOverMouse, vButton1Size.x, 453 + vButton1Size.y + nOverMouse, vButton1Size.x, vButton1Size.y);
			m_nSelectNum = BUTTON_RIGHT;     // マウスカーソルがゲームモードスプライト内にある
			if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
			{
				m_pGMain->m_pClick_Button->Play();
				bRet = TRUE;  // 終了
			}
		}
		else {
			m_pSprite->Draw(m_pImageTutorial, vButton1PosRight.x, vButton1PosRight.y, vButton1Size.x, 453, vButton1Size.x, vButton1Size.y + nOverMouse);
		}
	}

	if (finish) {
		// 終了ボタン
		if (CheckMousePos(vButton2Pos.x, vButton2Pos.y, vButton2Size.x, vButton2Size.y))
		{
			m_pSprite->Draw(m_pImageTutorial, vButton2Pos.x, vButton2Pos.y + nOverMouse, vButton1Size.x * 2, 453 + vButton2Size.y + nOverMouse, vButton2Size.x, vButton2Size.y);
			m_nSelectNum = BUTTON_FINISH;     // マウスカーソルがゲームモードスプライト内にある
			if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
			{
				m_pGMain->m_pClick_Button->Play();
				bRet = TRUE;  // 終了
			}
		}
		else {
			m_pSprite->Draw(m_pImageTutorial, vButton2Pos.x, vButton2Pos.y, vButton1Size.x * 2, 453, vButton2Size.x, vButton2Size.y + nOverMouse);
		}
	}
	return bRet;
}

// ---------------------------------------------------------------------------
// Upgradeダイアログに表示する選択肢の選定
// ---------------------------------------------------------------------------
void CBackForeProc::SetSelectAbility()
{
	// 初期化
	for (int i = 0; i < 3; i++) {
		m_nSelectAbility[i] = -1;
	}

	for (int i = 0; i < 3; i++) {
		int num = Random(0, WEAPON_KIND * ABILITY_KIND - 1);

		// 決定するまで無限ループ
		while (1) {
			// 既に選ばれた値か
			for (int j = 0; j < 3 && j < i; j++) {
				if (num == m_nSelectAbility[j])
				{
					// 選ばれていた場合
					num++;
					if (num == WEAPON_KIND * ABILITY_KIND)
					{
						num = 0;
					}
					j = -1;
					continue;
				}
			}

			// 最大強化済みか
			if (!m_pGMain->m_pPcProc->GetAbility()->CheckAbilityLevel(num))
			{
				// 最大強化済み
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
// UIの振動スタート処理
// ---------------------------------------------------------------------------
void CBackForeProc::StartSwing(float angle)
{
	if (!m_bIsSwing) {
		// 揺れていなければ
		m_fSwingAngle = angle;		// 揺らす角度
		m_fSwingWidth = 20.0f;		// 揺らす幅
		m_bIsSwing = TRUE;			// Update処理で揺らすフラグ
	}
}

//------------------------------------------------------------------------
// 敵が画面外から攻撃した際に攻撃した方向を指す矢印の描画開始処理
//------------------------------------------------------------------------
void CBackForeProc::DrawArrowAngle(MATRIX4X4 mWorld)
{
	// ワールド座標からスクリーン座標に変換
	VECTOR3 vScreenPos = XMVector3Project(GetPositionVector(mWorld), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
										 0.0f, 1.0f, m_pGMain->m_mProj, m_pGMain->m_mView, XMMatrixIdentity());

	// 画面外にいるかのチェック
	if ((0 <= vScreenPos.x && vScreenPos.x <= WINDOW_WIDTH) &&
		(0 <= vScreenPos.y && vScreenPos.y <= WINDOW_HEIGHT)) {
	}else
	{
		// 画面の中心とスクリーン座標の角度
		VECTOR2 vCenterPos = VECTOR2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
		float fAngle = XMConvertToDegrees(atan2(vScreenPos.x - vCenterPos.x, vCenterPos.y - vScreenPos.y));

		// 対応する要素を計算
		int index = (int)fAngle / 10;
		if (index < 0)
		{
			return;
		}
		// 既に描画が開始されているか
		if (m_nArrowDrawTime[index] <= 0) {
			// 描画が開始されていない
			m_nArrowDrawTime[index] = 1;
		}
	}
}

//------------------------------------------------------------------------
// マウスカーソルが指定した矩形内にあるかどうかを判定する
//
// 引数  int nPosX,  int nPosY     矩形左上座標
//       int nWidth, int nHeight   矩形の幅と高さ
//
// 戻り値  BOOL  TRUE:矩形内にある    FALSE:矩形内に無い
//------------------------------------------------------------------------
BOOL CBackForeProc::CheckMousePos(int nPosX, int nPosY, int nWidth, int nHeight)
{
	POINT MPos = m_pGMain->m_pDI->GetMousePos();
	if (MPos.x >= nPosX && MPos.x <= nPosX + nWidth &&
		MPos.y >= nPosY && MPos.y <= nPosY + nHeight)
	{
		return TRUE;  // 矩形内にある
	}
	else {
		return FALSE;  // 矩形内に無い
	}
}

// ---------------------------------------------------------------------------
// マップ移動時のフェードイン、フェードアウトする処理
// ---------------------------------------------------------------------------
void CBackForeProc::UpdateFade()
{
	VECTOR2 size = VECTOR2(1366.0f, 1152.0f);
	const int fadeSpeed = 24;
	int i;
	if (m_bIsFade) {
		// フェードイン
		m_pGMain->m_pPcProc->GetPcObjPtr()->SetCanMove(FALSE);
		if (m_nFadePosY > -(WINDOW_HEIGHT / 2) + 200) {
			// フェードイン中
			m_nFadePosY -= fadeSpeed;
		}
		else {
			// フェードイン完了
			m_bIsFadeInFinish = TRUE;			// フェードイン完了通知
			m_bIsFadeOutFinish = FALSE;			// フェードアウト完了通知リセット
			m_bIsAnim = TRUE;					// アニメーションスタート
		}

		// 透過部の描画
		for (i = 0; i < 100; i++) {
			// 徐々に不透明にしてグラデーションを出す
			m_pSprite->Draw(m_pImageFade, 0, m_nFadePosY + i * 2, 0, m_nFadeDrawPosY - 100 * 2 + i * 2, size.x, 2, 0.01 * i);
		}
		// 不透明部分を描画
		m_pSprite->Draw(m_pImageFade, 0, m_nFadePosY + i * 2, 0, m_nFadeDrawPosY, WINDOW_WIDTH, WINDOW_HEIGHT);
	}

	else {
		// フェードアウト
		if (m_nFadePosY <= WINDOW_HEIGHT) {
			// フェードアウト中
			m_nFadePosY += fadeSpeed;
		}
		else {
			// フェードアウト終了
			m_bIsFadeOutFinish = TRUE;			// フェードアウト完了通知
			m_bIsFadeInFinish = FALSE;			// フェードイン完了通知
		}

		// 透過部の描画
		for (i = 0; i < 100; i++) {
			// 徐々に不透明にしてグラデーションを出す
			m_pSprite->Draw(m_pImageFade, 0, m_nFadePosY + i * 2, 0, m_nFadeDrawPosY - 100 * 2 + i * 2, size.x, 2, 0.01 * i);
		}
		// 不透明部分を描画
		m_pSprite->Draw(m_pImageFade, 0, m_nFadePosY + i * 2, 0, m_nFadeDrawPosY, WINDOW_WIDTH, WINDOW_HEIGHT);
	}
}

// ---------------------------------------------------------------------------
// マップ移動時のアニメーション処理
// ---------------------------------------------------------------------------
void CBackForeProc::UpdateAnim()
{
	const int moveNum = 187;		// 各階層どおしの幅
	const int moveSpeed = 2;		// アイコン移動速度

	// 音再生
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

	// 描画
	// 塔のイラスト
	m_pSprite->Draw(m_pImageFade, 0, 0, 0, m_nFadeDrawPosY - m_nAnimMove, WINDOW_WIDTH, WINDOW_HEIGHT);
	// プレイヤーアイコン
	m_pSprite->Draw(m_pImagePcIcon, 995, 335, 0, 0, m_pImagePcIcon->m_dwImageWidth, m_pImagePcIcon->m_dwImageHeight);

	// 次階層に到達してからの待機時間
	if (moveNum <= m_nAnimMove) {
		m_pGMain->m_pAnimWalk->Stop();
		// 待機
		if (m_nCnt > 0) {
			m_nCnt--;
		}

		// アニメーション処理終了
		else {
			m_nCnt = 60;						// 待機時間リセット
			m_bIsAnim = FALSE;					// アニメーションフラグ
			m_bIsWalkAudio = TRUE;
			m_nFadeDrawPosY -= m_nAnimMove;		// 塔のイラストの描画位置
			m_nAnimMove = 0;					// 次回アニメーション用に現在の合計移動量をリセット
			m_nDialogKind = DIALOG_NONE;		// フェードイン以前にダイアログが表示されている場合の対策
			m_pGMain->m_pPcProc->GetPcObjPtr()->SetCanMove(TRUE);
		}
	}
}

// ---------------------------------------------------------------------------
// ブラックアウト処理
// ---------------------------------------------------------------------------
void CBackForeProc::UpdateBlackOut()
{
	// ブラックアウト処理をする
	if (m_bIsBlackOut)
	{
		// 黒の長方形を指定の透過値で描画
		m_pSprite->DrawRect(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0, m_fBlackOutAlpha);

		if (!m_bIsWhiteOut) {
			// 徐々に透過値を下げていく
			m_fBlackOutAlpha += 0.04f;
			if (m_fBlackOutAlpha > 1.00f)
			{
				// 完全に黒になったら透過値を上げていくようにフラグを立てる
				m_bIsWhiteOut = TRUE;
			}
		}
		else {
			// 徐々に透過値を上げていく
			m_fBlackOutAlpha -= 0.04f;
			if (m_fBlackOutAlpha <= 0.00f)
			{
				// 透明になったら処理を終了する
				m_bIsBlackOut = FALSE;
				m_bIsWhiteOut = FALSE;		// フラグをリセット
			}
		}
	}
}

// ===================================================================
// アクセス関数
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
