//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１２　　　　　　                 ver 3.1        2022.2.16
//		４分木空間分割　ソースファイル							SpPart.cpp
//=============================================================================

#include "SpPart.h"

//-----------------------------------------------------------------------------
//
// ４分木空間分割コンストラクタ
//
// 引数
//   CGameMain*	pGMain
//
//-----------------------------------------------------------------------------
CSpPart::CSpPart(CGameMain*	pGMain)
{
	m_pGMain = pGMain;
	m_Num = 0;
	m_PartLevel = 0;
}

//-----------------------------------------------------------------------------
//
// ４分木空間分割デストラクタ
//
//
//-----------------------------------------------------------------------------
CSpPart::~CSpPart()
{
	m_SpArray.clear();					// 配列削除
	m_SpArray.shrink_to_fit();			// 不要メモリ解放

}

//-----------------------------------------------------------------------------
//
// ４分木空間の作成
// (線形４分木空間の配列を作成する)
//
// 引数
//   VECTOR3 vMin  : 全体空間を取り囲むバウンディングボックスAABBのvMin
//   VECTOR3 vMax  : 全体空間を取り囲むバウンディングボックスAABBのvMax
//   int PartLevel : 最大分割数。省略値は３
//
//-----------------------------------------------------------------------------
void CSpPart::CleateSpPart( VECTOR3 vMin, VECTOR3 vMax, int PartLevel)
{
	m_BasePoint = vMin;                // 空間分割の基点（最小点）
	m_FullSize = vMax - vMin;          // 空間の全体サイズ
	m_PartLevel = PartLevel;           // 最大分割数
	m_CellSize = m_FullSize / pow(2,m_PartLevel);    // 最大分割時の一つのセルサイズ

	m_SpArray.clear();			// 配列削除
	m_SpArray.shrink_to_fit();	// 不要メモリ解放

	// 要素数を計算する
	int Num = 0;
	for (int i = 0; i <= m_PartLevel; i++)
	{
		Num += pow(4, i);
	}
	m_Num = Num;
	m_SpArray.resize(Num);   // 指定要素数の配列にする

	ClearList();             // ４分木空間のリストをすべてクリヤーする
}
//-----------------------------------------------------------------------------
//
// ４分木空間のリストをすべてクリヤーする
//
// 引数
//
//-----------------------------------------------------------------------------
void CSpPart::ClearList()
{
	// 
	for (int i = 0; i < m_SpArray.size(); i++)
	{
		m_SpArray[i].SpList.clear();
	}
}

//-----------------------------------------------------------------------------
//
// 線形４分木空間にオブジェクトを登録する
//
// 引数
//   VECTOR3 vMin  : オブジェクトを取り囲むバウンディングボックスAABBのvMin
//   VECTOR3 vMax  : オブジェクトを取り囲むバウンディングボックスAABBのvMax
//   CBaseObj* pObj: オブジェクトアドレス
//
//-----------------------------------------------------------------------------
void CSpPart::SetObj(VECTOR3 vMin, VECTOR3 vMax, CBaseObj* pObj)
{
	// 空間モートン番号を算出
	DWORD MinMtn = Get2DMtnNumber(vMin-m_BasePoint);
	DWORD MaxMtn = Get2DMtnNumber(vMax-m_BasePoint);

	// レベル番号とそのレベルのモートン番号を求める
	DWORD Level, LvlMtn;
	Get2DLevelAndMtnNumber( MinMtn, MaxMtn, Level, LvlMtn);

	// 線形４分木配列の添字を求める
	DWORD Idx = LvlMtn + ((pow(4, Level) - 1) / 3);

	// 線形４分木配列のリストにオブジェクトのアドレスを登録する
	m_SpArray[Idx].SpList.push_back(pObj);
}


void CSpPart::RemoveObj(CBaseObj* obj, int Idx)
{
	/*
	std::list<CBaseObj*>::iterator it = m_SpArray[Idx].SpList.begin();
	for (int i = 1; i < place; i++) {
		it++;
	}
	m_SpArray[Idx].SpList.erase(it);
	*/
	m_SpArray[Idx].SpList.remove(obj);

}

//-----------------------------------------------------------------------------
// 線形４分木空間に登録されているオブジェクトを探索して当たり判定を行う
// CheckPoint3
//
// 引数
//   int Idx						: 探索する空間要素数
//   vector<CCellList>& pList		: 当たり判定を行う相手のリスト
//   vector<CBaseObj*>& stacList	: 上位空間にいるオブジェクト
//	 vector<CBaseObj*>& hitList		: ヒットしたオブジェクト配列
//-----------------------------------------------------------------------------
void CSpPart::AllHitcheck(int Idx, std::vector<CCellList>& pList, std::vector<CBaseObj*>& stacList, std::vector<CBaseObj*>& hitList){
	std::list<CBaseObj*>::iterator pIt;
	std::list<CBaseObj*>::iterator it;

	// ① 空間内のオブジェクト同士の衝突リスト作成
	if (m_SpArray[Idx].SpList.size() > 0) {
		// listの空間にオブジェクトが割り当てられていれば
		
		if (pList[Idx].SpList.size() > 0) {
			// 敵の空間にオブジェクトが入っていれば
			pIt = pList[Idx].SpList.begin();

			while (pIt != pList[Idx].SpList.end()) {
				// 敵リストのすべてのオブジェクトを参照
				it = m_SpArray[Idx].SpList.begin();
				while (it != m_SpArray[Idx].SpList.end()) {
					// 同じ空間の敵オブジェクトとの当たり判定リストに入れる
					hitList.push_back(*it);
					hitList.push_back(*pIt);
					it++;
				}
				pIt++;
			}
		}
		std::list<CBaseObj*>::iterator stacit = m_SpArray[Idx].SpList.begin();
		// 上位空間との当たり判定
		for (int j = 0; j < m_SpArray[Idx].SpList.size(); j++) {
			for (int i = 0; i < stacList.size(); i++) {
				hitList.push_back(*stacit);
				hitList.push_back(stacList[i]);
			}
			stacit++;
		}
		
	}

	bool ChildFlag = false;
	// ③ 子空間に移動
	DWORD ObjNum = 0;
	DWORD i, NextElem;
	for (i = 0; i < 4; i++) {
		NextElem = Idx * 4 + 1 + i;
		if (NextElem < m_Num) {
			if (pList[Idx].SpList.size() != 0) {
				if (!ChildFlag) {
					// ④ 登録オブジェクトをスタックに追加
					it = pList[Idx].SpList.begin();
					for (int j = 0; j < pList[Idx].SpList.size(); j++) {
						stacList.push_back(*it);
						ObjNum++;
						it++;
					}
				}
			}
			ChildFlag = true;
			AllHitcheck(NextElem, pList, stacList, hitList);
		}
	}

	// ⑤ スタックからオブジェクトを外す
	if (ChildFlag) {
		for (i = 0; i < ObjNum; i++)
			stacList.pop_back();
	}
}
 
//-----------------------------------------------------------------------------
//
// 線形４分木空間に登録されているオブジェクトを探索して当たり判定を行う
//
// 引数
//   CBaseObj* pObj : 接触判定対象オブジェクト
//   VECTOR3 vPos   : 探索する位置
//
//-----------------------------------------------------------------------------
BOOL CSpPart::HitcheckSpPart(CBaseObj* pObj, VECTOR3 vPos)
{
	BOOL bRet = FALSE;

	// 空間モートン番号を算出
	DWORD Mtn = Get2DMtnNumber(vPos-m_BasePoint);
	DWORD LvlMtn;

	// 空間レベル毎の接触判定
	for (int i = 0; i <= m_PartLevel; i++)
	{
		LvlMtn = Mtn >> ((m_PartLevel-i) * 2);
		if (HitcheckSpPartSub(i, LvlMtn, pObj))
		{
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

//-----------------------------------------------------------------------------
//
// 線形４分木空間に登録されているオブジェクトを探索して実際に当たり判定を行うサブ関数
//
// 引数
//   DWORD Level    : 空間レベル
//   DWORD LvlMtn   : レベルモートン番号
//   CBaseObj* pObj : 接触判定対象オブジェクト
//
//-----------------------------------------------------------------------------
BOOL CSpPart::HitcheckSpPartSub(DWORD Level, DWORD LvlMtn, CBaseObj* pObj)
{
	// 線形４分木配列の添字を求める
	DWORD Idx = LvlMtn + ((pow(4, Level) - 1) / 3);

	std::list<CBaseObj*>::iterator itr;    // リストのイテレータ
	BOOL bRet = FALSE;
	
	// リストをたどって当たり判定を行う
	for (itr = m_SpArray[Idx].SpList.begin(); itr != m_SpArray[Idx].SpList.end(); itr++)
	{
		if (*itr)     // リストがあるとき（オブジェクトが登録されているとき）
		{
			// バウンディングボックスとレイ（直線）の当たり判定を行う
			if (pObj->Hitcheck(*itr, (*itr)->GetBBox()))
			{
				bRet = TRUE;
			}
		}
	}
	return bRet;
}

//-----------------------------------------------------------------------------
//
// 線形４分木空間に登録されているオブジェクトを探索して離反判定を行う
//
// 引数
//   CBaseObj* pObj : 接触判定対象オブジェクト
//   VECTOR3 vPos   : 探索する位置
//
//-----------------------------------------------------------------------------
BOOL CSpPart::SetKeepOffset(CBaseObj* pObj, VECTOR3 vPos)
{
	BOOL bRet = FALSE;

	// 空間モートン番号を算出
	DWORD Mtn = Get2DMtnNumber(vPos - m_BasePoint);
	DWORD LvlMtn;

	// 空間レベル毎の接触判定
	for (int i = 0; i <= m_PartLevel; i++)
	{
		LvlMtn = Mtn >> ((m_PartLevel - i) * 2);
		if (SetKeepOffsetSub(i, LvlMtn, pObj))
		{
			bRet = TRUE;
		}
	}

	return bRet;
}

//-----------------------------------------------------------------------------
//
// 線形４分木空間に登録されているオブジェクトを探索して実際に離反判定を行うサブ関数
//
// 引数
//   DWORD Level    : 空間レベル
//   DWORD LvlMtn   : レベルモートン番号
//   CBaseObj* pObj : 接触判定対象オブジェクト
//
//-----------------------------------------------------------------------------
BOOL CSpPart::SetKeepOffsetSub(DWORD Level, DWORD LvlMtn, CBaseObj* pObj)
{
	// 線形４分木配列の添字を求める
	DWORD Idx = LvlMtn + ((pow(4, Level) - 1) / 3);

	std::list<CBaseObj*>::iterator itr;    // リストのイテレータ
	BOOL bRet = FALSE;

	// リストをたどって当たり判定を行う
	for (itr = m_SpArray[Idx].SpList.begin(); itr != m_SpArray[Idx].SpList.end(); itr++)
	{
		if (*itr)     // リストがあるとき（オブジェクトが登録されているとき）
		{
			m_pGMain->m_nScore++;

			// バウンディングボックスとレイ（直線）の当たり判定を行う
			if (pObj->SetKeepOffset(*itr))
			{
				bRet = TRUE;
			}
		}
	}
	return bRet;
}


//-----------------------------------------------------------------------------
//
// ビット分割関数
//
//-----------------------------------------------------------------------------
DWORD CSpPart::BitSeparate32(DWORD n)
{
	n = (n | (n << 8)) & 0x00ff00ff;
	n = (n | (n << 4)) & 0x0f0f0f0f;
	n = (n | (n << 2)) & 0x33333333;
	return (n | (n << 1)) & 0x55555555;
}

//-----------------------------------------------------------------------------
//
// 2D空間のモートン番号を算出
//
//-----------------------------------------------------------------------------
DWORD CSpPart::Get2DMtnNumber(VECTOR3 vPos)
{
	DWORD x, z;

	x = (int)vPos.x / m_CellSize.x;
	z = (int)vPos.z / m_CellSize.z;

	return (BitSeparate32(x) | (BitSeparate32(z) << 1));
}

//-----------------------------------------------------------------------------
//
// 2D空間の空間レベルとそのレベルのモートン番号を算出
//
// 引数
//   DWORD MinMtn   最小モートン番号
//   DWORD MaxMtn   最大モートン番号
//   DWORD& Level   空間レベル(Out)
//   DWORD& LvlMtn  そのレベルのモートン番号(Out)
//
//-----------------------------------------------------------------------------
void CSpPart::Get2DLevelAndMtnNumber(DWORD MinMtn, DWORD MaxMtn, DWORD& Level, DWORD& LvlMtn)
{
	DWORD XORMtn = MaxMtn ^ MinMtn;  // 排他的論理和をとる

	Level = m_PartLevel;
	DWORD WXORMtn;

	// 空間レベルを求める
	if (XORMtn == 0)
	{
		// 空間レベル最下位レベルに属する
		Level = m_PartLevel;
	}
	else {
		// 空間レベル０～最下位－１レベルに属する
		for (int i = 0; i < m_PartLevel; i++)
		{
			// ２ビットづつ判定して値があるかどうか判定する
			// 値のある最上位のビット位置が空間レベルである
			WXORMtn = XORMtn >> (i*2);
			WXORMtn &= 0x00000003;
			if (WXORMtn != 0)  Level = (m_PartLevel-1) - i;
		}
	}

	// そのレベルのモートン番号を求める
	LvlMtn = MaxMtn >> ((m_PartLevel - Level) * 2);

}

//------------------------------------------------------------------------
//
// v1～v2直線の長さで、半径rの幅のバウンディングボックス（AABB）を作成する
//
// const VECTOR3& v1 : 直線の始点座標
// const VECTOR3& v2 : 直線の終点座標
// const FLOAT& r    : バウンディングボックスの大きさ（半径）
// VECTOR3& vMin     : AABBのvMin(Out)
// VECTOR3& vMax     : AABBのvMax(Out)
//
//------------------------------------------------------------------------
void CSpPart::MakeAABB(const VECTOR3& v1, const VECTOR3& v2, const FLOAT& r, VECTOR3& vMin, VECTOR3& vMax)
{
	if (v1.x > v2.x) {
		vMin.x = v2.x;
	}
	else {
		vMin.x = v1.x;
	}
	vMin.x -= r;

	if (v1.y > v2.y) {
		vMin.y = v2.y;
	}
	else {
		vMin.y = v1.y;
	}
	vMin.y -= r;

	if (v1.z > v2.z) {
		vMin.z = v2.z;
	}
	else {
		vMin.z = v1.z;
	}
	vMin.z -= r;

	if (v1.x < v2.x) {
		vMax.x = v2.x;
	}
	else {
		vMax.x = v1.x;
	}
	vMax.x += r;

	if (v1.y < v2.y) {
		vMax.y = v2.y;
	}
	else {
		vMax.y = v1.y;
	}
	vMax.y += r;

	if (v1.z < v2.z) {
		vMax.z = v2.z;
	}
	else {
		vMax.z = v1.z;
	}
	vMax.z += r;
}

