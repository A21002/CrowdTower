//=============================================================================
//																GameMain.h
//=============================================================================

#pragma once

//�x����\��
#pragma warning(disable : 4005)
#pragma warning(disable : 4244)
#pragma warning(disable : 4018)


//�w�b�_�[�t�@�C���̃C���N���[�h
#include <stdio.h>
#include <windows.h>
#include <vector>

#include "Main.h"
#include "Direct3D.h"
#include "Shader.h"
#include "FbxMesh.h"
#include "XAudio.h"
#include "Dinput.h"
#include "Sprite3D.h"
#include "Collision.h"
#include "Displace.h"
#include "BBox.h"
#include "BranchTree.h"
#include "BaseProc.h"
#include "MyImgui.h"


// �X�e�[�^�X�^�C�v
#define  NONE				0x00000000
#define  TITLE				0x00000001
#define  TITLEANIM			0x00000080
#define  STAGEDEMO  		0x00000002
#define  GAMEMAIN			0x00000004
#define  GAMEOVER			0x00000008
#define  GAMECLEAR			0x00000010
#define  GAMEEND			0x00000020
#define  GAMERETRY			0x00000040
#define  GAMESAVE			0x00000100

#define  NORMAL				0x00000001
#define  WAIT				0x00000002
#define  DAMAGE				0x00000004
#define  FLASH				0x00000008
#define  DEAD				0x00000010

// �X�e�[�^�X�T�u
#define  ATTACKNONE			0x00000001
#define  ATTACKMOVE			0x00000002
#define  ATTACKLASER		0x00000004
#define  ATTACKCANNON		0x00000008
#define  ATTACKBOM			0x00000010
#define  ATTACKEARTH		0x00000020
#define  ATTACKSWORD		0x00000040
#define  ATTACKFLAME		0x00000080
#define  ATTACKWAIT			0x00000100

// �I�u�W�F�N�g�敪
#define  PC					0x00000001
#define  NONE				0x00000002
#define  ENM				0x00000004
#define  WEAPON				0x00000008
#define  ALLY				0x00000010

// �A�N�V����ID
#define ATTACK_NONE			0
#define ATTACK_SWORD		1
#define ATTACK_ALLOW		2
#define ATTACK_AXE			3
#define ATTACK_GUN			4
#define ATTACK_AXESHOT		5
#define ATTACK_SHOTSWORD	6

class CMain;
class CShader;
class CSpriteImage;
class CSprite;
class CFontTexture;
class CCollision;
class CWave;
class CBBox;
class CFbxMeshCtrl;

class CBaseObj;
class CBaseProc;
class CPcProc;
class CAllyProc;
class CCameraProc;
class CEnmProc;
class CEnmBossProc;
class CMapProc;
class CWeaponProc;
class CEffectProc;
class CBackForeProc;
class CTitleProc;
class CSearchRoute;

// --------------------------------------------------------------------------------------------------
//  �Q�[�����C���N���X
// --------------------------------------------------------------------------------------------------
class CGameMain
{
public:
	// �����o�ϐ�
	CMain*			m_pMain;		// ���C���N���X
	CDirect3D*		m_pD3D;			//	Direct3D�I�u�W�F�N�g
	CXAudio*		m_pXAudio;		//	XAudio�I�u�W�F�N�g
	CDirectInput*	m_pDI;			//	DirectInputD�I�u�W�F�N�g
	CFontTexture*	m_pFont;		//	�t�H���g�e�N�X�`���[
	CShader*		m_pShader;		//	�V�F�[�_�[
	CFbxMeshCtrl*	m_pFbxMeshCtrl;	// ���b�V���R���g���[���N���X

	DWORD           m_dwGameStatus;
	
	// �J�����E���C�g�E�r���[
	VECTOR3         m_vEyePt;		//�J�����i���_�j�ʒu
	VECTOR3         m_vLookatPt;	//�����ʒu
	MATRIX4X4       m_mView;
	MATRIX4X4       m_mProj;
	VECTOR3         m_vLightDir;	//�f�B���N�V���i�����C�g�̕���


	// �v���V�[�W��
	CPcProc*		m_pPcProc;
	CAllyProc*		m_pAllyProc;
	CCameraProc*	m_pCameraProc;
	CEnmProc*		m_pEnmProc;
	CEnmBossProc*	m_pEnmBossProc;
	CMapProc*		m_pMapProc;
	CWeaponProc*	m_pWeaponProc;
	CEffectProc*	m_pEffectProc;
	CBackForeProc*	m_pBackForeProc;
	CTitleProc*		m_pTitleProc;

	// �T�E���h
	CXAudioSource*  m_pGameClear;
	CXAudioSource*  m_pSeNitro;
	CXAudioSource*  m_pSeDead;
	CXAudioSource*  m_pBossBgm;
	CXAudioSource*  m_pBgm1;
	CXAudioSource*	m_pGameOver;
	CXAudioSource*	m_pSpown;
	CXAudioSource*	m_pSlash;
	CXAudioSource*	m_pArrow;
	CXAudioSource*	m_pGun;
	CXAudioSource*  m_pClick_Button;
	CXAudioSource*  m_pClick_Ability;
	CXAudioSource*	m_pAnimWalk;
	CXAudioSource*	m_pDownCage;
	CXAudioSource*  m_pShotSlashCharge;

	// �X�R�A
	int             m_nScore;
	// �S�I�u�W�F�N�g�̓����𐧌�
	BOOL            m_isMove;
	BOOL			m_bIsTutorial;
	// �{�X���X�|�[���ς݂��ǂ���
	BOOL			m_bIsBoss;

	// �`���[�g���A���̕\���ς݃t���O
	BOOL			m_bIsBoxTutorial;

public:
	// ���\�b�h
	CGameMain(CMain* pMain);
	~CGameMain();

	HRESULT Init();
	void    Loop();
	void    Quit();
	void    Save(int mode);
	BOOL	Load();
	int		Cipher(int num);

	void    GameMain();
	HRESULT ChangeScreenMode(int nMode=-1);
};


