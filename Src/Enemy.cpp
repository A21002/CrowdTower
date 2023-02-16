//=============================================================================
//		敵キャラクターの処理
//																Enemy.cpp
//=============================================================================
#include  "Enemy.h"
#include  "Playchar.h"
#include  "Ally.h"
#include  "Weapon.h"
#include  "Map.h"
#include  "Effect.h"
#include  "BackFore.h"
#include  "SearchRoute.h"

//======================================================================
// 敵　定数クラス
//======================================================================
float const EnemyConstruct::SPEED = 0.08f;
float const EnemyConstruct::SWORD_RANGE = 1.5f;
VECTOR3 const EnemyConstruct::HITBOX_MIN = VECTOR3(-0.3f, 0.0f, -0.3f);
VECTOR3 const EnemyConstruct::HITBOX_MAX = VECTOR3(0.3f, 1.5f, 0.3f);

//============================================================================
// 敵プロシージャクラス
//============================================================================
// ---------------------------------------------------------------------------
// 敵プロシージャのコンストラクタ
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
// ---------------------------------------------------------------------------
CEnmProc::CEnmProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_dwProcID = ENM_RW_ID;                  // 敵ＲＷのID
	m_nNum = 0;

	m_bIsStartFlag = TRUE;

	// Fbxスキンメッシュの設定
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Char/Enemy1/Enemy1_Skin.mesh"));
	m_pMesh->LoadAnimation(_T("Data/Char/Enemy1/Idle.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Enemy1/Run.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Enemy1/Sword.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Enemy1/Arrow.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Enemy1/Hit.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Enemy1/Anim_Dive.anmx"), eRootAnimXZ);
	m_pMesh->LoadAnimation(_T("Data/Char/Enemy1/Anim_LookAround.anmx"));
	m_pMesh->m_fHeightMax = 0.01f;
	m_pMesh->m_iMaxDevide = 8;
	m_HoldItem.m_nHoldObjNo = 9;	// 手に持つ武器のオブジェ番号
	m_HoldItem.m_nPosMesh = 0;		// 根元位置のメッシュ番号
	m_HoldItem.m_nPosBone = 15;	// 根元位置のボーン番号
	m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.24f, 0.04f);  // 手に持つ武器の位置オフセット
	m_HoldItem.m_nOffsetRot = VECTOR3(00.0f, 0.0f, 0.0f);  // 手に持つ武器の角度オフセット

	for (int i = 0; i < ENM_RW_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmObj(this));  // m_pObjArrayにオブジェクトを生成する
	}
}

// ---------------------------------------------------------------------------
// 敵プロシージャのデストラクタ
// ---------------------------------------------------------------------------
CEnmProc::~CEnmProc()
{
}

// ---------------------------------------------------------------------------
// 敵の最大スポーン数の設定
// ---------------------------------------------------------------------------
void CEnmProc::SetMaxNum()
{
	m_nNum = m_pGMain->m_pMapProc->GetNavMap().size() * m_pGMain->m_pMapProc->GetHierarchy();
}

// ---------------------------------------------------------------------------
// 発生数を初期値にリセットする
// （最大発生数(m_nMaxNum)を発生数m_nNumに設定する
//
// 戻り値  なし
// ---------------------------------------------------------------------------
void CEnmProc::ResetEnmNum()
{
	m_nNum = m_pGMain->m_pMapProc->GetNavMap().size() * m_pGMain->m_pMapProc->GetHierarchy();
}

// ---------------------------------------------------------------------------
// 現在のアクティブのオブジェクトを計測
// ---------------------------------------------------------------------------
void CEnmProc::ResetStartFlag()
{
	m_bIsStartFlag = TRUE;
}

// ---------------------------------------------------------------------------
// 現在のアクティブのオブジェクトを計測
// ---------------------------------------------------------------------------
int CEnmProc::CountActiveEnm()
{
	return m_nNum;
}

// ---------------------------------------------------------------------------
// 敵プロシージャの手動発生処理
// ---------------------------------------------------------------------------
BOOL CEnmProc::Start()
{
	for (DWORD i = 0; i < m_pObjArray.size(); i++) {
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start();
			return TRUE;
		}
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// 敵プロシージャの手動発生処理
// 範囲内にランダムリスポーン
// ---------------------------------------------------------------------------
BOOL CEnmProc::Start(VECTOR3 vMin, VECTOR3 vMax, int nNum)
{
	// ナビゲーションマップの生成
	NAVIGATIONMAP nav;
	nav.vMax = vMax;
	nav.vMin = vMin;

	int i = 0, j = 0;
	// 敵を生成
	for (; i < nNum; i++)
	{
		for (; j < m_pObjArray.size(); j++)
		{
			if (!m_pObjArray[j]->GetActive())
			{
				((CEnmObj*)m_pObjArray[j])->SetNav(nav);  // 選ばれたナビマップをオブジェクトに登録する
				m_pObjArray[j]->Start();
				break;
			}
		}
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// 敵プロシージャの自動発生処理
// ---------------------------------------------------------------------------
void CEnmProc::Update()
{
	if (m_bIsStartFlag) {
		// まだスポーンさせていない
		m_bIsStartFlag = FALSE;
		int spownNum = m_pGMain->m_pMapProc->GetHierarchy();
		SetMaxNum();
		// スポーン地点を取得
		for (int i = 0; i < m_pGMain->m_pMapProc->GetNavMap().size(); i++) {
			for (int j = 0; j < spownNum; j++) {
				for (int k = 0; k < m_pObjArray.size(); k++) {
					if (!m_pObjArray[k]->GetActive())
					{
						((CEnmObj*)m_pObjArray[k])->SetNav(m_pGMain->m_pMapProc->GetNavMap()[i]);  // 選ばれたナビマップをオブジェクトに登録する
						m_pObjArray[k]->Start();	// 敵を発生させる
						break;
					}
				}
			}
		}
	}
}

// ---------------------------------------------------------------------------
// 敵プロシージャの当たり判定処理
// ---------------------------------------------------------------------------
void CEnmProc::UpdateHitcheck()
{
	std::vector<CBaseObj*> stac;
	std::vector<CBaseObj*> hit;
	m_pGMain->m_pMapProc->GetPcSpPart()->AllHitcheck(0, m_pGMain->m_pMapProc->GetEnmSpPart()->GetSpArray(), stac, hit);

	int hitNum = hit.size() / 2;
	for (int i = 0; i < hitNum; i++) {
		hit[i]->Hitcheck(hit[i + 1], hit[i + 1]->GetBBox());
	}
}

//============================================================================
// 敵オブジェクトクラス
//============================================================================
// ---------------------------------------------------------------------------
// 敵オブジェクトのコンストラクタ
//
//  引数
// ---------------------------------------------------------------------------
CEnmObj::CEnmObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nMaxHP = 100;      // 体力
	const int nAtc = 1;      // 体の攻撃力

	m_bActive = FALSE;
	m_dwStatus = NORMAL;

	m_vTarget = GetPositionVector(m_mWorld);

	// バウンディングボックスの設定
	m_pBBox = new CBBox(m_pGMain->m_pShader, EnemyConstruct::HITBOX_MIN, EnemyConstruct::HITBOX_MAX);

	m_nHp = m_nMaxHp = nMaxHP;
	m_nAtc = nAtc;
	m_nTag = ENM;

	// 経路探索マップの設定
	m_pSearchRoute = new CSearchRoute(m_pGMain);

	m_HoldItem.m_nHoldObjNo = 0;	// 手に持つ武器のオブジェ番号
	m_HoldItem.m_nPosMesh = 0;		// 根元位置のメッシュ番号
	m_HoldItem.m_nPosBone = 15;	// 根元位置のボーン番号
	m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.24f, 0.04f);  // 手に持つ武器の位置オフセット
	m_HoldItem.m_nOffsetRot = VECTOR3(00.0f, 0.0f, 0.0f);  // 手に持つ武器の角度オフセット
}
// ---------------------------------------------------------------------------
// 敵オブジェクトのデストラクタ
//
//   なお、CBBoxのdeleteはCBaseObjのデストラクタで行うため不要
// ---------------------------------------------------------------------------
CEnmObj::~CEnmObj()
{
	SAFE_DELETE(m_pSearchRoute);
}

// ---------------------------------------------------------------------------
// 敵オブジェクトの発生
// ---------------------------------------------------------------------------
BOOL CEnmObj::Start()
{
	m_bActive = TRUE;	// 敵ＲＷオブジェクト
	m_dwStatus = NORMAL;
	m_bIsMove = true;
	// ナビゲーションマップの範囲内でランダムな位置にスポーンする
	m_mWorld = XMMatrixTranslation((float)Random(m_Nav.vMin.x, m_Nav.vMax.x),
									m_Nav.vMax.y, 
									(float)Random(m_Nav.vMin.z, m_Nav.vMax.z));
	// 向いている方向を反転
	m_mWorld = XMMatrixRotationY(XMConvertToRadians(180.0f)) * m_mWorld;
	m_mWorldOld = m_mWorld;

	m_nHp = m_nMaxHp;
	m_nCnt2 = 60 * 2;

	// 経路探索配列を生成する
	m_pSearchRoute->MakeANode();
	m_vTarget = GetPositionVector(m_mWorld);
	// 経路探索配列から経路図の作成処理
	VECTOR3 vStart = VECTOR3(m_vTarget.x, 0, m_vTarget.z);   // 開始位置

	VECTOR3 vEnd = GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());    // 目的地

	// 持つ武器の初期化
	m_HoldItem.m_nHoldObjNo = 0;	// 手に持つ武器のオブジェ番号
	m_HoldItem.m_nPosMesh = 0;		// 根元位置のメッシュ番号
	m_HoldItem.m_nPosBone = 15;	// 根元位置のボーン番号
	m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.24f, 0.04f);  // 手に持つ武器の位置オフセット
	m_HoldItem.m_nOffsetRot = VECTOR3(00.0f, 0.0f, 0.0f);  // 手に持つ武器の角度オフセット

	m_pGMain->m_pBackForeProc->AddDeadEnemyNum(1);
	return TRUE;
}

//-----------------------------------------------------------------------------
// 敵オブジェクトの制御
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CEnmObj::Update()
{
	if (!m_bActive)
	{
		return;
	}

	// 変数の初期化   ---------------------------------------
	m_vPosUp.x = m_vPosUp.y = m_vPosUp.z = 0.0f;
	m_vRotUp.x = m_vRotUp.y = m_vRotUp.z = 0.0f;
	m_mWorldOld = m_mWorld;  // 一つ前の位置

	// ステータスごとの処理  --------------------------------
	switch (m_dwStatus)
	{
	case  FLASH: // 敵がダメージから復帰したときの処理。breakをつけずに次のNORMALを行う。
	case  NORMAL: // 敵が通常状態のときの処理
		if (m_pGMain->m_pBackForeProc->GetFadeOutFinish()) {
			UpdateNormal();
		}
		break;

	case  DAMAGE: // 敵がダメージ状態のときの処理
		UpdateDamage();
		break;

	case  DEAD:	// 敵が死亡状態のときの処理
		UpdateDead();
		return;
		break;
	}

	// 他の敵と重ならないようにする処理  --------------------
	m_pGMain->m_pMapProc->GetEnmSpPart()->SetKeepOffset(this, GetPositionVector(m_mWorld));

	// マップコリジョンと自然落下  --------------------------
	m_fJumpY += (0.5f * GRAVITY * 0.00005f) * m_fJumpTime;	// 自然落下
	m_fJumpTime += 1.0f;
	if (m_pGMain->m_pMapProc->isCollisionMoveGravity(&m_mWorld, m_mWorldOld, m_pBBox->m_vMax.x, ENM) != 3)  // マップコリジョン
	{
		m_fJumpY = 0.0f;  // 自然落下の停止
		m_fJumpTime = 0.0f;
	}
	else {
		m_dwStatus = DEAD;
	}


	// オブジェクトを空間分割へ登録
	VECTOR3 vMin, vMax;
	m_pGMain->m_pMapProc->GetEnmSpPart()->MakeAABB(
		GetPositionVector(m_mWorldOld), GetPositionVector(m_mWorld), 2.0f, vMin, vMax);
	m_pGMain->m_pMapProc->GetEnmSpPart()->SetObj(vMin, vMax, this);

	// 描画処理  ---------------------------------------------
	// スキンメッシュレンダリング 
	//GetMesh()->RenderToon(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, normalize(VECTOR3(0.8f, 1, -1)), m_pGMain->m_vEyePt);
	GetMesh()->Render(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
	//GetMesh()->RenderDisplace(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);

	// バウンディングボックス
	m_pBBox->m_mWorld = m_mWorld;
	//   バウンディングボックスレンダリング
	//m_pBBox->Render(m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);

	MATRIX4X4 mHold;
	mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, m_HoldItem.m_nPosBone, m_HoldItem.m_nPosMesh);  // 手の位置のボーンマトリックス
	m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->Disp(mHold, m_HoldItem); // アイテム表示

	// 敵の頭上のステータス表示  -----------------------------
	VECTOR3 vEnmPos = GetPositionVector(m_mWorld);
	vEnmPos.y += 2.0f;
	float fHp = (float)m_nHp / m_nMaxHp;
	// ＨＰバーの表示
	m_pGMain->m_pBackForeProc->GetSprite()->Draw3D(m_pGMain->m_pBackForeProc->GetImage(), vEnmPos, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vEyePt, VECTOR2(1.2f * fHp, 0.2f), VECTOR2(320, 96), VECTOR2(125 * fHp, 16));
	VECTOR3 vPc = GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());
	float dis = GetDistance(vPc);
	if (dis <= 20)
	{
		m_pGMain->m_pBackForeProc->DrawArrowAngle(m_mWorld);
	}
}

//-----------------------------------------------------------------------------
// 敵オブジェクトの制御
//
// 敵が通常状態のときの処理
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CEnmObj::UpdateNormal()
{
	CPcObj* pPcObj = m_pGMain->m_pPcProc->GetPcObjPtr();
	MATRIX4X4 mTemp;
	BOOL isTargetAreaOut = FALSE;
	float fForward = EnemyConstruct::SPEED;

	// 敵がダメージから復帰したときの処理  ------------------------------
	if (m_dwStatus == FLASH)
	{
		m_nCnt1--;
		if (m_nCnt1 <= 0) m_dwStatus = NORMAL;
	}

	if (!TargetMove(m_vTarget, fForward, 180.0f))	// 目的地（Target）に達したかどうか
	{
		if (m_AnimStatus.animNum != EnemyConstruct::ANIM_WALK && m_AnimStatus.animNum != EnemyConstruct::ANIM_SWORD && m_AnimStatus.animNum != EnemyConstruct::ANIM_ARROW) {
			// アニメーションが移動、攻撃以外であれば移動アニメーションをセット
			m_AnimStatus.SetNum(EnemyConstruct::ANIM_WALK);
		}
		if (m_bIsMove) {
			mTemp = XMMatrixRotationY(XMConvertToRadians(m_vRotUp.y));					// 目的地（Target）へ向かう角度
			m_mWorld = mTemp * m_mWorld;
			mTemp = XMMatrixTranslation(m_vPosUp.x, m_vPosUp.y + m_fJumpY, m_vPosUp.z);	// 目的地（Target）へ向かう
			m_mWorld = mTemp * m_mWorld;
		}
	}
	if (m_AnimStatus.animNum != EnemyConstruct::ANIM_WALK && m_AnimStatus.animNum != EnemyConstruct::ANIM_SWORD && m_AnimStatus.animNum != EnemyConstruct::ANIM_ARROW) {
		m_AnimStatus.SetNum(EnemyConstruct::ANIM_IDLE);
	}
	// 経路探索の次の位置をセットする
	m_vTarget = GetPositionVector(m_mWorld);
	VECTOR3 vStart = VECTOR3(m_vTarget.x, 0, m_vTarget.z);   // 開始位置（現在位置）

	VECTOR3 vEnd = GetPositionVector(pPcObj->GetWorld());
	m_vTarget = m_pSearchRoute->GetNextPos(vStart, vEnd, &isTargetAreaOut);
		
	if (!isTargetAreaOut)
	{
		UpdateAttack();
	}
}

//-----------------------------------------------------------------------------
// 敵オブジェクトの制御
//
// 攻撃判定の処理
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void CEnmObj::UpdateAttack()
{
	VECTOR3 pcPos = GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());
	VECTOR3 Hit = VECTOR3(0.0f, 0.0f, 0.0f);
	VECTOR3 Normal = VECTOR3(0.0f, 0.0f, 0.0f);

	// 弓攻撃への遷移処理
	if (m_nCnt2 <= 0 && m_AnimStatus.animNum != EnemyConstruct::ANIM_ARROW && !m_pGMain->m_pMapProc->Hitcheck(pcPos, GetPositionVector(m_mWorld), &Hit, &Normal))
	{
		// プレイヤーの方を向く
		float y = GetTargetRotateVector(m_mWorld, pcPos).y;
		// ランダムにプレイヤーの方角からブレる
		int recoil = Random(-EnemyConstruct::RECOIL, EnemyConstruct::RECOIL);
		MATRIX4X4 mTemp = XMMatrixRotationY(XMConvertToRadians(y + recoil));
		m_mWorld = mTemp * m_mWorld;
		m_AnimStatus.SetNum(EnemyConstruct::ANIM_ARROW);
		SetHoldItem(ATTACK_ALLOW);
		m_bIsMove = false;
	}

	// 剣攻撃
	if (m_nCnt2 <= 0 && m_AnimStatus.animNum != EnemyConstruct::ANIM_SWORD && GetDistance(pcPos) <= EnemyConstruct::SWORD_RANGE) {
		float y = GetTargetRotateVector(m_mWorld, pcPos).y;
		MATRIX4X4 mTemp = XMMatrixRotationY(XMConvertToRadians(y));
		m_mWorld = mTemp * m_mWorld;
		m_AnimStatus.SetNum(EnemyConstruct::ANIM_SWORD);
		SetHoldItem(ATTACK_SWORD);
		m_bIsMove = false;
	}

	// 弓アニメーションの終了時、矢を発射
	if (m_AnimStatus.isEnd(EnemyConstruct::ANIM_ARROW)) {
		m_pGMain->m_pWeaponProc->m_pWeaponArrowProc->Start(m_mWorld, ENM); // レーザー発射
		m_AnimStatus.SetNum(EnemyConstruct::ANIM_IDLE);
		SetHoldItem(ATTACK_NONE);
		m_nCnt2 = 60 * EnemyConstruct::COOLTIME;
		m_bIsMove = true;
	}
	
	// 剣攻撃
	if (m_AnimStatus.animNum == EnemyConstruct::ANIM_SWORD)
	{
		MATRIX4X4 mSword, mHold;
		VECTOR3 vMin, vMax;

		mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, GetHoldItem().m_nPosBone, GetHoldItem().m_nPosMesh);  // 手の位置のボーンマトリックス
		m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->GetInfo(mHold, GetHoldItem(), mSword, vMin, vMax); // ボーンマトリックスを指定して手に持つアイテム情報を得る
		m_pGMain->m_pWeaponProc->m_pWeaponSwordProc->Start(mSword, vMin, vMax, ENM);  // 剣攻撃の開始
		// 剣アニメーション終了時、初期状態に戻す
		if (m_AnimStatus.isEnd(2))
		{
			m_AnimStatus.SetNum(0);
			SetHoldItem(ATTACK_NONE);
			m_nCnt2 = 60 * EnemyConstruct::COOLTIME;
			m_bIsMove = TRUE;
		}
	}

	if (m_nCnt2 > 0) {
		m_nCnt2--;
	}
}

//-----------------------------------------------------------------------------
// 敵オブジェクトの制御
//
// 敵がダメージ状態のときの処理
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CEnmObj::UpdateDamage()
{
	MATRIX4X4 mTemp;
	const int nFlashTime = 5;  // 無敵状態の時間

	if (m_pHitObj->GetTag() != PC && m_pHitObj->GetTag() != ALLY) {
		m_pGMain->m_pEffectProc->m_pBillProc->Start(GetPositionVector(m_mWorld) + VECTOR3(0.0f, 1.5f, 0.0f));
		mTemp = XMMatrixTranslation(0, 0, -0.5f);	// バックする
		m_mWorld = mTemp * m_mWorld;
		m_nHp -= m_pHitObj->GetAtc();	// 攻撃を受けたダメージ
		if (m_nHp <= 0)
		{
			m_nHp = 0;
			m_dwStatus = DEAD;		// HPが０なので死亡へ
		}
		else {
			m_nCnt1 = nFlashTime;   // 無敵状態の時間
			m_dwStatus = FLASH;     // ダメージからの復帰処理を行う
		}
	}
	else {
		m_dwStatus = NORMAL;
	}
}

//-----------------------------------------------------------------------------
// 敵オブジェクトの制御
//
// 敵が死亡状態のときの処理
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CEnmObj::UpdateDead()
{
	// 味方オブジェクトが100体未満であれば、現在地点に生成
	if (m_pGMain->m_pPcProc->GetPcObjPtr()->GetAllyNum() < 100) {
		m_pGMain->m_pBackForeProc->AddAllAllyNum(1);
		m_pGMain->m_pAllyProc->Start(GetPositionVector(m_mWorld), FALSE);
		m_pGMain->m_pEffectProc->m_pParticleProc->Start(GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld()) + VECTOR3(0.0f, 1.5f, 0.0f), VECTOR3(0.0f, 1.0f, 0.0f));

	}
	// UIを振動
	m_pGMain->m_pBackForeProc->StartSwing(Random(0, 360));
	m_pGMain->m_pEnmProc->AddNum(-1);
	m_bActive = FALSE;
}

// ---------------------------------------------------------------------------
// 敵の武器持ち替え関数
// ---------------------------------------------------------------------------
void CEnmObj::SetHoldItem(DWORD weaponId)
{
	// 各武器ごとに遷移
	switch (weaponId) {
	case ATTACK_ALLOW:
		m_HoldItem.m_nHoldObjNo = 8;	// 手に持つ武器のオブジェ番号(透明な剣)
		m_HoldItem.m_nPosMesh = 0;		// 剣の根元位置のメッシュ番号
		m_HoldItem.m_nPosBone = 17;	// 剣の根元位置のボーン番号
		m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.18f, -0.03f);  // 手に持つ武器の位置オフセット
		m_HoldItem.m_nOffsetRot = VECTOR3(68.0f, 230.0f, 0.0f);  // 手に持つ武器の角度オフセット
		break;

	case ATTACK_SWORD:
		m_HoldItem.m_nHoldObjNo = 9;	// 手に持つ武器のオブジェ番号
		m_HoldItem.m_nPosMesh = 0;		// 根元位置のメッシュ番号
		m_HoldItem.m_nPosBone = 15;	// 根元位置のボーン番号
		m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.24f, 0.04f);  // 手に持つ武器の位置オフセット
		m_HoldItem.m_nOffsetRot = VECTOR3(00.0f, 0.0f, 0.0f);  // 手に持つ武器の角度オフセット
		break;

	default:
		m_HoldItem.m_nHoldObjNo = 0;	// 手に持つ武器のオブジェ番号
		m_HoldItem.m_nPosMesh = 0;		// 根元位置のメッシュ番号
		m_HoldItem.m_nPosBone = 15;	// 根元位置のボーン番号
		m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.24f, 0.04f);  // 手に持つ武器の位置オフセット
		m_HoldItem.m_nOffsetRot = VECTOR3(00.0f, 0.0f, 0.0f);  // 手に持つ武器の角度オフセット
	}
}

//============================================================================
// アクセス関数
//============================================================================
void CEnmObj::SetNav(NAVIGATIONMAP  Nav) 
{ 
	m_Nav = Nav; 
}