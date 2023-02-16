//=============================================================================
//		武器の処理
//=============================================================================
#include  "Enemy.h"
#include  "Playchar.h"
#include  "Ally.h"
#include  "Weapon.h"
#include  "Map.h"
#include  "Effect.h"
#include  "BackFore.h"

//=============================================================================================================================
// 武器メインプロシージャのコンストラクタ
//
//   なお、プロシージャのdeleteはCBaseProcのデストラクタで行うため不要
// ----------------------------------------------------------------------------------------------------------------------------
CWeaponProc::CWeaponProc(CGameMain* pGMain) : CBaseProc(pGMain)
{

	m_pWeaponArrowProc = new CWeaponArrowProc(pGMain);		// 武器・弓矢プロシージャ
	m_pProcArray.push_back(m_pWeaponArrowProc);				// プロシージャをプロシージャ配列に登録する

	m_pWeaponItemBoxProc = new CWeaponItemBoxProc(pGMain);	// アイテム・宝箱プロシージャ
	m_pProcArray.push_back(m_pWeaponItemBoxProc);				// プロシージャをプロシージャ配列に登録する

	m_pWeaponSwordProc = new CWeaponSwordProc(pGMain);		// 武器・剣プロシージャ
	m_pProcArray.push_back(m_pWeaponSwordProc);				// プロシージャをプロシージャ配列に登録する

	m_pWeaponShotSlashProc = new CWeaponShotSlashProc(pGMain);
	m_pProcArray.push_back(m_pWeaponShotSlashProc);

	m_pWeaponGunProc = new CWeaponGunProc(pGMain);					// 武器・銃プロシージャ
	m_pProcArray.push_back(m_pWeaponGunProc);						// プロシージャをプロシージャ配列に登録する

	m_pWeaponAxeProc = new CWeaponAxeProc(pGMain);					// 武器・斧プロシージャ
	m_pProcArray.push_back(m_pWeaponAxeProc);						// プロシージャをプロシージャ配列に登録する

	m_pWeaponAxeShotProc = new CWeaponAxeShotProc(pGMain);			// 武器・遠投プロシージャ
	m_pProcArray.push_back(m_pWeaponAxeShotProc);					// プロシージャをプロシージャ配列に登録する

	m_pWeaponDamageAreaProc = new CWeaponDamageAreaProc(pGMain);	// 武器・ダメージエリアプロシージャ
	m_pProcArray.push_back(m_pWeaponDamageAreaProc);				// プロシージャをプロシージャ配列に登録する

	m_pWeaponAttackBoxProc = new CWeaponAttackBoxProc(pGMain);		// 武器・攻撃範囲プロシージャ
	m_pProcArray.push_back(m_pWeaponAttackBoxProc);					// プロシージャをプロシージャ配列に登録する

	m_pWeaponHoldProc = new CWeaponHoldProc(pGMain);				// 武器・手に持つアイテムのプロシージャ
	m_pProcArray.push_back(m_pWeaponHoldProc);						// プロシージャをプロシージャ配列に登録する
}

// ================================================================================================================================
// 定数定義
// ================================================================================================================================
const float WeaponConstruct::ARROW_SPEED = 0.5f;
const VECTOR3 WeaponConstruct::ARROW_START_DIFF = VECTOR3(0.0f, 0.3f, 1.0f);
const VECTOR3 WeaponConstruct::ARROW_HITBOX_MIN = VECTOR3(-0.1f, -0.1f, -0.4f);
const VECTOR3 WeaponConstruct::ARROW_HITBOX_MAX = VECTOR3(0.1f, 0.1f, 0.4f);

const VECTOR3 WeaponConstruct::BOX_HITBOX_MIN = VECTOR3(-1.7f, 0.0f, 18.9f);
const VECTOR3 WeaponConstruct::BOX_HITBOX_MAX = VECTOR3(1.7f, 2.2f, 21.1f);

const VECTOR3 WeaponConstruct::SHOTSLASH_HITBOX_MIN = VECTOR3(-0.5f, 0.0f, -0.1f);
const VECTOR3 WeaponConstruct::SHOTSLASH_HITBOX_MAX = VECTOR3(0.5f, 0.5f, 0.1f);
const float WeaponConstruct::SHOTSLASH_SPEED = 0.4f;

const float WeaponConstruct::GUN_SPEED = 0.5f;
const VECTOR3 WeaponConstruct::GUN_HITBOX_MIN = VECTOR3(-0.05f, -0.05f, -0.05f);
const VECTOR3 WeaponConstruct::GUN_HITBOX_MAX = VECTOR3(0.05f, 0.05f, 0.05f);

const VECTOR3 WeaponConstruct::SHOTAXE_HITBOX_MIN = VECTOR3(-0.4f, -0.6f, -0.2f);
const VECTOR3 WeaponConstruct::SHOTAXE_HITBOX_MAX = VECTOR3(0.4f, 0.6f, 0.2f);

const float WeaponConstruct::SHOTAXE_SPEED = 0.3f;
const float WeaponConstruct::SHOTAXE_ROTSPEED = 3.0f;

// ================================================================================================================================
// 各武器プロシージャ毎の処理
// ================================================================================================================================
// ========================================================================================
// 弓矢プロシージャクラス
// ========================================================================================
// ---------------------------------------------------------------------------
// 武器・弓矢プロシージャのコンストラクタ
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
// ---------------------------------------------------------------------------
CWeaponArrowProc::CWeaponArrowProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// 弓矢メッシュの設定
	m_pMesh = new CFbxMesh( m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Arrow/Allow_Test.mesh"));

	for (int i = 0; i < ARROW_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponArrowObj(this));	// m_pObjArrayに武器・弓矢オブジェクトを生成する
	}
}

// ---------------------------------------------------------------------------
// 武器・弓矢プロシージャのデストラクタ
// ---------------------------------------------------------------------------
CWeaponArrowProc::~CWeaponArrowProc()
{
	;
}

// ---------------------------------------------------------------------------
// 武器・弓矢プロシージャの発射
//
//  MATRIX4X4  mWorld       ：弓矢の発射位置のマトリックス
//  DWORD       dwOwner      ：武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
// ---------------------------------------------------------------------------
BOOL CWeaponArrowProc::Start(MATRIX4X4 mWorld, DWORD dwOwner)
{
	for (int i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(mWorld, dwOwner);	       // 武器・弓矢オブジェクトの発射
			return TRUE;
		}
	}
	return FALSE;
}

// ========================================================================================
// 弓矢オブジェクトクラス
// ========================================================================================
// ---------------------------------------------------------------------------
// 武器・弓矢オブジェクトのコンストラクタ
// ---------------------------------------------------------------------------
CWeaponArrowObj::CWeaponArrowObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	m_bActive = FALSE;
	m_dwStatus = NORMAL;

	m_nAtc = WeaponConstruct::ARROW_ATTACK;
	m_dwOwner = 0;
	m_nReflectionCount = 0;
	m_nHitCount = 0;

	// 当たり判定
	m_pBBox = new CBBox(m_pGMain->m_pShader, WeaponConstruct::ARROW_HITBOX_MIN, WeaponConstruct::ARROW_HITBOX_MAX);
}

// ---------------------------------------------------------------------------
// 武器・弓矢オブジェクトのデストラクタ
// ---------------------------------------------------------------------------
CWeaponArrowObj::~CWeaponArrowObj()
{
}

// ---------------------------------------------------------------------------
// 武器・弓矢オブジェクトの発射
//
//     MATRIX4X4 mStartWorld     弓矢の発射位置のマトリックス
//     DWORD      dwOwner         武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
BOOL CWeaponArrowObj::Start(MATRIX4X4 mStartWorld, DWORD dwOwner)
{
	if (m_bActive) return FALSE;

	m_bActive = TRUE;
	ResetStatus();
	m_mWorld = XMMatrixTranslationFromVector(WeaponConstruct::ARROW_START_DIFF) * mStartWorld;
	m_vPosUp = VECTOR3(0, 0, WeaponConstruct::ARROW_SPEED);
	m_nTag = WEAPON;
	m_dwOwner = dwOwner;

	// 能力を参照
	if (dwOwner == PC)
	{
		m_nReflectionCount = m_pGMain->m_pPcProc->GetAbility()->GetAbilityLevel(1);
		m_nHitCount = m_pGMain->m_pPcProc->GetAbility()->GetAbilityLevel(2);
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
// 武器・弓矢オブジェクトの更新
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponArrowObj::Update()
{
	if (m_bActive)
	{
		// 移動処理
		m_mWorldOld = m_mWorld;
		m_mWorld = XMMatrixTranslationFromVector(m_vPosUp) * m_mWorld;
		m_pBBox->m_mWorld = m_mWorld;

		// マップとのあたり判定
		VECTOR3 vHit, vNormal;
		if (m_pGMain->m_pMapProc->Hitcheck(GetPositionVector(m_mWorld), GetPositionVector(m_mWorldOld), &vHit, &vNormal))
		{
			if (m_nReflectionCount > 0) {
				// 反射する
				m_nReflectionCount--;
				if (vNormal.x == 1.0f) {
					// |  の壁の場合
					float y = GetRotateVector(m_mWorld).y;
					if (y < 0) {
						y *= -1;
					}
					float temp = y * 2.0f;
					MATRIX4X4 rot = XMMatrixRotationY(XMConvertToRadians(temp));
					m_mWorld = rot * m_mWorld;
				}

				else if (vNormal.z == -1.0f) {
					// - の壁の場合
					float y = GetRotateVector(m_mWorld).y;
					float temp = 180.0f - y * 2.0f;
					MATRIX4X4 rot = XMMatrixRotationY(XMConvertToRadians(temp));
					m_mWorld = rot * m_mWorld;
				}

				else if (vNormal.x == -1.0f) {
					//  |　の壁の場合
					float y = GetRotateVector(m_mWorld).y;
					if (y < 0) {
						y *= -1;
					}
					float temp = y * 2.0f * -1.0f;
					MATRIX4X4 rot = XMMatrixRotationY(XMConvertToRadians(temp));
					m_mWorld = rot * m_mWorld;
				}

				else if (vNormal.z == 1.0f) {
					// _ の壁の場合
					float y = GetRotateVector(m_mWorld).y;
					float temp = 180.0f - y * 2.0f;
					MATRIX4X4 rot = XMMatrixRotationY(XMConvertToRadians(temp));
					m_mWorld = rot * m_mWorld;
				}
				else {
					m_bActive = FALSE;	// 消す
				}
					
			}
			else {
				// 反射せずに削除する
				m_bActive = FALSE;	// 消す
			}
				
		}

		// マップ範囲外にいるオブジェクトは当たり判定を行わない
		VECTOR3 pos = GetPositionVector(m_mWorld);
		if ((pos.x > -30.0f && pos.x < 30.0f) && (pos.z > -30.0f && pos.z < 30.0f)) {
			// 敵やＰＣとのあたり判定
			if ((m_dwOwner != ENM && (m_pGMain->m_pMapProc->GetEnmSpPart()->HitcheckSpPart(this, GetPositionVector(m_mWorld))))
				|| (m_dwOwner != PC && (m_pGMain->m_pMapProc->GetPcSpPart()->HitcheckSpPart(this, GetPositionVector(m_mWorld)))))
			{
				if (m_nHitCount <= 0) {
					m_bActive = FALSE;
				}
				else {
					m_dwStatus = NORMAL;
					m_nHitCount--;
				}
			}
		}
		// レンダリング
		Render();
	}
}

// ---------------------------------------------------------------------------
// 武器・弓矢オブジェクトのレンダリング
//
//   陰影をつけないレンダリング　光の方向が(0,0,0)
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponArrowObj::Render()
{
	GetMesh()->Render(m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, VECTOR3(0, 0, 0), m_pGMain->m_vEyePt);
}

// ========================================================================================
// 宝箱プロシージャクラス
// ========================================================================================
//------------------------------------------------------------------------
// アイテム・宝箱プロシージャのコンストラクタ
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
// ---------------------------------------------------------------------------
CWeaponItemBoxProc::CWeaponItemBoxProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// 宝箱メッシュの設定
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Map/MapItem/box/box.mesh"));

	for (int i = 0; i < ITEMBOX_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponItemBoxObj(this));	// m_pObjArrayにアイテム・宝箱オブジェクトを生成する
	}
}

//------------------------------------------------------------------------
// アイテム・宝箱プロシージャのデストラクタ
// ---------------------------------------------------------------------------
CWeaponItemBoxProc::~CWeaponItemBoxProc()
{
	;
}

// ---------------------------------------------------------------------------
// アイテム・宝箱プロシージャの発射
//
//  VECTOR3 vPos	       ：生成位置
// ---------------------------------------------------------------------------
BOOL CWeaponItemBoxProc::Start(VECTOR3 vPos)
{
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(vPos);	// アイテム・宝箱オブジェクト
			return TRUE;
		}
	}
	return FALSE;
}

// ========================================================================================
// 宝箱オブジェクトクラス
// ========================================================================================
//------------------------------------------------------------------------
// アイテム・宝箱オブジェクトのコンストラクタ
// ---------------------------------------------------------------------------
CWeaponItemBoxObj::CWeaponItemBoxObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	// 当たり判定
	m_pBBox = new CBBox(m_pGMain->m_pShader, WeaponConstruct::BOX_HITBOX_MIN, WeaponConstruct::BOX_HITBOX_MAX);

	m_bActive = FALSE;
	m_dwStatus = NORMAL;

	m_nAtc = 0;
}

// ---------------------------------------------------------------------------
// アイテム・宝箱オブジェクトのデストラクタ
// ---------------------------------------------------------------------------
CWeaponItemBoxObj::~CWeaponItemBoxObj()
{
	;
}

// ---------------------------------------------------------------------------
// アイテム・宝箱オブジェクトの発射
//
//     VECTOR3 vPos		       ：生成位置
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
BOOL CWeaponItemBoxObj::Start(VECTOR3 vPos)
{
	m_mWorld = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
	m_mWorldOld = m_mWorld;
	m_bActive = TRUE;
	ResetStatus();
	return TRUE;
}

// ---------------------------------------------------------------------------
// アイテム・宝箱オブジェクトの更新
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponItemBoxObj::Update()
{
	if (m_bActive)
	{
		// 敵やＰＣとのあたり判定
		if (m_pGMain->m_pPcProc->Hitcheck(this, m_pBBox))
		{
			// AbilityUIの表示
			if (m_pGMain->m_pMapProc->GetHierarchy() == 20) {
				m_pGMain->m_dwGameStatus = GAMECLEAR;
			}
			else {
				m_pGMain->m_pBackForeProc->m_nDialogKind = DIALOG_CHECK;
				m_pGMain->m_pBackForeProc->SetSelectAbility();
				m_pGMain->m_isMove = false;
				m_bActive = FALSE;
			}
			
		}
		// レンダリング
		Render();
	}
}

// ---------------------------------------------------------------------------
// アイテム・宝箱オブジェクトのレンダリング
//
//   陰影をつけないレンダリング　光の方向が(0,0,0)
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponItemBoxObj::Render()
{
	GetMesh()->Render(m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, VECTOR3(0, 0, 0), m_pGMain->m_vEyePt);        // -- 2022.2.16
}

// ========================================================================================
// 剣プロシージャクラス
// ========================================================================================
// ---------------------------------------------------------------------------
// 武器・剣プロシージャのコンストラクタ
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
// ---------------------------------------------------------------------------
CWeaponSwordProc::CWeaponSwordProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (int i = 0; i < SWORD_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponSwordObj(this));	// m_pObjArrayに武器・剣オブジェクトを生成する
	}
}

// ---------------------------------------------------------------------------
// 武器剣・プロシージャの発生
// ---------------------------------------------------------------------------
BOOL CWeaponSwordProc::Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner)
{
	for (int i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(mMat, vMin, vMax, dwOwner);	       // 武器・剣オブジェクトの発射
			return TRUE;
		}
	}
	return FALSE;
}

// ========================================================================================
// 剣オブジェクトクラス
// ========================================================================================
// ---------------------------------------------------------------------------
// 武器・剣オブジェクトのコンストラクタ
// ---------------------------------------------------------------------------
CWeaponSwordObj::CWeaponSwordObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nAtc = WeaponConstruct::SWORD_ATTACK;    // 剣の攻撃力

	m_bActive = FALSE;
	m_dwStatus = NORMAL;

	// BBoxの設定(大きさは、仮に設定)
	m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.1f, -0.1f, -0.1f), VECTOR3(0.1f, 0.1f, 0.1f));
	m_nAtc = nAtc;
	m_dwOwner = 0;
}

// ---------------------------------------------------------------------------
// 武器・剣オブジェクトのデストラクタ
// ---------------------------------------------------------------------------
CWeaponSwordObj::~CWeaponSwordObj()
{
	;
}

// ---------------------------------------------------------------------------
// 武器・剣オブジェクトの発生と処理、描画
//
//     MATRIX4X4  mMat        ：剣の位置
//     VECTOR3 vMin        ：バウンディングボックスの最小値
//     VECTOR3 vMax        ：バウンディングボックスの最大値
//     DWORD       dwOwner     ：武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
BOOL CWeaponSwordObj::Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner)
{
	m_bActive = TRUE;  // スタート時に表示開始
	ResetStatus();
	// バウンディングボックスのサイズが変わっていたら作り替える
	if (m_pBBox->m_vMin.x != vMin.x || m_pBBox->m_vMin.y != vMin.y || m_pBBox->m_vMin.z != vMin.z
		||
		m_pBBox->m_vMax.x != vMax.x || m_pBBox->m_vMax.y != vMax.y || m_pBBox->m_vMax.z != vMax.z)
	{
		m_pBBox->InitBBox(vMin, vMax);   // バウンディングボックスの大きさ変更
	}
	m_pBBox->m_mWorld = mMat;   // バウンディングボックスのワールドマトリックスをセット

	m_nTag = WEAPON;
	m_dwOwner = dwOwner;

	return TRUE;
}
// ---------------------------------------------------------------------------
// 武器・剣オブジェクトの更新
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponSwordObj::Update()
{
	if (m_bActive) {
		// 敵やＰＣとのあたり判定
		if ((m_dwOwner != ENM && (m_pGMain->m_pMapProc->GetEnmSpPart()->HitcheckSpPart(this, GetPositionVector(m_pBBox->m_mWorld))))
			|| (m_dwOwner != PC && (m_pGMain->m_pMapProc->GetPcSpPart()->HitcheckSpPart(this, GetPositionVector(m_pBBox->m_mWorld))))) {
			m_bActive = FALSE;
		}
		m_bActive = FALSE;  // １回処理したら、消す
	}
}

// ========================================================================================
// 斬撃プロシージャクラス
// ========================================================================================
// ---------------------------------------------------------------------------
// 武器・斬撃プロシージャのコンストラクタ
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
// ---------------------------------------------------------------------------
CWeaponShotSlashProc::CWeaponShotSlashProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (int i = 0; i < SWORDSHOT_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponShotSlashObj(this));	// m_pObjArrayに武器・斬撃オブジェクトを生成する
	}
}

// ---------------------------------------------------------------------------
// 斬撃プロシージャのデストラクタ
// ---------------------------------------------------------------------------
CWeaponShotSlashProc::~CWeaponShotSlashProc()
{
	;
}

// ---------------------------------------------------------------------------
// 武器・斬撃プロシージャの発射
//
//  MATRIX4X4  mWorld       ：レーザーの発射位置のマトリックス
//  DWORD       dwOwner      ：武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
// ---------------------------------------------------------------------------
BOOL CWeaponShotSlashProc::Start(MATRIX4X4 mWorld, DWORD dwOwner)
{
	for (int i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(mWorld, dwOwner);	       // 武器・斬撃オブジェクトの発射
			return TRUE;
		}
	}
	return FALSE;
}

// ========================================================================================
// 斬撃オブジェクトクラス
// ========================================================================================
// ---------------------------------------------------------------------------
// 武器・斬撃オブジェクトのコンストラクタ
// ---------------------------------------------------------------------------
CWeaponShotSlashObj::CWeaponShotSlashObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nAtc = WeaponConstruct::SHOTSLASH_ATTACK;				// レーザーの攻撃力

	m_bActive = FALSE;
	m_dwStatus = NORMAL;

	// BBoxの設定(大きさは、仮に設定)
	m_pBBox = new CBBox(m_pGMain->m_pShader, WeaponConstruct::SHOTSLASH_HITBOX_MIN, WeaponConstruct::SHOTSLASH_HITBOX_MAX);
	m_pEffect = NULL;
	m_fLength = 0.0f;

	m_nAtc = nAtc;
}

// ---------------------------------------------------------------------------
// 武器・斬撃オブジェクトのデストラクタ
// ---------------------------------------------------------------------------
CWeaponShotSlashObj::~CWeaponShotSlashObj()
{
	;
}

// ---------------------------------------------------------------------------
// 武器・斬撃オブジェクトの発射
//
//     MATRIX4X4 mStartWorld     レーザーの発射位置のマトリックス
//     DWORD      dwOwner         武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
BOOL CWeaponShotSlashObj::Start(MATRIX4X4 mStartWorld, DWORD dwOwner)
{
	m_fLength = WeaponConstruct::SHOTSLASH_LENGTH * m_pGMain->m_pPcProc->GetAbility()->GetAbilityLevel(4);
	if (m_bActive) return FALSE;

	m_bActive = TRUE;
	ResetStatus();
	m_mWorld = XMMatrixTranslation(0.0f, 0.3f, 1.0f) * mStartWorld;
	m_vPosUp = VECTOR3(0, 0, WeaponConstruct::SHOTSLASH_SPEED);
	m_pEffect = m_pGMain->m_pEffectProc->m_pBillSlashShotProc->Start(m_mWorld);
	m_nTag = WEAPON;

	return TRUE;
}

// ---------------------------------------------------------------------------
// 武器・斬撃オブジェクトの更新
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponShotSlashObj::Update()
{
	if (m_bActive)
	{
		m_bActive = m_pEffect->GetActive();
		m_mWorldOld = m_mWorld;
		m_mWorld = XMMatrixTranslationFromVector(m_vPosUp) * m_mWorld;
		m_pBBox->m_mWorld = m_mWorld;

		m_fLength -= m_vPosUp.z;
		if (m_fLength <= 0.0f) {
			m_dwStatus = DAMAGE;
		}

		switch (m_dwStatus)
		{
		case DAMAGE:
			m_bActive = FALSE;
			m_pEffect->SetActive(FALSE);
			return;
		}
		
		// 敵とのあたり判定
		if (m_pGMain->m_pMapProc->GetEnmSpPart()->HitcheckSpPart(this, GetPositionVector(m_mWorld)))
		{
			m_bActive = FALSE;	// 消す
			m_pEffect->SetActive(FALSE);
		}
		
		// マップとのあたり判定
		VECTOR3 vHit, vNormal;
		if (m_pGMain->m_pMapProc->Hitcheck(GetPositionVector(m_mWorld), GetPositionVector(m_mWorldOld), &vHit, &vNormal))
		{
			m_bActive = FALSE;	// 消す
			m_pEffect->SetActive(FALSE);
		}
		
		m_pEffect->SetWorld(m_mWorld);
	}
}

// ========================================================================================
// 銃プロシージャクラス
// ========================================================================================
// ---------------------------------------------------------------------------
// 武器・銃プロシージャのコンストラクタ
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
// ---------------------------------------------------------------------------
CWeaponGunProc::CWeaponGunProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// 弾丸メッシュの設定
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Mark.mesh"));

	for (int i = 0; i < GUN_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponGunObj(this));	// m_pObjArrayに武器・銃オブジェクトを生成する
	}
}

//============================================================================
// 武器・銃プロシージャのデストラクタ
// ---------------------------------------------------------------------------
CWeaponGunProc::~CWeaponGunProc()
{
	;
}

// ---------------------------------------------------------------------------
// 武器・銃プロシージャの発射
//
//  VECTOR3 vStart       ：発射位置
//  DWORD       dwOwner      ：武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
// ---------------------------------------------------------------------------
BOOL CWeaponGunProc::Start(MATRIX4X4 mWorld, DWORD dwOwner)
{
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(mWorld, dwOwner);	// 武器・銃オブジェクト
			return TRUE;
		}
	}
	return FALSE;
}

// ========================================================================================
// 銃オブジェクトクラス
// ========================================================================================
// ---------------------------------------------------------------------------
// 武器・銃オブジェクトのコンストラクタ
// ---------------------------------------------------------------------------
CWeaponGunObj::CWeaponGunObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nAtc = WeaponConstruct::GUN_ATTACK;  // 攻撃力

	m_bActive = FALSE;
	m_dwStatus = NORMAL;
	m_nAtc = nAtc;

	// Boxの設定
	m_pBBox = new CBBox(m_pGMain->m_pShader, WeaponConstruct::GUN_HITBOX_MIN, WeaponConstruct::GUN_HITBOX_MAX);
}

// ---------------------------------------------------------------------------
// 武器・銃オブジェクトのデストラクタ
// ---------------------------------------------------------------------------
CWeaponGunObj::~CWeaponGunObj()
{
	;
}

// ---------------------------------------------------------------------------
// 武器・銃オブジェクトの発射
//
//     MATRIX4X4 mWorld     ：弾丸の発射位置
//     DWORD   dwOwner      ：武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
BOOL CWeaponGunObj::Start(MATRIX4X4 mWorld, DWORD dwOwner)
{
	m_bActive = TRUE;
	m_mWorld = mWorld;
	m_mWorldOld = m_mWorld;
	m_nAtc = 50 + m_pGMain->m_pPcProc->GetAbility()->GetAbilityLevel(0) * 20;

	m_nTag = WEAPON;
	m_vPosUp = VECTOR3(0, 0, 0.5f);

	return TRUE;
}

// ---------------------------------------------------------------------------
// 武器・銃オブジェクトの更新
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponGunObj::Update()
{

	if (m_bActive)
	{
		m_mWorldOld = m_mWorld;
		m_pBBox->m_mWorld = m_mWorld;

		MATRIX4X4 mTran;
		mTran = XMMatrixTranslation(m_vPosUp.x, m_vPosUp.y, m_vPosUp.z);
		m_mWorld = mTran * m_mWorld;

		// 敵やＰＣとのあたり判定
		if (m_pGMain->m_pMapProc->GetEnmSpPart()->HitcheckSpPart(this, GetPositionVector(m_mWorld)))
		{
			m_bActive = FALSE;		
		}
		// マップとのあたり判定
		VECTOR3 vHit, vNormal;
		if (m_pGMain->m_pMapProc->Hitcheck(GetPositionVector(m_mWorld), GetPositionVector(m_mWorldOld), &vHit, &vNormal))
		{
			m_bActive = FALSE;
		}
		// レンダリング
		Render();
	}
}

// ---------------------------------------------------------------------------
// 武器・銃オブジェクトのレンダリング
//
//   陰影をつけないレンダリング　光の方向が(0,0,0)
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponGunObj::Render()
{
	// 銃のレンダリング
	GetMesh()->Render(m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, VECTOR3(0, 0, 0), m_pGMain->m_vEyePt);   
}

// ========================================================================================
// 斧プロシージャクラス
// ========================================================================================
// ---------------------------------------------------------------------------
// 武器・斧プロシージャのコンストラクタ
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
// ---------------------------------------------------------------------------
CWeaponAxeProc::CWeaponAxeProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (int i = 0; i < AXE_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponAxeObj(this));	// m_pObjArrayに武器・斧オブジェクトを生成する
	}
}

// ---------------------------------------------------------------------------
// 武器・斧プロシージャのデストラクタ
// ---------------------------------------------------------------------------
CWeaponAxeProc::~CWeaponAxeProc()
{
	;
}

// ---------------------------------------------------------------------------
// 武器・斧プロシージャの発射
//
//  MATRIX4X4  mGun         ：銃の位置マトリックス
//  VECTOR3    vOffset      ：位置の微調整
//  MATRIX4X4  mOwnerWorld  ：発射するキャラのマトリックス
//  DWORD      dwOwner      ：武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
// ---------------------------------------------------------------------------
BOOL CWeaponAxeProc::Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner)
{
	for (int i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(mMat, vMin, vMax, dwOwner);	       // 武器・斧オブジェクトの発射
			return TRUE;
		}
	}
	return FALSE;
}

// ========================================================================================
// 斧オブジェクトクラス
// ========================================================================================
// ---------------------------------------------------------------------------
// 武器・斧オブジェクトのコンストラクタ
// ---------------------------------------------------------------------------
CWeaponAxeObj::CWeaponAxeObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nAtc = WeaponConstruct::AXE_ATTACK;    // 剣の攻撃力

	m_bActive = FALSE;
	m_dwStatus = NORMAL;

	// BBoxの設定(大きさは、仮に設定)
	m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.1f, -0.1f, -0.1f), VECTOR3(0.1f, 0.1f, 0.1f));
	m_nAtc = nAtc;
}

// ---------------------------------------------------------------------------
// 武器・斧オブジェクトのデストラクタ
// ---------------------------------------------------------------------------
CWeaponAxeObj::~CWeaponAxeObj()
{
	;
}

// ---------------------------------------------------------------------------
// 武器・斧オブジェクトの発射
//
//     MATRIX4X4 mStartWorld     斧の発射位置のマトリックス
//     DWORD      dwOwner        武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
BOOL CWeaponAxeObj::Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner)
{
	m_bActive = TRUE;  // スタート時に表示開始
	ResetStatus();
	// 能力値に応じて当たり判定を拡大
	m_pBBox->InitBBox(vMin, vMax + VECTOR3(0.0f, 0.0f, m_pGMain->m_pPcProc->GetAbility()->GetAbilityLevel(5)));
	m_pBBox->m_mWorld = mMat;   // バウンディングボックスのワールドマトリックスをセット

	m_nTag = WEAPON;

	return TRUE;
}

// ---------------------------------------------------------------------------
// 武器・斧オブジェクトの更新
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponAxeObj::Update()
{

	if (m_bActive) {
		// 敵やＰＣとのあたり判定
		if (m_pGMain->m_pMapProc->GetEnmSpPart()->HitcheckSpPart(this, GetPositionVector(m_pBBox->m_mWorld)))
		{
			m_bActive = FALSE;
		}
		m_bActive = FALSE;  // １回処理したら、消す
	}
}

// ========================================================================================
// 斧投げプロシージャクラス
// ========================================================================================
// ---------------------------------------------------------------------------
// 武器・斧投げプロシージャのコンストラクタ
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
// ---------------------------------------------------------------------------
CWeaponAxeShotProc::CWeaponAxeShotProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Axe/Axe.mesh"));

	for (int i = 0; i < AXESHOT_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponAxeShotObj(this));	// m_pObjArrayに武器・斧投げオブジェクトを生成する
	}
}

// ---------------------------------------------------------------------------
// 武器・斧投げプロシージャのデストラクタ
// ---------------------------------------------------------------------------
CWeaponAxeShotProc::~CWeaponAxeShotProc()
{
}

// ---------------------------------------------------------------------------
// 武器・斧投げプロシージャの発射
//
//  MATRIX4X4  mGun         ：銃の位置マトリックス。マトリックスの位置のみ使用する
//  VECTOR3    vOffset      ：位置の微調整
//  MATRIX4X4  mOwnerWorld  ：発射するキャラのマトリックス。マトリックスの角度のみ使用する
//  DWORD      dwOwner      ：武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
// ---------------------------------------------------------------------------
BOOL CWeaponAxeShotProc::Start(MATRIX4X4 mGun, DWORD dwLength, DWORD dwOwner)
{
	for (int i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(mGun, dwLength, dwOwner);	       // 武器・スプレーオブジェクトの発射
			break;
		}
	}
	return FALSE;
}

// ========================================================================================
// 斧投げオブジェクトクラス
// ========================================================================================
// ---------------------------------------------------------------------------
// 武器・斧投げオブジェクトのコンストラクタ
// ---------------------------------------------------------------------------
CWeaponAxeShotObj::CWeaponAxeShotObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nAtc = WeaponConstruct::SHOTAXE_ATTACK;  // 攻撃力

	m_pEffect = NULL;

	m_bActive = FALSE;
	m_bBackFlag = FALSE;

	m_dwStatus = NORMAL;
	m_nAtc = nAtc;
	m_dwOwner = 0;
	m_fMoveRot = 0.0f;
	m_fLength = 0.0f;
	m_fAnimRot = 0.0f;

	m_pBBox = new CBBox(m_pGMain->m_pShader, WeaponConstruct::SHOTAXE_HITBOX_MIN, WeaponConstruct::SHOTAXE_HITBOX_MAX);
}

// ---------------------------------------------------------------------------
// 武器・斧投げオブジェクトのデストラクタ
// ---------------------------------------------------------------------------
CWeaponAxeShotObj::~CWeaponAxeShotObj()
{
}

// ---------------------------------------------------------------------------
// 武器・斧投げオブジェクトの発射
//
//     MATRIX4X4 mStartWorld     スプレーの発射位置のマトリックス
//     DWORD      dwOwner        武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
BOOL CWeaponAxeShotObj::Start(MATRIX4X4 mStartWorld, DWORD dwLength, DWORD dwOwner)
{
	m_bActive = TRUE;
	m_mWorld = mStartWorld;
	m_mWorldOld = mStartWorld;
	m_fLength = dwLength;
	m_fMoveRot = GetRotateVector(m_mWorld).y;
	m_fAnimRot = 0.0f;

	// 能力値に応じて当たり判定を拡大
	m_pBBox->InitBBox(WeaponConstruct::SHOTAXE_HITBOX_MIN, WeaponConstruct::SHOTAXE_HITBOX_MAX + VECTOR3(0.0f, 0.0f, m_pGMain->m_pPcProc->GetAbility()->GetAbilityLevel(5)));

	m_vPosUp = VECTOR3(0, 0, WeaponConstruct::SHOTAXE_SPEED + WeaponConstruct::SHOTAXE_SPEED * (0.3 * m_pGMain->m_pPcProc->GetAbility()->GetAbilityLevel(3)));
	m_vRotUp = VECTOR3(0.0f, WeaponConstruct::SHOTAXE_ROTSPEED, 0.0f);
	m_dwOwner = dwOwner;
	m_pEffect = m_pGMain->m_pEffectProc->m_pBillAxeShotProc->Start(m_mWorld);
	m_bBackFlag = FALSE;
	m_dwStatus = NORMAL;
	m_nTag = WEAPON;
	return TRUE;
}

// ---------------------------------------------------------------------------
//
// 武器・斧投げオブジェクトの更新
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponAxeShotObj::Update()
{
	if (m_bActive)
	{
		m_mWorldOld = m_mWorld;
		MATRIX4X4 mTran;
		VECTOR3 vPos = GetPositionVector(m_mWorld);
		m_mWorld = XMMatrixRotationY(XMConvertToRadians(m_fMoveRot)) * XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
		if (!m_bBackFlag)
		{
			mTran = XMMatrixTranslation(m_vPosUp.x, m_vPosUp.y, m_vPosUp.z);
			m_fLength = m_fLength - m_vPosUp.z;
			if (m_fLength <= 0) {
				m_bBackFlag = TRUE;
			}
			m_mWorld = mTran * m_mWorld;
		}
		else {
			// 手元に戻ってくる
			m_vPosUp.z = WeaponConstruct::SHOTAXE_SPEED;
			VECTOR3 m_vTarget = GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());
			if (!TargetMove(m_vTarget, m_vPosUp.z, 180.0f))	// 目的地（Target）に達したかどうか
			{
				MATRIX4X4 mTemp = XMMatrixRotationY(XMConvertToRadians(m_vRotUp.y));					// 目的地（Target）へ向かう角度
				m_mWorld = mTemp * m_mWorld;
				mTemp = XMMatrixTranslation(m_vPosUp.x, m_vPosUp.y + m_fJumpY, m_vPosUp.z);	// 目的地（Target）へ向かう
				m_mWorld = mTemp * m_mWorld;
			}
			else {
				// 戻った場合
				m_pEffect->SetActive(FALSE);
				m_bActive = FALSE;
				m_pGMain->m_pPcProc->GetPcObjPtr()->SetIsHaveAxe(TRUE);
			}
		}
		m_pBBox->m_mWorld = m_mWorld;
		m_pEffect->SetWorld(m_mWorld);

		// マップとのあたり判定
		VECTOR3 vHit, vNormal;
		if (m_pGMain->m_pMapProc->Hitcheck(GetPositionVector(m_mWorld), GetPositionVector(m_mWorldOld), &vHit, &vNormal))
		{
			m_bBackFlag = TRUE;
		}

		// 敵やPCとの当たり判定
		if ((m_dwOwner != ENM && (m_pGMain->m_pMapProc->GetEnmSpPart()->HitcheckSpPart(this, GetPositionVector(m_pBBox->m_mWorld))))
			|| (m_dwOwner != PC && (m_pGMain->m_pMapProc->GetPcSpPart()->HitcheckSpPart(this, GetPositionVector(m_pBBox->m_mWorld)))))
		{
			// ヒットしたとき
			m_dwStatus = NORMAL;
		}

		// 回転アニメーション
		m_mWorld = XMMatrixRotationX(XMConvertToRadians(90.0f)) * m_mWorld;
		m_mWorld = XMMatrixRotationZ(XMConvertToRadians(m_fAnimRot)) * m_mWorld;
		m_fAnimRot += 60.0f;
		if (360.0f <= m_fAnimRot)
		{
			m_fAnimRot = 0.0f;
		}

		// レンダリング
		Render();
	}
}

// ---------------------------------------------------------------------------
//
// 武器・斧投げオブジェクトのレンダリング
//
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponAxeShotObj::Render()
{
	GetMesh()->Render(m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, VECTOR3(0, 0, 0), m_pGMain->m_vEyePt);
}

// ========================================================================================
// ダメージエリアプロシージャクラス
// ========================================================================================
// ---------------------------------------------------------------------------
// 武器・ダメージエリアプロシージャのコンストラクタ
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
// ---------------------------------------------------------------------------
CWeaponDamageAreaProc::CWeaponDamageAreaProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_dwDamageTime = 60;
	m_dwKind = 0;
	m_fAlfa = 0.0f;
	m_bIsInObject = FALSE;
	for (int i = 0; i < DAMAGEAREA_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponDamageAreaObj(this));	// m_pObjArrayに武器・ダメージエリアオブジェクトを生成する
	}
}

// ---------------------------------------------------------------------------
// 武器・ダメージエリアプロシージャのデストラクタ
// ---------------------------------------------------------------------------
CWeaponDamageAreaProc::~CWeaponDamageAreaProc()
{
}

// ---------------------------------------------------------------------------
// 武器・ダメージエリアプロシージャの発射
//
//  VECTOR3	   min          ：Boxコリジョンの最小位置
//  VECTOR3    max          ：Boxコリジョンの最大位置
//  DWORD      kind         ：ダメージエリアの種類(POISION, RAVAなど)
// ---------------------------------------------------------------------------
BOOL CWeaponDamageAreaProc::Start(VECTOR3 min, VECTOR3 max, DWORD Kind)
{
	m_dwKind = Kind;
	for (int i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(min, max, Kind);	       // 武器・ダメージエリアオブジェクトの開始
			break;
		}
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// 武器・ダメージエリアプロシージャの更新処理
// ---------------------------------------------------------------------------
void CWeaponDamageAreaProc::Update()
{
	if (m_bIsInObject)
	{
		m_dwDamageTime--;
		if (m_dwDamageTime <= 0) {
			m_pGMain->m_pAllyProc->DeleteObj(1);
			m_dwDamageTime = 60;
		}

		if (m_fAlfa < 0.5f)
		{
			m_fAlfa += 0.008f;
		}
	}
	else {
		if (m_fAlfa <= 0.0f)
		{
			m_fAlfa = 0.0f;
		}
		else {
			m_fAlfa -= 0.016f;
		}
	}

	if (m_fAlfa >= 0.0f)
	{
		// 画面に色を重ねる
		if (m_dwKind == POISON)
		{
			// 紫
			m_pGMain->m_pBackForeProc->GetSprite()->DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(218, 66, 220), m_fAlfa);
		}
		else if (m_dwKind == LAVA)
		{
			m_pGMain->m_pBackForeProc->GetSprite()->DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(255, 0, 0), m_fAlfa);
		}
	}
	m_bIsInObject = FALSE;
}

// ========================================================================================
// アクセス関数
// ========================================================================================
void CWeaponDamageAreaProc::SetIsInObject(BOOL flag)
{
	m_bIsInObject = flag;
}

// ========================================================================================
// ダメージエリアオブジェクトクラス
// ========================================================================================
// ---------------------------------------------------------------------------
// 武器・ダメージエリアオブジェクトのコンストラクタ
// ---------------------------------------------------------------------------
CWeaponDamageAreaObj::CWeaponDamageAreaObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nAtc = 0;  // 攻撃力
	m_nAtc = nAtc;
	m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-1, -1, -1), VECTOR3(1, 1, 1));
}

// ---------------------------------------------------------------------------
// 武器・ダメージエリアオブジェクトのデストラクタ
// ---------------------------------------------------------------------------
CWeaponDamageAreaObj::~CWeaponDamageAreaObj()
{
}

// ---------------------------------------------------------------------------
// 武器・ダメージエリアオブジェクトの発射
//
//     MATRIX4X4 mStartWorld     スプレーの発射位置のマトリックス
//     DWORD      dwOwner        武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
BOOL CWeaponDamageAreaObj::Start(VECTOR3 vStart, VECTOR3 vTarget, DWORD dwOwner)
{
	m_bActive = TRUE;
	m_pBBox->InitBBox(vStart, vTarget);
	m_dwStatus = NORMAL;
	return TRUE;
}

// ---------------------------------------------------------------------------
//
// 武器・ダメージエリアオブジェクトの更新
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponDamageAreaObj::Update()
{
	if (m_bActive)
	{
		m_dwStatus = NORMAL;
		VECTOR3 vHit = VECTOR3(0.0f, 0.0f, 0.0f), vNrm = VECTOR3(0.0f, 1.0f, 0.0f);
		if (m_pBBox->OBBCollisionDetection(m_pGMain->m_pPcProc->GetPcObjPtr()->GetBBox(), &vHit, &vNrm) &&
			m_pGMain->m_pPcProc->GetPcObjPtr()->GetStatus() == NORMAL)
		{
			m_pGMain->m_pWeaponProc->m_pWeaponDamageAreaProc->SetIsInObject(TRUE);

		}
	}
}

// ========================================================================================
// 攻撃範囲プロシージャクラス
// ========================================================================================
// ---------------------------------------------------------------------------
// 武器・攻撃範囲プロシージャのコンストラクタ
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
// ---------------------------------------------------------------------------
CWeaponAttackBoxProc::CWeaponAttackBoxProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (int i = 0; i < ATTACKBOX_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponAttackBoxObj(this));	// m_pObjArrayに武器・攻撃範囲オブジェクトを生成する
	}
}

// ---------------------------------------------------------------------------
// 武器・攻撃範囲プロシージャのデストラクタ
// ---------------------------------------------------------------------------
CWeaponAttackBoxProc::~CWeaponAttackBoxProc()
{
}

// ---------------------------------------------------------------------------
// 武器・攻撃範囲プロシージャの発射
//
//  VECTOR3	   min          ：Boxコリジョンの最小位置
//  VECTOR3    max          ：Boxコリジョンの最大位置
//  DWORD      kind         ：ダメージエリアの種類(POISION, RAVAなど)
// ---------------------------------------------------------------------------
BOOL CWeaponAttackBoxProc::Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner)
{
	for (int i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(mMat, vMin, vMax, dwOwner);	       // 武器・攻撃範囲オブジェクトの開始
			break;
		}
	}
	return FALSE;
}

// ========================================================================================
// 攻撃範囲オブジェクトクラス
// ========================================================================================
// ---------------------------------------------------------------------------
// 武器・攻撃範囲オブジェクトのコンストラクタ
// ---------------------------------------------------------------------------
CWeaponAttackBoxObj::CWeaponAttackBoxObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nAtc = 100;  // 攻撃力
	m_nAtc = nAtc;
	m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-1, -1, -1), VECTOR3(1, 1, 1));
}

// ---------------------------------------------------------------------------
// 武器・攻撃範囲オブジェクトのデストラクタ
// ---------------------------------------------------------------------------
CWeaponAttackBoxObj::~CWeaponAttackBoxObj()
{
}

// ---------------------------------------------------------------------------
// 武器・攻撃範囲オブジェクトの発射
//
//     MATRIX4X4 mStartWorld     スプレーの発射位置のマトリックス
//     DWORD      dwOwner        武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
BOOL CWeaponAttackBoxObj::Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner)
{
	m_bActive = TRUE;
	m_pBBox->m_mWorld = mMat;
	m_pBBox->InitBBox(vMin, vMax);
	m_dwStatus = NORMAL;
	m_pGMain->m_pPcProc->Hitcheck(this, m_pBBox);
	m_pGMain->m_pAllyProc->Hitcheck(this, m_pBBox);

	Render();
	m_bActive = FALSE;
	return TRUE;
}

// ---------------------------------------------------------------------------
//
// 武器・攻撃範囲オブジェクトのレンダリング
//
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponAttackBoxObj::Render()
{
	// m_pBBox->Render(m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
}


// ============================================================================================================================
//============================================================================
//
// 武器・手に持つアイテムプロシージャのコンストラクタ
//
// ・直接攻撃する武器ではなく、武器を表示したり、位置情報を返したりする
//
//   メッシュはプロシージャではなく、オブジェクトで読み込む
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
//
// ---------------------------------------------------------------------------
CWeaponHoldProc::CWeaponHoldProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (int i = 0; i < HOLDITEM_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponHoldObj(this));	// m_pObjArrayにオブジェクトを生成する
		((CWeaponHoldObj*)m_pObjArray.back())->Init();   // オブジェクトの初期化
	}
}
// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムプロシージャのデストラクタ
//
// ---------------------------------------------------------------------------
CWeaponHoldProc::~CWeaponHoldProc()
{
	;
}

// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムプロシージャの情報を得る
//
//     MATRIX4X4  mHold       ：剣の位置
//     BASEHOLDITEM HoldItem  :手に持つ武器の情報
//     MATRIX4X4& mMat        :手に持つ武器のマトリックス(Out)
//     VECTOR3& vMin          :バウンディングボックスのMin(Out)
//     VECTOR3& vMax          :バウンディングボックスのMax(Out)
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
void  CWeaponHoldProc::GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vMin, VECTOR3& vMax)
{
	((CWeaponHoldObj*)m_pObjArray[HoldItem.m_nHoldObjNo])->GetInfo(mHold, HoldItem, mMat, vMin, vMax);	// 武器・剣オブジェクト

	return;
}
// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムプロシージャの情報を得る
//
//     MATRIX4X4  mHold       ：剣の位置
//     BASEHOLDITEM HoldItem  :手に持つ武器の情報
//     MATRIX4X4& mMat        :手に持つ武器のマトリックス(Out)
//     VECTOR3& vOffset       :銃口までのオフセット(Out)
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
void  CWeaponHoldProc::GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vOffset)
{
	((CWeaponHoldObj*)m_pObjArray[HoldItem.m_nHoldObjNo])->GetInfo(mHold, HoldItem, mMat, vOffset);	// 武器・銃オブジェクト

	return;
}

// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムプロシージャのバウンディングボックスを得る
//
//     MATRIX4X4  mHold       ：剣の位置
//     BASEHOLDITEM HoldItem  :手に持つ武器の情報
//
//     戻り値　　CBBox* バウンディングボックス
// ---------------------------------------------------------------------------
CBBox*  CWeaponHoldProc::GetBBox(MATRIX4X4 mHold, BASEHOLDITEM HoldItem)
{
	return ((CWeaponHoldObj*)m_pObjArray[HoldItem.m_nHoldObjNo])->GetBBox(mHold, HoldItem);	// 武器・剣オブジェクト
}

// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムプロシージャの表示
//
//     MATRIX4X4  mHold       ：剣の位置
//     BASEHOLDITEM HoldItem  :手に持つ武器の情報
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
BOOL CWeaponHoldProc::Disp(MATRIX4X4 mHold, BASEHOLDITEM HoldItem)
{

	((CWeaponHoldObj*)m_pObjArray[HoldItem.m_nHoldObjNo])->Disp(mHold, HoldItem);	// 武器・剣オブジェクト

	return TRUE;
}
//============================================================================
//
// 武器・手に持つアイテムオブジェクトのコンストラクタ
//
// ・直接攻撃する武器ではなく、武器を表示したり、位置情報を返したりする
//
// ---------------------------------------------------------------------------
CWeaponHoldObj::CWeaponHoldObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	m_nKindNo = 0;
	m_pMesh = NULL;
	m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // オフセットの設定
}
//============================================================================
//
// 武器・手に持つアイテムオブジェクトの初期化
//
// ・メッシュは、プロシージャではなくオブジェクトの中に設定する
//
// ---------------------------------------------------------------------------
void CWeaponHoldObj::Init()
{

	switch (m_dwObjNo)
	{
	case 0:     // 未設定
		m_nKindNo = 0;
		m_pMesh = NULL; // オブジェクトは無し
		m_pBBox = NULL; // BBoxの設定
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // オフセットなし
		break;

	case 1:     // 透明な剣の設定
		m_nKindNo = 1;  // 1:種別　剣
		m_pMesh = NULL; // オブジェクトは無し
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.15f, -0.15f, -0.15f), VECTOR3(0.15f, 1.8f, 0.15f));// BBoxの設定
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // 銃口までのオフセットの設定
		break;

	case 2:     // 剣（日本刀）メッシュの設定
		m_nKindNo = 1;  // 1:種別　剣
		m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Sword.mesh"));
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.15f, 0.1f, -0.15f), VECTOR3(0.15f, 1.8f, 0.15f));// BBoxの設定
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // 銃口までのオフセットの設定
		break;

	case 3:     // 剣（ソード）メッシュの設定
		m_nKindNo = 1;  // 1:種別　剣
		m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Sword2.mesh"));
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.15f, 0.1f, -0.15f), VECTOR3(0.15f, 1.2f, 0.15f));// BBoxの設定
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // 銃口までのオフセットの設定
		break;

	case 4:     // 透明な銃の設定
		m_nKindNo = 2;  // 2:種別　銃
		m_pMesh = NULL; // オブジェクトは無し
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.51f);   // 銃口までのオフセットの設定
		break;

	case 5:     // 銃（ライフル）メッシュの設定
		m_nKindNo = 2;  // 2:種別　銃
		m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Gun.mesh"));
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.51f);   // 銃口までのオフセットの設定
		break;

	case 6:     // 銃（ピストル）メッシュの設定
		m_nKindNo = 2;  // 2:種別　銃
		m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Pistol.mesh"));
		m_vOffset = VECTOR3(0.0f, 0.07f, 0.17f);   // 銃口までのオフセットの設定
		break;

	case 7:     // 透明な盾の設定
		m_nKindNo = 1;  // 1:種別　剣
		m_pMesh = NULL; // オブジェクトは無し
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-1.0f, -1.5f, -0.3f), VECTOR3(1.0f, 1.5f, 0.3f));// BBoxの設定
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // 銃口までのオフセットの設定
		break;

	case 8:		// 弓矢
		m_nKindNo = 2;  // 1:種別　剣
		m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Arrow/Arrow.mesh"));
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.15f, 0.1f, -0.15f), VECTOR3(0.15f, 1.0f, 0.15f));// BBoxの設定
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // 銃口までのオフセットの設定
		break;

	case 9:		// 剣
		m_nKindNo = 1;
		m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Sword/Sword.mesh"));
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.17f, 0.0f, -0.09f), VECTOR3(0.17f, 1.1f, 0.09f));// BBoxの設定
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // 銃口までのオフセットの設定
		break;

	case 10:	// 銃
		m_nKindNo = 2;
		m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Gun/Gun.mesh"));
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.15f, 0.1f, -0.15f), VECTOR3(0.15f, 1.0f, 0.15f));// BBoxの設定
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // 銃口までのオフセットの設定
		break;

	case 11:	// 斧
		m_nKindNo = 1;
		m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Axe/Axe.mesh"));
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.15f, 0.1f, -0.15f), VECTOR3(0.15f, 1.0f, 0.15f));// BBoxの設定
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // 銃口までのオフセットの設定
		break;

	case 12:
		m_nKindNo = 0;
		m_pMesh = NULL; // オブジェクトは無し
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.9f, -0.9f, -0.9f), VECTOR3(0.9f, 2.0f, 0.9f));// BBoxの設定
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // 銃口までのオフセットの設定
		break;

	case 13:
		m_nKindNo = 0;
		m_pMesh = NULL; // オブジェクトは無し
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-1.0f, -0.8f, -1.0f), VECTOR3(1.0f, 2.0f, 1.0f));// BBoxの設定
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // 銃口までのオフセットの設定
		break;

	case 14:
		m_nKindNo = 0;
		m_pMesh = NULL; // オブジェクトは無し
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-3.0f, -4.0f, -1.0f), VECTOR3(3.0f, 9.0f, 1.0f));// BBoxの設定
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // 銃口までのオフセットの設定
		break;

	default:
		m_nKindNo = 0;
		m_pMesh = NULL; // オブジェクトは無し
		m_pBBox = NULL; // BBoxの設定
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // オフセットなし
		break;
	}

	m_bActive = FALSE;
	m_dwStatus = NORMAL;

}
// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムオブジェクトのデストラクタ
//
// ---------------------------------------------------------------------------
CWeaponHoldObj::~CWeaponHoldObj()
{
	SAFE_DELETE(m_pMesh);
}

// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムオブジェクトの情報を得る
//
//     MATRIX4X4  mHold       ：剣の位置
//     BASEHOLDITEM HoldItem  :手に持つ武器の情報
//     MATRIX4X4& mMat        :手に持つ武器のマトリックス(Out)
//     VECTOR3& vMin          :バウンディングボックスのMin(Out)
//     VECTOR3& vMax          :バウンディングボックスのMax(Out)
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
void  CWeaponHoldObj::GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vMin, VECTOR3& vMax)
{
	// オフセットマトリックスの計算
	MATRIX4X4 mPos = XMMatrixTranslation(HoldItem.m_nOffsetPos.x, HoldItem.m_nOffsetPos.y, HoldItem.m_nOffsetPos.z);

	MATRIX4X4 mRotX = XMMatrixRotationX(XMConvertToRadians(HoldItem.m_nOffsetRot.x));
	MATRIX4X4 mRotY = XMMatrixRotationY(XMConvertToRadians(HoldItem.m_nOffsetRot.y));
	MATRIX4X4 mRotZ = XMMatrixRotationZ(XMConvertToRadians(HoldItem.m_nOffsetRot.z));

	MATRIX4X4 mOffset = mRotY * mRotX * mRotZ;
	mOffset = mOffset * mPos;

	// 
	// ワールドマトリックスの計算
	// 
	// 但し、このときm_mWorldやm_pBBox->m_mWorldの設定は行っていない!!
	// 
	mMat = mOffset * mHold;     // 剣の位置のワールドマトリックス

	vMin = m_pBBox->m_vMin;
	vMax = m_pBBox->m_vMax;

	return;
}

// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムオブジェクトの情報を得る
//
//     MATRIX4X4  mHold       ：銃の位置
//     BASEHOLDITEM HoldItem  :手に持つ武器の情報
//     MATRIX4X4& mMat        :手に持つ武器のマトリックス(Out)
//     VECTOR3& vOffset       :銃口までのオフセット(Out)
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
void  CWeaponHoldObj::GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vOffset)
{
	// オフセットマトリックスの計算
	MATRIX4X4 mPos = XMMatrixTranslation(HoldItem.m_nOffsetPos.x, HoldItem.m_nOffsetPos.y, HoldItem.m_nOffsetPos.z);

	MATRIX4X4 mRotX = XMMatrixRotationX(XMConvertToRadians(HoldItem.m_nOffsetRot.x));
	MATRIX4X4 mRotY = XMMatrixRotationY(XMConvertToRadians(HoldItem.m_nOffsetRot.y));
	MATRIX4X4 mRotZ = XMMatrixRotationZ(XMConvertToRadians(HoldItem.m_nOffsetRot.z));

	MATRIX4X4 mOffset = mRotY * mRotX * mRotZ;
	mOffset = mOffset * mPos;

	// ワールドマトリックスの計算
	// 
	// 但し、このときm_mWorldの設定は行っていない!!
	// 
	mMat = mOffset * mHold;     // 銃の位置のワールドマトリックス

	vOffset = m_vOffset;

	return;
}

// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムオブジェクトのバウンディングボックスを得る
//
//     MATRIX4X4  mHold       ：剣の位置
//     BASEHOLDITEM HoldItem  :手に持つ武器の情報
//
//     戻り値　　CBBox* バウンディングボックス
// ---------------------------------------------------------------------------
CBBox* CWeaponHoldObj::GetBBox(MATRIX4X4 mHold, BASEHOLDITEM HoldItem)
{

	// オフセットマトリックスの計算
	MATRIX4X4 mPos = XMMatrixTranslation(HoldItem.m_nOffsetPos.x, HoldItem.m_nOffsetPos.y, HoldItem.m_nOffsetPos.z);

	MATRIX4X4 mRotX = XMMatrixRotationX(XMConvertToRadians(HoldItem.m_nOffsetRot.x));
	MATRIX4X4 mRotY = XMMatrixRotationY(XMConvertToRadians(HoldItem.m_nOffsetRot.y));
	MATRIX4X4 mRotZ = XMMatrixRotationZ(XMConvertToRadians(HoldItem.m_nOffsetRot.z));

	MATRIX4X4 mOffset = mRotY * mRotX * mRotZ;
	mOffset = mOffset * mPos;

	// ワールドマトリックスの計算
	// 
	// 但し、このときm_mWorldとm_pBBox->m_mWorldの設定を行っている!!
	// 
	m_mWorld = mOffset * mHold;     // 剣の位置のワールドマトリックス
	m_pBBox->m_mWorld = m_mWorld;   // バウンディングボックスのワールドマトリックスをセット

	return m_pBBox;

}


// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムオブジェクトの描画
//
//     MATRIX4X4  mHold        ：アイテムの位置
//     VECTOR3    vPos         :位置のオフセット
//     VECTOR3    vRot         :角度のオフセット
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
BOOL CWeaponHoldObj::Disp(MATRIX4X4 mHold, BASEHOLDITEM HoldItem)
{
	m_bActive = TRUE;  // スタート時に表示開始

	// オフセットマトリックスの計算
	MATRIX4X4 mPos = XMMatrixTranslation(HoldItem.m_nOffsetPos.x, HoldItem.m_nOffsetPos.y, HoldItem.m_nOffsetPos.z);

	MATRIX4X4 mRotX = XMMatrixRotationX(XMConvertToRadians(HoldItem.m_nOffsetRot.x));
	MATRIX4X4 mRotY = XMMatrixRotationY(XMConvertToRadians(HoldItem.m_nOffsetRot.y));
	MATRIX4X4 mRotZ = XMMatrixRotationZ(XMConvertToRadians(HoldItem.m_nOffsetRot.z));

	MATRIX4X4 mOffset = mRotY * mRotX * mRotZ;
	mOffset = mOffset * mPos;

	// ワールドマトリックスの計算
	m_mWorld = mOffset * mHold;     // 剣の位置のワールドマトリックス
	if (m_pBBox) m_pBBox->m_mWorld = m_mWorld;   // バウンディングボックスのワールドマトリックスをセット

	// レンダリング
	Render();

	m_bActive = FALSE;  // １回処理したら、消す

	return TRUE;
}

// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムオブジェクトの更新
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponHoldObj::Update()
{
	;
}
// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムオブジェクトの描画
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponHoldObj::Render()
{

	// 手に持つアイテムのレンダリング
	if (m_pMesh) m_pMesh->RenderToon(m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, normalize(VECTOR3(0.8f, 1, -1)), m_pGMain->m_vEyePt);

	// バウンディングボックスレンダリング
	//if (m_pBBox) m_pBBox->Render(m_pGMain->m_mView, m_pGMain->m_mProj, VECTOR3(0, 0, 0), m_pGMain->m_vEyePt);
}
