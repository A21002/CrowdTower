//=============================================================================
//		タイトル・ゲームクリヤー・ゲームオーバーの処理
//																Title.cpp
//=============================================================================
#include  "Title.h"
#include  "Map.h"
#include  "Playchar.h"
#include  "BackFore.h"
#include  "Camera.h"
#include  "Ability.h"

// ---------------------------------------------------------------------------
// タイトルプロシージャのコンストラクタ
// ---------------------------------------------------------------------------
CTitleProc::CTitleProc(CGameMain* pGMain)
{
	m_pGMain = pGMain;

	m_pImageTitle = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/TITLE.png")); // イメージのロード
	m_pImageClear = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/gameClearBack.png")); // イメージのロード
	m_pImageClearScore = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/ClearScore.png")); // イメージのロード
	m_pImageOver  = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/OVER.png")); // イメージのロード
	m_pImageButton = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/Button.png"));
	m_pImageClearFore = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/gameClearFore.png"));
	m_pSprite = new CSprite(m_pGMain->m_pShader); // スプライトの生成

	m_dwCntTime = 0;
	m_nSelectNum = 0;
	m_nBackImageMoveNum = 0;
	m_nIconMoveNum = 0;
	m_nCnt = 1 * 60;
	m_bIsSaveData = FALSE;
}

// ---------------------------------------------------------------------------
// タイトルタイトルプロシージャのデストラクタ
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
//	タイトルの表示	
//------------------------------------------------------------------------
BOOL CTitleProc::Title()
{
	BOOL bRet = FALSE;
	// 表示
	m_pSprite->Draw(m_pImageTitle, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);

	// ボタンの描画
	int bottonWidth = 331;
	int bottonHeight = 79;
	VECTOR2 button1Pos = VECTOR2(517, 410);
	VECTOR2 button2Pos = VECTOR2(517, 520);
	VECTOR2 button3Pos = VECTOR2(517, 630);
	int mouseOver = 8;

	// 最初からスタートボタン
	if (CheckMousePos(button1Pos.x, button1Pos.y, bottonWidth, bottonHeight) && !m_pGMain->m_bIsTutorial)
	{
		m_pSprite->Draw(m_pImageButton, button1Pos.x, button1Pos.y + mouseOver, 1201, 0, bottonWidth, bottonHeight - mouseOver);
		m_nSelectNum = 0;     // マウスカーソルがゲームモードスプライト内にある
		if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
		{
			m_pGMain->m_pClick_Button->Play();
			bRet = TRUE;  // 終了
		}
	}
	else {
		m_pSprite->Draw(m_pImageButton, button1Pos.x, button1Pos.y, 1533, 0, bottonWidth, bottonHeight);
	}

	if (m_bIsSaveData) {
		// 続きからスタート
		if (CheckMousePos(button2Pos.x, button2Pos.y, bottonWidth, bottonHeight) && !m_pGMain->m_bIsTutorial)
		{
			m_pSprite->Draw(m_pImageButton, button2Pos.x, button2Pos.y + mouseOver, 1200, bottonHeight - 8, bottonWidth + 1, bottonHeight - mouseOver);
			m_nSelectNum = 1;     // マウスカーソルがゲームモードスプライト内にある
			if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
			{
				m_pGMain->m_pClick_Button->Play();
				bRet = TRUE;  // 終了
			}
		}
		else {
			m_pSprite->Draw(m_pImageButton, button2Pos.x, button2Pos.y, 1533, bottonHeight, bottonWidth, bottonHeight);
		}
	}

	// チュートリアルの表示ボタン
	if (CheckMousePos(button3Pos.x, button3Pos.y, bottonWidth, bottonHeight) && !m_pGMain->m_bIsTutorial)
	{
		m_pSprite->Draw(m_pImageButton, button3Pos.x, button3Pos.y + mouseOver, 1200, (bottonHeight - mouseOver) * 2, bottonWidth + 1, bottonHeight - mouseOver);
		m_nSelectNum = 2;     // マウスカーソルがゲームモードスプライト内にある
		if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
		{
			m_pGMain->m_pClick_Button->Play();
			bRet = TRUE;  // 終了
		}
	}
	else {
		m_pSprite->Draw(m_pImageButton, button3Pos.x, button3Pos.y, 1533, bottonHeight * 2, bottonWidth, bottonHeight);
	}

	return bRet;
}

//------------------------------------------------------------------------
//	ゲームスタート時のアニメーション	
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

	// 描画
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
				// 最初はマップ１のロード
				m_pGMain->m_pMapProc->SetHierarchy(1);
				m_pGMain->m_pMapProc->LoadMap1();
				//m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetCamBase();
				m_pGMain->m_pBackForeProc->SetBackImageMovePos(backImage->m_dwImageHeight - WINDOW_HEIGHT - m_nBackImageMoveNum);
			}
		}
	}
}

//------------------------------------------------------------------------
//	ゲームクリヤーの表示	
//------------------------------------------------------------------------
void CTitleProc::GameClear()
{
	// PCを描画
	m_pGMain->m_pPcProc->GetPcObjPtr()->UpdateGameClear();
	// カメラを描画
	m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetCamBase();
	m_pGMain->m_pCameraProc->UpdateAll();

	// 表示
	m_pSprite->Draw(m_pImageClearFore, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);

	int spownAlly = m_pGMain->m_pBackForeProc->GetAllAllyNum();
	int deadAlly = m_pGMain->m_pBackForeProc->GetDeadAllyNum();
	int spownEnemy = m_pGMain->m_pBackForeProc->GetDeadEnemyNum();
	int retry = m_pGMain->m_pBackForeProc->GetDeadNum();

	VECTOR2 numPos = VECTOR2(902, 484);
	int numDiff = 72;
	TCHAR str[256] = _T("");

	_stprintf_s(str, _T("%d"), spownAlly);
	m_pGMain->m_pFont->Draw(numPos.x, numPos.y, str, 48, RGB(230, 31, 31), 1.0f, _T("GN-キルゴUかなNA"));

	_stprintf_s(str, _T("%d"), deadAlly);
	m_pGMain->m_pFont->Draw(numPos.x, numPos.y + numDiff, str, 48, RGB(230, 31, 31), 1.0f, _T("GN-キルゴUかなNA"));

	_stprintf_s(str, _T("%d"), spownEnemy);
	m_pGMain->m_pFont->Draw(numPos.x, numPos.y + numDiff * 2, str, 48, RGB(230, 31, 31), 1.0f, _T("GN-キルゴUかなNA"));

	_stprintf_s(str, _T("%d"), retry);
	m_pGMain->m_pFont->Draw(numPos.x, numPos.y + numDiff * 3, str, 48, RGB(230, 31, 31), 1.0f, _T("GN-キルゴUかなNA"));

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON) ||  m_dwCntTime > 600 )	//
	{
		m_pGMain->m_pBgm1->Stop();

		m_pGMain->m_dwGameStatus = GAMEEND;

		m_dwCntTime = 0;
	}
}

//------------------------------------------------------------------------
//	ゲームオーバーの表示	
//------------------------------------------------------------------------
BOOL CTitleProc::GameOver()
{
	// PCを描画
	m_pGMain->m_pPcProc->GetPcObjPtr()->UpdateGameOver();
	// カメラを描画
	//m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetCamBase();
	m_pGMain->m_pCameraProc->UpdateAll();

	// 表示
	m_pSprite->Draw(m_pImageButton, 420, 46, 0, 400, 493, 280);

	BOOL bRet = FALSE;
	// マップ画像の下に表示する文字

	const int nWidth = 400, nHeight = 100;
	const int nStartX = 483, nStartY = 500;
	const int nSrcX = 0, nSrcY = 0;
	const int mouseOver = 8;

	// リトライボタン
	if (CheckMousePos(nStartX, nStartY, nWidth, nHeight + mouseOver))
	{
		m_pSprite->Draw(m_pImageButton, nStartX, nStartY + mouseOver, 400, 0, nWidth, nHeight);
		m_nSelectNum = 0;     // マウスカーソルがゲームモードスプライト内にある
		if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
		{
			m_pGMain->m_pClick_Button->Play();
			bRet = TRUE;  // 終了
		}
	}
	else {
		m_pSprite->Draw(m_pImageButton, nStartX, nStartY, 800, 0, nWidth, nHeight + mouseOver);
	}

	// EXITボタン
	if (CheckMousePos(nStartX, nStartY + nHeight + 50, nWidth, nHeight + mouseOver))
	{
		m_pSprite->Draw(m_pImageButton, nStartX, nStartY + nHeight + 50 + mouseOver, 400, 101, nWidth, nHeight);
		m_nSelectNum = 1;     // マウスカーソルがゲームモードスプライト内にある
		if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
		{
			m_pGMain->m_pClick_Button->Play();
			bRet = TRUE;  // 終了
		}
	}
	else {
		m_pSprite->Draw(m_pImageButton, nStartX, nStartY + nHeight + 50, 800, 108, nWidth, nHeight + mouseOver);
	}
	
	return bRet;
}

//------------------------------------------------------------------------
// リトライ時、引き継ぐ武器を選択
//------------------------------------------------------------------------
BOOL CTitleProc::SelectWeapon()
{
	BOOL bRet = FALSE;
	CAbility* ability = m_pGMain->m_pPcProc->GetAbility();
	VECTOR2 Button1 = VECTOR2(303, 428);
	VECTOR2 Button2 = VECTOR2(863, 428);
	VECTOR2 ButtonSize = VECTOR2(200, 200);

	// PCを描画
	m_pGMain->m_pPcProc->GetPcObjPtr()->UpdateGameOver();
	// カメラを描画
	m_pGMain->m_pCameraProc->UpdateAll();

	// 「引き継ぐ武器を選択」の表示
	m_pSprite->Draw(m_pImageButton, 174, 232, 500, 400, 1000, 100);

	int shortWeapon = ability->GetShortWeapon();
	int longWeapon = ability->GetLongWeapon();
	// 武器アイコンボタンの表示
	m_pSprite->Draw(m_pImageButton, Button1.x, Button1.y, 200 * longWeapon, 200, ButtonSize.x, ButtonSize.y);
	m_pSprite->Draw(m_pImageButton, Button2.x, Button2.y, 200 * shortWeapon, 200, ButtonSize.x, ButtonSize.y);

	// 遠距離武器の能力の描画開始位置の調整
	int abilityNum = ability->GetLongAbilityNum();
	VECTOR2 abilitySize = VECTOR2(130, 88);
	VECTOR2 abilityPos = VECTOR2(Button1.x + ButtonSize.x / 2 - abilitySize.x / 2 * abilityNum, Button1.y - 100);
	if (CheckMousePos(Button1.x, Button1.y, ButtonSize.x, ButtonSize.y)) {
		m_nSelectNum = longWeapon + 1;
		// 取得済み能力を上に表示
		for (int i = 0; i < ABILITY_KIND; i++)
		{
			if (ability->GetAbilityLevel(i) != 0)
			{
				m_pSprite->Draw(m_pGMain->m_pBackForeProc->GetImageAbilityDialog(), abilityPos.x, abilityPos.y, 600 + 130 * i, 500 + 100 * longWeapon, abilitySize.x, abilitySize.y);
				// 星の描画
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
			bRet = TRUE;  // 終了
		}
	}

	abilityNum = ability->GetShortAbilityNum();
	abilitySize = VECTOR2(130, 88);
	abilityPos = VECTOR2(Button2.x + ButtonSize.x / 2 - abilitySize.x / 2 * abilityNum, Button2.y - 100);
	if (CheckMousePos(Button2.x, Button2.y, ButtonSize.x, ButtonSize.y)) {
		m_nSelectNum = shortWeapon + 1;
		// 取得済み能力を上に表示
		for (int i = 0; i < ABILITY_KIND; i++)
		{
			if (ability->GetAbilityLevel(i + 3) != 0)
			{
				m_pSprite->Draw(m_pGMain->m_pBackForeProc->GetImageAbilityDialog(), abilityPos.x, abilityPos.y, 600 + 130 * i, 500 + 100 * shortWeapon, abilitySize.x, abilitySize.y);
				// 星の描画
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
			bRet = TRUE;  // 終了
		}
	}

	return bRet;
}

//------------------------------------------------------------------------
//	ゲーム終了時、セーブするかの選択の表示
//------------------------------------------------------------------------
BOOL CTitleProc::Save()
{
	BOOL bRet = FALSE;

	// PCを描画
	m_pGMain->m_pPcProc->GetPcObjPtr()->UpdateGameOver();
	// カメラを描画
	m_pGMain->m_pCameraProc->UpdateAll();

	m_pSprite->Draw(m_pImageButton, 328, 293, 0, 683, 711, 358);

	// Yes
	if (CheckMousePos(389, 608, 277, 75)) {
		m_pSprite->Draw(m_pImageButton, 389, 608 + 8, 1076, 782, 276, 75);
		m_nSelectNum = 1;
		if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_TRG, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))	//
		{
			m_pGMain->m_pClick_Button->Play();
			bRet = TRUE;  // 終了
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
			bRet = TRUE;  // 終了
		}
	}
	else {
		m_pSprite->Draw(m_pImageButton, 701, 608, 800, 701, 275, 82);
	}


	return bRet;
}

//------------------------------------------------------------------------
// マウスカーソルが指定した矩形内にあるかどうかを判定する
//
// 引数  int nPosX,  int nPosY     矩形左上座標
//       int nWidth, int nHeight   矩形の幅と高さ
//
// 戻り値  BOOL  TRUE:矩形内にある    FALSE:矩形内に無い
//------------------------------------------------------------------------
BOOL CTitleProc::CheckMousePos(int nPosX, int nPosY, int nWidth, int nHeight)
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

//------------------------------------------------------------------------
//	セーブデータが存在するか確認
//------------------------------------------------------------------------
void CTitleProc::CheckSaveData()
{
	const int BUFSIZE = 512;
	TCHAR str[BUFSIZ];
	FILE* fp;

	fp = _tfopen(_T("Data/SaveData.csv"), _T("r"));

	int tmp = 0;
	_fgetts(str, BUFSIZE, fp);
	_stscanf_s(str, _T("%d"), &tmp);  // セーブデータの有無
	if (m_pGMain->Cipher(tmp) == 0) {
		// セーブデータなし
		m_bIsSaveData = FALSE;
	}
	else {
		m_bIsSaveData = TRUE;
	}
}
