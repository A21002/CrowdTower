//=============================================================================
//		  マップの処理
//																	Map.cpp
//=============================================================================
#include  "BaseProc.h"
#include  "Map.h"
#include  "Playchar.h"
#include  "Enemy.h"
#include  "EnemyBoss.h"
#include  "Ally.h"
#include  "Weapon.h"
#include  "Camera.h"
#include  "BackFore.h"


//-----------------------------------------------------------------------------
// マッププロシージャのコンストラクタ
//-----------------------------------------------------------------------------
CMapProc::CMapProc(CGameMain*	pGMain)
{
	// クラス全体のクリヤー
	// (vectorクラスは初期化不要)
	m_pGMain = pGMain;
	m_dwMapNo = 0;
	m_bActive = FALSE;
	m_bIsMoveMap = FALSE;
	m_bIsTutorial = FALSE;
	m_pColMesh   = NULL;					// コリジョンメッシュへのポインタ
	m_pPcColMesh = NULL;
	m_pEnmSpPart = NULL;
	m_pPcSpPart = NULL;
	m_nEvtIdx = 0;	
	m_MapIdArray[0] = { 0 };
	m_nHierarchy = 0;
}

//-----------------------------------------------------------------------------
// マッププロシージャのデストラクタ
//-----------------------------------------------------------------------------
CMapProc::~CMapProc()
{
	DestroyAll();
}

//-----------------------------------------------------------------------------
// マップの削除
//-----------------------------------------------------------------------------
void CMapProc::DestroyAll()
{
	m_dwMapNo = 0;
	m_bActive = FALSE;
	m_nEvtIdx = 0;			

	// メッシュのデリート
	for(DWORD i = 0; i<m_SkyMap.size(); i++)
	{
		SAFE_DELETE(m_SkyMap[i].m_pMesh);
		SAFE_DELETE(m_SkyMap[i].m_pWave);    // 使用していないが念のため削除
	}
	for(DWORD i = 0; i<m_StageMap.size(); i++)
	{
		SAFE_DELETE(m_StageMap[i].m_pMesh);
		SAFE_DELETE(m_StageMap[i].m_pWave);
	}
	for (DWORD i = 0; i<m_MoveMap.size(); i++)
	{
		SAFE_DELETE(m_MoveMap[i].m_pMesh);
		SAFE_DELETE(m_MoveMap[i].m_pColMesh);
	}
	for (DWORD i = 0; i < m_EventMap.size(); i++)  
	{
		SAFE_DELETE(m_EventMap[i].m_pBBox);
	}
	SAFE_DELETE(m_pColMesh);
	SAFE_DELETE(m_pPcColMesh);

	SAFE_DELETE(m_pPcSpPart);
	SAFE_DELETE(m_pEnmSpPart);

	// vectorの削除
	m_SkyMap.clear();					// 空のマップ  配列削除
	m_SkyMap.shrink_to_fit();			// 空のマップ  不要メモリ解放
	m_StageMap.clear();					// ステージのマップ  配列削除
	m_StageMap.shrink_to_fit();			// ステージのマップ 不要メモリ解放
	m_MoveMap.clear();					// 移動マップ  配列削除
	m_MoveMap.shrink_to_fit();			// 移動マップ 不要メモリ解放
	m_NavMap.clear();					// ナビゲーションマップ配列削除
	m_NavMap.shrink_to_fit();			// ナビゲーションマップ不要メモリ解放
	m_EventMap.clear();					// イベントマップ配列削除
	m_EventMap.shrink_to_fit();			// イベントマップ不要メモリ解放
	m_MapKind.clear();					// イベントマップ配列削除
	m_MapKind.shrink_to_fit();			// イベントマップ不要メモリ解放
	m_RouteColMap.m_Collision.clear();			// ルート探索コリジョン配列削除
	m_RouteColMap.m_Collision.shrink_to_fit();	// ルート探索コリジョン配列不要メモリ解放
	m_RouteColMap.m_RouteTable.clear();			// ルート探索コリジョン配列削除
	m_RouteColMap.m_RouteTable.shrink_to_fit();	// ルート探索コリジョン配列不要メモリ解放
}

//-----------------------------------------------------------------------------
// マップの作成と移動の共通処理
//-----------------------------------------------------------------------------
void CMapProc::LoadMap(const char* fileName)
{
	// vector初期化用の構造体
	const STAGEMAP      sm;
	const MOVEMAP       mm;
	const NAVIGATIONMAP nv;

	// 全マップの削除 -----------------------------------------------
	DestroyAll();

	// 全ての敵・武器・効果の非表示化 
	m_pGMain->m_pEnmProc->SetNonActive();
	m_pGMain->m_pEnmProc->ResetStartFlag();
	m_pGMain->m_pEnmBossProc->SetNonActive();
	m_pGMain->m_pAllyProc->SetNonActive();
	m_pGMain->m_pWeaponProc->SetNonActive();
	m_pGMain->m_pEffectProc->SetNonActive();
	m_pGMain->m_pCameraProc->GetCameraObjPtr()->ResetMovieFlag();
	m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetLookStairsFlag(FALSE);

	// ＰＣを(0,0,-20)の位置に置く -------------------------------------
	m_pGMain->m_pPcProc->GetPcObjPtr()->SetWorld(XMMatrixTranslation(0.0f, 0.05f, -20.0f));
	m_pGMain->m_pPcProc->GetPcObjPtr()->SetWorldOld(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());
	m_pGMain->m_pPcProc->GetPcObjPtr()->SetLocalRotY(0.0f);  // ローカル軸(Y軸)を0度にする
	m_pGMain->m_pPcProc->GetPcObjPtr()->ResetStatus();
	m_pGMain->m_pPcProc->GetPcObjPtr()->SetAllyObj();

	// ステージマップの設定・コリジョンマップの設定 -----------------------------------------
	m_pColMesh = new CCollision(m_pGMain->m_pFbxMeshCtrl);     // コリジョンマップの生成

		// 石壁の設定
	m_StageMap.push_back(sm);
	m_StageMap.back().m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Map/MapItem//Wall.mesh"));
	m_StageMap.back().m_mWorld = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	if (m_nHierarchy != 9)
	{
		// ドアメッシュの設定[1]
		m_StageMap.push_back(sm);
		m_StageMap.back().m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Map/MapItem/stairs/Staircase.mesh"));
		m_StageMap.back().m_mWorld = XMMatrixTranslation(0.0f, 0.0f, 26.0f);
		m_pColMesh->AddFbxLoad(_T("Data/Map/MapItem/stairs/Staircase.mesh"), VECTOR3(0.0f, 0.0f, 26.0f)); // マップメッシュコリジョンの設定
		SetEvent(VECTOR3(0.0f, 0.0f, -4.35f), VECTOR3(4.5f, 2.8f, 4.35f), m_StageMap.back().m_mWorld, eEvtChangeMap, 1);  // イベントの設定(マップ6へ移動する)

		m_MoveMap.push_back(mm);
		m_MoveMap.back().m_bActive = TRUE;
		m_MoveMap.back().m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Map/MapItem/stairs/Jeil.mesh"));
		//m_MoveMap.back().m_pMesh = NULL;
		m_MoveMap.back().m_pColMesh = new CCollision(m_pGMain->m_pFbxMeshCtrl);
		m_MoveMap.back().m_pColMesh->AddFbxLoad(_T("Data/Map/MapItem/stairs/Jeil_Collision.mesh"), VECTOR3(0.0f, 0.0f, 0.0f)); // マップメッシュコリジョンの設定
		m_MoveMap.back().m_pColMesh->SetWorldMatrix(XMMatrixTranslation(0.0f, 0.0f, 24.0f));						// 移動マップの位置
		m_MoveMap.back().m_pColMesh->m_mWorldOld = m_MoveMap.back().m_pColMesh->m_mWorld;							// ワールドマトリックス一つ前
		m_MoveMap.back().m_nMoveFlag = -1;																			// 初期は移動なし
		m_MoveMap.back().m_vUp = VECTOR3(0, -0.05f, 0);																// 上下移動増分
		m_MoveMap.back().m_vMin = VECTOR3(0, -6.5f, 0);																// 下限値
		m_MoveMap.back().m_nChangeFlag = 0;																			// チェンジフラグのリセット
		m_MoveMap.back().m_nLoop = 0;																				// ループしない
	}
	else {
		// ボスのスポーン
		m_pGMain->m_pEnmBossProc->Start(VECTOR3(0.0f, 0.05f, 21.0f));
	}

	m_StageMap.push_back(sm);
	m_StageMap.back().m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Map/Sky/sky2.mesh"));
	m_StageMap.back().m_mWorld = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	
	// スクリプト読み取り
	FILE* fp = fopen(fileName, "r");
	if (fp != NULL)
	{
		// 「-1」を読み取るまで無限ループ
		while (1)
		{
			int id = -1;								// 行う処理の種類
			int areaID = 0;								// ダメージエリアの種類
			char address[256];							// UTF-8形式での文字列
			TCHAR Taddress[256];						// TCHAR形式での文字列
			VECTOR3 vPos = VECTOR3(0.0f, 0.0f, 0.0f);	// メッシュ・コリジョンの座標
			VECTOR3 vMin = VECTOR3(0.0f, 0.0f, 0.0f);	// ボックスの左下の座標
			VECTOR3 vMax = VECTOR3(0.0f, 0.0f, 0.0f);	// ボックスの右上の座標

			// 行う処理の種類を読み取り
			fscanf(fp, "%d,", &id);
			switch (id) {
			case 0:
				// メッシュの表示
				fscanf(fp, "%s", address);
				fscanf(fp, "%f, %f, %f", &vPos.x, &vPos.y, &vPos.z);
				MyImgui::ConvertU8ToU16(address, Taddress);				// UTF-8形式からTCHAR形式に変換
				m_StageMap.push_back(sm);
				m_StageMap.back().m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, Taddress);
				m_StageMap.back().m_mWorld = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
				break;

			case 1:
				// コリジョンメッシュの設定
				fscanf(fp, "%s", address);
				fscanf(fp, "%f, %f, %f", &vPos.x, &vPos.y, &vPos.z);
				MyImgui::ConvertU8ToU16(address, Taddress);				// UTF-8形式からTCHAR形式に変換
				m_pColMesh->AddFbxLoad(Taddress, vPos);
				break;

			case 2:
				// カメラとPCの直線上にあると透過させるオブジェクトのボックス
				fscanf(fp, "%f, %f, %f, %f, %f, %f", &vMin.x, &vMin.y, &vMin.z, &vMax.x, &vMax.y, &vMax.z);
				m_StageMap.back().m_pBox = new CBBox(m_pGMain->m_pShader, vMin, vMax);
				m_StageMap.back().m_pBox->m_mWorld = m_StageMap.back().m_mWorld;
				break;

			case 3:
				if (m_nHierarchy != 9)
				{
					// 敵のスポーン範囲
					fscanf(fp, "%f, %f, %f, %f, %f, %f", &vMin.x, &vMin.y, &vMin.z, &vMax.x, &vMax.y, &vMax.z);
					m_NavMap.push_back(nv);
					m_NavMap.back().vMin = vMin;
					m_NavMap.back().vMax = vMax;
				}
				break;

			case 4:
				// ダメージエリアの範囲
				fscanf(fp, "%f, %f, %f, %f, %f, %f, %d", &vMin.x, &vMin.y, &vMin.z, &vMax.x, &vMax.y, &vMax.z, &areaID);
				m_pGMain->m_pWeaponProc->m_pWeaponDamageAreaProc->Start(vMin, vMax, areaID);
				break;

			case -1:
				break;
			}

			if (id == -1)
			{
				break;
			}
		}
		fclose(fp);
	}

	m_pGMain->m_pEnmProc->ResetEnmNum();      // 敵の発生数のリセット

	// 空間分割の生成             
	VECTOR3 vMin, vMax;
	m_pColMesh->GetChkAABB(vMin, vMax);
	m_pPcSpPart = new CSpPart(m_pGMain);
	m_pPcSpPart->CleateSpPart(vMin, vMax, 3);

	m_pEnmSpPart = new CSpPart(m_pGMain);
	m_pEnmSpPart->CleateSpPart(vMin, vMax, 3);

	m_bActive = TRUE;
}

//-----------------------------------------------------------------------------
// マップ１の作成と移動の処理
//-----------------------------------------------------------------------------
void CMapProc::LoadMap1()
{
	// vector初期化用の構造体
	const MOVEMAP       mm;

	LoadMap("Data/Map/Tower/Map1/Script.txt");

	// 柵の設置
	m_MoveMap.push_back(mm);
	m_MoveMap.back().m_bActive = TRUE;
	m_MoveMap.back().m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Map/Tower/Map1/LeftWall.mesh"));
	m_MoveMap.back().m_pColMesh = new CCollision(m_pGMain->m_pFbxMeshCtrl);
	m_MoveMap.back().m_pColMesh->AddFbxLoad(_T("Data/Map/Tower/Map1/Wall_Collision.mesh"), VECTOR3(0, 0, 0)); // マップメッシュコリジョンの設定
	m_MoveMap.back().m_pColMesh->SetWorldMatrix(XMMatrixTranslation(-7.5f, 0.0f, -15.5f));						// 移動マップの位置
	m_MoveMap.back().m_pColMesh->m_mWorldOld = m_MoveMap.back().m_pColMesh->m_mWorld;							// ワールドマトリックス一つ前
	m_MoveMap.back().m_nMoveFlag = 0;																			// 初期は移動なし
	m_MoveMap.back().m_vUp = VECTOR3(-0.05f, 0, 0);																// 上下移動増分
	m_MoveMap.back().m_vMin = VECTOR3(-22.5f, 0, 0);																// 下限値
	m_MoveMap.back().m_nChangeFlag = 0;																			// チェンジフラグのリセット
	m_MoveMap.back().m_nLoop = 0;

	m_MoveMap.push_back(mm);
	m_MoveMap.back().m_bActive = TRUE;
	m_MoveMap.back().m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Map/Tower/Map1/RightWall.mesh"));
	m_MoveMap.back().m_pColMesh = new CCollision(m_pGMain->m_pFbxMeshCtrl);
	m_MoveMap.back().m_pColMesh->AddFbxLoad(_T("Data/Map/Tower/Map1/Wall_Collision.mesh"), VECTOR3(0, 0, 0)); // マップメッシュコリジョンの設定
	m_MoveMap.back().m_pColMesh->SetWorldMatrix(XMMatrixTranslation(7.5f, 0.0f, -15.5f));						// 移動マップの位置
	m_MoveMap.back().m_pColMesh->m_mWorldOld = m_MoveMap.back().m_pColMesh->m_mWorld;							// ワールドマトリックス一つ前
	m_MoveMap.back().m_nMoveFlag = 0;																			// 初期は移動なし
	m_MoveMap.back().m_vUp = VECTOR3(0.05f, 0, 0);																// 上下移動増分
	m_MoveMap.back().m_vMax = VECTOR3(22.5f, 0, 0);																// 下限値
	m_MoveMap.back().m_nChangeFlag = 0;																			// チェンジフラグのリセット
	m_MoveMap.back().m_nLoop = 0;

	SetEvent(VECTOR3(-7.5f, 0.0f, -1.5f), VECTOR3(7.5f, 0.0f, 1.5f), XMMatrixTranslation(0.0f, 0.0f, -15.5f), eEvtMoveMap, 1, 2, 1);  // イベントの設定(マップ6へ移動する)

	// ルート探索コリジョンマップの作成  --------------------------------
	MakeRouteColMap(VECTOR2(50, 40), 2.0f, VECTOR3(-25, 0, -15), "Data/Map/Tower/Map1/RouteTable.csv");

	const int MapNum = 10;
	for (int i = 1; i < MapNum; i++) {
		m_MapKind.push_back(i + 1);
	}
	
	// マップの順番をランダムで設定
	for (int i = 0; i < MapNum - 1; i++) {
		int tmp = Random(0, m_MapKind.size() - 1);
		m_MapIdArray[i] = m_MapKind[tmp];
		m_MapKind.erase(m_MapKind.begin() + tmp);
	}

	// チュートリアルの表示
	if (m_bIsTutorial) {
		m_pGMain->m_pBackForeProc->m_nDialogKind = DIALOG_TUTORIAL1;
		m_pGMain->m_isMove = FALSE;
	}	

}

//-----------------------------------------------------------------------------
// マップ２の作成と移動の処理
//-----------------------------------------------------------------------------
void CMapProc::LoadMap2()
{
	LoadMap("Data/Map/Tower/Map2/Script.txt");

	// ルート探索コリジョンマップの作成  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map2/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// マップ３の作成と移動の処理
//-----------------------------------------------------------------------------
void CMapProc::LoadMap3()
{
	LoadMap("Data/Map/Tower/Map3/Script.txt");

	// ルート探索コリジョンマップの作成  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map3/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// マップ４の作成と移動の処理
//-----------------------------------------------------------------------------
void CMapProc::LoadMap4()
{
	LoadMap("Data/Map/Tower/Map4/Script.txt");

	// ルート探索コリジョンマップの作成  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map4/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// マップ５の作成と移動の処理
//-----------------------------------------------------------------------------
void CMapProc::LoadMap5()
{
	LoadMap("Data/Map/Tower/Map5/Script.txt");

	// ルート探索コリジョンマップの作成  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map5/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// マップ６の作成と移動の処理
//-----------------------------------------------------------------------------
void CMapProc::LoadMap6()
{
	LoadMap("Data/Map/Tower/Map6/Script.txt");

	// ルート探索コリジョンマップの作成  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map6/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// マップ７の作成と移動の処理
//-----------------------------------------------------------------------------
void CMapProc::LoadMap7()
{
	LoadMap("Data/Map/Tower/Map7/Script.txt");

	// PC専用コリジョン
	m_pPcColMesh = new CCollision(m_pGMain->m_pFbxMeshCtrl);
	m_pPcColMesh->AddFbxLoad(_T("Data/Map/Tower/Map7/PC_Collision.mesh"), VECTOR3(0.0f, 0.0f, 0.0f));
	
	// ルート探索コリジョンマップの作成  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map7/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// マップ８の作成と移動の処理
//-----------------------------------------------------------------------------
void CMapProc::LoadMap8()
{
	LoadMap("Data/Map/Tower/Map8/Script.txt");

	// PC専用コリジョン
	m_pPcColMesh = new CCollision(m_pGMain->m_pFbxMeshCtrl);
	m_pPcColMesh->AddFbxLoad(_T("Data/Map/Tower/Map8/PC_Collision.mesh"), VECTOR3(0.0f, 0.0f, 0.0f));

	// ルート探索コリジョンマップの作成  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map8/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// マップ９の作成と移動の処理
//-----------------------------------------------------------------------------
void CMapProc::LoadMap9()
{
	// vector初期化用の構造体
	LoadMap("Data/Map/Tower/Map9/Script.txt");

	// ルート探索コリジョンマップの作成  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map9/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// マップ１０の作成と移動の処理
//-----------------------------------------------------------------------------
void CMapProc::LoadMap10()
{
	LoadMap("Data/Map/Tower/Map10/Script.txt");

	// ルート探索コリジョンマップの作成  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map10/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// マップ１１の作成と移動の処理
//-----------------------------------------------------------------------------
void CMapProc::LoadMap11()
{
	LoadMap("Data/Map/Tower/Map11/Script.txt");

	// PC専用コリジョン
	m_pPcColMesh = new CCollision(m_pGMain->m_pFbxMeshCtrl);
	m_pPcColMesh->AddFbxLoad(_T("Data/Map/Tower/Map11/PC_Collision.mesh"), VECTOR3(0.0f, 0.0f, 0.0f));

	// ルート探索コリジョンマップの作成  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map11/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// マップ１２の作成と移動の処理
//-----------------------------------------------------------------------------
void CMapProc::LoadMap12()
{
	LoadMap("Data/Map/Tower/Map12/Script.txt");

	// PC専用コリジョン
	m_pPcColMesh = new CCollision(m_pGMain->m_pFbxMeshCtrl);
	m_pPcColMesh->AddFbxLoad(_T("Data/Map/Tower/Map12/PC_Collision.mesh"), VECTOR3(0.0f, 0.0f, 0.0f));

	// ルート探索コリジョンマップの作成  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map12/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// マップ１３の作成と移動の処理
//-----------------------------------------------------------------------------
void CMapProc::LoadMap13()
{
	LoadMap("Data/Map/Tower/Map13/Script.txt");

	// ルート探索コリジョンマップの作成  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map13/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// マップ１４の作成と移動の処理
//-----------------------------------------------------------------------------
void CMapProc::LoadMap14()
{
	LoadMap("Data/Map/Tower/Map14/Script.txt");

	// ルート探索コリジョンマップの作成  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map14/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// マップ１５の作成と移動の処理
//-----------------------------------------------------------------------------
void CMapProc::LoadMap15()
{
	LoadMap("Data/Map/Tower/Map15/Script.txt");

	// ルート探索コリジョンマップの作成  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map15/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// マップ１６の作成と移動の処理
//-----------------------------------------------------------------------------
void CMapProc::LoadMap16()
{
	LoadMap("Data/Map/Tower/Map16/Script.txt");

	// ルート探索コリジョンマップの作成  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map16/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// マップ１７の作成と移動の処理
//-----------------------------------------------------------------------------
void CMapProc::LoadMap17()
{
	LoadMap("Data/Map/Tower/Map17/Script.txt");

	// ルート探索コリジョンマップの作成  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map17/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// マップ１８の作成と移動の処理
//-----------------------------------------------------------------------------
void CMapProc::LoadMap18()
{
	LoadMap("Data/Map/Tower/Map18/Script.txt");

	// ルート探索コリジョンマップの作成  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map18/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// マップ１９の作成と移動の処理
//-----------------------------------------------------------------------------
void CMapProc::LoadMap19()
{
	LoadMap("Data/Map/Tower/Map19/Script.txt");

	// ルート探索コリジョンマップの作成  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map19/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// マップ２０の作成と移動の処理
//-----------------------------------------------------------------------------
void CMapProc::LoadMap20()
{
	LoadMap("Data/Map/Tower/Map20/Script.txt");

	// ルート探索コリジョンマップの作成  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map20/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// マッププロシージャのあたり判定
//
// ・相手（主にＰＣの武器）とコリジョンマップとのあたり判定
//
//   CBaseObj*    pObj      相手のオブジェクト
//   VECTOR3* pHit		接触点の座標（出力）
//   VECTOR3* pNormal	接触点の法線ベクトル（出力）

//
//   戻り値　BOOL
//           TRUE:ヒットしたとき       FALSE：ヒットしていないとき
//-----------------------------------------------------------------------------
BOOL   CMapProc::Hitcheck(CBaseObj* pObj, VECTOR3* pHit, VECTOR3* pNormal)
{
	return  Hitcheck(GetPositionVector(pObj->GetWorld()), GetPositionVector(pObj->GetWorldOld()), pHit, pNormal);
}

//-----------------------------------------------------------------------------
// マッププロシージャのあたり判定
//
// ・相手（主にＰＣの武器）とコリジョンマップとのあたり判定
//
//   VECTOR3 vNow       相手の現在位置
//   VECTOR3 vOld       相手の一つ前の位置
//   VECTOR3* vHit		接触点の座標（出力）
//   VECTOR3* vNormal	接触点の法線ベクトル（出力）
//
//   戻り値　BOOL
//           TRUE:ヒットしたとき       FALSE：ヒットしていないとき
//-----------------------------------------------------------------------------
BOOL   CMapProc::Hitcheck(VECTOR3 vNow, VECTOR3 vOld, VECTOR3* pHit, VECTOR3* pNormal)
{
	BOOL  bRet;
	for (int i = 0; i < m_MoveMap.size(); i++)
	{
		if (m_MoveMap[i].m_pColMesh && m_MoveMap[i].m_bActive)
		{
			bRet = m_MoveMap[i].m_pColMesh->isCollisionLay(vNow, vOld, *pHit, *pNormal);
			if (bRet) return bRet;
		}
	}

	if (m_pColMesh && m_pColMesh->isCollisionLay(vNow, vOld, *pHit, *pNormal))
	{
		return TRUE;
	}
	else {
		return FALSE;
	}
}

//----------------------------------------------------------------------------- 
// マッププロシージャのあたり判定
//
// ・相手（主にＰＣの武器）の球とコリジョンマップとのあたり判定
//
//   VECTOR3 vNow       相手の現在のオブジェクト中心位置
//   VECTOR3 vOld       相手の一つ前のオブジェクト中心位置
//   float& fRadius     球の半径
//   VECTOR3 &vHit      接触時のオブジェクト中心位置の座標（出力）
//   VECTOR3 &vNormal   接触点の法線ベクトル（出力）
//
//   戻り値　BOOL
//           TRUE:ヒットしたとき       FALSE：ヒットしていないとき
//-----------------------------------------------------------------------------
BOOL   CMapProc::Hitcheck(VECTOR3 vNow, VECTOR3 vOld, float fRadius, VECTOR3* pHit, VECTOR3* pNormal)
{
	BOOL  bRet;
	for (int i = 0; i < m_MoveMap.size(); i++)
	{
		if (m_MoveMap[i].m_pColMesh && m_MoveMap[i].m_bActive)
		{
			bRet = m_MoveMap[i].m_pColMesh->isCollisionSphere(vNow, vOld, fRadius, *pHit, *pNormal);
			if (bRet) return bRet;
		}
	}

	if (m_pColMesh && m_pColMesh->isCollisionSphere(vNow, vOld, fRadius, *pHit, *pNormal))
	{
		return TRUE;
	}
	else {
		return FALSE;
	}

}

//-----------------------------------------------------------------------------
// オブジェクトのレイとメッシュ接触判定用配列との接触判定とスリスリ動かす制御
//	高低差と重力を考慮した、接触判定を行う						
//  
//  MATRIX4X4* pWorld     オブジェクトの現在のマトリックス(in,out)
//  MATRIX4X4  mWorldOld  オブジェクトの一つ前のマトリックス		
//  float fRadius          オブジェクトの半径（省略値は0.2）
//  
//  戻り値　int
//		エラー		= -1
//		面上を移動	= 1
//		着地		= 2
//		落下中		= 3
//-----------------------------------------------------------------------------
int  CMapProc::isCollisionMoveGravity(MATRIX4X4* pWorld, MATRIX4X4 mWorldOld, float fRadius, DWORD dwTag)
{
	int  nRet = 0, nRetMove = 0; 

	if (dwTag != ENM) {
		// 移動マップとの接触判定と移動
		for (int i = 0; i < m_MoveMap.size(); i++)
		{
			if (m_MoveMap[i].m_pColMesh && m_MoveMap[i].m_bActive)
			{
				nRetMove = m_MoveMap[i].m_pColMesh->isCollisionMoveGravity(*pWorld, mWorldOld, fRadius);
				if (nRetMove == 1 || nRetMove == 2) break;   // 移動マップと接触したとき
			}
		}
	}

	// 通常のマップとの接触判定と移動
	if (m_pColMesh)
	{
		nRet = m_pColMesh->isCollisionMoveGravity(*pWorld, mWorldOld, fRadius);  
	}

	if (nRetMove == 1 || nRetMove == 2)  // 移動マップと接触していたとき  
	{
		return nRetMove;
	}
	else {
		return nRet;
	}
}

//-----------------------------------------------------------------------------
// オブジェクトのレイとメッシュ接触判定用配列との接触判定とスリスリ動かすPC専用の制御
//	高低差と重力を考慮した、接触判定を行う
//  
//  MATRIX4X4* pWorld     オブジェクトの現在のマトリックス(in,out)
//  MATRIX4X4  mWorldOld  オブジェクトの一つ前のマトリックス		
//  float fRadius          オブジェクトの半径（省略値は0.2）
//  
//  戻り値　int
//		エラー		= -1
//		面上を移動	= 1
//		着地		= 2
//		落下中		= 3
//-----------------------------------------------------------------------------
int CMapProc::isCollisionMoveGravityPc(MATRIX4X4* pWorld, MATRIX4X4 mWorldOld, float fRadius)
{
	int nRet = 0, nRetMove = 0;
	if (m_pPcColMesh) {
		// 移動マップとの接触判定と移動
			for (int i = 0; i < m_MoveMap.size(); i++)
			{
				if (m_MoveMap[i].m_pColMesh && m_MoveMap[i].m_bActive)
				{
					nRetMove = m_MoveMap[i].m_pColMesh->isCollisionMoveGravity(*pWorld, mWorldOld, fRadius);
					if (nRetMove == 1 || nRetMove == 2) break;   // 移動マップと接触したとき
				}
			}

		// 通常のマップとの接触判定と移動
		if (m_pPcColMesh)
		{
			nRet = m_pPcColMesh->isCollisionMoveGravity(*pWorld, mWorldOld, fRadius);
		}
	}
	else {
		// 設定されていない場合
		nRet = isCollisionMoveGravity(pWorld, mWorldOld, fRadius);
	}

	if (nRetMove == 1 || nRetMove == 2)  // 移動マップと接触していたとき
	{
		return nRetMove;
	}
	else {
		return nRet;
	}
}

//-----------------------------------------------------------------------------
// オブジェクトのレイとメッシュ接触判定用配列との接触判定とスリスリ動かす制御
//	重力を考えず、接触判定を行う									
//  
//  MATRIX4X4* pWorld     オブジェクトの現在のマトリックス(in,out)
//  MATRIX4X4  mWorldOld  オブジェクトの一つ前のマトリックス		
//  float fRadius          オブジェクトの半径（省略値は0.2）
//  
//  戻り値　int
//		接触したとき　		１
//		接触していないとき	０
//-----------------------------------------------------------------------------
int CMapProc::isCollisionMove(MATRIX4X4* pWorld, MATRIX4X4 mWorldOld, float fRadius)
{
	int  nRet = 0, nRetMove = 0;  // -- 2019.9.3

	// 移動マップとの接触判定と移動
	for (int i = 0; i < m_MoveMap.size(); i++)
	{
		if (m_MoveMap[i].m_pColMesh && m_MoveMap[i].m_bActive)
		{
			nRetMove = m_MoveMap[i].m_pColMesh->isCollisionMove(*pWorld, mWorldOld, fRadius);
			if (nRetMove == 1) break;
		}
	}

	// 通常のマップとの接触判定と移動
	if (m_pColMesh)
	{
		nRet = m_pColMesh->isCollisionMove(*pWorld, mWorldOld, fRadius); 
	}

	if (nRetMove == 1)  // 移動マップと接触していたとき     // -- 2019.9.3
	{
		return nRetMove;
	}
	else {
		return nRet;
	}
}

//-----------------------------------------------------------------------------
// オブジェクトのレイとメッシュ接触判定用配列との接触判定とスリスリ動かす制御
//	重力を考えず、接触判定を行う									
//  
//  MATRIX4X4* pWorld     オブジェクトの現在のマトリックス(in,out)
//  MATRIX4X4  mWorldOld  オブジェクトの一つ前のマトリックス		
//  float fRadius          オブジェクトの半径（省略値は0.2）
//  
//  戻り値　int
//		接触したとき　		１
//		接触していないとき	０
//-----------------------------------------------------------------------------
int CMapProc::isCollisionMoveWall(MATRIX4X4 mWorld, MATRIX4X4 mWorldOld, float fRadius)
{
	int ret = 0;
	ret = m_pColMesh->CheckWallMove(mWorld, mWorldOld, fRadius);
	if (ret == 1) {
		m_pColMesh->CheckWallMove(mWorld, mWorldOld, fRadius);
	}

	return ret;
}

//-----------------------------------------------------------------------------
// 経路探索用のコリジョン配列の生成
//
// 引数
//   VECTOR2 vMapSizeXZ : 経路探索配列の平面上の大きさ（ｍ）
//   float   partssize  : 配列要素（ノード）一つの大きさ（ｍ）
//   VECTOR3 vBasepoint : 経路探索配列左手前の基点（ｍ）
//-----------------------------------------------------------------------------
void CMapProc::MakeRouteColMap(VECTOR2 vMapSizeXZ, float partssize, VECTOR3 vBasepoint, const char* fileName)
{
	
	const int LimitHeight = 0.7f;    // 壁と見なす高さ

	m_RouteColMap.m_Partssize = partssize;		// ノード一つの大きさ
	m_RouteColMap.m_Basepoint = vBasepoint;		// 配列の左手前の座標値

	int znum = (int)(vMapSizeXZ.y / partssize);   // 1次元目要素数
	int xnum = (int)(vMapSizeXZ.x / partssize);   // 2次元目要素数
	m_RouteColMap.m_MapNumXZ = VECTOR2((float)xnum, (float)znum);   // ノード配列の大きさ(要素数)

	m_RouteColMap.m_Collision.clear();			// ルート探索コリジョン配列削除
	m_RouteColMap.m_Collision.shrink_to_fit();	// ルート探索コリジョン配列不要メモリ解放
	m_RouteColMap.m_RouteTable.clear();
	m_RouteColMap.m_RouteTable.shrink_to_fit();

	//	経路コリジョン配列のサイズの初期化
	m_RouteColMap.m_Collision.resize(znum, std::vector<int>(xnum));
	m_RouteColMap.m_RouteTable.resize(znum * xnum, std::vector<int>(znum * xnum));
		
	FILE* fp = fopen(fileName, "r");
	
	for (int i = 0; i < xnum * znum; i++)
	{
		for (int j = 0; j < xnum * znum; j++)
		{
			fscanf(fp, "%d,", &m_RouteColMap.m_RouteTable[i][j]);
		}
	}
	fclose(fp);
}

//-----------------------------------------------------------------------------
// 経路探索配列の平面上の大きさ（ｍ）と左手前の基点（ｍ）を求める	
//
// (コリジョンマップAABBの最小値vMin,最大値vMaxから算出する)
//
// 引数
//   VECTOR2 vMapSizeXZ : 経路探索配列の平面上の大きさ（ｍ）(Out)
//   VECTOR3 vBasepoint : 経路探索配列左手前の基点（ｍ）(Out)
//-----------------------------------------------------------------------------
void CMapProc::GetMapsizeAndBasepoint(VECTOR2& vMapSizeXZ, VECTOR3& vBasepoint)
{

	VECTOR3 vMin;
	VECTOR3 vMax;

	// コリジョンマップの大きさを得る
	// (大きさは、マップAABBの最小値と最大値で得られる)
	m_pColMesh->GetChkAABB(vMin, vMax);

	vMapSizeXZ = VECTOR2(vMax.x - vMin.x, vMax.z - vMin.z);
	vBasepoint.x = vMin.x;
	vBasepoint.y = 0;
	vBasepoint.z = vMin.z;
}

//-----------------------------------------------------------------------------
// マッププロシージャの更新
//-----------------------------------------------------------------------------
void CMapProc::Update()
{
	CDirectInput* pDI = m_pGMain->m_pDI;

	// イベントの更新処理                   
	UpdateEvent();

	// 空の更新　　カメラを中心とした位置に置き、Ｙ軸回転させる
	if (m_SkyMap.size() > 0)
	{
		MATRIX4X4 mRot;
		mRot = XMMatrixRotationY(XMConvertToRadians(0.005f));
		mRot = mRot * GetRotateMatrix(m_SkyMap[0].m_mWorld);
		m_SkyMap[0].m_mWorld = XMMatrixTranslation( m_pGMain->m_vEyePt.x, m_pGMain->m_vEyePt.y, m_pGMain->m_vEyePt.z);
		m_SkyMap[0].m_mWorld = mRot * m_SkyMap[0].m_mWorld;
	}

	// 移動マップ更新処理
	UpdateMoveMap();

	// マップ移動
	if (m_bIsMoveMap) {
		// 階段に触れている
		if (m_nHierarchy == 10) {
			// クリア画面への遷移
			if (m_pGMain->m_pBackForeProc->GetWhiteOut()) {
				m_pGMain->m_dwGameStatus = GAMECLEAR;
				m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetCtrl(CAMERA_GAMECLEAR);
			}
		}
		else
		if (m_pGMain->m_pBackForeProc->GetFadeInFinish())
		{
			// フェードアウトが終わっている
			MoveMap();
			m_bIsMoveMap = FALSE;
			m_pGMain->m_pBackForeProc->SetFade(FALSE);
		}
	}

	//レンダリング
	Render();

	// 透過をリセット
	ResetStageMeshAlfa();
}

//----------------------------------------------------------------------------- 
// イベントのセット
//
// 引数
//   VECTOR3 vMin         : バウンディングボックス最小値
//   VECTOR3 vMax         : バウンディングボックス最大値
//   MATRIX4X4 mWorld     : バウンディングボックスのワールドマトリックス
//   EVENTKIND nEvtKind   : イベントの種類 eEvtChangeMap:マップ移動, eEvtMoveMap:移動マップの制御
//   int nEvtNo           : イベント番号 eEvtChangeMapのときは移動先のマップ番号、eEvtMoveMapのときは移動マップの要素番号
//   int nEvtOpe1         : eEvtMoveMapのみ。1:移動マップの移動Activeの指定。2:移動マップの移動フラグの指定(省略値0)
//   int nEvtOpe2         : eEvtMoveMapのみ。移動Activeのときは0:非表示 1:表示  移動フラグのときは　0:移動無し　1:平行移動　2:回転　3:拡大縮小(省略値0)
//   int nEvtKeyPush      : 0:バウンディングボックスに接触するとイベント発動、 1:バウンディングボックスに接触して、ENTERキーを押すとイベント発動(省略値0)
//
// 戻り値
//   int セットしたイベント配列の要素番号
//-----------------------------------------------------------------------------
int  CMapProc::SetEvent(VECTOR3 vMin, VECTOR3 vMax, MATRIX4X4 mWorld, EVENTKIND nEvtKind, int nEvtNo, int nEvtOpe1, int nEvtOpe2, int nEvtKeyPush)
{
	const EVENTMAP      em;

	m_EventMap.push_back(em);

	// バウンディングボックスの設定
	m_EventMap[m_nEvtIdx].m_pBBox = new CBBox(m_pGMain->m_pShader, vMin, vMax);
	m_EventMap[m_nEvtIdx].m_pBBox->m_mWorld = mWorld;

	// 各項目の設定
	m_EventMap[m_nEvtIdx].m_nEvtCycle = 0;
	m_EventMap[m_nEvtIdx].m_nEvtKind = nEvtKind;
	m_EventMap[m_nEvtIdx].m_nEvtNo = nEvtNo;
	m_EventMap[m_nEvtIdx].m_nEvtOpe1 = nEvtOpe1;
	m_EventMap[m_nEvtIdx].m_nEvtOpe2 = nEvtOpe2;
	m_EventMap[m_nEvtIdx].m_nEvtKeyPush = nEvtKeyPush;

	m_nEvtIdx++;
	return  m_nEvtIdx - 1;
}

//-----------------------------------------------------------------------------  
// イベントの更新・実行
//-----------------------------------------------------------------------------
void  CMapProc::UpdateEvent()
{
	VECTOR3 vHit, vNrm;

	for (int i = 0; i < m_EventMap.size(); i++)
	{
		// PCのバウンディングボックスに接触しているかの判定
		if (m_EventMap[i].m_pBBox && m_EventMap[i].m_pBBox->OBBCollisionDetection(m_pGMain->m_pPcProc->GetPcObjPtr()->GetBBox(), &vHit, &vNrm))
		{
			// PCのバウンディングボックスに接触しているとき
			if (m_EventMap[i].m_nEvtCycle == 0)  // イベントサイクルが０のとき(まだ接触していなかったとき)
			{
				if (m_EventMap[i].m_nEvtKeyPush == 1 && !(m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_RETURN) || m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A)))  // Enterキープッシュが必要なとき
				{
					VECTOR3 vPc = GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());
					vPc.y += 2.0f;
					// PCの頭上に、Enterキープッシュが行われていない事の表示をする
					m_pGMain->m_pFont->Draw3D(vPc, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vEyePt, _T("Push Enter Key!"), VECTOR2(0.8f, 0.2f), RGB(255, 0, 0), 1.0f, _T("HGP創英角ｺﾞｼｯｸUB"));
				}
				else {
					// イベント実行
					RunEvent(m_EventMap[i]);
				}
			}
		}
		else {
			// PCのバウンディングボックスに接触していないとき
			if (m_EventMap[i].m_nEvtCycle == 2)  // イベントサイクルが２のとき(既に接触していて終了待ちのとき)
			{
				m_EventMap[i].m_nEvtCycle = 0;  // イベントサイクルを０に戻す
			}
		}
	}
}

//----------------------------------------------------------------------------- 
// イベントの実行
//
// 引数  EVENTMAP& EventMap:イベントマップ
//-----------------------------------------------------------------------------
void  CMapProc::RunEvent(EVENTMAP& EventMap)
{
	int i;
	if (EventMap.m_nEvtKind == eEvtChangeMap)   // マップ移動のとき
	{
		if (!m_bIsMoveMap) {
			if (m_nHierarchy == 10) {
				m_pGMain->m_pBackForeProc->SetBlackOut(TRUE);
			}
			else {
				m_pGMain->m_pBackForeProc->SetFade(TRUE);
			}
			m_bIsMoveMap = TRUE;
		}
	}
	else if (EventMap.m_nEvtKind == eEvtMoveMap)  // 移動マップの制御のとき
	{
		i = EventMap.m_nEvtNo;
		if (EventMap.m_nEvtOpe1 == 1)   // 表示有無に関する処理
		{
			m_MoveMap[i].m_bActive = EventMap.m_nEvtOpe2;
		}
		else if (EventMap.m_nEvtOpe1 == 2)   // 移動に関する処理
		{
			m_MoveMap[i].m_nMoveFlag = EventMap.m_nEvtOpe2;
			m_MoveMap[i + 1].m_nMoveFlag = EventMap.m_nEvtOpe2;
		}
		EventMap.m_nEvtCycle = 1;  // イベントサイクルを２にする
	}
}

//-----------------------------------------------------------------------------
// 移動マップの更新
//
// 移動マップの平行移動や回転の更新を行う
// 限界値（上端下端等）に達したときは、反転（change）を行う
// 反転（change）をしたときは、m_nChangeFlagに１が返る
// ループをしないときは反転のタイミングで停止する   
//-----------------------------------------------------------------------------
void CMapProc::UpdateMoveMap()
{
	for (DWORD i = 0; i < m_MoveMap.size(); i++)
	{
		if (m_MoveMap[i].m_pColMesh && m_MoveMap[i].m_bActive)   // 移動マップの更新処理
		{
			m_MoveMap[i].m_pColMesh->m_mWorldOld = m_MoveMap[i].m_pColMesh->m_mWorld;   // ワールドマトリックス一つ前
			if (m_pGMain->m_pEnmProc->CountActiveEnm() == 0 && m_MoveMap[0].m_nMoveFlag == -1) {
				// 敵が全滅した時
				m_MoveMap[0].m_nMoveFlag = 1;
				m_pGMain->m_pDownCage->Play(AUDIO_LOOP);
			}
			
			m_MoveMap[i].m_nChangeFlag = 0;      // チェンジフラグのリセット
			if (m_MoveMap[i].m_nMoveFlag == 1)   // 平行移動の処理
			{
				MATRIX4X4 mPosUp;
				mPosUp = XMMatrixTranslation(m_MoveMap[i].m_vUp.x, m_MoveMap[i].m_vUp.y, m_MoveMap[i].m_vUp.z);
				m_MoveMap[i].m_pColMesh->SetWorldMatrix(mPosUp * m_MoveMap[i].m_pColMesh->m_mWorld);  // 移動増分の処理
				if (m_MoveMap[i].m_vUp.x != 0)  // 左右に動かす
				{
					if (GetPositionVector(m_MoveMap[i].m_pColMesh->m_mWorld).x < m_MoveMap[i].m_vMin.x ||  // 左右に動かすため右端左端で反転
						GetPositionVector(m_MoveMap[i].m_pColMesh->m_mWorld).x > m_MoveMap[i].m_vMax.x )
					{
						m_MoveMap[i].m_vUp.x *= -1;
						m_MoveMap[i].m_nChangeFlag = 1;
					}
				}
				if (m_MoveMap[i].m_vUp.y != 0)  // 上下に動かす
				{
					if (GetPositionVector(m_MoveMap[i].m_pColMesh->m_mWorld).y < m_MoveMap[i].m_vMin.y ||  // 上下に動かすため上端下端で反転
						GetPositionVector(m_MoveMap[i].m_pColMesh->m_mWorld).y > m_MoveMap[i].m_vMax.y)
					{
						m_MoveMap[i].m_vUp = VECTOR3(0.0f, 0.0f, 0.0f);
						m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetLookStairsFlag(false);
						m_pGMain->m_pDownCage->Stop();
						m_pGMain->m_pWeaponProc->m_pWeaponItemBoxProc->Start(VECTOR3(0.0f, 0.0f, 20.0f));
						if (m_nHierarchy == 1 && !m_pGMain->m_bIsBoxTutorial)
						{
							m_pGMain->m_bIsBoxTutorial = TRUE;
							m_pGMain->m_pBackForeProc->m_nDialogKind = DIALOG_TUTORIAL3;
							m_pGMain->m_isMove = FALSE;
						}
					}
				}
				if (m_MoveMap[i].m_vUp.z != 0)  // 前後に動かす
				{
					if (GetPositionVector(m_MoveMap[i].m_pColMesh->m_mWorld).z < m_MoveMap[i].m_vMin.z ||  // 前後に動かすため前端後端で反転
						GetPositionVector(m_MoveMap[i].m_pColMesh->m_mWorld).z > m_MoveMap[i].m_vMax.z)
					{
						m_MoveMap[i].m_vUp.z *= -1;
						m_MoveMap[i].m_nChangeFlag = 1;
					}
				}
			}
			else if (m_MoveMap[i].m_nMoveFlag == 2){   // 回転の処理
				MATRIX4X4 mRotUp;
				if (m_MoveMap[i].m_vUp.x != 0)  // Ｘ回転
				{
					mRotUp = XMMatrixRotationX(XMConvertToRadians(m_MoveMap[i].m_vUp.x));
					m_MoveMap[i].m_pColMesh->SetWorldMatrix(mRotUp * m_MoveMap[i].m_pColMesh->m_mWorld);  // Ｘ回転増分の処理
					if (GetRotateVector(m_MoveMap[i].m_pColMesh->m_mWorld).x < m_MoveMap[i].m_vMin.x ||  // Ｘ回転するため端で反転
						GetRotateVector(m_MoveMap[i].m_pColMesh->m_mWorld).x > m_MoveMap[i].m_vMax.x)
					{
						m_MoveMap[i].m_vUp.x *= -1;  // Ｘ回転するため端で反転
						m_MoveMap[i].m_nChangeFlag = 1;
					}
				}else if (m_MoveMap[i].m_vUp.y != 0)  // Ｙ回転
				{
					mRotUp = XMMatrixRotationY(XMConvertToRadians(m_MoveMap[i].m_vUp.y));
					m_MoveMap[i].m_pColMesh->SetWorldMatrix(mRotUp * m_MoveMap[i].m_pColMesh->m_mWorld);  // Ｘ回転増分の処理
					if (GetRotateVector(m_MoveMap[i].m_pColMesh->m_mWorld).y < m_MoveMap[i].m_vMin.y ||  // Ｙ回転するため端で反転
						GetRotateVector(m_MoveMap[i].m_pColMesh->m_mWorld).y > m_MoveMap[i].m_vMax.y)
					{
						m_MoveMap[i].m_vUp.y *= -1;  // Ｙ回転するため端で反転
						m_MoveMap[i].m_nChangeFlag = 1;
					}
				}
				else if (m_MoveMap[i].m_vUp.z != 0)  // Ｚ回転
				{
					mRotUp = XMMatrixRotationZ(XMConvertToRadians(m_MoveMap[i].m_vUp.z));
					m_MoveMap[i].m_pColMesh->SetWorldMatrix(mRotUp * m_MoveMap[i].m_pColMesh->m_mWorld);  // Ｘ回転増分の処理
					if (GetRotateVector(m_MoveMap[i].m_pColMesh->m_mWorld).z < m_MoveMap[i].m_vMin.z ||  // Ｚ回転するため端で反転
						GetRotateVector(m_MoveMap[i].m_pColMesh->m_mWorld).z > m_MoveMap[i].m_vMax.z)
					{
						m_MoveMap[i].m_vUp.z *= -1;  // Ｚ回転するため端で反転
						m_MoveMap[i].m_nChangeFlag = 1;
					}
				}
			}
			else if (m_MoveMap[i].m_nMoveFlag == 3) {   // 拡大縮小の処理      // -- 2019.12.30
				MATRIX4X4 mScaleUp;
				mScaleUp = XMMatrixScaling(m_MoveMap[i].m_vUp.x, m_MoveMap[i].m_vUp.y, m_MoveMap[i].m_vUp.z);
				m_MoveMap[i].m_pColMesh->SetWorldMatrix(mScaleUp * m_MoveMap[i].m_pColMesh->m_mWorld);  // 拡大縮小増分の処理
				VECTOR3 vScaleNow = GetScaleVector(m_MoveMap[i].m_pColMesh->m_mWorld);     // 現在の拡大率を得る
				if (vScaleNow.x < m_MoveMap[i].m_vMin.x ||      // 規定の拡大縮小に達したか
					vScaleNow.x > m_MoveMap[i].m_vMax.x ||
					vScaleNow.y < m_MoveMap[i].m_vMin.y ||
					vScaleNow.y > m_MoveMap[i].m_vMax.y ||
					vScaleNow.z < m_MoveMap[i].m_vMin.z ||
					vScaleNow.z > m_MoveMap[i].m_vMax.z)
				{
					m_MoveMap[i].m_vUp.x = 1 / avoidZero(m_MoveMap[i].m_vUp.x);  // 拡大縮小の反転
					m_MoveMap[i].m_vUp.y = 1 / avoidZero(m_MoveMap[i].m_vUp.y);  // 拡大縮小の反転
					m_MoveMap[i].m_vUp.z = 1 / avoidZero(m_MoveMap[i].m_vUp.z);  // 拡大縮小の反転
					m_MoveMap[i].m_nChangeFlag = 1;
				}
			}
			
			// ループをしないときの停止判断。反転のタイミングで停止する        
			if (m_MoveMap[i].m_nLoop == 0 && m_MoveMap[i].m_nChangeFlag == 1)
			{
				m_MoveMap[i].m_nMoveFlag = 0;    // 移動しない
			}
		}
	}
}

//============================================================================
//  マッププロシージャのレンダリング
// 
//    引数　　なし
// 
//    戻り値　なし
//============================================================================
void  CMapProc::Render(void)
{
	if (m_bActive) {

		// 空のレンダリング
		if (m_SkyMap.size() > 0)
		{
			// 空のレンダリング   陰影をつけないレンダリング 
			// Zバッファを無効化
			m_pGMain->m_pD3D->SetZBuffer(FALSE);
			m_SkyMap[0].m_pMesh->Render(m_SkyMap[0].m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, VECTOR3(0, 0, 0), m_pGMain->m_vEyePt);  // 陰影をつけないレンダリング 
			// Zバッファを有効化
			m_pGMain->m_pD3D->SetZBuffer(TRUE);
		}

		// マップレンダリング
		for (DWORD i = 0; i<m_StageMap.size(); i++)
		{
			if (m_StageMap[i].m_nMaterialFlag == 0)  // 通常のブレンドステート
			{
				if (m_StageMap[i].m_pMesh) m_StageMap[i].m_pMesh->RenderToon(m_StageMap[i].m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, normalize(VECTOR3(0.8f, 1, -1)), m_pGMain->m_vEyePt);
			}
			else if (m_StageMap[i].m_nMaterialFlag == 2)  // ディスプレイスメントマッピング
			{
				// ディスプレイスメントマップレンダリング
				if (m_StageMap[i].m_pMesh) 	m_StageMap[i].m_pMesh->RenderDisplace(m_StageMap[i].m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
			}
		}

		// 移動マップレンダリング     * このプログラムでは使用していない
		for (DWORD i = 0; i<m_MoveMap.size(); i++)
		{
			if( m_MoveMap[i].m_nMaterialFlag == 0 )  // 通常のブレンドステート
			{
				if (m_MoveMap[i].m_pMesh && m_MoveMap[i].m_bActive)	m_MoveMap[i].m_pMesh->RenderToon(m_MoveMap[i].m_pColMesh->m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, normalize(VECTOR3(0.8f, 1, -1)), m_pGMain->m_vEyePt);
			}
			else if( m_MoveMap[i].m_nMaterialFlag == 2 )  // ディスプレイスメントマッピング
			{
				if (m_MoveMap[i].m_pMesh && m_MoveMap[i].m_bActive)	m_MoveMap[i].m_pMesh->RenderDisplace(m_MoveMap[i].m_pColMesh->m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
			}
		}
	}
}

//============================================================================
//  マッププロシージャのレンダリング その２
// 
//  半透明のマップオブジェクトを描画するため、主ループの最後に実行される
// 
//    引数　　なし
// 
//    戻り値　なし
//============================================================================
void  CMapProc::Render2(void)
{
	if (m_bActive) {

		// マップレンダリング
		for (DWORD i = 0; i<m_StageMap.size(); i++)
		{
			if (m_StageMap[i].m_nMaterialFlag == 1)  // 透明色のブレンドステート
			{
				// 透明色のレンダリング
				if (m_StageMap[i].m_pMesh) m_StageMap[i].m_pMesh->RenderToon(m_StageMap[i].m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, normalize(VECTOR3(0.8f, 1, -1)), m_pGMain->m_vEyePt);
			}
		}

		// 移動マップレンダリング     * このプログラムでは使用していない
		for (DWORD i = 0; i<m_MoveMap.size(); i++)
		{
			if (m_MoveMap[i].m_nMaterialFlag == 1)  // 透明色のブレンドステート
			{
				if (m_MoveMap[i].m_pMesh && m_MoveMap[i].m_bActive)	m_MoveMap[i].m_pMesh->RenderToon(m_MoveMap[i].m_pColMesh->m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, normalize(VECTOR3(0.8f, 1, -1)), m_pGMain->m_vEyePt);
			}
		}

		// 波のレンダリング
		for (DWORD i = 0; i<m_StageMap.size(); i++)
		{
			if (m_StageMap[i].m_nMaterialFlag == 3)  
			{
				// 波レンダリング
				if (m_StageMap[i].m_pWave)
					m_StageMap[i].m_pWave->Render(m_StageMap[i].m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
			}
		}
	}
}

//-----------------------------------------------------------------------------
//  ランダム並んだ数値を基にマップ遷移
//-----------------------------------------------------------------------------
void CMapProc::MoveMap()
{
	switch (m_MapIdArray[m_nHierarchy - 1])
	{
	case 2:
		LoadMap2();
		break;
	case 3:
		LoadMap3();
		break;
	case 4:
		LoadMap4();
		break;
	case 5:
		LoadMap5();
		break;
	case 6:
		LoadMap6();
		break;
	case 7:
		LoadMap7();
		break;
	case 8:
		LoadMap8();
		break;
	case 9:
		LoadMap9();
		break;
	case 10:
		LoadMap10();
		break;
	case 11:
		LoadMap11();
		break;
	case 12:
		LoadMap12();
		break;
	case 13:
		LoadMap13();
		break;
	case 14:
		LoadMap14();
		break;
	case 15:
		LoadMap15();
		break;
	case 16:
		LoadMap16();
		break;
	case 17:
		LoadMap17();
		break;
	case 18:
		LoadMap18();
		break;
	case 19:
		LoadMap19();
		break;
	case 20:
		LoadMap20();
		break;
	}
	m_nHierarchy++;
}

//-----------------------------------------------------------------------------
//  カメラとPlayerモデルの間にマップオブジェクトがある場合、透過させる
//-----------------------------------------------------------------------------
void CMapProc::CheckCrossStageMesh(VECTOR3 vNow, VECTOR3 vOld)
{
	VECTOR3 vHit = VECTOR3(0.0f, 0.0f, 0.0f);
	VECTOR3 vNrm = VECTOR3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < m_StageMap.size(); i++) {
		// 透過判定を行わないオブジェクトをパスする
		if (m_StageMap[i].m_pBox != NULL)
		{
			if (m_StageMap[i].m_pBox->OBBCollisionLay(vNow, vOld, &vHit, &vNrm))
			{
				m_StageMap[i].m_pMesh->m_vDiffuse.w = 0.3f;
				m_StageMap[i].m_nMaterialFlag = 1;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//  マップの透過情報をリセットする
//-----------------------------------------------------------------------------
void CMapProc::ResetStageMeshAlfa()
{
	for (int i = 0; i < m_StageMap.size(); i++)
	{
		m_StageMap[i].m_pMesh->m_vDiffuse.w = 1.0f;
		m_StageMap[i].m_nMaterialFlag = 0;
	}
}

//============================================================================
//  アクセス関数
//============================================================================
std::vector<NAVIGATIONMAP> CMapProc::GetNavMap() 
{ 
	return m_NavMap; 
}

DWORD CMapProc::GetMapNo() 
{ 
	return m_dwMapNo; 
}

void CMapProc::SetTutorial(BOOL flag)
{
	m_bIsTutorial = flag;
}

CCollision* CMapProc::GetCollision() 
{ 
	return m_pColMesh; 
}

ROUTECOLLISION* CMapProc::GetRouteColMap() 
{ 
	return  &m_RouteColMap;
}

int CMapProc::GetHierarchy() 
{ 
	return m_nHierarchy; 
}

void CMapProc::SetHierarchy(int hierarchy)
{ 
	m_nHierarchy = hierarchy; 
}

CSpPart* CMapProc::GetPcSpPart() 
{ 
	return m_pPcSpPart; 
}

CSpPart* CMapProc::GetEnmSpPart() 
{ 
	return m_pEnmSpPart; 
}
