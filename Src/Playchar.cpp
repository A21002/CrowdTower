//=============================================================================
//		vCLN^[Μ§δ
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
// PCθNX
//============================================================================
// ϊ»
const float PcConstruct::SPEED = 0.08f;
const float PcConstruct::ROT_SPEED = 8.0f;
const float PcConstruct::WAITTIME_GUN = 0.2f;
const VECTOR3 PcConstruct::HITBOX_MIN = VECTOR3(-0.3f, -0.05f, -0.3f);
const VECTOR3 PcConstruct::HITBOX_MAX = VECTOR3(0.3f, 2.0f, 0.3f);

//============================================================================
// PCvV[WNX
//============================================================================
// ---------------------------------------------------------------------------
// obvV[WΜRXgN^
//
//   Θ¨AIuWFNgΜdeleteΝCBaseProcΜfXgN^Εs€½ίsv
// ---------------------------------------------------------------------------
CPcProc::CPcProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// LN^[@||||||||||||||||||||||||||||||
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Char/PC/PC_Skin.mesh"));    // LN^[
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
	m_HoldItem.m_nHoldObjNo = 0;	// θΙΒνΜIuWFΤ
	m_HoldItem.m_nPosMesh = 0;		// ͺ³ΚuΜbVΤ
	m_HoldItem.m_nPosBone = 0;	// ͺ³ΚuΜ{[Τ
	m_HoldItem.m_nOffsetPos = VECTOR3(0.0f, 0.0f, 0.0f);  // θΙΒνΜΚuItZbg
	m_HoldItem.m_nOffsetRot = VECTOR3(0.0f, 0.0f, 0.0f);  // θΙΒνΜpxItZbg
	
	// -------------------------------------------------------------------------
	m_pObjArray.push_back(new CPcObj(this));	// m_pObjArrayΙIuWFNgπo^·ι

	// ------------------------------------------------------------------------- 
	m_pAbility = new CAbility(pGMain);
}

// ---------------------------------------------------------------------------
// obvV[WΜfXgN^
// ---------------------------------------------------------------------------
CPcProc::~CPcProc()
{
	delete(m_pAbility);
}

//============================================================================
// ANZXΦ
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
// PCIuWFNgNX
//============================================================================
// ---------------------------------------------------------------------------
// obIuWFNgΜRXgN^
//
//  ψ
//  CBaseProc* pProc		F evV[WΦΜ|C^
// ---------------------------------------------------------------------------
CPcObj::CPcObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	m_fLocalRotY = 0.0f;               

	m_fSpeed = PcConstruct::SPEED;						// Ϊ?¬x      
	m_fRotSpeed = PcConstruct::ROT_SPEED;				// ρ]¬x      
	m_nAllyNum = PcConstruct::START_ALLY_NUM;			// ϊΜ‘ϋΜ

	// oEfBO{bNXΜέθ
	m_pBBox = new CBBox(m_pGMain->m_pShader, PcConstruct::HITBOX_MIN, PcConstruct::HITBOX_MAX);
	m_pBBox->m_mWorld = m_mWorld;                          

	m_nMaxHp = m_nAllyNum + 1;
	m_nAtc = 0;
	m_nTag = PC;

	// obIuWFNgΜϊ»
	m_bActive = TRUE;
	ResetStatus();

	// Aj[VΜϊ»
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
// obIuWFNgΜfXgN^
// ---------------------------------------------------------------------------
CPcObj::~CPcObj() 
{
}

// ---------------------------------------------------------------------------
// MapJΪγAJΪOΜAllyOBjΜΎ―ΔΆ¬
// ---------------------------------------------------------------------------
void CPcObj::SetAllyObj()
{
	for (int i = 0; i < m_nAllyNum; i++) {
		m_pGMain->m_pAllyProc->Start(GetPositionVector(m_mWorld), TRUE);
	}
}

// ---------------------------------------------------------------------------
// gCγAOρΜΕIKwπQΖ΅ΔAllyNumπέθ
// ---------------------------------------------------------------------------
void CPcObj::SetRetryAllyNum()
{
	int hierarchy = m_pGMain->m_pMapProc->GetHierarchy();
	m_nAllyNum = PcConstruct::START_ALLY_NUM + hierarchy / 4;
	m_pGMain->m_pBackForeProc->AddAllAllyNum(m_nAllyNum);
}

//-----------------------------------------------------------------------------
// obIuWFNgΜXV
//
//@L[{[hΕobπ?©·
//@obΜAj[VAWvA©RΊ
//@[hΐWΤ?A}bvΖΜΪG»θ
//@obΜUπs€
//
//   ψ@@@Θ΅
//-----------------------------------------------------------------------------
void	CPcObj::Update()
{
	// κΒOΜΚuπΫΆ
	m_mWorldOld = m_mWorld;

	// Uνήπϊ»
	m_nAttackKind = ATTACK_NONE;

	// AllyNumΖHPπ―ϊ·ι
	m_nHp = m_nAllyNum + 1;
	// HPΘ΅ASAj[VΙΪs΅Δ’ι©
	if (m_nHp <= 0 && m_AnimStatus.animNum != PcConstruct::ANIM_DEAD) {
		m_dwStatus = DEAD;
		m_AnimStatus.SetNum(PcConstruct::ANIM_DEAD);
		m_nCnt1 = 60 * PcConstruct::WAITTIME_DEAD;
	}

	// CͺΪ?πΒ΅Δ’ισΤΜέ
	if (m_pGMain->m_isMove) {
		// eσΤΙΆ½
		switch (m_dwStatus) {
		case  FLASH:  // obͺ_[W©ηA΅½Ζ«Μ
		case  NORMAL: // obͺΚνσΤΜΖ«Μ
			UpdateNormal();  // obIuWFNgΜXV ΚνσΤiNORMALj
			break;

		case  DAMAGE: // obͺ_[WσΤΜΖ«Μ
			UpdateDamage();  // obIuWFNgΜXV _[WσΤiDAMAGEj
			break;

		case  DEAD:	// obͺSσΤΜΖ«Μ
			UpdateDead();  // obIuWFNgΜXV SσΤiDEADj
			break;

		}
		// {XΖΜdΘθ
		SetKeepOffset(m_pGMain->m_pEnmBossProc->GetEnmBossObjPtr());

		// }bvRWΖ©RΊ
		m_fJumpY += (0.5f * GRAVITY * 0.00005f) * m_fJumpTime;	// ©RΊ
		m_fJumpTime += 1.0f;
		if (m_pGMain->m_pMapProc->isCollisionMoveGravityPc(&m_mWorld, m_mWorldOld) != 3)  // }bvRW
		{
			m_fJumpY = 0.0f;  // ©RΊΜβ~
			m_fJumpTime = 0.0f;
		}

		// IuWFNgπσΤͺΦo^ 
		// SetObj()ͺAIuWFNgπσΤͺΦo^·ιΦΕ ι
		VECTOR3 vMin, vMax;
		m_pGMain->m_pMapProc->GetPcSpPart()->MakeAABB(
			GetPositionVector(m_mWorldOld), GetPositionVector(m_mWorld), 2.0f, vMin, vMax);
		m_pGMain->m_pMapProc->GetPcSpPart()->SetObj(vMin, vMax, this);
	}
	else {
		// Ϊ?sΒΝAIdle[VΕ?@
		m_AnimStatus.SetNum(PcConstruct::ANIM_IDLE);
		m_bIsKeyPush = FALSE;
	}
	
	// JΖPCΜΚuΕCπςΞ΅AdΘΑ½}bvbVπ§ί·ι
	m_pGMain->m_pMapProc->CheckCrossStageMesh(GetPositionVector(m_mWorld), GetPositionVector(m_pGMain->m_pCameraProc->GetCameraObjPtr()->GetWorld()));

	//_O
	GetMesh()->Render(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
	// oEfBO{bNX
	m_pBBox->m_mWorld = m_mWorld;

	// θΙΒνβACeΜ\¦π·ι
	if (GetHoldItem().m_nHoldObjNo != 0) // θΙΒνβACeͺ ιΖ«
	{
		MATRIX4X4 mHold;
		mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, GetHoldItem().m_nPosBone, GetHoldItem().m_nPosMesh);  // θΜΚuΜ{[}gbNX
		m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->Disp(mHold, GetHoldItem()); // ACe\¦
	}
}

//-----------------------------------------------------------------------------
// obIuWFNgΜXV ΚνσΤiNORMALj
//
//@L[{[hΕobπ?©·
//@obΜAj[VAWvA©RΊ
//@[hΐWΤ?A}bvΖΜΪG»θ
//@obΜUπs€
//
//
//   ψ@Θ΅
//-----------------------------------------------------------------------------
void	CPcObj::UpdateNormal()
{
	if (m_dwStatus == FLASH) // obͺ_[W©ηA΅½Ζ«Μ
	{
		m_nCnt1--;
		if (m_nCnt1 <= 0) m_dwStatus = NORMAL;
	}
	UpdateNormalMove();      // obIuWFNgΜΪ?
	UpdateNormalAttack();    // obIuWFNgΜU
}

//-----------------------------------------------------------------------------
// obIuWFNgΜΪ?
//
//@L[{[h^}EXΕobπ?©·
//@obΜAj[VAWvA©RΊ
//@[hΐWΤ?A}bvΖΜΪG»θ
//
//
//   ψ@Θ΅
//-----------------------------------------------------------------------------
void	CPcObj::UpdateNormalMove()
{
	CDirectInput* pDI = m_pGMain->m_pDI;

	MATRIX4X4 mYaw;  // obx²ρ]}gbNX
	MATRIX4X4 mPos;  // obΪ?}gbNX
	m_bIsKeyPush = FALSE;// L[π΅½©Η€©Μ`FbN
	m_bIsToLateKeyPush = FALSE;		// 2ΒΘγΜL[ͺ³κΔ’ι©

	// ©RΊΜϊl
	mPos = XMMatrixTranslation(0.0f, m_fJumpY, 0.0f);

	// ImGuiΜ\¦
	static bool bImgOpen = false;
	if (pDI->CheckKey(KD_TRG, DIK_J))
		bImgOpen = true;
	if (bImgOpen) ShowImgui(&bImgOpen);

	// obΜΪ?  ---------------------------------------------
	// Oi
	if ((pDI->CheckKey(KD_DAT, DIK_W) || pDI->CheckJoy(KD_DAT, DIJ_UP)) && m_bIsCanMove) //	ͺL[
	{
		m_bIsKeyPush = TRUE;
		mPos = UpdateNormalMoveKeystate(DIK_W);
	}
	// γή
	if ((pDI->CheckKey(KD_DAT, DIK_S) || pDI->CheckJoy(KD_DAT, DIJ_DOWN)) && m_bIsCanMove) //«L[
	{
		m_bIsKeyPush = TRUE;
		mPos = UpdateNormalMoveKeystate(DIK_S);
	}
	// EΪ?
	if ((pDI->CheckKey(KD_DAT, DIK_D) || pDI->CheckJoy(KD_DAT, DIJ_RIGHT)) && m_bIsCanMove)//¨L[
	{
		m_bIsKeyPush = TRUE;
		mPos = UpdateNormalMoveKeystate(DIK_D);
	}
	// ΆΪ?
	if ((pDI->CheckKey(KD_DAT, DIK_A) || pDI->CheckJoy(KD_DAT, DIJ_LEFT)) && m_bIsCanMove)//©L[
	{
		m_bIsKeyPush = TRUE;
		mPos = UpdateNormalMoveKeystate(DIK_A);
	}

	// ΪWpΦρ]
	UpdateMoveRot();

	if (!m_bIsKeyPush && m_AnimStatus.animNum < 2)  // L[π΅Δ’Θ’Ζ« && U[VΕΝΘ’
	{
		m_AnimStatus.SetNum(0);
	}

	// [J²πξΖ΅ΔAobπΪ?³Ήι[h}gbNXπέθ
	// // U[VΪ?πsνΘ’
	// y²ρ]ΜέπK
	if (m_bIsCanMove) {
		mYaw = XMMatrixRotationY(XMConvertToRadians(GetRotateVector(m_pGMain->m_mView).y - 90.0f));
		mYaw = XMMatrixRotationY(XMConvertToRadians(m_vRotUp.y)) * mYaw;
		m_mWorld = mPos * mYaw * GetLocalMatrix();
	}
}

//----------------------------------------------------------------------------- 
// obIuWFNgΜΪ?@L[μΜeνΪ?
//
//   ψ@DWORD Key L[νΚ@DirectInputΜL[R[h
//
//   ίθl  Ϊ?}gbNX
//-----------------------------------------------------------------------------
MATRIX4X4 CPcObj::UpdateNormalMoveKeystate(DWORD DIKey)
{
	MATRIX4X4 mPos;

	if (m_AnimStatus.animNum == 0) m_AnimStatus.SetNum(1);

	// isϋόΙρ]΅Δ©ηOi
		// [g{[Aj[Vπs€©Η€©[gAj[V^CvπmF·ι
	if (GetMesh()->GetRootAnimType(m_AnimStatus.animNum) == eRootAnimNone)
	{
		// [g{[Aj[VπsνΈΕθΜOiΪ?l
		mPos = XMMatrixTranslation(0.0f, m_fJumpY, m_fSpeed);
	}
	else {
		// [g{[Aj[VΕΜOiΪ?l
		mPos = XMMatrixTranslation(0.0f, m_fJumpY, 0.0f) * GetMesh()->GetRootAnimUpMatrices(m_AnimStatus);
	}

	// isϋόΙρ]·ι
	// Eρ]pxΝm_vRotUp.yΙ ιB-180`180x
	// EPρΜρ]¬xΝm_fRotSpeed
	if (m_vRotUp.y > 180.0f)  m_vRotUp.y -= 360.0f;
	if (m_vRotUp.y < -180.0f) m_vRotUp.y += 360.0f;

	if (!m_bIsToLateKeyPush)
	{
		switch (DIKey)
		{
		case DIK_W:    // Oi
			m_fAngle = 0.0f;
			break;
		case DIK_S:    // γή
			if (m_vRotUp.y >= 0)  // Ε¬ρ]ΙΘιζ€ΙϋόπνΉι
			{
				m_fAngle = 180.0f;
			}
			else {
				m_fAngle = -180.0f;
			}
			break;
		case DIK_D:    // E
			m_fAngle = 90.0f;
			if (m_vRotUp.y == -180.0f) m_vRotUp.y = 180.0f;  // Ε¬ρ]ΙΘιζ€ΙϋόπνΉι
			break;
		case DIK_A:    // Ά
			m_fAngle = -90.0f;
			if (m_vRotUp.y == 180.0f) m_vRotUp.y = -180.0f;  // Ε¬ρ]ΙΘιζ€ΙϋόπνΉι
			break;
		}
		m_bIsToLateKeyPush = TRUE;
	}
	else {
		switch (DIKey)
		{
		case DIK_S:    // γή
			// OiΖγήΜ―΅
			m_fAngle = 0.0f;
			mPos = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
			break;

		case DIK_D:    // E
			if (m_fAngle == 0.0f) {
				// OiΖE
				m_fAngle += 45.0f;
			}
			else {
				// γήΖE
				m_fAngle -= 45.0f;
			}
			break;

		case DIK_A:    // Ά
			if (m_fAngle == 0.0f) {
				// OiΖΆ
				m_fAngle -= 45.0f;
			}
			else {
				// γήΖE
				m_fAngle += 45.0f;
			}
			break;
		}
	}
	return mPos;
}

//-----------------------------------------------------------------------------
// ΪWρ]pΙό©ΑΔρ]·ι
//-----------------------------------------------------------------------------
void CPcObj::UpdateMoveRot()
{
	if (m_vRotUp.y > m_fAngle)  // Άρ]
	{
		m_vRotUp.y -= m_fRotSpeed;
		if (m_vRotUp.y < m_fAngle) m_vRotUp.y = m_fAngle;
	}
	if (m_vRotUp.y < m_fAngle)  // Eρ]
	{
		m_vRotUp.y += m_fRotSpeed;
		if (m_vRotUp.y > m_fAngle) m_vRotUp.y = m_fAngle;
	}
}

//-----------------------------------------------------------------------------
// obIuWFNgΜU
//
//@L[{[h^}EXΕobΜUπs€
//
//
//   ψ@Θ΅
//-----------------------------------------------------------------------------
void	CPcObj::UpdateNormalAttack()
{
	CDirectInput* pDI = m_pGMain->m_pDI;
	CBaseProc* pProc = m_pGMain->m_pPcProc;
	CAbility* ability = m_pGMain->m_pPcProc->GetAbility();

	// ------------------------------------------------------------- 
	// uν
	// »έΜ΅Δ’ινπQΖ
	switch (ability->GetLongWeapon())
	{
	case ARROW:
		if (pDI->CheckJoy(KD_DAT, DIJ_A) || pDI->CheckMouse(KD_TRG, DIM_LBUTTON) && m_AnimStatus.animNum != PcConstruct::ANIM_ARROW && m_bIsCanMove) // XL[
		{
			m_nAttackKind = ATTACK_ALLOW;
			// J[\ΜΚuπό­
			GetCursorRotate();
			// |πΒ
			pProc->SetHoldItem(ATTACK_ALLOW);
			// Aj[Vέθ
			m_AnimStatus.SetNum(PcConstruct::ANIM_ARROW);
			m_AnimStatus.animSpeed = ability->GetAbilityLevel(0) * 0.3f + 1;
			// Aj[VΪ?sΒ
			m_bIsCanMove = FALSE;
			m_pGMain->m_pArrow->Play();
		}
		break;

	case GUN:
		// NbNJnAeAj[VσΤΕΜ·΅
		if ((pDI->CheckJoy(KD_DAT, DIJ_A) || pDI->CheckMouse(KD_DAT, DIM_LBUTTON) && m_AnimStatus.animNum == PcConstruct::ANIM_GUN) ||
			pDI->CheckMouse(KD_TRG, DIM_LBUTTON) && m_bIsCanMove)
		{
			if (m_nCnt2 <= 0) {
				// [hAN[^CΕΝΘ’
				m_bIsCanMove = FALSE;
				if (m_nGunMagazine > 0) {
					// }KWce θ
					if (m_AnimStatus.animNum != PcConstruct::ANIM_GUN)
					{
						// Aj[VAΑΔ’ιACeΜέθ
						m_AnimStatus.SetNum(PcConstruct::ANIM_GUN);
						pProc->SetHoldItem(ATTACK_GUN);
					}
					m_nGunMagazine--;
					GetCursorRotate();
					m_pGMain->m_pGun->Play();
					m_nAttackKind = ATTACK_GUN;
					m_pGMain->m_pWeaponProc->m_pWeaponGunProc->Start(XMMatrixTranslation(0.0f, 0.7f, 0.5f) * m_mWorld, PC);
					if (m_nGunMagazine <= 0) {
						// [h
						m_nCnt2 = 60 * PcConstruct::WAITTIME_RELOAD - 20 * ability->GetAbilityLevel(1);
					}
					else {
						// ­ΛΤu
						m_nCnt2 = 60 * PcConstruct::WAITTIME_GUN;
					}
				}
			}
		}
		else {
			if (m_AnimStatus.animNum == PcConstruct::ANIM_GUN) {
				// eπΏ~ί½ΌγAAj[VΖνππ
				m_AnimStatus.SetNum(PcConstruct::ANIM_IDLE);
				pProc->ResetHoldItem();
				m_AnimStatus.animSpeed = 1;
				m_bIsCanMove = TRUE;
			}
		}

		if (pDI->CheckJoy(KD_DAT, DIJ_A) || pDI->CheckKey(KD_TRG, DIK_R) && m_bIsCanMove) // RL[
		{
			// [h
			m_nCnt2 = 60 * PcConstruct::WAITTIME_RELOAD - 20 * ability->GetAbilityLevel(1);
			m_nGunMagazine = 0;

		}

		// [h
		if (m_nGunMagazine <= 0 && m_nCnt2 <= 0) {
			m_nGunMagazine = PcConstruct::BASE_MAGAZINE_NUM + 5 * ability->GetAbilityLevel(7);
		}
		break;

	default:
		break;
	}

	if (m_AnimStatus.isEnd(PcConstruct::ANIM_ARROW))
	{
		// |ξAj[VIΉ
		m_pGMain->m_pWeaponProc->m_pWeaponArrowProc->Start(m_mWorld, PC); // |ξ­Λ
		m_AnimStatus.SetNum(0);
		pProc->ResetHoldItem();
		m_AnimStatus.animSpeed = 1;
		m_bIsCanMove = TRUE;
	}

	// ------------------------------------------------------------- 
	// ίΪU
	switch (ability->GetShortWeapon()) {
	case SWORD:
		if (pDI->CheckJoy(KD_TRG, DIJ_C) || pDI->CheckMouse(KD_TRG, DIM_RBUTTON) && m_AnimStatus.animNum != PcConstruct::ANIM_SWORD && m_nCoolTime <= 0 && m_bIsCanMove) // ZL[
		{
			m_nAttackKind = ATTACK_SWORD;
			// J[\Μϋόπό­
			GetCursorRotate();
			// πΒ
			pProc->SetHoldItem(ATTACK_SWORD);
			if (ability->GetAbilityLevel(4) > 0) {
				m_nAttackKind = ATTACK_SHOTSWORD;
				m_pGMain->m_pWeaponProc->m_pWeaponShotSlashProc->Start(m_mWorld, PC);	// a
			}
			m_pGMain->m_pEffectProc->m_pBillSlashProc->Start(m_mWorld);	// ΜGtFNg
			// Aj[Vέθ
			m_AnimStatus.SetNum(PcConstruct::ANIM_SWORD);
			m_AnimStatus.animSpeed = ability->GetAbilityLevel(3) * 0.3f + 1;
			// U[VΪ?sΒ
			m_bIsCanMove = FALSE;
			m_pGMain->m_pSlash->Play();
			m_nCoolTime = 60 * PcConstruct::WAITTIME_MELEE_WEAPON * (1 - 0.3 * ability->GetAbilityLevel(3));
		}
		break;

	case AXE:
		if ((pDI->CheckJoy(KD_TRG, DIJ_C) || pDI->CheckMouse(KD_TRG, DIM_RBUTTON)) && m_AnimStatus.animNum != PcConstruct::ANIM_AXE && m_nCoolTime <= 0 && m_bIsCanMove) // ZL[
		{
			m_nAttackKind = ATTACK_AXE;
			// J[\Μϋόπό­
			GetCursorRotate();
			if (ability->GetAbilityLevel(4) > 0) {
				// 
				if (m_bIsHaveAxe)
				{
					// άΎ°Δ’Θ’
					m_pGMain->m_pWeaponProc->m_pWeaponAxeShotProc->Start(m_mWorld* XMMatrixTranslation(0.0f, 1.0f, 1.0f), 5 + 2 * ability->GetAbilityLevel(4), PC);
					m_bIsHaveAxe = FALSE;
					m_nAttackKind = ATTACK_AXESHOT;
				}
			}
			else {
				// Κν
				m_pGMain->m_pEffectProc->m_pBillSlashProc->Start(m_mWorld);	// GtFNg
				pProc->SetHoldItem(ATTACK_AXE);
				m_nCoolTime = 60 * PcConstruct::WAITTIME_MELEE_WEAPON * (1 - ability->GetAbilityLevel(3) * 0.3);
			}
			// Aj[Vέθ
			m_AnimStatus.SetNum(PcConstruct::ANIM_AXE);
			m_AnimStatus.animSpeed = ability->GetAbilityLevel(3) * 0.3f + 1;
			// U[VΪ?sΒ
			m_bIsCanMove = FALSE;
			m_pGMain->m_pSlash->Play();
		}
		break;

	default:
		break;
	}

	// ίΪUΜ»θ
	if (m_AnimStatus.animNum == PcConstruct::ANIM_SWORD || m_AnimStatus.animNum == PcConstruct::ANIM_AXE)
	{
		MATRIX4X4 mSword, mHold;
		VECTOR3 vMin, vMax;
		if (GetHoldItem().m_nHoldObjNo != 0)
		{
			mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, GetHoldItem().m_nPosBone, GetHoldItem().m_nPosMesh);  // θΜΚuΜ{[}gbNX
			m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->GetInfo(mHold, GetHoldItem(), mSword, vMin, vMax); // {[}gbNXπwθ΅ΔθΙΒACeξρπΎι
			if (m_AnimStatus.animNum == PcConstruct::ANIM_AXE && ability->GetAbilityLevel(10) == 0) {
				// ’ϋΎΜΜU»θ
				m_pGMain->m_pWeaponProc->m_pWeaponAxeProc->Start(mSword, vMin, vMax, PC);
			}
			if (m_AnimStatus.animNum == PcConstruct::ANIM_SWORD) {
				// UΜ»θ
				m_pGMain->m_pWeaponProc->m_pWeaponSwordProc->Start(mSword, vMin, vMax, PC);  
			}
		}

		if (m_AnimStatus.isEnd(PcConstruct::ANIM_SWORD))
		{
			// UΜIΉ
			m_AnimStatus.SetNum(PcConstruct::ANIM_IDLE);
			m_AnimStatus.animSpeed = 1;
			pProc->ResetHoldItem();
			m_bIsCanMove = TRUE;
		}

		if (m_AnimStatus.isEnd(PcConstruct::ANIM_AXE))
		{
			// UΜIΉ
			m_AnimStatus.SetNum(PcConstruct::ANIM_IDLE);
			pProc->ResetHoldItem();
			m_AnimStatus.animSpeed = 1;
			m_bIsCanMove = TRUE;
		}
	}

	// JEg_E
	if (m_nCnt2 > 0) {
		m_nCnt2--;
	}

	if (m_nCoolTime > 0) {
		m_nCoolTime--;
	}
}

//-----------------------------------------------------------------------------
// obIuWFNgΜXV _[WσΤiDAMAGEj
//
//@_[Wπσ―½Ζ«Μ
//
//
//   ψ@Θ΅
//-----------------------------------------------------------------------------
void	CPcObj::UpdateDamage()
{
	MATRIX4X4 mTemp;

	mTemp = XMMatrixTranslation(0, 0, -0.25f);	// obN·ι
	m_mWorld = mTemp * m_mWorld;

	// Uπσ―½_[W
	if (m_pHitObj->GetAtc() > 0) {
		// AllyIuWFNgπν
		m_pGMain->m_pAllyProc->DeleteObj(1);
	}
	if (m_nHp <= 0)		// HPͺOΘΜΕSΦ
	{
		m_nHp = 0;
		m_dwStatus = DEAD;		// HPͺOΘΜΕSΦ
		m_AnimStatus.SetNum(PcConstruct::ANIM_DEAD);	// S[VΙ·ι
		m_nCnt1 = PcConstruct::WAITTIME_DEAD;			// SΤΜέθ
	}
	else {
		m_nCnt1 = PcConstruct::WAITTIME_FLASH;   // ³GσΤΜΤ
		m_dwStatus = FLASH;     // _[W©ηΜAπs€
	}
}
//----------------------------------------------------------------------------- 
// obIuWFNgΜXV SσΤiDEADj
//
//@SσΤΜΖ«Μ
//
//
//   ψ@Θ΅
//-----------------------------------------------------------------------------
void	CPcObj::UpdateDead()
{
	m_pGMain->m_pBackForeProc->GetSprite()->DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(255, 0, 0), 0.5f); // ζΚπΤ­·ι
	m_bIsCanMove = TRUE;
	m_pGMain->m_pPcProc->ResetHoldItem();
	m_pGMain->m_pAllyProc->ResetHoldItem();

	if (m_AnimStatus.isEnd(PcConstruct::ANIM_DEAD))  // S[VͺIνΑ½Ζ«
	{
		m_AnimStatus.playAnim = false;	// Aj[Vπ~ίι
		m_AnimStatus.animFrame = m_AnimStatus.endFrame - 1; // t[GhΜκΒθOΙ·ι
	}
	if (--m_nCnt1 <= 0) // SΤͺIνΑ½Ζ«
	{
		m_pGMain->m_pBackForeProc->SetBlackOut(TRUE);
		if (m_pGMain->m_pBackForeProc->GetWhiteOut()) {
			m_pGMain->m_pBackForeProc->AddDeadNum(1);
			m_pGMain->m_dwGameStatus = GAMEOVER;	// Q[I[o[Φ
			m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetCtrl(CAMERA_GAMEOVER);	// JΜ[hπQ[I[o[Ι
			m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetCamBase();				// JΜΚuπΟX
			m_pGMain->m_pBgm1->Stop();
			m_pGMain->m_pBossBgm->Stop();
			//m_pGMain->m_pGameOver->Play(AUDIO_LOOP);
		}
	}
}

//----------------------------------------------------------------------------- 
// Q[I[o[Μ
//
//   ψ@Θ΅
//-----------------------------------------------------------------------------
void CPcObj::UpdateGameOver()
{
	m_mWorld = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	m_mWorld = XMMatrixRotationY(XMConvertToRadians(90.0f));

	// ΊAj[VΙ·ι
	m_AnimStatus.SetNum(PcConstruct::ANIM_FALL);
	m_AnimStatus.playAnim = true;

	// VECTOR3ΙΟ·
	VECTOR3 pos = GetPositionVector(m_mWorld);
	// vC[ΜΚuπΊ°ι
	m_mWorld = XMMatrixTranslation(0.0f, -1.0f, 0.0f) * m_mWorld;
	m_mWorld = XMMatrixRotationX(XMConvertToRadians(20)) * m_mWorld;

	// z²Ι2iίι
	pos.z += 2;
	// Spriteπ`ζ
	m_nGameOverPosY += 5;
	if (m_nGameOverPosY >= WINDOW_HEIGHT)
	{
		m_nGameOverPosY = 0;
	}
	m_pGMain->m_pBackForeProc->GetSprite()->Draw3D(m_pGMain->m_pTitleProc->GetImageOver(), pos, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vEyePt, VECTOR2(WINDOW_WIDTH / 60, WINDOW_HEIGHT / 60), VECTOR2(0, m_nGameOverPosY), VECTOR2(WINDOW_WIDTH, WINDOW_HEIGHT));

	// PCπ`ζ
	GetMesh()->Render(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
}

//----------------------------------------------------------------------------- 
// Q[NAΜ
//
//   ψ@Θ΅
//-----------------------------------------------------------------------------
void CPcObj::UpdateGameClear()
{
	m_mWorld = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	m_mWorld = XMMatrixRotationY(XMConvertToRadians(180.0f));
	m_mWorld = XMMatrixRotationX(XMConvertToRadians(-45.0f)) * m_mWorld;

	// ΊAj[VΙ·ι
	m_AnimStatus.SetNum(PcConstruct::ANIM_VICTORY);
	m_AnimStatus.playAnim = true;

	// VECTOR3ΙΟ·
	VECTOR3 pos = GetPositionVector(m_mWorld);
	// z²Ι2iίι
	pos.z += 2;
	// vC[ΜΚuπΊ°ι
	m_mWorld = XMMatrixTranslation(0.0f, -1.5f, 0.0f) * m_mWorld;
	m_mWorld = XMMatrixRotationX(XMConvertToRadians(30)) * m_mWorld;
	// Spriteπ`ζ
	m_pGMain->m_pBackForeProc->GetSprite()->Draw3D(m_pGMain->m_pTitleProc->GetImageClear(), pos, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vEyePt, VECTOR2(WINDOW_WIDTH / 60, WINDOW_HEIGHT / 60), VECTOR2(0, 0), VECTOR2(WINDOW_WIDTH, WINDOW_HEIGHT));
	//m_mWorld = XMMatrixTranslation(0.0f, -1.0f, 0.0f) * m_mWorld;
	// PCπ`ζ
	GetMesh()->Render(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
}

//----------------------------------------------------------------------------------------------------
//»έΜL[Μϋόπό­pxπΤ·
//----------------------------------------------------------------------------------------------------
void CPcObj::GetCursorRotate()
{
	// J[\ΜΐW
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
// ANZXΦ
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

// ImGui\¦
void CPcObj::ShowImgui(bool* p_open)
{
	// EBhEΚuΖTCYπέθ΅ά·BImGuiCond_Once ΙζθAρΜέέθ³κά·B
	ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 420), ImGuiCond_Once);
	// et[Μgό
	ImGui::GetStyle().FrameBorderSize = 1.0f; // gόΜΎ³
	// EBhEθ`Μέθ
	// EBeginΕEChEΜΌOέθBΘ¨AΌOΝEChE²ΖΙj[NΙΘιζ€Ι·ι±Ζ
	// Ep_openΝAΒΆι{^π΅½Ζ«falseͺΤι
	if (ImGui::Begin(u8"ImGui fobNEChE", p_open))
	{
		if (ImGui::Button(u8"oHe[uΆ¬", ImVec2(300, 20)))
		{
			CSearchRoute* pSearch = new CSearchRoute(m_pGMain);
			pSearch->MakeANode();

			pSearch->MakeRouteTable();
			SAFE_DELETE(pSearch);

		}

		ImGui::Separator();
		if (ImGui::Button(u8"GπSΕ³Ήι", ImVec2(150, 20)))
		{
			m_pGMain->m_pEnmProc->SetNonActive();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"‘ϋΜΗΑ", ImVec2(60, 20)))
		{
			m_pGMain->m_pAllyProc->Start(GetPositionVector(m_mWorld), TRUE);
			m_nAllyNum++;
		}

		ImGui::Separator();
		if (ImGui::Button(u8"KwΑ", ImVec2(60, 20)))
		{
			m_pGMain->m_pMapProc->SetHierarchy(m_pGMain->m_pMapProc->GetHierarchy() + 1);
		}

		ImGui::Separator();
		if (ImGui::Button(u8"{XX|[", ImVec2(60, 20)))
		{
			m_pGMain->m_pEnmBossProc->Start(VECTOR3(0.0f, 0.0f, 0.0f));
		}

		ImGui::Separator();
		if (ImGui::Button(u8"\Ν­»", ImVec2(60, 20)))
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
		// {^όΝ
		ImGui::Text(" ");
		ImGui::SameLine();
		if (ImGui::Button(u8"IΉ", ImVec2(60, 20)))
		{
			// {^π΅½Ζ«A*p_openΙfalseπΤ·
			*p_open = false;
		}
	}
	ImGui::End();
}