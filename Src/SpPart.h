//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１２　　　　　　                 ver 3.1        2022.2.16
//		４分木空間分割　ヘッダファイル							SpPart.h
//=============================================================================
#pragma once
#include <list>
#include "GameMain.h"

struct CCellList
{
	std::list<CBaseObj*> SpList;		// 空間分割配列の中に設定するオブジェクトのリスト
};

class CSpPart
{
protected:
	CGameMain*				m_pGMain;		// ゲームメイン
	VECTOR3					m_BasePoint;	// 空間分割の基点（最小点）
	VECTOR3					m_FullSize;		// 空間の全体サイズ
	int						m_PartLevel;	// 最大分割数
	VECTOR3					m_CellSize;		// 最大分割時の一つのセルサイズ
	std::vector<CCellList>	m_SpArray;		// 空間分割配列
	int						m_Num;			// 空間数

public:
	void CleateSpPart(VECTOR3 vMin, VECTOR3 vMax, int PartLevel=3);
	void ClearList();
	void SetObj(VECTOR3 vMin, VECTOR3 vMax, CBaseObj* pObj);
	void  RemoveObj(CBaseObj* obj, int Idx);
	// 近接オブジェクトリスト
	void AllHitcheck(int, std::vector<CCellList>&, std::vector<CBaseObj*>&, std::vector<CBaseObj*>&);

	// 当たり判定
	BOOL HitcheckSpPart(CBaseObj* pObj, VECTOR3 vPos);
	BOOL HitcheckSpPartSub(DWORD Level, DWORD LvlMtn, CBaseObj* pObj);
	// 近接判定
	BOOL SetKeepOffset(CBaseObj* pObj, VECTOR3 vPos);
	BOOL SetKeepOffsetSub(DWORD Level, DWORD LvlMtn, CBaseObj* pObj);

	DWORD BitSeparate32(DWORD n);
	DWORD Get2DMtnNumber(VECTOR3 vPos);
	void Get2DLevelAndMtnNumber(DWORD MinMtn, DWORD MaxMtn, DWORD& Level, DWORD& XOR);

	void MakeAABB(const VECTOR3& v1, const VECTOR3& v2, const FLOAT& r, VECTOR3& vMin, VECTOR3& vMax);

	// アクセス関数
	std::vector<CCellList> GetSpArray() { return m_SpArray; }

	CSpPart(CGameMain*	pGMain);
	~CSpPart();

};