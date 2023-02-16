//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１２　　　　　　                 ver 3.1        2022.2.16
//
//		ゲームのメイン処理
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
// CGameMain ゲームメインクラスの処理
//
// ============================================================================================
//------------------------------------------------------------------------
//
//	ゲームメインクラスのコンストラクタ	
//
//  引数　なし
//
//------------------------------------------------------------------------
CGameMain::CGameMain(CMain*	pMain)
{
	ZeroMemory(this, sizeof(CGameMain));
	m_pMain = pMain;
}
//------------------------------------------------------------------------
//
//	ゲームメインクラスのデストラクタ	
//
//------------------------------------------------------------------------
CGameMain::~CGameMain()
{
	MyImgui::ImguiQuit();          // -- 2020.11.15    // MyImguiの終了処理

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

	CoUninitialize();   // COMの解放
}

//------------------------------------------------------------------------
//
//	アプリケーションの初期化処理	
//
//  引数　なし
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CGameMain::Init()
{
	// ＣＯＭの初期化
	//CoInitializeEx(NULL, COINIT_MULTITHREADED);
	CoInitialize( NULL );

	// Direct3Dの初期化
	m_pD3D = new CDirect3D;
	if (FAILED(m_pD3D->InitD3D(m_pMain->m_hWnd, WINDOW_WIDTH, WINDOW_HEIGHT))) {
		MessageBox(0, _T("Direct3Dを初期化出来ません"), NULL, MB_OK);
		return E_FAIL;
	}

	// シェーダーの初期化
	m_pShader = new CShader(m_pD3D);
	if (FAILED(m_pShader->InitShader())) {
		MessageBox(0, _T("Shaderを生成・初期化出来ません"), NULL, MB_OK);
		return E_FAIL;
	}

	// XAudio2の初期化
	m_pXAudio = new CXAudio;
	if (FAILED(m_pXAudio->InitAudio(m_pMain->m_hWnd))) {
		MessageBox(0, _T("XAudio2を初期化出来ません"), NULL, MB_OK);
		return E_FAIL;
	}

	// DirectInputの初期化
	m_pDI = new CDirectInput;
	if (!(m_pDI->StartDirectInput(m_pMain->m_hInstance, m_pMain->m_hWnd, 
                    INIT_KEYBOARD | INIT_MOUSE | INIT_JOYSTICK, WINDOW_WIDTH, WINDOW_HEIGHT))) {
		MessageBox(0, _T("DirectInputを初期化出来ません"), NULL, MB_OK);
		return E_FAIL;
	}

	// フォントテクスチャーの初期化
	m_pFont = new CFontTexture(m_pShader);

	// MyImguiの初期化
	MyImgui::ImguiInit(m_pMain->m_hWnd, m_pD3D, WINDOW_WIDTH, WINDOW_HEIGHT);           // -- 2020.11.15

	// Fbxメッシュコントロールクラス
	m_pFbxMeshCtrl = new CFbxMeshCtrl(m_pShader);                       // -- 2021.2.4

	if (FAILED(m_pD3D->CreateShaderResourceViewFromFile(_T("Data/Image/ToonPaint.png"), &m_pD3D->m_pToonTexture, 3)))
	{
		MessageBox(NULL, _T("Data/Image/ToonPaint.png"), _T("トゥーンテクスチャファイルがありません"), MB_OK);
		return E_FAIL;
	}

	// ----------------------------------------------------------------------------------------
	// 変数の初期化
	m_dwGameStatus = TITLE;
	m_vLightDir = normalize( VECTOR3(0.8f, 1, -1) );  // 光源座標の初期値。正規化する
	m_isMove = TRUE;
	m_bIsTutorial = FALSE;
	m_bIsBoss = FALSE;
	m_bIsBoxTutorial = FALSE;

	// プロジェクショントランスフォーム（射影変換）の初期値
	//m_mProj = XMMatrixPerspectiveFovLH((FLOAT)(XM_PI / 4), (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 1000.0);
	m_mProj = XMMatrixPerspectiveFovLH(XMConvertToRadians(38.0f), (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 1000.0);

	// ビュートランスフォーム（視点座標変換）の初期値
	VECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置
	m_vEyePt = VECTOR3(0.0f, 1.0f, 0.0f);
	m_vLookatPt = VECTOR3(0.0f, 1.0f, 1.0f);
	m_mView = XMMatrixLookAtLH(m_vEyePt, m_vLookatPt, vUpVec);

	// 各プロシージャの初期化
	m_pPcProc = new CPcProc(this);		// ＰＣプロシージャ

	m_pAllyProc = new CAllyProc(this);	// 味方プロシージャ

	m_pCameraProc = new CCameraProc(this);	// カメラプロシージャ

	m_pEnmProc = new CEnmProc(this);	// 敵プロシージャ

	m_pEnmBossProc = new CEnmBossProc(this);	// 敵ボスプロシージャ

	m_pEffectProc = new CEffectProc(this);  // ３Ｄ効果

	m_pWeaponProc = new CWeaponProc(this);	// Weaponプロシージャ

	m_pMapProc = new CMapProc(this);	// Mapプロシージャ

	m_pBackForeProc = new CBackForeProc(this);  // 前景・背景

	m_pTitleProc = new CTitleProc(this);  // タイトル。ゲームクリヤー。ゲームオーバー
	m_pTitleProc->CheckSaveData();

	// サウンド
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
//	ゲームのループ処理	
//
//  引数　なし
//
//	戻り値 なし
//
//------------------------------------------------------------------------
void CGameMain::Loop()
{

	m_pDI->GetInput();			// 入力情報の取得
	m_pFont->Refresh();         // フォント情報のリフレッシュ
	MyImgui::ImguiNewFrame();   // MyImgui描画前処理

		//画面クリア（実際は単色で画面を塗りつぶす処理）
	float ClearColor[4] = { 0,0,0,1 };// クリア色作成　RGBAの順
	const int firstFloorPosY = 3826;

	m_pD3D->ClearRenderTarget(ClearColor); // 画面クリア

	if (m_pDI->CheckKey(KD_TRG, DIK_F4))  ChangeScreenMode();   // フルスクリーンの切り替え

	switch (m_dwGameStatus)
	{
	case TITLE:				// ゲーム開始タイトル
		if (m_pTitleProc->Title())
		{
			switch (m_pTitleProc->m_nSelectNum) {
			case 0:
				// そのままスタート
				m_dwGameStatus = TITLEANIM;
				m_pMapProc->SetTutorial(TRUE);
				m_pAnimWalk->Play(AUDIO_LOOP);
				break;

			case 1:
				// 続きからスタート
				m_pCameraProc->GetCameraObjPtr()->SetCtrl(CAMERA_GAMEOVER);
				m_pCameraProc->GetCameraObjPtr()->SetCamBase();
				Load();
				m_dwGameStatus = GAMERETRY;
				break;

			case 2:
				// チュートリアル
				m_bIsTutorial = TRUE;
				m_pBackForeProc->m_nDialogKind = DIALOG_TUTORIAL1;
				break;
			}
		}

		// チュートリアルの表示
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

	case GAMEMAIN:			// ゲーム主処理
		GameMain();
		break;

	case GAMECLEAR:			// ゲームクリヤー処理
		m_pTitleProc->GameClear();
		m_pBackForeProc->UpdateBlackOut();
		break;

	case GAMEOVER:			// ゲームオーバー処理
		if (m_pTitleProc->GameOver()) {
			switch (m_pTitleProc->m_nSelectNum) {
			case 0:
				// RETRYボタン
				m_dwGameStatus = GAMERETRY;
				m_bIsBoss = FALSE;
				break;

			case 1:
				// EXITボタン
				m_dwGameStatus = GAMESAVE;
				break;
			}
		}
		// 暗転処理
		m_pBackForeProc->UpdateBlackOut();
		break;

	case GAMERETRY:
		if (m_pTitleProc->SelectWeapon()) {
			switch (m_pTitleProc->m_nSelectNum % 2) {
			case 0:			// 遠隔武器をリセット
				m_pPcProc->GetAbility()->ResetLLongAbility();
				break;

			case 1:			// 近接武器をリセット
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


	case GAMEEND:			// ゲーム終了
		PostQuitMessage(0);
		break;

	}

	MyImgui::ImguiRender();      // MyImgui実描画処理    // -- 2020.11.15

	//画面更新（バックバッファをフロントバッファに）
	//m_pD3D->m_pSwapChain->Present(1, 0);                   // 60fps Vsync
	m_pD3D->m_pSwapChain->Present(0, 0);                   // Vsyncなし



}

//------------------------------------------------------------------------
//
//	ゲームのメイン処理	
//
//  引数　なし
//
//	戻り値 なし
//
//------------------------------------------------------------------------
void CGameMain::GameMain()
{

	//画面クリア（実際は単色で画面を塗りつぶす処理）
	float ClearColor[4] = { 0,0,0,1 };// クリア色作成　RGBAの順
	m_pD3D->ClearRenderTarget(ClearColor); // 画面クリア

	// Mapの更新
	m_pMapProc->Update();

	// ＰＣの更新
	m_pPcProc->UpdateAll();

	// 味方の更新
	m_pAllyProc->UpdateAll();

	// 敵の更新
	m_pEnmProc->UpdateAll();
	m_pEnmProc->UpdateHitcheck();

	// 敵ボスの更新
	m_pEnmBossProc->UpdateAll();

	// 武器の更新
	m_pWeaponProc->UpdateAll();

	// 空間分割のリストを毎回すべてクリヤーする   
	// （敵あるいは武器の中で空間分割を使用するときは、ここで初期クリヤーを行う）
	m_pMapProc->GetPcSpPart()->ClearList();
	m_pMapProc->GetEnmSpPart()->ClearList();
	// ３Ｄ効果の更新
	m_pEffectProc->UpdateAll();

	// Mapの描画2
	// 半透明のマップオブジェクトを描画するため、主ループの最後に実行
	m_pMapProc->Render2();

	// 前景・背景の更新
	m_pBackForeProc->Update();
	m_nScore = 0;

	// カメラの更新
	m_pCameraProc->UpdateAll();

}

//------------------------------------------------------------------------
//
//	アプリケーションの終了処理	
//
//  引数　なし
//
//	戻り値 なし
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
		// セーブデータありに設定
		fprintf(fp, "%d\n", Cipher(mode));
		// 階層数
		fprintf(fp, "%d\n", Cipher(m_pMapProc->GetHierarchy()));
		// 近接武器
		fprintf(fp, "%d,%d,%d,%d\n", Cipher(ab->GetShortWeapon()), Cipher(ab->GetAbilityLevel(0)), Cipher(ab->GetAbilityLevel(1)), Cipher(ab->GetAbilityLevel(2)));
		// 遠隔武器
		fprintf(fp, "%d,%d,%d,%d\n", Cipher(ab->GetLongWeapon()), Cipher(ab->GetAbilityLevel(3)), Cipher(ab->GetAbilityLevel(4)), Cipher(ab->GetAbilityLevel(5)));
		// 各リザルトデータ
		fprintf(fp, "%d,%d,%d,%d\n", Cipher(m_pBackForeProc->GetAllAllyNum()), Cipher(m_pBackForeProc->GetDeadAllyNum()), Cipher(m_pBackForeProc->GetDeadEnemyNum()), Cipher(m_pBackForeProc->GetDeadNum()));
		// チュートリアル、ムービーのフラグ
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
		_stscanf_s(str, _T("%d"), &tmp);  // セーブデータの有無
		if (Cipher(tmp) == 0) {
			// セーブデータなし
			return FALSE;
		}
		// セーブデータあり
		_fgetts(str, BUFSIZE, fp);
		_stscanf_s(str, _T("%d"), &hierarchry);  // 前回階層数

		_fgetts(str, BUFSIZE, fp);
		int tmp1 = 0, tmp2 = 0, tmp3 = 0;
		_stscanf_s(str, _T("%d,%d,%d,%d"), &tmp, &tmp1, &tmp2, &tmp3);  // 近接武器
		ab->SetShortWeapon(Cipher(tmp));
		ab->SetShortWeaponAbility(Cipher(tmp1), Cipher(tmp2), Cipher(tmp3));

		_fgetts(str, BUFSIZE, fp);
		_stscanf_s(str, _T("%d,%d,%d,%d"), &tmp, &tmp1, &tmp2, &tmp3);  // 遠隔
		ab->SetLongWeapon(Cipher(tmp));
		ab->SetLongWeaponAbility(Cipher(tmp1), Cipher(tmp2), Cipher(tmp3));

		_fgetts(str, BUFSIZE, fp);
		_stscanf_s(str, _T("%d,%d,%d,%d"), &tmp, &tmp1, &tmp2, &tmp3);  // リザルトデータ
		m_pBackForeProc->SetAllAllyNum(Cipher(tmp));
		m_pBackForeProc->SetDeadAllyNum(Cipher(tmp1));
		m_pBackForeProc->SetDeadEnemyNum(Cipher(tmp2));
		m_pBackForeProc->SetDeadNum(Cipher(tmp3));

		_fgetts(str, BUFSIZE, fp);
		_stscanf_s(str, _T("%d,%d,%d"), &tmp, &tmp1, &tmp2);  // チュートリアル表示フラグ
		m_bIsBoxTutorial = Cipher(tmp);
		m_pBackForeProc->SetFirstAxe(Cipher(tmp1));
		m_pBackForeProc->SetFirstGun(Cipher(tmp2));
		fclose(fp);
	}
}

int CGameMain::Cipher(int num)
{
	int key = 0x45af6e5d; //任意の暗号化キー
	num = num ^ key;
	return num;
}

//------------------------------------------------------------------------   
//
//	ウィンドウモードとフルスクリーンモードを切り替える処理	
//
//  引数　モード  0:ウインドウモード　1:フルスクリーン　-1:モード切り替え（省略値）
//
//	戻り値 なし
//
//------------------------------------------------------------------------
HRESULT CGameMain::ChangeScreenMode(int nMode)
{
	HRESULT Ret = S_OK;
	BOOL bFullScreen;  // フルスクリーンモードか？

	//	現在のスクリーンモードを得る
	m_pD3D->m_pSwapChain->GetFullscreenState(&bFullScreen, NULL);

	switch (nMode)
	{
	case 0:    // ウインドウモードにする
		if( bFullScreen ) Ret = m_pD3D->m_pSwapChain->SetFullscreenState(false, NULL);
		break;
	case 1:    // フルスクリーンモードにする
		if( !bFullScreen ) Ret = m_pD3D->m_pSwapChain->SetFullscreenState(true, NULL);
		break;
	default:   // 現在のモードを切り替える
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

