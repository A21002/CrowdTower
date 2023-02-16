#include "BranchTree.h"

//-----------------------------------------------------------------------------------------------------
//
// CLinerCollisionManagerクラス
//
//-----------------------------------------------------------------------------------------------------
// コンストラクタ
CLinerCollisionManager::CLinerCollisionManager()
{
	m_nuLevel = 0;
	m_fW = 0.0f;
	m_fH = 0.0f;
	m_fLeft = 0.0f;
	m_fTop = 0.0f;
	m_fuW = 0.0f;
	m_fuH = 0.0f;
	m_nCellNum = 0;
	ppCellAry = NULL;
	
	m_nPow[0] = 1;
	for (int i = 1; i < MAX_BRANCH_LEVEL + 1; i++)
	{
		m_nPow[i] = m_nPow[i - 1] * 4;
	}
}

// デストラクタ
CLinerCollisionManager::~CLinerCollisionManager()
{
	for (int i = 0; i < m_nCellNum; i++)
	{
		if (ppCellAry[i] != NULL)
		{
			delete ppCellAry[i];
		}
	}
	delete[] ppCellAry;
}

// 初期化処理
bool CLinerCollisionManager::Init(int level, float left, float top, float right, float bottom)
{
	// レベルが最高レベル以下か
	if (level > MAX_BRANCH_LEVEL) {
		return false;
	}

	// 配列作成
	m_nCellNum = (m_nPow[level + 1] - 1) / 3;
	ppCellAry = new CCell * [m_nCellNum];
	ZeroMemory(ppCellAry, sizeof(CCell*) * m_nCellNum);

	// 領域を登録
	m_fLeft = left;
	m_fTop = top;
	m_fW = right - left;
	m_fH = bottom - top;
	m_fuW = m_fW / (1 << level);
	m_fuH = m_fH / (1 << level);

	m_nuLevel = level;

	return true;
}

// 座標から空間番号を算出
DWORD CLinerCollisionManager::GetMortonNumber(float left, float top, float right, float bottom)
{
	// 最小レベルにおける各軸位置を算出
	DWORD LT = GetPointElem(left, top);
	DWORD RB = GetPointElem(right, bottom);

	// 空間番号の排他的論理和から
	// 所属レベルを算出
	DWORD Def = RB ^ LT;
	unsigned int HiLevel = 0;
	unsigned int i;
	for (i = 0; i < m_nuLevel; i++)
	{
		DWORD Check = (Def >> (i * 2)) & 0x3;
		if (Check != 0)
			HiLevel = i + 1;
	}
	DWORD SpaceNum = RB >> (HiLevel * 2);
	DWORD AddNum = (m_nPow[m_nuLevel - HiLevel] - 1) / 3;
	SpaceNum += AddNum;

	if (SpaceNum > m_nCellNum)
		return 0xffffffff;

	return SpaceNum;
}

DWORD CLinerCollisionManager::GetPointElem(float pos_x, float pos_y)
{
	return Get2DMortonNumber((WORD)((pos_x - m_fLeft) / m_fuW), (WORD)((pos_y - m_fTop) / m_fuH));
}

WORD CLinerCollisionManager::Get2DMortonNumber(WORD x, WORD y)
{
	return (WORD)(BitSeparate32(x) | (BitSeparate32(y) << 1));
}

DWORD CLinerCollisionManager::BitSeparate32(DWORD n)
{
	n = (n | (n << 8)) & 0x00ff00ff;
	n = (n | (n << 4)) & 0x0f0f0f0f;
	n = (n | (n << 2)) & 0x33333333;
	return (n | (n << 1)) & 0x55555555;
}

bool CLinerCollisionManager::Set(float left, float top, float right, float bottom, CCellToObj* spOFT)
{
	// オブジェクトの境界範囲から登録モートン番号を算出
	DWORD Elem = GetMortonNumber(left, top, right, bottom);
	if (Elem < m_nCellNum) {
		// 空間が無い場合は新規作成
		if (!ppCellAry[Elem])
			CreateNewCell(Elem);
		return ppCellAry[Elem]->Push(spOFT);
	}
	return false;	// 登録失敗
}

// 空間を生成
bool CLinerCollisionManager::CreateNewCell(DWORD Elem)
{
	// 引数の要素番号
	while (!ppCellAry[Elem])
	{
		// 指定の要素番号に空間を新規作成
		ppCellAry[Elem] = new CCell();

		// 親空間にジャンプ
		Elem = (Elem - 1) >> 2;
		if (Elem >= m_nCellNum) break;
	}
	return true;
}

// 衝突判定リストを作成する
DWORD CLinerCollisionManager::GetAllCollisionList(CollisionList** colList)
{
	// リスト（配列）は必ず初期化します
	m_CollList.ResetPos();

	// ルート空間の存在をチェック
	if (ppCellAry[0] == NULL)
		return 0;	// 空間が存在していない

	// ルート空間を処理
	std:: list<CBaseObj*> ColStac;
	GetCollisionList(0, ColStac);

	*colList = &m_CollList;

	return (DWORD)m_CollList.GetSize();
}

// 空間内衝突判定リストを作成
bool CLinerCollisionManager::GetCollisionList(DWORD Elem, std::list<CBaseObj*>& ColStac)
{
	std::list<CBaseObj*>::iterator it;
	// ① 空間内のオブジェクト同士の衝突リスト作成
	CCellToObj* spOFT1 = ppCellAry[Elem]->GetFirstObj();
	while (spOFT1 != 0)
	{
		CCellToObj* spOFT2 = spOFT1->m_pNext;
		while (spOFT2 != 0) {
			m_CollList.wright(spOFT1->m_pObj, spOFT2->m_pObj);
			spOFT2 = spOFT2->m_pNext;
		}
		// ② 衝突スタックとの衝突リスト作成
		for (it = ColStac.begin(); it != ColStac.end(); it++) {
			m_CollList.wright(spOFT1->m_pObj, *it);
		}
		spOFT1 = spOFT1->m_pNext;
	}

	bool ChildFlag = false;
	// ③ 子空間に移動
	DWORD ObjNum = 0;
	DWORD i, NextElem;
	for (i = 0; i < 4; i++) {
		NextElem = Elem * 4 + 1 + i;
		if (NextElem < m_nCellNum && ppCellAry[Elem * 4 + 1 + i]) {
			if (!ChildFlag) {
				// ④ 登録オブジェクトをスタックに追加
				spOFT1 = ppCellAry[Elem]->GetFirstObj();
				while (spOFT1 != 0) {
					ColStac.push_back(spOFT1->m_pObj);
					ObjNum++;
					spOFT1 = spOFT1->m_pNext;
				}
			}
			ChildFlag = true;
			GetCollisionList(Elem * 4 + 1 + i, ColStac);	// 子空間へ
		}
	}

	// ⑤ スタックからオブジェクトを外す
	if (ChildFlag) {
		for (i = 0; i < ObjNum; i++)
			ColStac.pop_back();
	}

	return true;
}
//-----------------------------------------------------------------------------------------------------
//
// CCellクラス
//
//-----------------------------------------------------------------------------------------------------
// コンストラクタ
CCell::CCell()
{
	m_pLate = 0;
}

// デストラクタ
CCell::~CCell()
{
	//if (m_pLate != NULL)
	//{
	//	ResetLink(m_pLate);
	//}
}

// リンクを全てリセットする
void CCell::ResetLink(CCellToObj* spOFT)
{
	if (spOFT->m_pPre != 0) {
		ResetLink(spOFT->m_pPre);
	}
	spOFT = 0;
}

// OFTをプッシュ
bool CCell::Push(CCellToObj* spOFT)
{
	if (spOFT == 0) return false;	// 無効オブジェクトは登録しない
	if (spOFT->m_pCell == this) return false;	// 2重登録チェック
	if (m_pLate == 0) {
		m_pLate = spOFT;	// 空間に新規登録
	}
	else
	{
		// 最新OFTオブジェクトを更新
		spOFT->m_pNext = m_pLate;
		m_pLate->m_pPre = spOFT;
		m_pLate = spOFT;
	}
	spOFT->m_pCell = this;	// 空間を登録
	return true;
}

CCellToObj* CCell::GetFirstObj()
{
	if (m_pLate == NULL) {
		return NULL;
	}
	if (m_pLate->m_pObj->GetActive())
	{
		return m_pLate;
	}
	else {
		return NULL;
	}
}

// 削除されるオブジェクトをチェック
bool CCell::OnRemove(CCellToObj* pRemoveObj)
{
	if (m_pLate == pRemoveObj) {
		// 次のオブジェクトに挿げ替え
		if (m_pLate != NULL)
			m_pLate = m_pLate->m_pNext;
	}
	return true;
}

//-----------------------------------------------------------------------------------------------------
//
// CCellToObjクラス
//
//-----------------------------------------------------------------------------------------------------
// コンストラクタ
CCellToObj::CCellToObj(int id)
{
	m_id = 0;
	m_pCell = NULL;
	m_pObj = NULL;
	m_pPre = NULL;
	m_pNext = NULL;
}

// デストラクタ
CCellToObj::~CCellToObj()
{

}

// 自らリストから外れる
bool CCellToObj::Remove()
{
	// 既に外れている場合は終了
	if (!m_pCell)
	{
		return false;
	}

	// 自身の登録している空間に通知
	if (!m_pCell->OnRemove(this))
	{
		return false;
	}

	// 逸脱処理
	if (m_pPre != 0) 
	{
		m_pPre->m_pNext = m_pNext;
	}
	if (m_pNext != 0) 
	{
		m_pNext->m_pPre = m_pPre;
	}
	m_pPre = 0;
	m_pNext = 0;
	m_pCell = NULL;
	return true;
}

//-----------------------------------------------------------------------------------------------------
//
// CollisionListクラス
//
//-----------------------------------------------------------------------------------------------------
// コンストラクタ
CollisionList::CollisionList()
{
	root_ = 0;
	pos_ = 0;
	mallocSize_ = 0;
	root_ = (CBaseObj**)malloc(0);
}

// デストラクタ
CollisionList::~CollisionList()
{
	reflesh();
}

size_t CollisionList::GetSize()
{
	return pos_;
}

CBaseObj** CollisionList::GetRootPtr()
{
	return root_;
}

void CollisionList::ResetPos()
{
	pos_ = 0;
}

void CollisionList::wright(CBaseObj* obj1, CBaseObj* obj2)
{
	if (pos_ >= mallocSize_) {
		root_ = (CBaseObj**)realloc(root_, sizeof(CBaseObj*) * (mallocSize_ + COLLISIONLIST_REALLOCSIZE));
		mallocSize_ += COLLISIONLIST_REALLOCSIZE;
	}
	root_[pos_++] = obj1;
	root_[pos_++] = obj2;
}

void CollisionList::reflesh()
{
	if (root_)
	{
		free(root_);
	}
}