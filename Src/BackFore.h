#pragma once
#include "GameMain.h"

#define DIALOG_NONE 0
#define DIALOG_ABILITY 1
#define DIALOG_CHECK 2
#define DIALOG_ERROR 3
#define DIALOG_TUTORIAL1 4
#define DIALOG_TUTORIAL2 5
#define DIALOG_TUTORIAL3 6
#define DIALOG_TUTORIAL_SWORD 7
#define DIALOG_TUTORIAL_ARROW 8
#define DIALOG_TUTORIAL_AXE   9
#define DIALOG_TUTORIAL_GUN	  10

#define BUTTON_LEFT 0
#define BUTTON_RIGHT 1
#define BUTTON_FINISH 2

// --------------------------------------------------------------------
// 前景・背景　プロシージャクラス
// 
// (注意)基本プロシージャクラスは継承していない
// --------------------------------------------------------------------
class CBackForeProc 
{
protected:
	CGameMain*		m_pGMain;
	CSpriteImage*	m_pImageFore;
	CSpriteImage*   m_pImageUI;
	CSpriteImage*	m_pImageAbilityDialog;
	CSpriteImage*	m_pImageFade;
	CSpriteImage*	m_pImagePcIcon;
	CSpriteImage*	m_pImageTutorial;
	CSpriteImage*	m_pImageArrow;
	CSprite*        m_pSprite;
	
	int				m_ntest;
	int				m_nSelectAbility[3];	// Upgradeダイアログでのランダムに選ばれた能力のID
	int				m_nSelectNum;			// 選択したボタンの位置
	int				m_nFadePosY;			// 透明な部分も含めたフェードイン、アウトする位置
	int				m_nFadeDrawPosY;		// フェードイン、フェードアウトする際のスプライトの位置
	int				m_nAnimMove;			// アニメーション時、アイコンの移動距離
	int				m_nCnt;					// アニメーション後、フェードアウトまでの待機時間
	int				m_nOpenAllyNum;			// 宝箱を開けるのに必要なAllyの数
	float			m_nArrowDrawTime[36];	// 敵の攻撃位置角度矢印の描画時間
	float			m_fBlackOutAlpha;		// ブラックアウトする際の透過値
	float			m_fSwingAngle;			// 振動させる方向
	float			m_fSwingWidth;			// 振動させる幅
	VECTOR2			m_vSwingWidth;			// 振動による移動距離

	// 集計データ
	int				m_nDeadNum;				// ゲームオーバーの回数
	int				m_nAllAllyNum;			// ゲーム内で出現したすべての味方の数
	int				m_nDeadAllyNum;			// 死んだ味方の数
	int				m_nDeadEnemyNum;		// 死んだ敵の数

	BOOL			m_bIsFade;				// フェード開始フラグ
	BOOL            m_bIsFadeInFinish;		// フェードイン完了通知
	BOOL			m_bIsFadeOutFinish;		// フェードアウト完了通知
	BOOL			m_bIsAnim;				// アニメーション開始フラグ
	BOOL			m_bIsBlackOut;			// ブラックアウト開始フラグ
	BOOL			m_bIsWhiteOut;			// 明転フラグ
	BOOL			m_bIsSwing;				// 振動処理フラグ
	BOOL			m_bIsWalkAudio;			// 階層移動音フラグ
	BOOL			m_bIsFirstAxe;
	BOOL			m_bIsFirstGun;

public:
	int				m_nDialogKind;
	int             m_nLogNum;
	void			Update();						// GAMEMAIN処理
	BOOL			UpdateCheckDialog();			// 宝箱開封ダイアログの表示
	BOOL			UpdateErrorDialog();			// 宝箱開封時、人数が足りないときのダイアログ
	BOOL			UpdateDialog();					// 能力強化ダイアログ
	BOOL			UpdateTutorialDialog(int page, bool left, bool right, bool finish);	// チュートリアル１枚目のダイアログ
	void			UpdateFade();					// フェードアウト、イン処理
	void			UpdateAnim();					// フェードイン後のアニメーション処理
	void			UpdateBlackOut();				// ブラックアウト処理

	void			SetSelectAbility();												// ランダムに３つの強化内容の選択処理
	void			StartSwing(float angle);										// 振動開始処理
	void			DrawArrowAngle(MATRIX4X4 mWorld);
	BOOL			CheckMousePos(int nPosX, int nPosY, int nWidth, int nHeight);	// カーソルが範囲に入っているか

	// アクセス関数
	CSpriteImage* GetImage();
	CSprite* GetSprite();
	void			SetFade(BOOL fade);
	BOOL			GetFadeInFinish();
	BOOL			GetFadeOutFinish();
	BOOL			GetBlackOut();
	BOOL			GetWhiteOut();
	void			SetBlackOut(BOOL flag);
	CSpriteImage* GetImageFade();
	CSpriteImage* GetPcIcon();
	CSpriteImage* GetImageAbilityDialog();
	void			SetBackImageMovePos(int pos);
	void			ResetOpenAllyNum();
	int				GetSelectNum();
	void			SetSelectNum(int id);
	void			SetFirstAxe(bool flag);
	BOOL			GetFirstAxe();
	void			SetFirstGun(bool flag);
	BOOL			GetFirstGun();
	void			AddDeadNum(int num);
	void			AddDeadAllyNum(int num);
	void			AddDeadEnemyNum(int num);
	void			AddAllAllyNum(int num);
	void			SetDeadNum(int num);
	void			SetDeadAllyNum(int num);
	void			SetDeadEnemyNum(int num);
	void			SetAllAllyNum(int num);
	int				GetDeadNum();
	int				GetDeadAllyNum();
	int				GetDeadEnemyNum();
	int				GetAllAllyNum();

	// コンストラクタ
	CBackForeProc(CGameMain* pGMain);	
	~CBackForeProc();
};

