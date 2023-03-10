//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１２                             ver 3.1        2022.2.16
//
//		カメラ制御
//
//		(視点変更処理・障害物回避処理が入っている)
//
//																Camera.h
//=============================================================================
#pragma once
#include "GameMain.h"

#define CAMERA_GAMEMAIN 1
#define CAMERA_GAMEOVER 2
#define CAMERA_GAMECLEAR 3

//======================================================================
// カメラ　オブジェクトクラス
//======================================================================
class CCameraObj : public CBaseObj
{
protected:
	int     m_nCtrl;                       // カメラ制御（0:ＴＰＳ視点  1:ＦＰＳ視点）
	MATRIX4X4 m_mBaseWorld;                // 注視点・視点の基点となる基点ワールドマトリックス
	VECTOR3 m_vLocalLookat;                // ローカル注視点ベクトル
	VECTOR3 m_vLocalEye;                   // ローカル視点ベクトル
	VECTOR3 m_vLocalEyeRotDist;            // ローカル視点ベクトルを生成するための角度と変位(TPSのみ)
	VECTOR3 m_vBaseLocalLookatTPS;         // ローカル注視点ベクトル初期値 TPS
	VECTOR3 m_vBaseLocalEyeRotDistTPS;     // ローカル視点ベクトル初期値(角度と変位) TPS
	VECTOR3 m_vBaseLocalLookatFPS;         // ローカル注視点ベクトル初期値 FPS
	VECTOR3 m_vBaseLocalEyeFPS;            // ローカル視点ベクトル初期値 FPS

	BOOL    m_isLookStairs;
	BOOL	m_isOnce;
	BOOL	m_IsBossMovie;
	BOOL	m_IsMovePc;
	BOOL	m_bIsDeadBoss;

public:
	void Update() override;
	void MoveBasePos();
	void EvasiveObstacle();                // -- 2020.11.19
	void ControlCam();                     // -- 2020.11.19
	void SetLocalEyeTPS(float fRotY, float fRotX, float fDist);
	void SetCamBase();
	int  GetCtrl() { return m_nCtrl; }
	void SetCtrl(int num);
	void SetLookStairsFlag(bool flag) { m_isLookStairs = flag; }
	void ResetMovieFlag();
	void SetMovePc(bool flag);
	void SetBossMovie(bool flag);
	void SetDeadBoss(bool flag);
	CCameraObj(CBaseProc* pProc);	// コンストラクタ    // -- 2022.2.16
	virtual	~CCameraObj();
};


//======================================================================
// カメラ　プロシージャクラス
//======================================================================
class CCameraProc : public CBaseProc
{
protected:
	;

public:
	CCameraObj*	 GetCameraObjPtr() { return (CCameraObj*)GetObjArrayPtr()[0]; }
	CCameraProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CCameraProc() { ; }
};

