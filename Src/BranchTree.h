#pragma once
#include "BaseProc.h"
#include <list>


#define MAX_BRANCH_LEVEL 5
#define COLLISIONLIST_REALLOCSIZE 100

class CCellToObj;
class CBaseObj;
// 空間クラス
class CCell
{
private:
	CCellToObj* m_pLate;
	
public:
	// コンストラクタ
	CCell();
	// デストラクタ
	~CCell();

	// リンクを全てリセットする
	void ResetLink(CCellToObj* spOFT);

	//空間にオブジェクトを登録
	bool Push(CCellToObj* spOFT);

	CCellToObj* GetFirstObj();

	// 削除されるオブジェクトをチェック
	bool OnRemove(CCellToObj* pRemoveObj);
};

// 空間とオブジェクトを結びつけるクラス
class CCellToObj
{
public:
	CBaseObj*	m_pObj;
	CCell*		m_pCell;

	// リストの前
	CCellToObj*	m_pPre;
	// リストの後ろ
	CCellToObj* m_pNext;
	int m_id;

	// コンストラクタ、デストラクタ
	CCellToObj(int id = 0);
	virtual ~CCellToObj();

	// 空間から外れる
	bool Remove();
};

// 衝突リストクラス
class CollisionList
{
private:
	CBaseObj** root_;		// リストルートポインタ
	size_t	pos_;
	size_t	mallocSize_;

public:
	size_t GetSize();
	CBaseObj** GetRootPtr();
	void ResetPos();
	void wright(CBaseObj* obj1, CBaseObj* obj2);
	void reflesh();

	// コンストラクタ、デストラクタ
	CollisionList();
	~CollisionList();
};

// 衝突対応リストクラス
class CLinerCollisionManager
{
protected:
	CCell** ppCellAry;		// 線型ポインタ配列
	unsigned int m_nPow[MAX_BRANCH_LEVEL + 1];
	float m_fW;
	float m_fH;
	float m_fLeft;
	float m_fTop;
	float m_fuW;
	float m_fuH;
	int m_nCellNum;
	unsigned int m_nuLevel;
	CollisionList m_CollList;

public:
	// コンストラクタ
	CLinerCollisionManager();
	// デストラクタ
	~CLinerCollisionManager();

	// 判定リストの作成
	bool Init(int level, float left, float top, float right, float bottom);
	// 空間を算出し、登録
	bool Set(float left, float top, float right, float bottom, CCellToObj* spOFT);
	// 衝突判定リストを作成
	DWORD GetAllCollisionList(CollisionList** colList);
	// 空間内で衝突判定リストを作成
	bool GetCollisionList(DWORD Elem, std::list<CBaseObj*>& ColStac);

	// 座標から空間番号を作成
	DWORD GetMortonNumber(float left, float top, float right, float bottom);
	// 座標→線形4分木要素番号変換関数
	DWORD GetPointElem(float pos_x, float pos_y);
	// 2Dモートン空間番号算出関数
	WORD Get2DMortonNumber(WORD x, WORD y);
	// ビット分割関数
	DWORD BitSeparate32(DWORD n);

	// 空間を生成
	bool CreateNewCell(DWORD Elem);
};