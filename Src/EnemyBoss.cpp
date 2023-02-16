//=============================================================================
//		敵キャラクターの処理
//																EnemyBoss.cpp
//=============================================================================
#include  "EnemyBoss.h"
#include  "Enemy.h"
#include  "Playchar.h"
#include  "Ally.h"
#include  "Weapon.h"
#include  "Map.h"
#include  "Effect.h"
#include  "BackFore.h"
#include  "SearchRoute.h"
#include  "Camera.h"

//======================================================================
// 敵ボス　定数クラス
//======================================================================
float const EnemyBossConstruct::SPEED = 0.07f;

//============================================================================
// 敵ボスプロシージャクラス
//============================================================================
// ---------------------------------------------------------------------------
// 敵ボスプロシージャのコンストラクタ
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
// ---------------------------------------------------------------------------
CEnmBossProc::CEnmBossProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_nNum = 0;

	// Fbxスキンメッシュの設定
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Char/EnemyBoss/EnemyBoss.mesh"));
	m_pMesh->LoadAnimation(_T("Data/Char/EnemyBoss/Idle.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/EnemyBoss/run.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/EnemyBoss/kick.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/EnemyBoss/kick_Left.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/EnemyBoss/stomp.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/EnemyBoss/stomp_Left.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/EnemyBoss/bodyStomp.anmx"), eRootAnimXZ);
	m_pMesh->LoadAnimation(_T("Data/Char/EnemyBoss/standUp.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/EnemyBoss/roar.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/EnemyBoss/die.anmx"));
	m_pMesh->m_fHeightMax = 0.01f;
	m_pMesh->m_iMaxDevide = 8;
	m_HoldItem.m_nHoldObjNo = 12;	// 手に持つ武器のオブジェ番号
	m_HoldItem.m_nPosMesh = 0;		// 根元位置のメッシュ番号
	m_HoldItem.m_nPosBone = 2;	// 根元位置のボーン番号
	m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.0f, 0.0f);  // 手に持つ武器の位置オフセット
	m_HoldItem.m_nOffsetRot = VECTOR3(0.0f, 0.0f, 0.0f);  // 手に持つ武器の角度オフセット

	for (int i = 0; i < ENM_BOSS_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmBossObj(this));  // m_pObjArrayにオブジェクトを生成する
	}
}

// ---------------------------------------------------------------------------
// 敵ボスプロシージャのデストラクタ
// ---------------------------------------------------------------------------
CEnmBossProc::~CEnmBossProc()
{
}

// ---------------------------------------------------------------------------
// 敵の最大スポーン数の設定
// ---------------------------------------------------------------------------
void CEnmBossProc::SetMaxNum()
{
	m_nNum = m_pGMain->m_pMapProc->GetNavMap().size() * m_pGMain->m_pMapProc->GetHierarchy();
}

// ---------------------------------------------------------------------------
// 武器の変更
// ---------------------------------------------------------------------------
void CEnmBossProc::SetHoldItem(DWORD weaponId)
{
	switch (weaponId)
	{
	case EnemyBossConstruct::ATTACK_KICK_RIGHT:
		m_HoldItem.m_nHoldObjNo = 12;	// 手に持つ武器のオブジェ番号
		m_HoldItem.m_nPosBone = 2;	// 根元位置のボーン番号
		break;

	case EnemyBossConstruct::ATTACK_KICK_LEFT:
		m_HoldItem.m_nHoldObjNo = 12;	// 手に持つ武器のオブジェ番号
		m_HoldItem.m_nPosBone = 5;	// 根元位置のボーン番号
		break;

	case EnemyBossConstruct::ATTACK_STOMP_RIGHT:
		m_HoldItem.m_nHoldObjNo = 13;	// 手に持つ武器のオブジェ番号
		m_HoldItem.m_nPosBone = 0;	// 根元位置のボーン番号
		break;

	case EnemyBossConstruct::ATTACK_STOMP_LEFT:
		m_HoldItem.m_nHoldObjNo = 13;	// 手に持つ武器のオブジェ番号
		m_HoldItem.m_nPosBone = 3;	// 根元位置のボーン番号
		break;

	case EnemyBossConstruct::ATTACK_BODYSTOMP:
		m_HoldItem.m_nHoldObjNo = 14;	// 手に持つ武器のオブジェ番号
		m_HoldItem.m_nPosBone = 8;	// 根元位置のボーン番号


	default:
		break;
	}
}

CEnmBossObj* CEnmBossProc::GetEnmBossObjPtr()
{
	return (CEnmBossObj*)GetObjArrayPtr()[0];
}

// ---------------------------------------------------------------------------
// 発生数を初期値にリセットする
// （最大発生数(m_nMaxNum)を発生数m_nNumに設定する
//
// 戻り値  なし
// ---------------------------------------------------------------------------
void CEnmBossProc::ResetEnmNum()
{
	m_nNum = m_pGMain->m_pMapProc->GetNavMap().size() * m_pGMain->m_pMapProc->GetHierarchy();
}

// ---------------------------------------------------------------------------
// 現在のアクティブのオブジェクトを計測
// ---------------------------------------------------------------------------
int CEnmBossProc::CountActiveEnm()
{
	return m_nNum;
}

// ---------------------------------------------------------------------------
// 敵ボスプロシージャの手動発生処理
// ---------------------------------------------------------------------------
BOOL CEnmBossProc::Start(VECTOR3 vPos)
{
	for (DWORD i = 0; i < m_pObjArray.size(); i++) {
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(vPos);
			return TRUE;
		}
	}
	return FALSE;
}

//============================================================================
// 敵ボスオブジェクトクラス
//============================================================================
// ---------------------------------------------------------------------------
// 敵ボスオブジェクトのコンストラクタ
//
//  引数
// ---------------------------------------------------------------------------
CEnmBossObj::CEnmBossObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nMaxHP = EnemyBossConstruct::HP;      // 体力
	const int nAtc = 1;      // 体の攻撃力

	m_bActive = FALSE;
	m_bIsMovieFinish = TRUE;
	m_bIsBossMovie = FALSE;
	m_dwStatus = NORMAL;

	m_vTarget = GetPositionVector(m_mWorld);

	// バウンディングボックスの設定
	VECTOR3 vMin(-2.5f, 0.0f, -2.5f);
	VECTOR3 vMax(2.5f, 12.4f, 2.5f);
	m_pBBox = new CBBox(m_pGMain->m_pShader, vMin, vMax);

	m_nHp = m_nMaxHp = nMaxHP;
	m_nAtc = nAtc;
	m_nTag = ENM;
	m_nRoarCoolTime = 60 * EnemyBossConstruct::ROAR_COOLTIME;

	// 経路探索マップの設定
	m_pSearchRoute = new CSearchRoute(m_pGMain);

}
// ---------------------------------------------------------------------------
// 敵ボスオブジェクトのデストラクタ
//
//   なお、CBBoxのdeleteはCBaseObjのデストラクタで行うため不要
// ---------------------------------------------------------------------------
CEnmBossObj::~CEnmBossObj()
{
	SAFE_DELETE(m_pSearchRoute);
}

// ---------------------------------------------------------------------------
// 敵ボスオブジェクトの発生
// ---------------------------------------------------------------------------
BOOL CEnmBossObj::Start(VECTOR3 vPos)
{
	m_bActive = TRUE;
	m_dwStatus = NORMAL;
	m_nOldAttackKind = ATTACK_NONE;
	m_bIsMove = true;
	if (!m_bIsBossMovie) {
		m_bIsBossMovie = TRUE;
		m_bIsMovieFinish = FALSE;
		m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetBossMovie(TRUE);
		m_nCnt2 = 60 * 2;
		m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetLocalEyeTPS(0.0f, 0.0f, -10.0f);
	}
	m_mWorld = XMMatrixTranslationFromVector(vPos);
	m_mWorld = XMMatrixRotationY(XMConvertToRadians(180.0f)) * m_mWorld;
	m_mWorldOld = m_mWorld;

	m_nHp = m_nMaxHp;
	// 経路探索配列を生成する
	m_pSearchRoute->MakeANode();
	m_vTarget = GetPositionVector(m_mWorld);
	// 経路探索配列から経路図の作成処理
	VECTOR3 vStart = VECTOR3(m_vTarget.x, 0, m_vTarget.z);   // 開始位置

	VECTOR3 vEnd = GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());

	m_pGMain->m_bIsBoss = TRUE;
	m_pGMain->m_pBgm1->Stop();
	m_pGMain->m_pBossBgm->Play(AUDIO_LOOP);
	return TRUE;
}

//-----------------------------------------------------------------------------
// 敵ボスオブジェクトの制御
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CEnmBossObj::Update()
{
	if (!m_bActive) return;  // 非表示のときは処理をしない

	// 変数の初期化   ---------------------------------------
	m_vPosUp.x = m_vPosUp.y = m_vPosUp.z = 0.0f;
	m_vRotUp.x = m_vRotUp.y = m_vRotUp.z = 0.0f;
	m_mWorldOld = m_mWorld;  // 一つ前の位置

	if (m_bIsMovieFinish)
	{
		UpdateGame();
	}
	else
	{
		UpdateMovie();
	}

	// マップコリジョンと自然落下  --------------------------
	m_fJumpY += (0.5f * GRAVITY * 0.00005f) * m_fJumpTime;	// 自然落下
	m_fJumpTime += 1.0f;
	if (m_pGMain->m_pMapProc->isCollisionMoveGravity(&m_mWorld, m_mWorldOld, m_pBBox->m_vMax.x, ENM) != 3)  // マップコリジョン
	{
		m_fJumpY = 0.0f;  // 自然落下の停止
		m_fJumpTime = 0.0f;
	}

	// オブジェクトを空間分割へ登録
	VECTOR3 vMin, vMax;
	m_pGMain->m_pMapProc->GetEnmSpPart()->MakeAABB(
		GetPositionVector(m_mWorldOld), GetPositionVector(m_mWorld), 2.0f, vMin, vMax);
	m_pGMain->m_pMapProc->GetEnmSpPart()->SetObj(vMin, vMax, this);

	// カメラとプレイヤーの直線状に重なるか
	VECTOR3 vHit = VECTOR3(0.0f, 0.0f, 0.0f);
	VECTOR3 vNrm = VECTOR3(0.0f, 0.0f, 0.0f);
	VECTOR3 vNow = GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());
	VECTOR3 vOld = GetPositionVector(m_pGMain->m_pCameraProc->GetCameraObjPtr()->GetWorld());

	if (m_pBBox->OBBCollisionLay(vNow, vOld, &vHit, &vNrm))
	{
		GetMesh()->m_vDiffuse.w = 0.3f;
	}
	else {
		GetMesh()->m_vDiffuse.w = 1.0f;
	}

	// 描画処理  ---------------------------------------------
	// スキンメッシュレンダリング 
	GetMesh()->RenderToon(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, normalize(VECTOR3(0.8f, 1, -1)), m_pGMain->m_vEyePt);

	// バウンディングボックス
	m_pBBox->m_mWorld = m_mWorld;
}

//-----------------------------------------------------------------------------
// 敵ボスオブジェクトのゲーム中の制御
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void CEnmBossObj::UpdateGame()
{
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
}

//-----------------------------------------------------------------------------
// 敵ボスオブジェクトのムービー制御
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void CEnmBossObj::UpdateMovie()
{
	if (m_nCnt2 > 0) {
		// フェードアウト待機
		m_nCnt2--;
		if (m_nCnt2 == 0)
		{
			// ムービーアニメーション開始
			m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_ROAR);
		}
	}
	else {
		if (m_AnimStatus.animFrame >= 150)
		{
			// 咆哮終了時
			m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetMovePc(TRUE);
		}
	}
}

//-----------------------------------------------------------------------------
// 敵ボスオブジェクトの制御
//
// 敵が通常状態のときの処理
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CEnmBossObj::UpdateNormal()
{
	CPcObj* pPcObj = m_pGMain->m_pPcProc->GetPcObjPtr();
	MATRIX4X4 mTemp;
	BOOL isTargetAreaOut = FALSE;
	float fForward = EnemyBossConstruct::SPEED;

	// 敵がダメージから復帰したときの処理  ------------------------------
	if (m_dwStatus == FLASH)
	{
		m_nCnt1--;
		if (m_nCnt1 <= 0) m_dwStatus = NORMAL;
	}
	if (m_bIsMove) {

		if (!TargetMove(m_vTarget, fForward))	// 目的地（Target）に達したかどうか
		{
			if (m_AnimStatus.animNum < EnemyBossConstruct::ANIM_WALK) {
				m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_WALK);
			}
			mTemp = XMMatrixRotationY(XMConvertToRadians(m_vRotUp.y));					// 目的地（Target）へ向かう角度
			m_mWorld = mTemp * m_mWorld;
			mTemp = XMMatrixTranslation(m_vPosUp.x, m_vPosUp.y + m_fJumpY, m_vPosUp.z);	// 目的地（Target）へ向かう
			m_mWorld = mTemp * m_mWorld;
		}
		else
		{
			m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_IDLE);
		}
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
	else {
		m_bIsMove = TRUE;
	}
}

//-----------------------------------------------------------------------------
// 敵ボスオブジェクトの制御
//
// 攻撃判定の処理
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void CEnmBossObj::UpdateAttack()
{
	VECTOR3 pcPos = GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());

	// 近距離攻撃を行う距離か
	if (GetDistance(pcPos) <= EnemyBossConstruct::MELEE_RANGE && m_bIsMove) {
		float rot = GetTargetRotateVector(m_mWorld, pcPos).y;
		if (rot >= 180) rot -= 360;
		if (rot <= -180) rot += 360;

		// 背後にいる場合、ボディプレスに移行する
		if (rot > 90.0f || rot < -90.0f)
		{
			m_bIsMove = FALSE;
			m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_BODYSTOMP);
			m_pGMain->m_pEnmBossProc->SetHoldItem(EnemyBossConstruct::ATTACK_BODYSTOMP);
		}else
			if (m_nOldAttackKind == ATTACK_NONE)
			{
				m_mWorld = XMMatrixRotationY(XMConvertToRadians(GetTargetRotateVector(m_mWorld, pcPos).y)) * m_mWorld;
				if (Random(0, 1) == 0)
				{
					// キック攻撃へ遷移
					if (m_AnimStatus.animNum != EnemyBossConstruct::ANIM_KICK && m_AnimStatus.animNum != EnemyBossConstruct::ANIM_KICK_LEFT)
					{
						m_bIsMove = FALSE;
						m_nOldAttackKind = EnemyBossConstruct::ATTACK_KICK_RIGHT;
						if (Random(0, 1) == 0) {
							m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_KICK);
							m_pGMain->m_pEnmBossProc->SetHoldItem(EnemyBossConstruct::ATTACK_KICK_RIGHT);
						}
						else {
							m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_KICK_LEFT);
							m_pGMain->m_pEnmBossProc->SetHoldItem(EnemyBossConstruct::ATTACK_KICK_LEFT);
						}
					}
				}
				else {
					// 踏みつけ攻撃へ遷移
					if (m_AnimStatus.animNum != EnemyBossConstruct::ANIM_STOMP && m_AnimStatus.animNum != EnemyBossConstruct::ANIM_STOMP_LEFT)
					{
						m_bIsMove = FALSE;
						m_nOldAttackKind = EnemyBossConstruct::ATTACK_STOMP_RIGHT;
						if (Random(0, 1)) {
							m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_STOMP);
							m_pGMain->m_pEnmBossProc->SetHoldItem(EnemyBossConstruct::ATTACK_STOMP_RIGHT);
						}
						else {
							m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_STOMP_LEFT);
							m_pGMain->m_pEnmBossProc->SetHoldItem(EnemyBossConstruct::ATTACK_STOMP_LEFT);
						}
					}
				}
			}
			else {
				m_mWorld = XMMatrixRotationY(XMConvertToRadians(GetTargetRotateVector(m_mWorld, pcPos).y)) * m_mWorld;
				// キック攻撃へ遷移
				if (m_AnimStatus.animNum != EnemyBossConstruct::ANIM_KICK && m_nOldAttackKind != EnemyBossConstruct::ATTACK_KICK_RIGHT)
				{
					m_bIsMove = FALSE;
					m_nOldAttackKind = ATTACK_NONE;
					if (Random(0, 1) == 0) {
						m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_KICK);
						m_pGMain->m_pEnmBossProc->SetHoldItem(EnemyBossConstruct::ATTACK_KICK_RIGHT);
					}
					else {
						m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_KICK_LEFT);
						m_pGMain->m_pEnmBossProc->SetHoldItem(EnemyBossConstruct::ATTACK_KICK_LEFT);
					}
				}
				else {
					// 踏みつけ攻撃へ遷移
					if (m_AnimStatus.animNum != EnemyBossConstruct::ANIM_STOMP && m_nOldAttackKind != EnemyBossConstruct::ATTACK_STOMP_RIGHT)
					{
						m_bIsMove = FALSE;
						m_nOldAttackKind = ATTACK_NONE;
						if (Random(0, 1)) {
							m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_STOMP);
							m_pGMain->m_pEnmBossProc->SetHoldItem(EnemyBossConstruct::ATTACK_STOMP_RIGHT);
						}
						else {
							m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_STOMP_LEFT);
							m_pGMain->m_pEnmBossProc->SetHoldItem(EnemyBossConstruct::ATTACK_STOMP_LEFT);
						}
					}
				}
			}

		// ボディプレス攻撃へ遷移
		if (m_AnimStatus.animNum != EnemyBossConstruct::ANIM_BODYSTOMP && m_bIsMove)
		{
		}
	}
	else {
		// 咆哮アニメーションへの遷移
		if (m_bIsMove && m_nRoarCoolTime <= 0 && m_AnimStatus.animNum != EnemyBossConstruct::ANIM_ROAR)
		{
			m_bIsMove = FALSE;
			m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_ROAR);
			VECTOR3 vMin = GetPositionVector(m_mWorld) - VECTOR3(m_pBBox->m_fLengthX, 0.0f, m_pBBox->m_fLengthZ);
			VECTOR3 vMax = GetPositionVector(m_mWorld) + VECTOR3(m_pBBox->m_fLengthX, 0.0f, m_pBBox->m_fLengthZ);
			m_pGMain->m_pEnmProc->Start(vMin, vMax, EnemyBossConstruct::ROAR_SPOWN_NUM);
		}

	}

	// クールタイムのカウントダウン
	if (m_nRoarCoolTime > 0)
	{
		m_nRoarCoolTime--;
	}

	// キック攻撃の判定
	if (m_AnimStatus.animNum == EnemyBossConstruct::ANIM_KICK || m_AnimStatus.animNum == EnemyBossConstruct::ANIM_KICK_LEFT)
	{
		MATRIX4X4 mSword, mHold;
		VECTOR3 vMin, vMax;

		mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, GetHoldItem().m_nPosBone, GetHoldItem().m_nPosMesh);  // 手の位置のボーンマトリックス
		m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->GetInfo(mHold, GetHoldItem(), mSword, vMin, vMax); // ボーンマトリックスを指定して手に持つアイテム情報を得る
		m_pGMain->m_pWeaponProc->m_pWeaponAttackBoxProc->Start(mSword, vMin, vMax, ENM);
	}
	
	// 踏みつけ攻撃の判定
	if ((m_AnimStatus.animNum == EnemyBossConstruct::ANIM_STOMP || m_AnimStatus.animNum == EnemyBossConstruct::ANIM_STOMP_LEFT) && m_AnimStatus.animFrame >= 50) {
		MATRIX4X4 mSword, mHold;
		VECTOR3 vMin, vMax;

		mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, GetHoldItem().m_nPosBone, GetHoldItem().m_nPosMesh);  // 手の位置のボーンマトリックス
		m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->GetInfo(mHold, GetHoldItem(), mSword, vMin, vMax); // ボーンマトリックスを指定して手に持つアイテム情報を得る
		m_pGMain->m_pWeaponProc->m_pWeaponAttackBoxProc->Start(mSword, vMin, vMax, ENM);
	}

	// ボディプレス攻撃の判定
	if (m_AnimStatus.animNum == EnemyBossConstruct::ANIM_BODYSTOMP)
	{
		MATRIX4X4 mPos = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		// 移動処理
		if (GetMesh()->GetRootAnimType(m_AnimStatus.animNum) == eRootAnimXZ)
		{
			// ルートボーンアニメーションでの前進移動値
			mPos = GetMesh()->GetRootAnimUpMatrices(m_AnimStatus);
			
		}
		m_mWorld = mPos * m_mWorld;

		// 当たり判定
		if (m_AnimStatus.animFrame >= 80)
		{
			MATRIX4X4 mSword, mHold;
			VECTOR3 vMin, vMax;

			mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, GetHoldItem().m_nPosBone, GetHoldItem().m_nPosMesh);  // 手の位置のボーンマトリックス
			m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->GetInfo(mHold, GetHoldItem(), mSword, vMin, vMax); // ボーンマトリックスを指定して手に持つアイテム情報を得る
			m_pGMain->m_pWeaponProc->m_pWeaponAttackBoxProc->Start(mSword, vMin, vMax, ENM);

		}
	}

	// 咆哮時のUI振動処理
	if (m_AnimStatus.animNum == EnemyBossConstruct::ANIM_ROAR && m_AnimStatus.animFrame >= 100 && m_AnimStatus.animFrame <= 250)
	{
		m_pGMain->m_pBackForeProc->StartSwing(Random(0, 360));
	}

	// 終了判定
	if (m_AnimStatus.isEnd(EnemyBossConstruct::ANIM_KICK) || m_AnimStatus.isEnd(EnemyBossConstruct::ANIM_KICK_LEFT) ||
		m_AnimStatus.isEnd(EnemyBossConstruct::ANIM_STOMP) || m_AnimStatus.isEnd(EnemyBossConstruct::ANIM_STOMP_LEFT))
	{
		m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_IDLE);
		m_bIsMove = TRUE;
	}
	if (m_AnimStatus.isEnd(EnemyBossConstruct::ANIM_ROAR))
	{
		m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_IDLE);
		m_bIsMove = TRUE;
		m_nRoarCoolTime = 60 * EnemyBossConstruct::ROAR_COOLTIME;

	}

	// ボディプレスの終了判定
	if (m_AnimStatus.isEnd(EnemyBossConstruct::ANIM_BODYSTOMP))
	{
		m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_STANDUP);
	}

	// 立ち上がりの終了判定
	if (m_AnimStatus.isEnd(EnemyBossConstruct::ANIM_STANDUP))
	{
		m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_IDLE);
		m_bIsMove = TRUE;
	}
}

//-----------------------------------------------------------------------------
// 敵ボスオブジェクトの制御
//
// 敵がダメージ状態のときの処理
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CEnmBossObj::UpdateDamage()
{
	const int nFlashTime = 5;  // 無敵状態の時間

	if (m_pHitObj->GetTag() != PC) {
		m_pGMain->m_pEffectProc->m_pBillProc->Start(GetPositionVector(m_mWorld) + VECTOR3(0.0f, 1.5f, 0.0f));
		m_nHp -= m_pHitObj->GetAtc();	// 攻撃を受けたダメージ
		if (m_nHp <= 0)
		{
			m_nHp = 0;
			m_pGMain->m_pBackForeProc->StartSwing(Random(0, 360));
			m_AnimStatus.SetNum(EnemyBossConstruct::ANIM_DIE);
			m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetDeadBoss(TRUE);
			m_dwStatus = DEAD;		// HPが０なので死亡へ
		}
		else {
			m_nCnt1 = nFlashTime;   // 無敵状態の時間
			m_dwStatus = FLASH;     // ダメージからの復帰処理を行う
			UpdateAttack();
		}
	}
	else {
		m_dwStatus = NORMAL;
	}
}

//-----------------------------------------------------------------------------
// 敵ボスオブジェクトの制御
//
// 敵が死亡状態のときの処理
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CEnmBossObj::UpdateDead()
{
	if (m_AnimStatus.isEnd(EnemyBossConstruct::ANIM_DIE))
	{
		m_pGMain->m_pBackForeProc->SetBlackOut(TRUE);
		m_AnimStatus.playAnim = false;
		m_AnimStatus.animFrame = m_AnimStatus.endFrame - 1; // フレームエンドの一つ手前にする

	}
	
	if (m_pGMain->m_pBackForeProc->GetWhiteOut()) {
		// クリア画面へ移行
		m_pGMain->m_dwGameStatus = GAMECLEAR;
		m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetDeadBoss(FALSE);
		m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetCtrl(CAMERA_GAMECLEAR);
		m_pGMain->m_pBossBgm->Stop();
		m_pGMain->m_pGameClear->Play(AUDIO_LOOP);
		m_bActive = FALSE;
	}
}

//============================================================================
// アクセス関数
//============================================================================
void CEnmBossObj::SetMovieFinish(bool flag)
{
	m_bIsMovieFinish = flag;
}

BOOL CEnmBossObj::GetMovieFinish()
{
	return m_bIsMovieFinish;
}
