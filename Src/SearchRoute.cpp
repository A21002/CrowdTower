//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１２　　　　　　                 ver 3.1        2022.2.16
// 
//		経路探索　ソースファイル								SearchRoute.cpp
// 
//		AStarアルゴリズムを利用して経路探索を行うプログラムである
// 
//=============================================================================
#include  "SearchRoute.h"
#include  "Map.h"

//-----------------------------------------------------------------------------
//
// 経路探索コンストラクタ
//
// 引数
//   CGameMain*	pGMain
//
//-----------------------------------------------------------------------------
CSearchRoute::CSearchRoute(CGameMain*	pGMain)
{
	m_pGMain = pGMain;
	m_Basepoint = VECTOR3(0, 0, 0);	// 配列の左手前の座標値
	m_Partssize = 0;				// ノード一つの大きさ
	m_StIdx = VECTOR3(0, 0, 0);		// 経路開始点
	m_EndIdx = VECTOR3(0, 0, 0);	// 経路目的点
	m_NowIdx = VECTOR3(0, 0, 0);	// 現在地点点
}

//-----------------------------------------------------------------------------
//
// 経路探索デストラクタ
//
//
//-----------------------------------------------------------------------------
CSearchRoute::~CSearchRoute()
{
	m_ANodeArray.clear();			// 経路探索配列削除
	m_ANodeArray.shrink_to_fit();	// 経路探索不要メモリ解放
	m_OpenNode.clear();				// 経路探索Open配列削除
	m_OpenNode.shrink_to_fit();		// 経路探索Open配列不要メモリ解放
}

//-----------------------------------------------------------------------------
//
// 経路探索配列の生成
// （マップのルート探索コリジョンを使用して、経路探索配列の生成、初期化）
//
// ・ 経路探索配列は、何度でも生成可。データは生成の都度クリヤーされる
//
// 引数
//
//-----------------------------------------------------------------------------
void CSearchRoute::MakeANode()
{
	m_Partssize = m_pGMain->m_pMapProc->GetRouteColMap()->m_Partssize;		// ノード一つの大きさ
	m_Basepoint = m_pGMain->m_pMapProc->GetRouteColMap()->m_Basepoint;		// 配列の左手前の座標値
	m_StIdx = VECTOR3(0, 0, 0);		// 経路開始点
	m_EndIdx = VECTOR3(0, 0, 0);	// 経路目的点
	m_NowIdx = VECTOR3(0, 0, 0);	// 現在地点点

	int znum = (int)(m_pGMain->m_pMapProc->GetRouteColMap()->m_MapNumXZ.y);   // 1次元目要素数
	int xnum = (int)(m_pGMain->m_pMapProc->GetRouteColMap()->m_MapNumXZ.x);   // 2次元目要素数

	// 経路探索配列・経路探索Open配列の初期化
	m_ANodeArray.clear();			// 経路探索配列削除
	m_ANodeArray.shrink_to_fit();	// 経路探索不要メモリ解放
	m_OpenNode.clear();				// 経路探索Open配列削除
	m_OpenNode.shrink_to_fit();		// 経路探索Open配列不要メモリ解放

	//	経路探索配列のサイズの初期化
	m_ANodeArray.resize(znum, std::vector<ANODE>(xnum));
}
//-----------------------------------------------------------------------------
//
// 経路探索配列の経路図の作成
//
// ・経路図は、開始位置・目的地を変えて、何度でも作成可。
// ・経路探索配列はクリヤーされ上書きされる。なお、コリジョン情報は再使用される。
//
// 引数
//   VECTOR3 vStart : 経路探索開始位置
//   VECTOR3 vEnd   : 経路探索目的地
//
// 戻り値
//   TRUE:経路確定   FALSE:経路なし
//-----------------------------------------------------------------------------
BOOL CSearchRoute::MakeANodeRoute(VECTOR3 vStart, VECTOR3 vEnd)
{
	// 開始点・目的地から経路探索配列添字の設定
	m_StIdx.x = (int)((vStart.x - m_Basepoint.x) / m_Partssize);
	m_StIdx.z = (int)((vStart.z - m_Basepoint.z) / m_Partssize);
	m_EndIdx.x = (int)((vEnd.x - m_Basepoint.x) / m_Partssize);
	m_EndIdx.z = (int)((vEnd.z - m_Basepoint.z) / m_Partssize);

	// ノードの初期クリヤー
	for (int z = 0; z < m_ANodeArray.size(); z++)
	{
		for (int x = 0; x < m_ANodeArray[z].size(); x++)
		{
			m_ANodeArray[z][x].Init();
		}
	}
	// 経路探索Open配列の初期化
	m_OpenNode.clear();				// 経路探索Open配列削除
	m_OpenNode.shrink_to_fit();		// 経路探索Open配列不要メモリ解放

	// 開始点の設定
	OpenNode(m_StIdx, VECTOR3(0, 0, 0), TRUE);  // 開始点のノードをOpen
	m_NowIdx = m_StIdx;

	// 目的地に到達するまで繰り返す
	while (!(m_NowIdx.x == m_EndIdx.x && m_NowIdx.z == m_EndIdx.z))
	{
		// ８方向のノードをＯｐｅｎする
		if (OpenOctPos(m_NowIdx) <= 0)
		{
			; // 周りのノードをひとつもOpenできなかった
		}

		// 元のm_NowIdxのノードはCloseする
		CloseNode(m_NowIdx);

		// 基準ノードの判定
		// Openノードの中で一番スコアが小さいノードを選択する
		VECTOR3 Idx;
		if (!SearchOpenNodeScore(Idx))
		{
			//  Openノードが一つも無いときは終了
			//MessageBox(NULL, _T("Openノードが一つも無い"), _T("経路探索処理"), IDOK);
			return FALSE;
		}

		// 選択したノードを基準ノードとする。
		m_NowIdx = Idx;

	}

	// 目的地から親ノードをさかのぼって経路を確定する
	DecisionRoute();

	return TRUE;
}
//-----------------------------------------------------------------------------
//
// 指定位置より８方向のノードをＯｐｅｎする
//
// 引数
//   VECTOR3 Idx    : オープンしようとする親の位置
//
// 戻り値
//   Openできたノード数を返す
//-----------------------------------------------------------------------------
int CSearchRoute::OpenOctPos(VECTOR3 Idx)
{
	int n = 0;

	if (OpenNode(VECTOR3(Idx.x - 1, Idx.y, Idx.z - 1), Idx)) n++;
	if (OpenNode(VECTOR3(Idx.x, Idx.y, Idx.z - 1), Idx)) n++;
	if (OpenNode(VECTOR3(Idx.x + 1, Idx.y, Idx.z - 1), Idx)) n++;
	if (OpenNode(VECTOR3(Idx.x - 1, Idx.y, Idx.z), Idx))     n++;
	if (OpenNode(VECTOR3(Idx.x + 1, Idx.y, Idx.z), Idx))     n++;
	if (OpenNode(VECTOR3(Idx.x - 1, Idx.y, Idx.z + 1), Idx)) n++;
	if (OpenNode(VECTOR3(Idx.x, Idx.y, Idx.z + 1), Idx)) n++;
	if (OpenNode(VECTOR3(Idx.x + 1, Idx.y, Idx.z + 1), Idx)) n++;

	return n;
}
//-----------------------------------------------------------------------------
//
// ノードをＯｐｅｎする
//
// 引数
//   VECTOR3 Idx    : オープン位置
//   VECTOR3 parent : 親の位置
//   BOOL bBegin    : 最初のノードのときTRUE（省略値FALSE）
//
// 戻り値
//   BOOL TRUE:オープンできたとき   FALSE: オープンできないとき
//-----------------------------------------------------------------------------
BOOL CSearchRoute::OpenNode(VECTOR3 Idx, VECTOR3 parent, BOOL bBegin)
{
	// Openしようとする位置
	int x = (int)Idx.x;
	int z = (int)Idx.z;

	// 親ノードの位置
	int px = (int)parent.x;  // 
	int pz = (int)parent.z;

	// Openできるかどうかのチェック
	if (!CheckOpenNode(x, z))
	{
		return FALSE;  // オープンできない
	}

	// Openする
	m_ANodeArray[z][x].status = eOpen;
	if (bBegin)   // 最初のノードか
	{
		m_ANodeArray[z][x].cost = 0;  // コストは0
	}
	else {
		m_ANodeArray[z][x].cost = m_ANodeArray[pz][px].cost + 1;  // 親のコスト＋１
	}
	m_ANodeArray[z][x].heuristic = Calcheuristic(x, z);
	m_ANodeArray[z][x].score = m_ANodeArray[z][x].cost
							+ m_ANodeArray[z][x].heuristic;
	m_ANodeArray[z][x].parent = parent;

	// Openノード配列にオープンしたノードのIdx番号を登録する
	m_OpenNode.push_back(VECTOR3(x, 0, z));

	return TRUE;
}
//-----------------------------------------------------------------------------
//
// ノードをＣｌｏｓｅする
//
// 引数
//   VECTOR3 Idx    : クロース位置
//
//-----------------------------------------------------------------------------
void CSearchRoute::CloseNode(VECTOR3 Idx)
{
	int x = (int)Idx.x;
	int z = (int)Idx.z;

	m_ANodeArray[z][x].status = eClose;

	// Openノード配列から削除する
	for (int i = 0; i < m_OpenNode.size(); i++)
	{
		if (m_OpenNode[i].x == x &&
			m_OpenNode[i].z == z)
		{
			m_OpenNode.erase(m_OpenNode.begin() + i);  // 配列から削除
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//
// 目的地から親ノードをさかのぼって経路を確定する
//
// ・目的地から親ノードparentをさかのぼって出発地まで戻る。
// ・このとき、次ノードnextに次のノードの添字を設定していく
// ・この処理が終わると、出発地からnextをたどって目的地まで行けるようになる
//
//-----------------------------------------------------------------------------
void CSearchRoute::DecisionRoute()
{
	VECTOR3 Idx = m_EndIdx;   // 最初が目的地となる
	VECTOR3 IdxOld = Idx;
	int     x, z, ox, oz;

	while (!(Idx.x == m_StIdx.x && Idx.z == m_StIdx.z))  // 終了は出発地となる
	{
		IdxOld = Idx;
		ox = (int)IdxOld.x;
		oz = (int)IdxOld.z;
		Idx = m_ANodeArray[oz][ox].parent;
		x = (int)Idx.x;
		z = (int)Idx.z;
		m_ANodeArray[z][x].next = IdxOld;  // 一つ前のノードの添字を設定する
	}
}

//-----------------------------------------------------------------------------
//
// Ｏｐｅｎノードの中で一番スコアの少ないノードを探索する
//
// 引数
//   VECTOR3 Idx    : 探索結果のノード位置
//
// 戻り値
//   BOOL TRUE:探索できたとき   FALSE: ノードが一つも無いとき
//
//-----------------------------------------------------------------------------
BOOL CSearchRoute::SearchOpenNodeScore(VECTOR3& Idx)
{
	if (m_OpenNode.size() <= 0)  return FALSE;

	float scoreMin = 999999;
	float costMin  = 999999;

	// Openノード配列を探索
	for (int i = 0; i < m_OpenNode.size(); i++)
	{
		int x = (int)m_OpenNode[i].x;
		int z = (int)m_OpenNode[i].z;

		// スコアが少ないか、同一スコアのときは実コストが少ないとき
		if (m_ANodeArray[z][x].score < scoreMin ||
			(m_ANodeArray[z][x].score == scoreMin && m_ANodeArray[z][x].cost < costMin ))  // 最低スコア・コストか？
		{
			scoreMin = m_ANodeArray[z][x].score;
			costMin  = m_ANodeArray[z][x].cost;
			Idx = VECTOR3(x, 0, z);
		}
	}

	return TRUE;
}
//-----------------------------------------------------------------------------
//
// ノードがＯｐｅｎできるかどうかチェックする
//
// 引数
//   int x : 添字ｘ
//   int z : 添字ｚ
//
// 戻り値
//   BOOL TRUE:Openできるとき   FALSE:Openできないとき
//
//-----------------------------------------------------------------------------
BOOL CSearchRoute::CheckOpenNode(int x, int z)
{
	// Openできるかどうかのチェック
	if (z < 0 || z >= m_ANodeArray.size())    return FALSE;
	if (x < 0 || x >= m_ANodeArray[z].size()) return FALSE;
	if (m_ANodeArray[z][x].status != eNone) return FALSE;
	if (m_pGMain->m_pMapProc->GetRouteColMap()->m_Collision[z][x] != 0) return FALSE;
	return TRUE;
}
//-----------------------------------------------------------------------------
//
// 推定コストの計算
//
//   斜め移動可のときの推定コストの計算
//
// 引数
//   int x : 添字ｘ
//   int z : 添字ｚ
//
// 戻り値
//   float 推定コスト
//
//-----------------------------------------------------------------------------
float CSearchRoute::Calcheuristic(int x, int z)
{
	// 縦横の距離を求める
	int dx = abs((int)m_EndIdx.x - x);
	int dy = abs((int)m_EndIdx.z - z);

	float heuristic;

	heuristic = dx + dy;   // ① 直角距離(マンハッタン距離)を推定コストとする

	//heuristic = magnitude(VECTOR2(dx, dy));  // ② 直線距離(ユークリッド距離)を推定コストとする

	//heuristic = (dx > dy) ? dx : dy;    // ③ 縦・横の多い方を推定コストとする

	return heuristic;
}

//-----------------------------------------------------------------------------
//
// 完成したノードを探索し、指定した位置添字から経路の次の位置添字を返す
//
// 引数
//   VECTOR3 NowIdx  : 今の位置添字
//   VECTOR3 NextIdx : 次の移動位置添字(Out)
//
// 戻り値
//   BOOL TRUE:次がある   FALSE:次がない（終了）
//
//-----------------------------------------------------------------------------
BOOL CSearchRoute::GetNextRoute(VECTOR3 NowIdx, VECTOR3& NextIdx)
{
	int x, z;

	// 経路探索配列添字
	x = (int)NowIdx.x;
	z = (int)NowIdx.z;

	if (m_ANodeArray[z][x].next.y == 999999)  // 次の位置がないとき
	{
		NextIdx = NowIdx;  // 今の位置を返す
		return FALSE;
	}
	NextIdx = m_ANodeArray[z][x].next;  // 次の位置を返す

	return TRUE;
}

//-----------------------------------------------------------------------------
//
// 位置添字をワールド位置に変換する
//
// 引数
//   VECTOR3 Idx : 位置添字
//
// 戻り値
//   VECTOR3 位置
//
//-----------------------------------------------------------------------------
VECTOR3  CSearchRoute::ConvIdxToPos(VECTOR3 Idx)
{
	VECTOR3 vPos = VECTOR3(0, 0, 0);

	// 位置添字から位置を求める
	vPos.x = Idx.x * m_Partssize + m_Basepoint.x - m_Partssize / 2;
	vPos.z = Idx.z * m_Partssize + m_Basepoint.z + m_Partssize / 2;

	return vPos;
}
//-----------------------------------------------------------------------------
//
// ワールド位置を位置添字に変換する
//
// 引数
//   VECTOR3 vPos : 位置
//
// 戻り値
//   VECTOR3  位置添字
//
//-----------------------------------------------------------------------------
VECTOR3  CSearchRoute::ConvPosToIdx(VECTOR3 vPos)
{
	VECTOR3 Idx = VECTOR3(0, 0, 0);

	// ワールド位置を位置添字に変換する
	Idx.x = (int)((vPos.x - m_Basepoint.x) / m_Partssize);
	Idx.z = (int)((vPos.z - m_Basepoint.z) / m_Partssize);

	return Idx;
}

// ---------------------------------------------------------------------------- -
//
// 経路テーブル添字を位置添字に変換する
//
// 引数
//   int MapId : 経路テーブル添字
//
// 戻り値
//   VECTOR3  位置添字
//
//-----------------------------------------------------------------------------
VECTOR3 CSearchRoute::ConvMapIdToArrId(int MapId)
{
	int xNum = (int)(m_pGMain->m_pMapProc->GetRouteColMap()->m_MapNumXZ.x);

	//MapId -= 1;
	VECTOR3 ret = VECTOR3(0, 0, 0);
	ret.x = MapId % xNum;
	ret.z = MapId / xNum;

	return ret;
}

// ---------------------------------------------------------------------------- -
//
// 位置添字を経路テーブル添字に変換する
//
// 引数
//   VECTOR3 : 位置添字
//
// 戻り値
//   int　経路テーブル添字
//
//-----------------------------------------------------------------------------
int CSearchRoute::ConvArrIdToMapId(VECTOR3 ArrId)
{
	int xNum = (int)(m_pGMain->m_pMapProc->GetRouteColMap()->m_MapNumXZ.x);
	int ret = 1;

	ret += ArrId.z * xNum;
	ret += ArrId.x;

	return ret;
}

//-----------------------------------------------------------------------------
// 経路テーブルの作成
// CheckPoint1
//-----------------------------------------------------------------------------
BOOL CSearchRoute::MakeRouteTable()
{
	int xNum = (int)(m_pGMain->m_pMapProc->GetRouteColMap()->m_MapNumXZ.x);
	int zNum = (int)(m_pGMain->m_pMapProc->GetRouteColMap()->m_MapNumXZ.y);
	int num = xNum * zNum;
	std::vector<std::vector<int>> Map(num, std::vector<int>(num, 0));
	CSearchRoute* pSearch = new CSearchRoute(m_pGMain);
	pSearch->MakeANode();

	// 現在地点
	for (int i = 0; i < num; i++) {
		// iを位置添字iIdに変換
		VECTOR3 iId = ConvMapIdToArrId(i);
		// iIdをワールド座標iPosに変換
		VECTOR3 iPos = ConvIdxToPos(iId);

		// 目的地
		for (int j = 0; j < num; j++) {
			// iとjが同じならMap[j][i]に-1をいれてbreak
			if (i == j) {
				Map[j][i] = -1;
			}
			else {
				// jを位置添字jIdに変換
				VECTOR3 jId = ConvMapIdToArrId(j);
				// jIdをワールド座標jPosに変換
				VECTOR3 jPos = ConvIdxToPos(jId);

				// Start:iPos、End:jPosで経路探索
				pSearch->MakeANodeRoute(iPos, jPos);

				VECTOR3 NextId = VECTOR3(0, 0, 0);
				// iIdの次をNextIdに入れる
				pSearch->GetNextRoute(iId, NextId);
				// NextIdをテーブル添字nに変換
				int n = ConvArrIdToMapId(NextId);
				// Map[j][i]にnを入れる
				Map[j][i] = n;
			}
		}
	}

	// ファイルに書き出し
	FILE* fp = fopen("RouteTable.csv", "w");
	for (int i = 0; i < num; i++) {
		for (int j = 0; j < num; j++) {
			fprintf(fp, "%d,", Map[i][j]);
		}
		fprintf(fp, "\n");
	}

	fclose(fp);

	return TRUE;
}

// ---------------------------------------------------------------------------- -
// 次のグリッドのワールド座標を返す
// CheckPoint2
//-----------------------------------------------------------------------------
VECTOR3 CSearchRoute::GetNextPos(VECTOR3 currentPos, VECTOR3 goalPos, BOOL* isAreaOut)
{
	if (currentPos.x == -25.0f) {
		currentPos.x += 1.0f;
	}
	if (currentPos.x == 25.0f) {
		currentPos.x -= 1.0f;
	}

	if (currentPos.z == -25.0f) {
		currentPos.z += 1.0f;
	}
	if (currentPos.z == 25.0f) {
		currentPos.z -= 1.0f;
	}

	VECTOR3 currentId = ConvPosToIdx(currentPos);
	VECTOR3 goalId = ConvPosToIdx(goalPos);

	// 経路探索範囲外に目的地が設定されている場合
	if (goalId.x < 0 || m_pGMain->m_pMapProc->GetRouteColMap()->m_MapNumXZ.x < goalId.x)
	{
		*isAreaOut = TRUE;
		return currentPos;
	}
	if (goalId.z < 0 || m_pGMain->m_pMapProc->GetRouteColMap()->m_MapNumXZ.y < goalId.z)
	{
		*isAreaOut = TRUE;
		return currentPos;
	}
	int c = ConvArrIdToMapId(currentId);
	int g = ConvArrIdToMapId(goalId);

	int r = m_pGMain->m_pMapProc->GetRouteColMap()->m_RouteTable[g - 1][c - 1];

	VECTOR3 retId;
	if (r == -1) {
		retId = currentId;
	}
	else {
		retId = ConvMapIdToArrId(r);
	}
	VECTOR3 retPos = ConvIdxToPos(retId);

	return retPos;
}