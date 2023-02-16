//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１２　　　　　　                 ver 3.1        2022.2.16
// 
//		経路探索　ヘッダファイル								SearchRoute.h
// 
//		AStarアルゴリズムを利用して経路探索を行うプログラムである
// 
//=============================================================================
#pragma once
#include "GameMain.h"

// --------------------------------------------------------------------
//
// 経路ノード
//
// --------------------------------------------------------------------
// ステータスenum
enum eStatus
{
	eNone = 0,
	eOpen = 1,
	eClose = 2,
};
// 経路ノード
struct ANODE
{
	eStatus status;         // ステータス  eNone,eOpen,eClose
	float   cost;           // 実コスト
	float   heuristic;      // 推定コスト
	float   score;          // スコア
	VECTOR3 parent;         // 親ノードの要素番号
	VECTOR3 next;           // 次ノードの要素番号

	ANODE()    // コンストラクタ
	{
		Init();
	}
	void Init()   // 初期化処理
	{
		status = eNone;
		cost = 0;
		heuristic = 0;
		score = 0;
		parent = VECTOR3(0, 0, 0);
		next = VECTOR3(0, 999999, 0);   // 次ノードの初期値。Y値を999999としておく（終了判断のため）
	}
};
// --------------------------------------------------------------------
//
// 経路探索クラス
//
// --------------------------------------------------------------------
class CSearchRoute
{
protected:
	CGameMain*							m_pGMain;		// ゲームメイン
	std::vector< std::vector<ANODE> >	m_ANodeArray;	// 経路探索配列
	VECTOR3								m_Basepoint;	// 配列の左手前の座標値
	float								m_Partssize;	// ノード一つの大きさ
	VECTOR3								m_StIdx;		// 経路開始点
	VECTOR3								m_EndIdx;		// 経路目的点
	VECTOR3								m_NowIdx;		// 現在地点
	std::vector< VECTOR3 >				m_OpenNode;		// オープンノードIdxの配列

public:
	void MakeANode();
	BOOL MakeANodeRoute(VECTOR3 vStart, VECTOR3 vEnd);
	void GetMapsizeAndBasepoint(VECTOR2& vMapSizeXZ, VECTOR3& vBasepoint);
	BOOL GetNextRoute(VECTOR3 vNow, VECTOR3& vNext);
	int  OpenOctPos(VECTOR3 Idx);
	BOOL OpenNode(VECTOR3 Idx, VECTOR3 parent, BOOL bBegin = FALSE);
	void CloseNode(VECTOR3 Idx);
	void DecisionRoute();
	BOOL CheckOpenNode(int x, int z);
	BOOL SearchOpenNodeScore(VECTOR3& Idx);
	float Calcheuristic(int x, int z);
	VECTOR3 ConvIdxToPos(VECTOR3 Idx);
	VECTOR3 ConvPosToIdx(VECTOR3 Pos);
	VECTOR3 ConvMapIdToArrId(int MapId);
	int ConvArrIdToMapId(VECTOR3 ArrId);

	BOOL MakeRouteTable();
	VECTOR3 GetNextPos(VECTOR3 currentPos, VECTOR3 goalPos);
	VECTOR3 GetNextPos(VECTOR3 currentPos, VECTOR3 goalPos, BOOL* isAreaOut);

	CSearchRoute(CGameMain*	pGMain);
	~CSearchRoute();

};

