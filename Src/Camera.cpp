//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１２                             ver 3.1        2022.2.16
//
//		カメラ制御
//
//		(視点変更処理・障害物回避処理が入っている)
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
// カメラプロシージャのコンストラクタ
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
//
// -------------------------------------------------------------------------------------------------------------------------
CCameraProc::CCameraProc(CGameMain* pGMain) : CBaseProc(pGMain)
{

	m_pObjArray.push_back(new CCameraObj(this));  // m_pObjArrayにオブジェクトを登録する

}

// --------------------------------------------------------------------------- 
//
// カメラオブジェクトのコンストラクタ
//
// ---------------------------------------------------------------------------
CCameraObj::CCameraObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	// カメラ制御フラグ
	m_nCtrl = CAMERA_GAMEMAIN;      // 最初はTPS視点のカメラ位置

	// 基点ワールドマトリックスの初期値
	m_mBaseWorld = XMMatrixTranslation(0, 0, 0);

	// 基点ワールドマトリックスからの変位初期値の設定
	// ① TPS視点のときの基点ワールドマトリックスからの変位値
	// ・注視点は、変位距離をm_vBaseLocalLookatTPSに設定する
	m_vBaseLocalLookatTPS = VECTOR3(0.0f, m_pGMain->m_pPcProc->GetPcObjPtr()->GetBBox()->m_vMax.y, 0.0f);

	// ・視点は、変位角度Ｘ，Ｙをm_vBaseLocalEyeRotDistTPS.x m_vBaseLocalEyeRotDistTPS.yに、変位直線距離をm_vBaseLocalEyeRotDistTPS.zに設定する
	m_vBaseLocalEyeRotDistTPS.z = -10.0f;  // 変位直線距離
	m_vBaseLocalEyeRotDistTPS.x = XMConvertToDegrees( atan2f(m_pGMain->m_pPcProc->GetPcObjPtr()->GetBBox()->m_vMax.y + 2.3f, -m_vBaseLocalEyeRotDistTPS.z)); // 変位角度Ｘ
	m_vBaseLocalEyeRotDistTPS.y = 0.0f;   // 変位角度Ｙ

	// ② FPS視点のときの基点ワールドマトリックスからの変位値
	// ・注視点・視点共に、変位距離をm_vBaseLocalEyeFPSに設定する
	m_vBaseLocalLookatFPS = VECTOR3(0.0f, 1.5f, 7.0f);
	m_vBaseLocalEyeFPS    = VECTOR3(0.0f, 1.5f, 0.3f);
	m_isLookStairs = false;
	m_isOnce = true;
	m_IsBossMovie = FALSE;
	m_IsMovePc = FALSE;
	m_bIsDeadBoss = FALSE;
	// カメラ変位初期値を設定する
	SetCamBase();
}
// ---------------------------------------------------------------------------
//
// カメラオブジェクトのデストラクタ
//
// ---------------------------------------------------------------------------
CCameraObj::~CCameraObj()
{
	;
}
//-----------------------------------------------------------------------------
// カメラオブジェクトの制御
//
//　ＰＣの動きに合わせてカメラを制御する
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CCameraObj::Update()
{
	if (m_pGMain->m_pEnmProc->CountActiveEnm() == 0 && m_isOnce && m_pGMain->m_pMapProc->GetHierarchy() != 10) {
		m_isLookStairs = true;
		m_isOnce = false;
	}
	// 注視点・視点の基点となる基点ワールドマトリックスの設定
	// (ここでは、ＰＣのローカル軸マトリックスを設定する)
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

	// カメラオブジェクトの位置を手動操作
	// また、カメラ制御を変更（ＴＰＳ視点かＦＰＳ視点か）
	//ControlCam();

	// 注視点のベクトルを作成する処理
	// (注視点のローカル座標と基点ワールドマトリックスを掛け合わせ、注視点マトリックスを得る)
	MATRIX4X4 mLookat;
	if (!m_isLookStairs) {
		// PCを見る
		mLookat = XMMatrixTranslationFromVector(m_vLocalLookat) * m_mBaseWorld;
	}
	else {
		// 階段を見る
		mLookat = XMMatrixTranslationFromVector(VECTOR3(0.0f, 0.0f, 22.0f));
	}

	if (m_nCtrl == CAMERA_GAMECLEAR || m_nCtrl == CAMERA_GAMEOVER) {
		mLookat = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	}
	m_pGMain->m_vLookatPt = GetPositionVector(mLookat);    // 注視点マトリックスから注視点位置を得る

	// カメラ（視点）ベクトルを作成する処理
	// (カメラ（視点）のローカル座標と基点ワールドマトリックスを掛け合わせ、視点マトリックスを得る)
	MATRIX4X4 mEye = XMMatrixTranslationFromVector(m_vLocalEye) * GetPositionMatrix(m_mBaseWorld);
	m_pGMain->m_vEyePt = GetPositionVector( mEye );        // 視点マトリックスから視点位置を得る

	// TPS視点の時、障害物を回避する処理
	//if (m_nCtrl == 0 && !m_isLookStairs) EvasiveObstacle();

	// 視点ベクトルと注視点ベクトルからカメラのワールドマトリックスを得る（今は使用していない）
	m_mWorld = GetLookatMatrix(m_pGMain->m_vEyePt, m_pGMain->m_vLookatPt);

	// ビューマトリックスの作成
	VECTOR3 vUpVec(0.0f, 1.0f, 0.0f);    // 上方位置を絶対座標の上方向とする
	// カメラ(視点)の位置、上方向、および注視点を使用して、左手座標系のビュー行列を作成
	m_pGMain->m_mView = XMMatrixLookAtLH(m_pGMain->m_vEyePt, m_pGMain->m_vLookatPt, vUpVec);

	// カメラの位置、上方向、およびカメラの向きを使用して、左手座標系のビュー行列を作成（使用していない）
	//VECTOR3 vEyeDir = XMVector3TransformCoord(VECTOR3(0, 0, 1), GetRotateMatrix(m_mWorld));
	//m_pGMain->m_mView = XMMatrixLookToLH(m_pGMain->m_vEyePt, vEyeDir, vUpVec);

	// ライト視点からのビュートランスフォーム（ShadowMap用）ライト視点をＰＣの位置に合わせるとき（使用していない）
	//m_pGMain->m_vLightEye = m_pGMain->m_vLookatPt + m_pGMain->m_vLightDir * 100.0f;	        // カメラ（視点）位置を光源の方向100ｍの位置にする
	//m_pGMain->m_mLightView = XMMatrixLookAtLH(m_pGMain->m_vLightEye, m_pGMain->m_vLookatPt, vUpVec);

}

//----------------------------------------------------------------------------- 
// BaseWorldがプレイヤーに近づく処理
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
		MATRIX4X4 mTemp = XMMatrixRotationY(XMConvertToRadians(m_vRotUp.y));					// 目的地（Target）へ向かう角度
		m_mBaseWorld = mTemp * m_mWorld;
		mTemp = XMMatrixTranslation(m_vPosUp.x, m_vPosUp.y + m_fJumpY, m_vPosUp.z);	// 目的地（Target）へ向かう
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
// 障害物を回避する処理
//
// ・視点と注視点との間に障害物があった場合、視点を障害物の前に移動する処理
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CCameraObj::EvasiveObstacle()
{
	VECTOR3 vHit;
	VECTOR3 vNorm;
	if (m_pGMain->m_pMapProc->Hitcheck(m_pGMain->m_vEyePt, m_pGMain->m_vLookatPt, &vHit, &vNorm))   // 障害物との接触チェック
	{
		MATRIX4X4 mTemp;
		mTemp = GetLookatMatrix(vHit, m_pGMain->m_vLookatPt);        // 障害物との接触点から注視位置を見るマトリックス
		mTemp = XMMatrixTranslation(0.0f, 0.0f, 0.01f) * mTemp;      // 障害物との接触点から1cm注視方向に移動した視点位置を得る
		m_pGMain->m_vEyePt = GetPositionVector(mTemp);
	}
}
//-----------------------------------------------------------------------------
// カメラオブジェクトの手動操作制御
//
//   カメラオブジェクトの位置を手動で操作する
//   また、カメラ制御を変更（ＴＰＳ視点かＦＰＳ視点か）
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CCameraObj::ControlCam()
{
	const float fRotUp  = 1.5f;     // 角度変更の増分値
	const float fDistUp = 0.05f;    // 距離変更の増分値

	// カメラ制御の変更（ＴＰＳ視点かＦＰＳ視点か）
	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_F)) // Fキー
	{
		m_nCtrl = (m_nCtrl + 1) % 2;   // 制御の変更（0:TPS視点と1:FPS視点）
		SetCamBase();   // カメラ変位初期値を設定する
	}

	// カメラ視点の相対位置の変更
	if (m_nCtrl == 0)   // TPS視点の時
	{

	/*
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD6)) // →
		{
			SetLocalEyeTPS(fRotUp, 0, 0);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD4)) // ←
		{
			SetLocalEyeTPS(-fRotUp, 0, 0);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD8)) // ↑
		{
			SetLocalEyeTPS(0, fRotUp, 0);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD2)) // ↓
		{
			SetLocalEyeTPS(0, -fRotUp, 0);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD7)) // 近づける
		{
			SetLocalEyeTPS(0, 0, -fDistUp);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD9)) // 遠ざかる
		{
			SetLocalEyeTPS(0, 0, fDistUp);
		}
	*/
		
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_RBRACKET)) // →  ]
		{
			//SetLocalEyeTPS(fRotUp, 0, 0);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_SEMICOLON)) // ←  ;
		{
			//SetLocalEyeTPS(-fRotUp, 0, 0);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_AT)) // ↑  @
		{
			SetLocalEyeTPS(0, fRotUp, 0);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_COLON)) // ↓  :
		{
			SetLocalEyeTPS(0, -fRotUp, 0);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_P)) // 近づける  P
		{
			SetLocalEyeTPS(0, 0, -fDistUp);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_LBRACKET)) // 遠ざかる  [
		{
			SetLocalEyeTPS(0, 0, fDistUp);
		}
		
	}
	else {    // FPS視点の時

	/*
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD6)) // →
		{
			m_vLocalEye.x -= fDistUp/5;
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD4)) // ←
		{
			m_vLocalEye.x += fDistUp/5;
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD8)) // ↑
		{
			m_vLocalLookat.y += fDistUp / 2;
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD2)) // ↓
		{
			m_vLocalLookat.y -= fDistUp / 2;
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD7)) // 近づける
		{
			m_vLocalEye.z += fDistUp / 10;
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_NUMPAD9)) // 遠ざかる
		{
			m_vLocalEye.z -= fDistUp / 10;
		}
	*/

		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_RBRACKET)) // →  ]
		{
			m_vLocalEye.x -= fDistUp / 5;
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_SEMICOLON)) // ←  ;
		{
			m_vLocalEye.x += fDistUp / 5;
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_AT)) // ↑  @
		{
			m_vLocalLookat.y += fDistUp / 2;
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_COLON)) // ↓  :
		{
			m_vLocalLookat.y -= fDistUp / 2;
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_P)) // 近づける  P
		{
			m_vLocalEye.z += fDistUp / 10;
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_LBRACKET)) // 遠ざかる  [
		{
			m_vLocalEye.z -= fDistUp / 10;
		}

	}

	// 基準値に戻す処理
	if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_HOME) || m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_H))
	{
		SetCamBase();    // カメラ変位初期値を設定する
	}
}

//----------------------------------------------------------------------------- // -- 2020.11.19
// ＴＰＳのときカメラ位置の設定処理
//
//   m_vLocalEyeRotDist.xとm_vLocalEyeRotDist.yに回転角度、m_vLocalEyeRotDist.zに
//   注視点から視点までの直線距離が入っている、
//   その値に、引数の回転と距離を加算し、その結果を基に、変位値を計算し、
//   その結果をm_vLocalEyeに設定する
//
//   引数	float fRotY : Ｙ回転
//			float fRotX : Ｘ回転
//			float fDist : 距離
//
//   戻り値　なし
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
// カメラ変位初期値を設定する処理
//
// ① TPS視点のとき
//    視点は、変位角度と変位直線距離を、一旦m_vLocalEyeRotDistに設定し、
//    その後SetLocalEyeTPS関数によって、m_vLocalEyeに設定する
//    注視点は、変位距離をm_vLocalLookatTPSに設定する
//
// ② FPS視点のとき
//    視点は、変位距離をm_vLocalEyeに設定する
//    注視点は、変位距離をm_vLocalLookatに設定する
//
//   引数	なし
//
//   戻り値　なし
//-----------------------------------------------------------------------------
void	CCameraObj::SetCamBase()
{
	// カメラ変位の初期値を設定する
	if (m_nCtrl == CAMERA_GAMEMAIN)
	{
		m_vLocalEyeRotDist = m_vBaseLocalEyeRotDistTPS;
		m_vLocalLookat = m_vBaseLocalLookatTPS;
		SetLocalEyeTPS(-45, 5, -5);
	}
	 else if(m_nCtrl == CAMERA_GAMEOVER || m_nCtrl == CAMERA_GAMECLEAR){
		// ゲームオーバー、クリア時のカメラ角度
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