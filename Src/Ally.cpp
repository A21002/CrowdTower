//=============================================================================
//		味方キャラクターの制御
//																Ally.cpp
//=============================================================================
#include  "Ally.h"
#include  "Weapon.h"
#include  "Map.h"
#include  "Effect.h"
#include  "Playchar.h"
#include  "EnemyBoss.h"
#include  "BackFore.h"
#include  "SearchRoute.h"

//======================================================================
// 味方キャラクター　定数クラス
//======================================================================
const float AllyConstruct::SPEED = 0.08f;
const float AllyConstruct::START_INVIN_TIME = 0.5f;
const VECTOR3 AllyConstruct::HITBOX_MIN = VECTOR3(-0.7f, -0.05f, -0.3f);
const VECTOR3 AllyConstruct::HITBOX_MAX = VECTOR3(0.7f, 2.0f, 0.3f);


//============================================================================
// 味方プロシージャクラス
//============================================================================
// ---------------------------------------------------------------------------
// 味方プロシージャのコンストラクタ
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
// ---------------------------------------------------------------------------
CAllyProc::CAllyProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// キャラクター　－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Char/Ally/Ally_Skin.mesh"));    // キャラクター
	m_pMesh->LoadAnimation(_T("Data/Char/Ally/Idle.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Ally/Run2.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Ally/Sword.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Ally/Arrow.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Ally/Axe.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Ally/Gun.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Ally/Hit.anmx"));
	m_pMesh->m_fHeightMax = 0.01f;
	m_pMesh->m_iMaxDevide = 16;
	ResetHoldItem();
	m_fAveTran = 0.0f;
	m_fAveRot = 0.0f;
	m_fOldAveTran = 0.0f;
	m_fOldAveRot = 0.0f;
	// -------------------------------------------------------------------------
	for (int i = 0; i < ALLY_MAX; i++)
	{
		m_pObjArray.push_back(new CAllyObj(this));  // m_pObjArrayにオブジェクトを生成する
	}
}

// ---------------------------------------------------------------------------
// 味方プロシージャのデストラクタ
// ---------------------------------------------------------------------------
CAllyProc::~CAllyProc()
{
}

// ---------------------------------------------------------------------------
// 味方の武器持ち替え関数
// ---------------------------------------------------------------------------
void CAllyProc::SetHoldItem(DWORD weaponId)
{
	// 各武器ごとに遷移
	switch (weaponId) {
	case ATTACK_ALLOW:
		m_HoldItem.m_nHoldObjNo = 8;	// 手に持つ武器のオブジェ番号
		m_HoldItem.m_nPosMesh = 0;		// 根元位置のメッシュ番号
		m_HoldItem.m_nPosBone = 16;	// 根元位置のボーン番号
		m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.18f, -0.03f);  // 手に持つ武器の位置オフセット
		m_HoldItem.m_nOffsetRot = VECTOR3(68.0f, 230.0f, 0.0f);  // 手に持つ武器の角度オフセット
		break;

	case ATTACK_SWORD:
		m_HoldItem.m_nHoldObjNo = 9;
		m_HoldItem.m_nPosMesh = 0;
		m_HoldItem.m_nPosBone = 14;	
		m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.24f, -0.07f);
		m_HoldItem.m_nOffsetRot = VECTOR3(0.0f, 0.0f, 0.0f); 
		break;

	case ATTACK_GUN:
		m_HoldItem.m_nHoldObjNo = 10;
		m_HoldItem.m_nPosMesh = 0;
		m_HoldItem.m_nPosBone = 14;
		m_HoldItem.m_nOffsetPos = VECTOR3(-0.1f, 0.25f, 0.06f);
		m_HoldItem.m_nOffsetRot = VECTOR3(90.0f, 1.0f, 108.0f);
		break;

	case ATTACK_AXE:
		m_HoldItem.m_nHoldObjNo = 11;
		m_HoldItem.m_nPosMesh = 0;
		m_HoldItem.m_nPosBone = 14;	
		m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.14f, 0.04f); 
		m_HoldItem.m_nOffsetRot = VECTOR3(0.0f, -90.0f, 0.0f);
		break;

	default:
		break;
	}
}

// ---------------------------------------------------------------------------
// 味方プロシージャの発生処理
// ---------------------------------------------------------------------------
BOOL CAllyProc::Start(VECTOR3 vPos, BOOL flag)
{
	for (DWORD i = 0; i < m_pObjArray.size(); i++) {
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(vPos, flag);
			return TRUE;
		}
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// 味方プロシージャの更新処理
// ---------------------------------------------------------------------------
void CAllyProc::Update()
{
	// 前回の全オブジェクトの移動、回転量を保存
	m_fOldAveTran = m_fAveTran;
	m_fOldAveRot = m_fAveRot;
	// 今回の移動、回転量の保存に向けてリセット
	m_fAveTran = 0.0f;
	m_fAveRot = 0.0f;
}

// ---------------------------------------------------------------------------
//
// 味方プロシージャの削除処理
//
// ---------------------------------------------------------------------------
void CAllyProc::DeleteObj(int num)
{
	m_pGMain->m_pPcProc->GetPcObjPtr()->AddAllyNum(-1 * num);
	for (int i = 0; i < num; i++) {
		for (int j = 0; j < m_pObjArray.size(); j++) {
			if (m_pObjArray[j]->GetActive()) {
				m_pObjArray[j]->SetActive(false);
				break;
			}
		}
	}
}

//============================================================================
// アクセス関数
//============================================================================
float CAllyProc::GetAveTran()
{
	return m_fOldAveTran;
}

float CAllyProc::GetAveRot()
{
	return m_fOldAveRot;
}

void CAllyProc::AddAveTran(float tran)
{
	m_fAveTran += tran;
}

void CAllyProc::AddAveRot(float rot)
{
	m_fAveRot += rot;
}

//============================================================================
// 味方オブジェクトクラス
//============================================================================
// ---------------------------------------------------------------------------
// 味方オブジェクトのコンストラクタ
//
//  引数
//  CBaseProc* pProc		： 親プロシージャへのポインタ
// ---------------------------------------------------------------------------
CAllyObj::CAllyObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nHP = 1;		// 体力
	const int nAtc = 0;		// 攻撃力

	m_bActive = FALSE;
	m_dwStatus = NORMAL;

	m_vTarget = GetPositionVector(m_mWorld);

	// バウンディングボックスの設定
	m_pBBox = new CBBox(m_pGMain->m_pShader, AllyConstruct::HITBOX_MIN, AllyConstruct::HITBOX_MAX);
	m_pBBox->m_mWorld = m_mWorld; 
	m_nMaxHp = nHP;
	m_nAtc = nAtc;
	m_nTag = ALLY;

	// 経路探索マップの設定
	m_pSearchRoute = new CSearchRoute(m_pGMain);
	m_nCnt2 = 0;
}

// ---------------------------------------------------------------------------
// 味方オブジェクトのデストラクタ
//
//   なお、CBBoxのdeleteはCBaseObjのデストラクタで行うため不要
// ---------------------------------------------------------------------------
CAllyObj::~CAllyObj()
{
	SAFE_DELETE(m_pSearchRoute);
}

// ---------------------------------------------------------------------------
// 味方オブジェクトの発生
// ---------------------------------------------------------------------------
BOOL CAllyObj::Start(VECTOR3 vPos, BOOL flag)
{
	m_bActive = TRUE;
	ResetStatus();
	m_dwStatus = FLASH;
	m_nCnt1 = 60 * AllyConstruct::START_INVIN_TIME;

	// 初期スポーンの場合、範囲内でランダムスポーンする
	if (flag) {
		vPos.x = Random(-5, 5);
		vPos.z = Random(-25, -20);
	}
	m_mWorld = XMMatrixTranslationFromVector(vPos);
	m_mWorldOld = m_mWorld;

	// 目的地を設定
	MATRIX4X4 m_mPcWorld = XMMatrixTranslation(0.0f, -1.0f, 0.0f) * m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld();
	m_vTarget = GetPositionVector(m_mPcWorld);

	// 初期化
	m_nOldAttackKind = ATTACK_NONE;
	m_nHp = m_nMaxHp;
	m_IsCanMove = TRUE;
	// 経路探索配列を初期化する
	m_pSearchRoute->MakeANode();

	m_bGroup = flag;
	if (!flag) {
		m_pGMain->m_pSpown->Play();
		m_pGMain->m_pPcProc->GetPcObjPtr()->AddAllyNum(1);
	}
	
	return TRUE;
}

//-----------------------------------------------------------------------------
// 味方オブジェクトの制御
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CAllyObj::Update()
{
	if (!m_bActive) return;  // 非表示のときは処理をしない

	// 変数の初期化   ---------------------------------------
	m_vPosUp.x = m_vPosUp.y = m_vPosUp.z = 0.0f;
	m_vRotUp.x = m_vRotUp.y = m_vRotUp.z = 0.0f;
	m_mWorldOld = m_mWorld;  // 一つ前の位置

	// ステータスごとの処理  --------------------------------
	switch (m_dwStatus)
	{
	case  FLASH:	// 無敵状態
		if (--m_nCnt1 <= 0)
		{
			m_dwStatus = NORMAL;
		}

	case  NORMAL:	// 味方が通常状態のときの処理
		UpdateNormal();
		break;

	case  DAMAGE:	// 味方がダメージ状態のときの処理
		if (m_AnimStatus.animNum != AllyConstruct::ANIM_HIT)
		{
			m_AnimStatus.SetNum(AllyConstruct::ANIM_HIT);
			m_pGMain->m_pEffectProc->m_pMinusParticleProc->Start(GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld()) + VECTOR3(0.0f, 1.5f, 0.0f), VECTOR3(0.0f, 1.0f, 0.0f));
			m_pGMain->m_pSeDead->Play();
		}
		if (m_AnimStatus.isEnd(AllyConstruct::ANIM_HIT))
		{
			m_dwStatus = DEAD;
		}
		break;

	case  DEAD:	// 味方が死亡状態のときの処理
		m_pGMain->m_pPcProc->GetPcObjPtr()->AddAllyNum(-1);
		m_pGMain->m_pBackForeProc->AddDeadAllyNum(1);
		m_bActive = false;
		return;
		break;
	}
	// ボスとの重なり判定
	SetKeepOffset(m_pGMain->m_pEnmBossProc->GetEnmBossObjPtr());

	// マップコリジョンと自然落下  --------------------------
	m_fJumpY += (0.5f * GRAVITY * 0.00005f) * m_fJumpTime;	// 自然落下
	m_fJumpTime += 1.0f;
	if (m_pGMain->m_pMapProc->isCollisionMoveGravity(&m_mWorld, m_mWorldOld, m_pBBox->m_vMax.x) != 3)  // マップコリジョン
	{
		m_fJumpY = 0.0f;  // 自然落下の停止
		m_fJumpTime = 0.0f;
	}

	// 死亡、ダメージ時は判定配列に加えない
	if (m_dwStatus == NORMAL) {
		// PC、味方との衝突回避
		m_pGMain->m_pMapProc->GetPcSpPart()->SetKeepOffset(this, GetPositionVector(m_mWorld));
		// 空間分割へ登録
		VECTOR3 vMin, vMax;
		m_pGMain->m_pMapProc->GetPcSpPart()->MakeAABB(
			GetPositionVector(m_mWorldOld), GetPositionVector(m_mWorld), 2.0f, vMin, vMax);
		m_pGMain->m_pMapProc->GetPcSpPart()->SetObj(vMin, vMax, this);
	}

	// マップコリジョンと自然落下  --------------------------
	m_fJumpY += (0.5f * GRAVITY * 0.00005f) * m_fJumpTime;	// 自然落下
	m_fJumpTime += 1.0f;
	if (m_pGMain->m_pMapProc->isCollisionMoveGravity(&m_mWorld, m_mWorldOld, m_pBBox->m_vMax.x) != 3)  // マップコリジョン
	{
		m_fJumpY = 0.0f;  // 自然落下の停止
		m_fJumpTime = 0.0f;
	}

	// 描画処理  ---------------------------------------------
	// スキンメッシュレンダリング 
	GetMesh()->Render(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);

	if (GetHoldItem().m_nHoldObjNo != 0 && m_bGroup) // 手に持つ武器やアイテムがあるとき
	{
		MATRIX4X4 mHold;
		mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, GetHoldItem().m_nPosBone, GetHoldItem().m_nPosMesh);  // 手の位置のボーンマトリックス
		m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->Disp(mHold, GetHoldItem()); // アイテム表示
	}

	// バウンディングボックス
	m_pBBox->m_mWorld = m_mWorld;
}

//-----------------------------------------------------------------------------
// 味方オブジェクトの制御
//
// 味方が通常状態のときの処理
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CAllyObj::UpdateNormal()
{
	if (m_IsCanMove) {
		// 攻撃アニメーションによって移動が制限されていない
		if (m_bGroup)
		{
			// グループに属している
			UpdateMoveBoid(AllyConstruct::SPEED);
		}
		else {
			// グループに属していない
			UpdateMoveSearch(AllyConstruct::SPEED);
		}
	}
	// グループに合流していれば攻撃する
	if (m_bGroup)
	{
		UpdateAttack();
	}
}

//-----------------------------------------------------------------------------
// 味方オブジェクトの攻撃制御
//
//　PCの情報を基に攻撃する
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void CAllyObj::UpdateAttack()
{
	// PCの攻撃入力情報
	int attackKind = m_pGMain->m_pPcProc->GetPcObjPtr()->GetAttackKind();
	CBaseProc* pProc = m_pGMain->m_pAllyProc;
	CAbility* ability = m_pGMain->m_pPcProc->GetAbility();

	switch (attackKind)
	{
	case ATTACK_NONE:
		// 攻撃を行っていない
		if (m_AnimStatus.isEnd(AllyConstruct::ANIM_GUN)) {
			// 銃攻撃アニメーションで、それが終了した場合
			m_AnimStatus.SetNum(AllyConstruct::ANIM_IDLE);
			m_IsCanMove = TRUE;
			m_AnimStatus.animSpeed = 1;
			pProc->ResetHoldItem();
		}
		break;

	case ATTACK_SHOTSWORD:
		// 剣攻撃(斬撃あり)
		// PCと同じ方向を向く
		m_mWorld = GetRotateMatrixY(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld()) * GetPositionMatrix(m_mWorld);
		// 攻撃モーション中行動不能
		m_IsCanMove = FALSE;
		// 剣を持つ
		pProc->SetHoldItem(ATTACK_SWORD);
		// エフェクト発生
		m_pGMain->m_pEffectProc->m_pBillSlashProc->Start(m_mWorld);

		if (ability->GetAbilityLevel(4) > 0) {
			// 斬撃能力取得済み
			m_pGMain->m_pWeaponProc->m_pWeaponShotSlashProc->Start(m_mWorld, PC);
		}
		// 剣攻撃アニメーション設定
		m_AnimStatus.SetNum(AllyConstruct::ANIM_SWORD);
		m_AnimStatus.animSpeed = ability->GetAbilityLevel(3) * 0.3f + 1;
		break;

	case ATTACK_SWORD:
		// 剣攻撃(斬撃なし)
				// PCと同じ方向を向く
		m_mWorld = GetRotateMatrixY(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld()) * GetPositionMatrix(m_mWorld);
		// 攻撃モーション中行動不能
		m_IsCanMove = FALSE;
		// 剣を持つ
		pProc->SetHoldItem(ATTACK_SWORD);
		// エフェクト発生
		m_pGMain->m_pEffectProc->m_pBillSlashProc->Start(m_mWorld);

		// 剣攻撃アニメーション設定
		m_AnimStatus.SetNum(AllyConstruct::ANIM_SWORD);
		m_AnimStatus.animSpeed = ability->GetAbilityLevel(3) * 0.3f + 1;
		break;

	case ATTACK_ALLOW:
		// 弓矢攻撃
		// PCと同じ方向を向く
		m_mWorld = GetRotateMatrixY(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld()) * GetPositionMatrix(m_mWorld);
		// 攻撃モーション中行動不能
		m_IsCanMove = FALSE;
		// 弓を持つ
		pProc->SetHoldItem(ATTACK_ALLOW);

		// 弓矢攻撃アニメーション設定
		m_AnimStatus.SetNum(AllyConstruct::ANIM_ARROW);
		m_AnimStatus.animSpeed = ability->GetAbilityLevel(0) * 0.3f + 1;
		break;

	case ATTACK_AXE:
		// 斧攻撃
		// PCと同じ方向を向く
		m_mWorld = GetRotateMatrixY(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld()) * GetPositionMatrix(m_mWorld);
		// 攻撃モーション中行動不能
		m_IsCanMove = FALSE;
		// 斧を持つ
		pProc->SetHoldItem(ATTACK_AXE);
		m_pGMain->m_pEffectProc->m_pBillSlashProc->Start(m_mWorld);	// エフェクト

		// 斧攻撃アニメーション設定
		m_AnimStatus.SetNum(AllyConstruct::ANIM_AXE);
		m_AnimStatus.animSpeed = ability->GetAbilityLevel(3) * 0.3f + 1;
		break;

	case ATTACK_AXESHOT:
		// 斧の遠投攻撃
		// PCと同じ方向を向く
		m_mWorld = GetRotateMatrixY(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld()) * GetPositionMatrix(m_mWorld);
		// 攻撃モーション中行動不能
		m_IsCanMove = FALSE;

		// 斧攻撃アニメーション設定
		m_AnimStatus.SetNum(AllyConstruct::ANIM_AXE);
		m_AnimStatus.animSpeed = ability->GetAbilityLevel(3) * 0.3f + 1;

		m_pGMain->m_pWeaponProc->m_pWeaponAxeShotProc->Start(m_mWorld * XMMatrixTranslation(0.0f, 1.0f, 1.0f), 5 + 2 * ability->GetAbilityLevel(4), PC);

		break;

	case ATTACK_GUN:
		// 銃攻撃
		// PCと同じ方向を向く
		m_mWorld = GetRotateMatrixY(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld()) * GetPositionMatrix(m_mWorld);
		// 斧攻撃アニメーション設定
		if (m_AnimStatus.animNum != AllyConstruct::ANIM_GUN)
		{
			m_IsCanMove = FALSE;
			m_AnimStatus.SetNum(AllyConstruct::ANIM_GUN);
			// 銃を持つ
			pProc->SetHoldItem(ATTACK_GUN);
		}

		// 発射
		m_pGMain->m_pWeaponProc->m_pWeaponGunProc->Start(XMMatrixTranslation(0.0f, 0.7f, 0.5f) * m_mWorld, PC);
		
		break;
	}
	m_nOldAttackKind = attackKind;

	// 弓攻撃
	if (m_AnimStatus.isEnd(AllyConstruct::ANIM_ARROW))
	{
		m_pGMain->m_pWeaponProc->m_pWeaponArrowProc->Start(m_mWorld, PC); // 矢発射
		m_AnimStatus.SetNum(AllyConstruct::ANIM_IDLE);
		m_AnimStatus.animSpeed = 1;
		pProc->ResetHoldItem();
		m_IsCanMove = TRUE;
	}

	// 剣攻撃
	if (m_AnimStatus.animNum == AllyConstruct::ANIM_SWORD)
	{
		if (m_AnimStatus.isEnd(AllyConstruct::ANIM_SWORD))
		{
			m_AnimStatus.SetNum(AllyConstruct::ANIM_IDLE);
			m_AnimStatus.animSpeed = 1;
			pProc->ResetHoldItem();
			m_IsCanMove = TRUE;
		}
		else {
			MATRIX4X4 mSword, mHold;
			VECTOR3 vMin, vMax;

			mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, GetHoldItem().m_nPosBone, GetHoldItem().m_nPosMesh);  // 手の位置のボーンマトリックス
			m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->GetInfo(mHold, GetHoldItem(), mSword, vMin, vMax); // ボーンマトリックスを指定して手に持つアイテム情報を得る
			m_pGMain->m_pWeaponProc->m_pWeaponSwordProc->Start(mSword, vMin, vMax, PC);  // 剣攻撃の開始

		}
	}

	// 斧攻撃
	if (m_AnimStatus.animNum == AllyConstruct::ANIM_AXE) {
		if (m_AnimStatus.isEnd(AllyConstruct::ANIM_AXE))
		{
			m_AnimStatus.SetNum(AllyConstruct::ANIM_IDLE);
			m_AnimStatus.animSpeed = 1;
			pProc->ResetHoldItem();
			m_IsCanMove = TRUE;
		}
		else if(GetHoldItem().m_nHoldObjNo != 0){
			MATRIX4X4 mSword, mHold;
			VECTOR3 vMin, vMax;
			mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, GetHoldItem().m_nPosBone, GetHoldItem().m_nPosMesh);  // 手の位置のボーンマトリックス
			m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->GetInfo(mHold, GetHoldItem(), mSword, vMin, vMax); // ボーンマトリックスを指定して手に持つアイテム情報を得る
			m_pGMain->m_pWeaponProc->m_pWeaponAxeProc->Start(mSword, vMin, vMax, PC);  // 斧攻撃の開始
		}
	}
}

//-----------------------------------------------------------------------------
// 味方オブジェクトのグループに属していないときの移動
//
// 経路探索をもとに移動する
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void CAllyObj::UpdateMoveSearch(float fForward)
{
	MATRIX4X4 mTemp;
	if (!TargetMove(m_vTarget, fForward, 180.0f))	// 目的地（Target）に達したかどうか
	{
		// 目的地へ移動
		if (m_AnimStatus.animNum != AllyConstruct::ANIM_WALK) {
			// 歩行アニメーションへ遷移
			m_AnimStatus.SetNum(AllyConstruct::ANIM_WALK);
		}
		mTemp = XMMatrixRotationY(XMConvertToRadians(m_vRotUp.y));					// 目的地（Target）へ向かう角度
		m_mWorld = mTemp * m_mWorld;
		mTemp = XMMatrixTranslation(m_vPosUp.x, m_vPosUp.y + m_fJumpY, m_vPosUp.z);	// 目的地（Target）へ向かう
		m_mWorld = mTemp * m_mWorld;
	}
	else {
		m_bGroup = TRUE;
	}

	// 経路探索の次の位置をセットする
	m_vTarget = GetPositionVector(m_mWorld);
	VECTOR3 vStart = VECTOR3(m_vTarget.x, 0, m_vTarget.z);   // 開始位置（現在位置）

	VECTOR3 vEnd = GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());
	BOOL isTargetAreaOut = TRUE;
	m_vTarget = m_pSearchRoute->GetNextPos(vStart, vEnd, &isTargetAreaOut);
	
}

//-----------------------------------------------------------------------------
// 味方オブジェクトのグループに属しているときの移動
//
// 群衆アルゴリズムをもとに移動する
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void CAllyObj::UpdateMoveBoid(float fForward)
{
	VECTOR3 pos = GetPositionVector(m_mWorld);
	float rot = GetRotateVector(m_mWorld).y;
	m_mWorld = XMMatrixTranslation(pos.x, pos.y, pos.z);
	// ①群れの中心に向かう
	VECTOR3 vTarget = GetPositionVector(XMMatrixTranslation(0.0f, 0.0f, -1.0f) * m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());
	float tran1 = GetDistance(vTarget);
	float rot1 = GetTargetRotateVector(m_mWorld, vTarget).y;

	// ②同じ速度、方向にする
	float aveTran = m_pGMain->m_pAllyProc->GetAveTran();
	float aveRot = m_pGMain->m_pAllyProc->GetAveRot();
	int num = m_pGMain->m_pPcProc->GetPcObjPtr()->GetHp() - 1;
	if (num >= 2) {
		aveTran /= num;
		aveRot /= num;
	}
	
	// 移動
	float movePos = 0.0f;
	float moveRot = 0.0f;
	if (m_pGMain->m_pPcProc->GetPcObjPtr()->GetKeyPush()) {
		movePos = tran1 * 1 + aveTran * 0.1;
		moveRot = rot1 * 1 + aveRot * 0.1;
		if (movePos > fForward) {
			movePos = fForward;
		}

		m_mWorld = XMMatrixTranslation(0.0f, 0.0f, movePos) * XMMatrixRotationY(XMConvertToRadians(moveRot)) * m_mWorld;
		
		if (m_AnimStatus.animNum < AllyConstruct::ANIM_SWORD) {
			m_AnimStatus.SetNum(AllyConstruct::ANIM_WALK);
		}
	}
	else {
		m_mWorld = XMMatrixRotationY(XMConvertToRadians(rot)) * m_mWorld;
		movePos = 0.0f;
		if (m_AnimStatus.animNum != AllyConstruct::ANIM_IDLE) {
			m_AnimStatus.SetNum(AllyConstruct::ANIM_IDLE);
		}
	}
	m_pGMain->m_pAllyProc->AddAveTran(movePos * 0.2);
	m_pGMain->m_pAllyProc->AddAveRot(moveRot * 0.2);
}