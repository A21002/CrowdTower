//=============================================================================
//		タイトル　ヘッダファイル							Title.h
//=============================================================================
#pragma once

#include "GameMain.h"

// --------------------------------------------------------------------
// タイトル　プロシージャクラス
// --------------------------------------------------------------------
class CTitleProc
{
public:
	CGameMain*		m_pGMain;

	CSpriteImage*   m_pImageTitle;
	CSpriteImage*   m_pImageClear;
	CSpriteImage*   m_pImageClearScore;
	CSpriteImage*   m_pImageOver;
	CSpriteImage*	m_pImageButton;
	CSpriteImage*	m_pImageClearFore;
	CSprite*        m_pSprite;

	DWORD           m_dwCntTime;
	int				m_nSelectNum;
	int				m_nBackImageMoveNum;
	int				m_nIconMoveNum;
	int				m_nCnt;
	BOOL			m_bIsSaveData;

public:
	BOOL Title();
	void TitleAnim();
	void GameClear();
	BOOL GameOver();
	BOOL SelectWeapon();
	BOOL Save();
	BOOL CheckMousePos(int nPosX, int nPosY, int nWidth, int nHeight);
	void CheckSaveData();

	CSpriteImage* GetImageOver() { return m_pImageOver;}
	CSpriteImage* GetImageClear() { return m_pImageClear; }

	CTitleProc(CGameMain* pGMain);	// コンストラクタ
	~CTitleProc();
};
