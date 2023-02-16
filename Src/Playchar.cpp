//=============================================================================
//		プレイキャラクターの制御
//																Playchar.cpp
//=============================================================================
#include  "Playchar.h"
#include  "Camera.h"
#include  "SearchRoute.h"
#include  "BackFore.h"
#include  "Weapon.h"
#include  "Map.h"
#include  "Effect.h"
#include  "Enemy.h"
#include  "EnemyBoss.h"
#include  "Ally.h"
#include  "Title.h"

//============================================================================
// PC定数クラス
//============================================================================
// 初期化
const float PcConstruct::SPEED = 0.08f;
const float PcConstruct::ROT_SPEED = 8.0f;
const float PcConstruct::WAITTIME_GUN = 0.2f;
const VECTOR3 PcConstruct::HITBOX_MIN = VECTOR3(-0.3f, -0.05f, -0.3f);
const VECTOR3 PcConstruct::HITBOX_MAX = VECTOR3(0.3f, 2.0f, 0.3f);

//============================================================================
// PCプロシージャクラス
//============================================================================
// ---------------------------------------------------------------------------
// ＰＣプロシージャのコンストラクタ
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
// ---------------------------------------------------------------------------
CPcProc::CPcProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// キャラクター　－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Char/PC/PC_Skin.mesh"));    // キャラクター
	m_pMesh->LoadAnimation(_T("Data/Char/PC/Idle.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/PC/Run3.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/PC/Sword.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/PC/Arrow.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/PC/Axe.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/PC/Gun.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/PC/Dead.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/PC/Fall.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/PC/Victory.anmx"));
	m_pMesh->m_fHeightMax = 0.01f;
	m_pMesh->m_iMaxDevide = 16;
	m_HoldItem.m_nHoldObjNo = 0;	// 手に持つ武器のオブジェ番号
	m_HoldItem.m_nPosMesh = 0;		// 根元位置のメッシュ番号
	m_HoldItem.m_nPosBone = 0;	// 根元位置のボーン番号
	m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.0f, 0.0f);  // 手に持つ武器の位置オフセット
	m_HoldItem.m_nOffsetRot = VECTOR3(0.0f, 0.0f, 0.0f);  // 手に持つ武器の角度オフセット
	
	// -------------------------------------------------------------------------
	m_pObjArray.push_back(new CPcObj(this));	// m_pObjArrayにオブジェクトを登録する

	// ------------------------------------------------------------------------- 
	m_pAbility = new CAbility(pGMain);
}

// ---------------------------------------------------------------------------
// ＰＣプロシージャのデストラクタ
// ---------------------------------------------------------------------------
CPcProc::~CPcProc()
{
	delete(m_pAbility);
}

//============================================================================
// アクセス関数
//============================================================================
CAbility* CPcProc::GetAbility()
{
	return m_pAbility;
}

CPcObj* CPcProc::GetPcObjPtr()
{
	return (CPcObj*)GetObjArrayPtr()[0];
}


//============================================================================
// PCオブジェクトクラス
//============================================================================
// ---------------------------------------------------------------------------
// ＰＣオブジェクトのコンストラクタ
//
//  引数
//  CBaseProc* pProc		： 親プロシージャへのポインタ
// ---------------------------------------------------------------------------
CPcObj::CPcObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	m_fLocalRotY = 0.0f;               

	m_fSpeed = PcConstruct::SPEED;						// 移動速度      
	m_fRotSpeed = PcConstruct::ROT_SPEED;				// 回転速度      
	m_nAllyNum = PcConstruct::START_ALLY_NUM;			// 初期の味方の数

	// バウンディングボックスの設定
	m_pBBox = new CBBox(m_pGMain->m_pShader, PcConstruct::HITBOX_MIN, PcConstruct::HITBOX_MAX);
	m_pBBox->m_mWorld = m_mWorld;                          

	m_nMaxHp = m_nAllyNum + 1;
	m_nAtc = 0;
	m_nTag = PC;

	// ＰＣオブジェクトの初期化処理
	m_bActive = TRUE;
	ResetStatus();

	// アニメーションの初期化
	m_AnimStatus.playAnim = true;
	m_AnimStatus.SetNum(PcConstruct::ANIM_IDLE);
	m_AnimStatus.animFrame = 0;

	m_nAttackKind = ATTACK_NONE;
	m_bIsCanMove = TRUE;
	m_bIsHaveAxe = TRUE;
	m_bIsToLateKeyPush = FALSE;

	m_nHp = m_nAllyNum + 1;
	m_nGunMagazine = PcConstruct::BASE_MAGAZINE_NUM;
	m_nCnt2 = 0;
	m_nGameOverPosY = 0;
	m_nCoolTime = 0;
	m_nShotSlashCoolTime = 0;
	m_fAngle = 0.0f;
}

// ---------------------------------------------------------------------------
// ＰＣオブジェクトのデストラクタ
// ---------------------------------------------------------------------------
CPcObj::~CPcObj() 
{
}

// ---------------------------------------------------------------------------
// Map遷移後、遷移前のAllyOBjの数だけ再生成
// ---------------------------------------------------------------------------
void CPcObj::SetAllyObj()
{
	for (int i = 0; i < m_nAllyNum; i++) {
		m_pGMain->m_pAllyProc->Start(GetPositionVector(m_mWorld), TRUE);
	}
}

// ---------------------------------------------------------------------------
// リトライ後、前回の最終階層数を参照してAllyNumを設定
// ---------------------------------------------------------------------------
void CPcObj::SetRetryAllyNum()
{
	int hierarchy = m_pGMain->m_pMapProc->GetHierarchy();
	m_nAllyNum = PcConstruct::START_ALLY_NUM + hierarchy / 4;
	m_pGMain->m_pBackForeProc->AddAllAllyNum(m_nAllyNum);
}

//-----------------------------------------------------------------------------
// ＰＣオブジェクトの更新
//
//　キーボードでＰＣを動かす
//　ＰＣのアニメーション、ジャンプ、自然落下
//　ワールド座標返還、マップとの接触判定
//　ＰＣの攻撃処理を行う
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CPcObj::Update()
{
	// 一つ前の位置を保存
	m_mWorldOld = m_mWorld;

	// 攻撃種類を初期化
	m_nAttackKind = ATTACK_NONE;

	// AllyNumとHPを同期する
	m_nHp = m_nAllyNum + 1;
	// HPなし、死亡アニメーションに移行しているか
	if (m_nHp <= 0 && m_AnimStatus.animNum != PcConstruct::ANIM_DEAD) {
		m_dwStatus = DEAD;
		m_AnimStatus.SetNum(PcConstruct::ANIM_DEAD);
		m_nCnt1 = 60 * PcConstruct::WAITTIME_DEAD;
	}

	// メインが移動を許可している状態のみ
	if (m_pGMain->m_isMove) {
		// 各状態に応じた処理
		switch (m_dwStatus) {
		case  FLASH:  // ＰＣがダメージから復帰したときの処理
		case  NORMAL: // ＰＣが通常状態のときの処理
			UpdateNormal();  // ＰＣオブジェクトの更新 通常状態（NORMAL）
			break;

		case  DAMAGE: // ＰＣがダメージ状態のときの処理
			UpdateDamage();  // ＰＣオブジェクトの更新 ダメージ状態（DAMAGE）
			break;

		case  DEAD:	// ＰＣが死亡状態のときの処理
			UpdateDead();  // ＰＣオブジェクトの更新 死亡状態（DEAD）
			break;

		}
		// ボスとの重なり処理
		SetKeepOffset(m_pGMain->m_pEnmBossProc->GetEnmBossObjPtr());

		// マップコリジョンと自然落下
		m_fJumpY += (0.5f * GRAVITY * 0.00005f) * m_fJumpTime;	// 自然落下
		m_fJumpTime += 1.0f;
		if (m_pGMain->m_pMapProc->isCollisionMoveGravityPc(&m_mWorld, m_mWorldOld) != 3)  // マップコリジョン
		{
			m_fJumpY = 0.0f;  // 自然落下の停止
			m_fJumpTime = 0.0f;
		}

		// オブジェクトを空間分割へ登録 
		// SetObj()が、オブジェクトを空間分割へ登録する関数である
		VECTOR3 vMin, vMax;
		m_pGMain->m_pMapProc->GetPcSpPart()->MakeAABB(
			GetPositionVector(m_mWorldOld), GetPositionVector(m_mWorld), 2.0f, vMin, vMax);
		m_pGMain->m_pMapProc->GetPcSpPart()->SetObj(vMin, vMax, this);
	}
	else {
		// 移動不可時は、Idleモーションで待機
		m_AnimStatus.SetNum(PcConstruct::ANIM_IDLE);
		m_bIsKeyPush = FALSE;
	}
	
	// カメラとPCの位置でレイを飛ばし、重なったマップメッシュを透過する
	m_pGMain->m_pMapProc->CheckCrossStageMesh(GetPositionVector(m_mWorld), GetPositionVector(m_pGMain->m_pCameraProc->GetCameraObjPtr()->GetWorld()));

	//レンダリング
	GetMesh()->Render(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
	// バウンディングボックス
	m_pBBox->m_mWorld = m_mWorld;

	// 手に持つ武器やアイテムの表示をする
	if (GetHoldItem().m_nHoldObjNo != 0) // 手に持つ武器やアイテムがあるとき
	{
		MATRIX4X4 mHold;
		mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, GetHoldItem().m_nPosBone, GetHoldItem().m_nPosMesh);  // 手の位置のボーンマトリックス
		m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->Disp(mHold, GetHoldItem()); // アイテム表示
	}
}

//-----------------------------------------------------------------------------
// ＰＣオブジェクトの更新 通常状態（NORMAL）
//
//　キーボードでＰＣを動かす
//　ＰＣのアニメーション、ジャンプ、自然落下
//　ワールド座標返還、マップとの接触判定
//　ＰＣの攻撃処理を行う
//
//
//   引数　なし
//-----------------------------------------------------------------------------
void	CPcObj::UpdateNormal()
{
	if (m_dwStatus == FLASH) // ＰＣがダメージから復帰したときの処理
	{
		m_nCnt1--;
		if (m_nCnt1 <= 0) m_dwStatus = NORMAL;
	}
	UpdateNormalMove();      // ＰＣオブジェクトの移動処理
	UpdateNormalAttack();    // ＰＣオブジェクトの攻撃処理
}

//-----------------------------------------------------------------------------
// ＰＣオブジェクトの移動処理
//
//　キーボード／マウスでＰＣを動かす
//　ＰＣのアニメーション、ジャンプ、自然落下
//　ワールド座標返還、マップとの接触判定
//
//
//   引数　なし
//-----------------------------------------------------------------------------
void	CPcObj::UpdateNormalMove()
{
	CDirectInput* pDI = m_pGMain->m_pDI;

	MATRIX4X4 mYaw;  // ＰＣＹ軸回転マトリックス
	MATRIX4X4 mPos;  // ＰＣ移動マトリックス
	m_bIsKeyPush = FALSE;// キーを押したかどうかのチェック
	m_bIsToLateKeyPush = FALSE;		// 2つ以上のキーが押されているか

	// 自然落下の初期値
	mPos = XMMatrixTranslation(0.0f, m_fJumpY, 0.0f);

	// ImGuiの表示
	static bool bImgOpen = false;
	if (pDI->CheckKey(KD_TRG, DIK_J))
		bImgOpen = true;
	if (bImgOpen) ShowImgui(&bImgOpen);

	// ＰＣの移動処理  ---------------------------------------------
	// 前進処理
	if ((pDI->CheckKey(KD_DAT, DIK_W) || pDI->CheckJoy(KD_DAT, DIJ_UP)) && m_bIsCanMove) //	↑キー
	{
		m_bIsKeyPush = TRUE;
		mPos = UpdateNormalMoveKeystate(DIK_W);
	}
	// 後退処理
	if ((pDI->CheckKey(KD_DAT, DIK_S) || pDI->CheckJoy(KD_DAT, DIJ_DOWN)) && m_bIsCanMove) //↓キー
	{
		m_bIsKeyPush = TRUE;
		mPos = UpdateNormalMoveKeystate(DIK_S);
	}
	// 右移動処理
	if ((pDI->CheckKey(KD_DAT, DIK_D) || pDI->CheckJoy(KD_DAT, DIJ_RIGHT)) && m_bIsCanMove)//→キー
	{
		m_bIsKeyPush = TRUE;
		mPos = UpdateNormalMoveKeystate(DIK_D);
	}
	// 左移動処理
	if ((pDI->CheckKey(KD_DAT, DIK_A) || pDI->CheckJoy(KD_DAT, DIJ_LEFT)) && m_bIsCanMove)//←キー
	{
		m_bIsKeyPush = TRUE;
		mPos = UpdateNormalMoveKeystate(DIK_A);
	}

	// 目標角へ回転
	UpdateMoveRot();

	if (!m_bIsKeyPush && m_AnimStatus.animNum < 2)  // キーを押していないとき && 攻撃モーション中ではない
	{
		m_AnimStatus.SetNum(0);
	}

	// ローカル軸を基準として、ＰＣを移動させるワールドマトリックスを設定
	// // 攻撃モーション中移動を行わない
	// y軸回転のみを適応
	if (m_bIsCanMove) {
		mYaw = XMMatrixRotationY(XMConvertToRadians(GetRotateVector(m_pGMain->m_mView).y - 90.0f));
		mYaw = XMMatrixRotationY(XMConvertToRadians(m_vRotUp.y)) * mYaw;
		m_mWorld = mPos * mYaw * GetLocalMatrix();
	}
}

//----------------------------------------------------------------------------- 
// ＰＣオブジェクトの移動処理　キー操作毎の各種移動処理
//
//   引数　DWORD Key キー種別　DirectInputのキーコード
//
//   戻り値  移動マトリックス
//-----------------------------------------------------------------------------
MATRIX4X4 CPcObj::UpdateNormalMoveKeystate(DWORD DIKey)
{
	MATRIX4X4 mPos;

	if (m_AnimStatus.animNum == 0) m_AnimStatus.SetNum(1);

	// 進行方向に回転してから前進
		// ルートボーンアニメーションを行うかどうかルートアニメーションタイプを確認する
	if (GetMesh()->GetRootAnimType(m_AnimStatus.animNum) == eRootAnimNone)
	{
		// ルートボーンアニメーションを行わず固定の前進移動値
		mPos = XMMatrixTranslation(0.0f, m_fJumpY, m_fSpeed);
	}
	else {
		// ルートボーンアニメーションでの前進移動値
		mPos = XMMatrixTranslation(0.0f, m_fJumpY, 0.0f) * GetMesh()->GetRootAnimUpMatrices(m_AnimStatus);
	}

	// 進行方向に回転する処理
	// ・回転角度はm_vRotUp.yにある。-180～180度
	// ・１回の回転速度はm_fRotSpeed
	if (m_vRotUp.y > 180.0f)  m_vRotUp.y -= 360.0f;
	if (m_vRotUp.y < -180.0f) m_vRotUp.y += 360.0f;

	if (!m_bIsToLateKeyPush)
	{
		switch (DIKey)
		{
		case DIK_W:    // 前進
			m_fAngle = 0.0f;
			break;
		case DIK_S:    // 後退
			if (m_vRotUp.y >= 0)  // 最小回転になるように方向を合わせる
			{
				m_fAngle = 180.0f;
			}
			else {
				m_fAngle = -180.0f;
			}
			break;
		case DIK_D:    // 右
			m_fAngle = 90.0f;
			if (m_vRotUp.y == -180.0f) m_vRotUp.y = 180.0f;  // 最小回転になるように方向を合わせる
			break;
		case DIK_A:    // 左
			m_fAngle = -90.0f;
			if (m_vRotUp.y == 180.0f) m_vRotUp.y = -180.0f;  // 最小回転になるように方向を合わせる
			break;
		}
		m_bIsToLateKeyPush = TRUE;
	}
	else {
		switch (DIKey)
		{
		case DIK_S:    // 後退
			// 前進と後退の同時押し
			m_fAngle = 0.0f;
			mPos = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
			break;

		case DIK_D:    // 右
			if (m_fAngle == 0.0f) {
				// 前進と右
				m_fAngle += 45.0f;
			}
			else {
				// 後退と右
				m_fAngle -= 45.0f;
			}
			break;

		case DIK_A:    // 左
			if (m_fAngle == 0.0f) {
				// 前進と左
				m_fAngle -= 45.0f;
			}
			else {
				// 後退と右
				m_fAngle += 45.0f;
			}
			break;
		}
	}
	return mPos;
}

//-----------------------------------------------------------------------------
// 目標回転角に向かって回転する処理
//-----------------------------------------------------------------------------
void CPcObj::UpdateMoveRot()
{
	if (m_vRotUp.y > m_fAngle)  // 左回転
	{
		m_vRotUp.y -= m_fRotSpeed;
		if (m_vRotUp.y < m_fAngle) m_vRotUp.y = m_fAngle;
	}
	if (m_vRotUp.y < m_fAngle)  // 右回転
	{
		m_vRotUp.y += m_fRotSpeed;
		if (m_vRotUp.y > m_fAngle) m_vRotUp.y = m_fAngle;
	}
}

//-----------------------------------------------------------------------------
// ＰＣオブジェクトの攻撃処理
//
//　キーボード／マウスでＰＣの攻撃処理を行う
//
//
//   引数　なし
//-----------------------------------------------------------------------------
void	CPcObj::UpdateNormalAttack()
{
	CDirectInput* pDI = m_pGMain->m_pDI;
	CBaseProc* pProc = m_pGMain->m_pPcProc;
	CAbility* ability = m_pGMain->m_pPcProc->GetAbility();

	// ------------------------------------------------------------- 
	// 遠隔武器
	// 現在の所持している武器を参照
	switch (ability->GetLongWeapon())
	{
	case ARROW:
		if (pDI->CheckJoy(KD_DAT, DIJ_A) || pDI->CheckMouse(KD_TRG, DIM_LBUTTON) && m_AnimStatus.animNum != PcConstruct::ANIM_ARROW && m_bIsCanMove) // Xキー
		{
			m_nAttackKind = ATTACK_ALLOW;
			// カーソルの位置を向く
			GetCursorRotate();
			// 弓を持つ
			pProc->SetHoldItem(ATTACK_ALLOW);
			// アニメーション設定
			m_AnimStatus.SetNum(PcConstruct::ANIM_ARROW);
			m_AnimStatus.animSpeed = ability->GetAbilityLevel(0) * 0.3f + 1;
			// アニメーション中移動不可
			m_bIsCanMove = FALSE;
			m_pGMain->m_pArrow->Play();
		}
		break;

	case GUN:
		// クリック開始時、銃アニメーション状態での長押し時処理
		if ((pDI->CheckJoy(KD_DAT, DIJ_A) || pDI->CheckMouse(KD_DAT, DIM_LBUTTON) && m_AnimStatus.animNum == PcConstruct::ANIM_GUN) ||
			pDI->CheckMouse(KD_TRG, DIM_LBUTTON) && m_bIsCanMove)
		{
			if (m_nCnt2 <= 0) {
				// リロード中、クールタイムではない
				m_bIsCanMove = FALSE;
				if (m_nGunMagazine > 0) {
					// マガジン残弾あり
					if (m_AnimStatus.animNum != PcConstruct::ANIM_GUN)
					{
						// アニメーション、持っているアイテムの設定
						m_AnimStatus.SetNum(PcConstruct::ANIM_GUN);
						pProc->SetHoldItem(ATTACK_GUN);
					}
					m_nGunMagazine--;
					GetCursorRotate();
					m_pGMain->m_pGun->Play();
					m_nAttackKind = ATTACK_GUN;
					m_pGMain->m_pWeaponProc->m_pWeaponGunProc->Start(XMMatrixTranslation(0.0f, 0.7f, 0.5f) * m_mWorld, PC);
					if (m_nGunMagazine <= 0) {
						// リロード
						m_nCnt2 = 60 * PcConstruct::WAITTIME_RELOAD - 20 * ability->GetAbilityLevel(1);
					}
					else {
						// 発射間隔
						m_nCnt2 = 60 * PcConstruct::WAITTIME_GUN;
					}
				}
			}
		}
		else {
			if (m_AnimStatus.animNum == PcConstruct::ANIM_GUN) {
				// 銃を撃ち止めた直後、アニメーションと武器を解除
				m_AnimStatus.SetNum(PcConstruct::ANIM_IDLE);
				pProc->ResetHoldItem();
				m_AnimStatus.animSpeed = 1;
				m_bIsCanMove = TRUE;
			}
		}

		if (pDI->CheckJoy(KD_DAT, DIJ_A) || pDI->CheckKey(KD_TRG, DIK_R) && m_bIsCanMove) // Rキー
		{
			// リロード
			m_nCnt2 = 60 * PcConstruct::WAITTIME_RELOAD - 20 * ability->GetAbilityLevel(1);
			m_nGunMagazine = 0;

		}

		// リロード処理
		if (m_nGunMagazine <= 0 && m_nCnt2 <= 0) {
			m_nGunMagazine = PcConstruct::BASE_MAGAZINE_NUM + 5 * ability->GetAbilityLevel(7);
		}
		break;

	default:
		break;
	}

	if (m_AnimStatus.isEnd(PcConstruct::ANIM_ARROW))
	{
		// 弓矢アニメーション終了時
		m_pGMain->m_pWeaponProc->m_pWeaponArrowProc->Start(m_mWorld, PC); // 弓矢発射
		m_AnimStatus.SetNum(0);
		pProc->ResetHoldItem();
		m_AnimStatus.animSpeed = 1;
		m_bIsCanMove = TRUE;
	}

	// ------------------------------------------------------------- 
	// 近接攻撃
	switch (ability->GetShortWeapon()) {
	case SWORD:
		if (pDI->CheckJoy(KD_TRG, DIJ_C) || pDI->CheckMouse(KD_TRG, DIM_RBUTTON) && m_AnimStatus.animNum != PcConstruct::ANIM_SWORD && m_nCoolTime <= 0 && m_bIsCanMove) // Zキー
		{
			m_nAttackKind = ATTACK_SWORD;
			// カーソルの方向を向く
			GetCursorRotate();
			// 剣を持つ
			pProc->SetHoldItem(ATTACK_SWORD);
			if (ability->GetAbilityLevel(4) > 0) {
				m_nAttackKind = ATTACK_SHOTSWORD;
				m_pGMain->m_pWeaponProc->m_pWeaponShotSlashProc->Start(m_mWorld, PC);	// 斬撃
			}
			m_pGMain->m_pEffectProc->m_pBillSlashProc->Start(m_mWorld);	// 剣のエフェクト
			// アニメーション設定
			m_AnimStatus.SetNum(PcConstruct::ANIM_SWORD);
			m_AnimStatus.animSpeed = ability->GetAbilityLevel(3) * 0.3f + 1;
			// 攻撃モーション中移動不可
			m_bIsCanMove = FALSE;
			m_pGMain->m_pSlash->Play();
			m_nCoolTime = 60 * PcConstruct::WAITTIME_MELEE_WEAPON * (1 - 0.3 * ability->GetAbilityLevel(3));
		}
		break;

	case AXE:
		if ((pDI->CheckJoy(KD_TRG, DIJ_C) || pDI->CheckMouse(KD_TRG, DIM_RBUTTON)) && m_AnimStatus.animNum != PcConstruct::ANIM_AXE && m_nCoolTime <= 0 && m_bIsCanMove) // Zキー
		{
			m_nAttackKind = ATTACK_AXE;
			// カーソルの方向を向く
			GetCursorRotate();
			if (ability->GetAbilityLevel(4) > 0) {
				// 遠投
				if (m_bIsHaveAxe)
				{
					// まだ投げていない
					m_pGMain->m_pWeaponProc->m_pWeaponAxeShotProc->Start(m_mWorld* XMMatrixTranslation(0.0f, 1.0f, 1.0f), 5 + 2 * ability->GetAbilityLevel(4), PC);
					m_bIsHaveAxe = FALSE;
					m_nAttackKind = ATTACK_AXESHOT;
				}
			}
			else {
				// 通常
				m_pGMain->m_pEffectProc->m_pBillSlashProc->Start(m_mWorld);	// エフェクト
				pProc->SetHoldItem(ATTACK_AXE);
				m_nCoolTime = 60 * PcConstruct::WAITTIME_MELEE_WEAPON * (1 - ability->GetAbilityLevel(3) * 0.3);
			}
			// アニメーション設定
			m_AnimStatus.SetNum(PcConstruct::ANIM_AXE);
			m_AnimStatus.animSpeed = ability->GetAbilityLevel(3) * 0.3f + 1;
			// 攻撃モーション中移動不可
			m_bIsCanMove = FALSE;
			m_pGMain->m_pSlash->Play();
		}
		break;

	default:
		break;
	}

	// 近接攻撃の判定
	if (m_AnimStatus.animNum == PcConstruct::ANIM_SWORD || m_AnimStatus.animNum == PcConstruct::ANIM_AXE)
	{
		MATRIX4X4 mSword, mHold;
		VECTOR3 vMin, vMax;
		if (GetHoldItem().m_nHoldObjNo != 0)
		{
			mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, GetHoldItem().m_nPosBone, GetHoldItem().m_nPosMesh);  // 手の位置のボーンマトリックス
			m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->GetInfo(mHold, GetHoldItem(), mSword, vMin, vMax); // ボーンマトリックスを指定して手に持つアイテム情報を得る
			if (m_AnimStatus.animNum == PcConstruct::ANIM_AXE && ability->GetAbilityLevel(10) == 0) {
				// 遠投未収得時の斧の攻撃判定
				m_pGMain->m_pWeaponProc->m_pWeaponAxeProc->Start(mSword, vMin, vMax, PC);
			}
			if (m_AnimStatus.animNum == PcConstruct::ANIM_SWORD) {
				// 剣攻撃の判定
				m_pGMain->m_pWeaponProc->m_pWeaponSwordProc->Start(mSword, vMin, vMax, PC);  
			}
		}

		if (m_AnimStatus.isEnd(PcConstruct::ANIM_SWORD))
		{
			// 剣攻撃の終了処理
			m_AnimStatus.SetNum(PcConstruct::ANIM_IDLE);
			m_AnimStatus.animSpeed = 1;
			pProc->ResetHoldItem();
			m_bIsCanMove = TRUE;
		}

		if (m_AnimStatus.isEnd(PcConstruct::ANIM_AXE))
		{
			// 斧攻撃の終了処理
			m_AnimStatus.SetNum(PcConstruct::ANIM_IDLE);
			pProc->ResetHoldItem();
			m_AnimStatus.animSpeed = 1;
			m_bIsCanMove = TRUE;
		}
	}

	// カウントダウン
	if (m_nCnt2 > 0) {
		m_nCnt2--;
	}

	if (m_nCoolTime > 0) {
		m_nCoolTime--;
	}
}

//-----------------------------------------------------------------------------
// ＰＣオブジェクトの更新 ダメージ状態（DAMAGE）
//
//　ダメージを受けたときの処理
//
//
//   引数　なし
//-----------------------------------------------------------------------------
void	CPcObj::UpdateDamage()
{
	MATRIX4X4 mTemp;

	mTemp = XMMatrixTranslation(0, 0, -0.25f);	// バックする
	m_mWorld = mTemp * m_mWorld;

	// 攻撃を受けたダメージ
	if (m_pHitObj->GetAtc() > 0) {
		// Allyオブジェクトを削除
		m_pGMain->m_pAllyProc->DeleteObj(1);
	}
	if (m_nHp <= 0)		// HPが０なので死亡へ
	{
		m_nHp = 0;
		m_dwStatus = DEAD;		// HPが０なので死亡へ
		m_AnimStatus.SetNum(PcConstruct::ANIM_DEAD);	// 死亡モーションにする
		m_nCnt1 = PcConstruct::WAITTIME_DEAD;			// 死亡時間の設定
	}
	else {
		m_nCnt1 = PcConstruct::WAITTIME_FLASH;   // 無敵状態の時間
		m_dwStatus = FLASH;     // ダメージからの復帰処理を行う
	}
}
//----------------------------------------------------------------------------- 
// ＰＣオブジェクトの更新 死亡状態（DEAD）
//
//　死亡状態のときの処理
//
//
//   引数　なし
//-----------------------------------------------------------------------------
void	CPcObj::UpdateDead()
{
	m_pGMain->m_pBackForeProc->GetSprite()->DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(255, 0, 0), 0.5f); // 画面を赤くする
	m_bIsCanMove = TRUE;
	m_pGMain->m_pPcProc->ResetHoldItem();
	m_pGMain->m_pAllyProc->ResetHoldItem();

	if (m_AnimStatus.isEnd(PcConstruct::ANIM_DEAD))  // 死亡モーションが終わったとき
	{
		m_AnimStatus.playAnim = false;	// アニメーションを止める
		m_AnimStatus.animFrame = m_AnimStatus.endFrame - 1; // フレームエンドの一つ手前にする
	}
	if (--m_nCnt1 <= 0) // 死亡時間が終わったとき
	{
		m_pGMain->m_pBackForeProc->SetBlackOut(TRUE);
		if (m_pGMain->m_pBackForeProc->GetWhiteOut()) {
			m_pGMain->m_pBackForeProc->AddDeadNum(1);
			m_pGMain->m_dwGameStatus = GAMEOVER;	// ゲームオーバーへ
			m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetCtrl(CAMERA_GAMEOVER);	// カメラのモードをゲームオーバーに
			m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetCamBase();				// カメラの位置を変更
			m_pGMain->m_pBgm1->Stop();
			m_pGMain->m_pBossBgm->Stop();
			//m_pGMain->m_pGameOver->Play(AUDIO_LOOP);
		}
	}
}

//----------------------------------------------------------------------------- 
// ゲームオーバー時の処理
//
//   引数　なし
//-----------------------------------------------------------------------------
void CPcObj::UpdateGameOver()
{
	m_mWorld = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	m_mWorld = XMMatrixRotationY(XMConvertToRadians(90.0f));

	// 落下アニメーションにする
	m_AnimStatus.SetNum(PcConstruct::ANIM_FALL);
	m_AnimStatus.playAnim = true;

	// VECTOR3に変換
	VECTOR3 pos = GetPositionVector(m_mWorld);
	// プレイヤーの位置を下げる
	m_mWorld = XMMatrixTranslation(0.0f, -1.0f, 0.0f) * m_mWorld;
	m_mWorld = XMMatrixRotationX(XMConvertToRadians(20)) * m_mWorld;

	// z軸に2進める
	pos.z += 2;
	// Spriteを描画
	m_nGameOverPosY += 5;
	if (m_nGameOverPosY >= WINDOW_HEIGHT)
	{
		m_nGameOverPosY = 0;
	}
	m_pGMain->m_pBackForeProc->GetSprite()->Draw3D(m_pGMain->m_pTitleProc->GetImageOver(), pos, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vEyePt, VECTOR2(WINDOW_WIDTH / 60, WINDOW_HEIGHT / 60), VECTOR2(0, m_nGameOverPosY), VECTOR2(WINDOW_WIDTH, WINDOW_HEIGHT));

	// PCを描画
	GetMesh()->Render(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
}

//----------------------------------------------------------------------------- 
// ゲームクリア時の処理
//
//   引数　なし
//-----------------------------------------------------------------------------
void CPcObj::UpdateGameClear()
{
	m_mWorld = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	m_mWorld = XMMatrixRotationY(XMConvertToRadians(180.0f));
	m_mWorld = XMMatrixRotationX(XMConvertToRadians(-45.0f)) * m_mWorld;

	// 落下アニメーションにする
	m_AnimStatus.SetNum(PcConstruct::ANIM_VICTORY);
	m_AnimStatus.playAnim = true;

	// VECTOR3に変換
	VECTOR3 pos = GetPositionVector(m_mWorld);
	// z軸に2進める
	pos.z += 2;
	// プレイヤーの位置を下げる
	m_mWorld = XMMatrixTranslation(0.0f, -1.5f, 0.0f) * m_mWorld;
	m_mWorld = XMMatrixRotationX(XMConvertToRadians(30)) * m_mWorld;
	// Spriteを描画
	m_pGMain->m_pBackForeProc->GetSprite()->Draw3D(m_pGMain->m_pTitleProc->GetImageClear(), pos, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vEyePt, VECTOR2(WINDOW_WIDTH / 60, WINDOW_HEIGHT / 60), VECTOR2(0, 0), VECTOR2(WINDOW_WIDTH, WINDOW_HEIGHT));
	//m_mWorld = XMMatrixTranslation(0.0f, -1.0f, 0.0f) * m_mWorld;
	// PCを描画
	GetMesh()->Render(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
}

//----------------------------------------------------------------------------------------------------
//現在のキーの方向を向く角度を返す
//----------------------------------------------------------------------------------------------------
void CPcObj::GetCursorRotate()
{
	// カーソルの座標
	VECTOR2 cursorPos;
	cursorPos.x= m_pGMain->m_pDI->GetMousePos().x;
	cursorPos.y = m_pGMain->m_pDI->GetMousePos().y;
	
	VECTOR3 nearpos, farpos, hitPos, normal;
	nearpos = XMVector3Unproject(VECTOR3(cursorPos.x, cursorPos.y, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, 1.0f,
		m_pGMain->m_mProj, m_pGMain->m_mView, XMMatrixIdentity());
	farpos = XMVector3Unproject(VECTOR3(cursorPos.x, cursorPos.y, 1.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, 1.0f,
		m_pGMain->m_mProj, m_pGMain->m_mView, XMMatrixIdentity());

	m_pGMain->m_pMapProc->Hitcheck(nearpos, farpos, &hitPos, &normal);
	float rot = GetTargetRotateVector(m_mWorld, hitPos).y;

	m_mWorld = XMMatrixRotationY(XMConvertToRadians(rot)) * m_mWorld;
}

//============================================================================
// アクセス関数
//============================================================================
MATRIX4X4 CPcObj::GetLocalMatrix()
{
	MATRIX4X4 mPos = GetPositionMatrix(m_mWorld);
	MATRIX4X4 mRotY;

	mRotY = XMMatrixRotationY(XMConvertToRadians(m_fLocalRotY));
	mRotY = mRotY * mPos;
	return mRotY;
}

void CPcObj::SetLocalRotY(float fRotY) 
{ 
	m_fLocalRotY = fRotY; 
}

void CPcObj::AddAllyNum(int num)
{ 
	m_nAllyNum += num; 
}

BOOL CPcObj::GetKeyPush() 
{ 
	return m_bIsKeyPush; 
}

int CPcObj::GetAttackKind() 
{ 
	return m_nAttackKind; 
}

int CPcObj::GetAllyNum() 
{ 
	return m_nAllyNum; 
}

int CPcObj::GetGunMagazine() 
{ 
	return m_nGunMagazine; 
}

BOOL CPcObj::GetIsHaveAxe()
{
	return m_bIsHaveAxe;
}

void CPcObj::SetIsHaveAxe(BOOL flag)
{
	m_bIsHaveAxe = flag;
	m_nCoolTime = 60 * PcConstruct::WAITTIME_MELEE_WEAPON * (1 - m_pGMain->m_pPcProc->GetAbility()->GetAbilityLevel(3) * 0.3);
}

int CPcObj::GetCoolTime()
{
	return m_nCoolTime;
}

void CPcObj::SetCanMove(BOOL flag)
{
	m_bIsCanMove = flag;
}

// ImGui表示処理
void CPcObj::ShowImgui(bool* p_open)
{
	// ウィンドウ位置とサイズを設定します。ImGuiCond_Once により、初回のみ設定されます。
	ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 420), ImGuiCond_Once);
	// 各フレームの枠線
	ImGui::GetStyle().FrameBorderSize = 1.0f; // 枠線の太さ
	// ウィンドウ定義の設定
	// ・Beginでウインドウの名前設定。なお、名前はウインドウごとにユニークになるようにすること
	// ・p_openは、閉じるボタンを押したときfalseが返る
	if (ImGui::Begin(u8"ImGui デバックウインドウ", p_open))
	{
		if (ImGui::Button(u8"経路テーブル生成", ImVec2(300, 20)))
		{
			CSearchRoute* pSearch = new CSearchRoute(m_pGMain);
			pSearch->MakeANode();

			pSearch->MakeRouteTable();
			SAFE_DELETE(pSearch);

		}

		ImGui::Separator();
		if (ImGui::Button(u8"敵を全滅させる", ImVec2(150, 20)))
		{
			m_pGMain->m_pEnmProc->SetNonActive();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"味方の追加", ImVec2(60, 20)))
		{
			m_pGMain->m_pAllyProc->Start(GetPositionVector(m_mWorld), TRUE);
			m_nAllyNum++;
		}

		ImGui::Separator();
		if (ImGui::Button(u8"階層数増加", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->SetHierarchy(m_pGMain->m_pMapProc->GetHierarchy() + 1);
		}

		ImGui::Separator();
		if (ImGui::Button(u8"ボススポーン", ImVec2(60, 20)))
		{
			m_pGMain->m_pEnmBossProc->Start(VECTOR3(0.0f, 0.0f, 0.0f));
		}

		ImGui::Separator();
		if (ImGui::Button(u8"能力強化", ImVec2(60, 20)))
		{
			m_pGMain->m_pPcProc->GetAbility()->AddAbilityLevel(9);
			m_pGMain->m_pPcProc->GetAbility()->AddAbilityLevel(10);
			m_pGMain->m_pPcProc->GetAbility()->AddAbilityLevel(11);
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map2", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap2();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map3", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap3();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map4", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap4();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map5", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap5();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map6", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap6();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map7", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap7();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map8", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap8();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map9", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap9();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map10", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap10();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map11", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap11();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map12", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap12();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map13", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap13();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map14", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap14();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map15", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap15();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map16", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap16();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map17", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap17();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map18", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap18();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map19", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap19();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Map20", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->LoadMap20();
		}

		ImGui::Separator();
		// ボタン入力
		ImGui::Text(" ");
		ImGui::SameLine();
		if (ImGui::Button(u8"終了", ImVec2(60, 20)))
		{
			// ボタンを押したとき、*p_openにfalseを返す
			*p_open = false;
		}
	}
	ImGui::End();
}