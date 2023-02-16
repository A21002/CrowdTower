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
// �O�i�E�w�i�@�v���V�[�W���N���X
// 
// (����)��{�v���V�[�W���N���X�͌p�����Ă��Ȃ�
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
	int				m_nSelectAbility[3];	// Upgrade�_�C�A���O�ł̃����_���ɑI�΂ꂽ�\�͂�ID
	int				m_nSelectNum;			// �I�������{�^���̈ʒu
	int				m_nFadePosY;			// �����ȕ������܂߂��t�F�[�h�C���A�A�E�g����ʒu
	int				m_nFadeDrawPosY;		// �t�F�[�h�C���A�t�F�[�h�A�E�g����ۂ̃X�v���C�g�̈ʒu
	int				m_nAnimMove;			// �A�j���[�V�������A�A�C�R���̈ړ�����
	int				m_nCnt;					// �A�j���[�V������A�t�F�[�h�A�E�g�܂ł̑ҋ@����
	int				m_nOpenAllyNum;			// �󔠂��J����̂ɕK�v��Ally�̐�
	float			m_nArrowDrawTime[36];	// �G�̍U���ʒu�p�x���̕`�掞��
	float			m_fBlackOutAlpha;		// �u���b�N�A�E�g����ۂ̓��ߒl
	float			m_fSwingAngle;			// �U�����������
	float			m_fSwingWidth;			// �U�������镝
	VECTOR2			m_vSwingWidth;			// �U���ɂ��ړ�����

	// �W�v�f�[�^
	int				m_nDeadNum;				// �Q�[���I�[�o�[�̉�
	int				m_nAllAllyNum;			// �Q�[�����ŏo���������ׂĂ̖����̐�
	int				m_nDeadAllyNum;			// ���񂾖����̐�
	int				m_nDeadEnemyNum;		// ���񂾓G�̐�

	BOOL			m_bIsFade;				// �t�F�[�h�J�n�t���O
	BOOL            m_bIsFadeInFinish;		// �t�F�[�h�C�������ʒm
	BOOL			m_bIsFadeOutFinish;		// �t�F�[�h�A�E�g�����ʒm
	BOOL			m_bIsAnim;				// �A�j���[�V�����J�n�t���O
	BOOL			m_bIsBlackOut;			// �u���b�N�A�E�g�J�n�t���O
	BOOL			m_bIsWhiteOut;			// ���]�t���O
	BOOL			m_bIsSwing;				// �U�������t���O
	BOOL			m_bIsWalkAudio;			// �K�w�ړ����t���O
	BOOL			m_bIsFirstAxe;
	BOOL			m_bIsFirstGun;

public:
	int				m_nDialogKind;
	int             m_nLogNum;
	void			Update();						// GAMEMAIN����
	BOOL			UpdateCheckDialog();			// �󔠊J���_�C�A���O�̕\��
	BOOL			UpdateErrorDialog();			// �󔠊J�����A�l��������Ȃ��Ƃ��̃_�C�A���O
	BOOL			UpdateDialog();					// �\�͋����_�C�A���O
	BOOL			UpdateTutorialDialog(int page, bool left, bool right, bool finish);	// �`���[�g���A���P���ڂ̃_�C�A���O
	void			UpdateFade();					// �t�F�[�h�A�E�g�A�C������
	void			UpdateAnim();					// �t�F�[�h�C����̃A�j���[�V��������
	void			UpdateBlackOut();				// �u���b�N�A�E�g����

	void			SetSelectAbility();												// �����_���ɂR�̋������e�̑I������
	void			StartSwing(float angle);										// �U���J�n����
	void			DrawArrowAngle(MATRIX4X4 mWorld);
	BOOL			CheckMousePos(int nPosX, int nPosY, int nWidth, int nHeight);	// �J�[�\�����͈͂ɓ����Ă��邩

	// �A�N�Z�X�֐�
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

	// �R���X�g���N�^
	CBackForeProc(CGameMain* pGMain);	
	~CBackForeProc();
};

