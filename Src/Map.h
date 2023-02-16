//=============================================================================
//		�}�b�v�@�w�b�_�t�@�C��											Map.h
//=============================================================================
#pragma once
#include "GameMain.h"
#include "SpPart.h"

#define POISON 1
#define LAVA   2

// --------------------------------------------------------------------
// �X�e�[�W�}�b�v
// --------------------------------------------------------------------
struct STAGEMAP
{
	CFbxMesh*				m_pMesh;			// �X�^�e�B�b�N���b�V���ւ̃|�C���^
	CBBox*					m_pBox;				// ���ߔ��� NULL�F���ߏ����Ȃ�
	CWave*					m_pWave;			// �g�̃��b�V���ւ̃|�C���^
	MATRIX4X4				m_mWorld;			// ���b�V���̃��[���h�}�g���b�N�X
	int						m_nMaterialFlag;	// �}�e���A���t���O�@0:�ʏ�@1:�����F�@2:�f�B�X�v���C�X�����g�}�b�s���O 3:�g

	STAGEMAP()    // �R���X�g���N�^
	{
		Init();
	}
	~STAGEMAP()    // �f�X�g���N�^
	{
		// ���̒��Ń��b�V����DELETE�����Ă͂Ȃ�Ȃ�
	}
	void Init()    // ���������\�b�h
	{
		m_pMesh = NULL;
		m_pBox = NULL;
		m_pWave = NULL;
		m_mWorld = XMMatrixIdentity();
		m_nMaterialFlag = 0;
	}
};

// --------------------------------------------------------------------
// �ړ��}�b�v            �����̃v���O�������ł́A�g�p���Ȃ�
// --------------------------------------------------------------------
struct MOVEMAP
{
	BOOL					m_bActive;			// �ړ�Active
	CFbxMesh*				m_pMesh;			// �X�^�e�B�b�N���b�V���ւ̃|�C���^
	int						m_nMaterialFlag;	// �}�e���A���t���O�@0:�ʏ�@1:�����F�@2:�f�B�X�v���C�X�����g�}�b�s���O
	CCollision*				m_pColMesh;			// �ړ��R���W�������b�V���ւ̃|�C���^
	int						m_nMoveFlag;		// �ړ��t���O�@0:�ړ������@1:���s�ړ��@2:��]�@3:�g��k��  
	VECTOR3					m_vUp;				// �ړ�����
	VECTOR3					m_vMin;				// �ړ��ŏ��l
	VECTOR3					m_vMax;				// �ړ��ő�l
	int						m_nChangeFlag;		// �ړ������E�l�ɓ��B���ĕ���������ւ��Ƃ��@0:�ړ����@1:�����`�F���W
	int						m_nLoop;			// �ړ����J��Ԃ���  0:�`�F���W�̉ӏ��Œ�~���ړ��t���O��0:�ړ������ɖ߂��B 1:�ړ����J��Ԃ�

	MOVEMAP()    // �R���X�g���N�^
	{
		Init();
	}
	~MOVEMAP()    // �f�X�g���N�^
	{
		// ���̒��Ń��b�V����DELETE�����Ă͂Ȃ�Ȃ�
	}
	void Init()
	{
		m_bActive = FALSE;
		m_pMesh = NULL;
		m_nMaterialFlag = 0;
		m_pColMesh = NULL;
		m_nMoveFlag = 0;
		m_vUp = VECTOR3(0, 0, 0);
		m_vMin = VECTOR3(-9999, -9999, -9999);
		m_vMax = VECTOR3(9999, 9999, 9999);
		m_nChangeFlag = 0;
		m_nLoop = 1;           // �����ݒ�̓��[�v 
	}
};

// --------------------------------------------------------------------
// �i�r�Q�[�V�����}�b�v
// --------------------------------------------------------------------
struct NAVIGATIONMAP
{
	VECTOR3 vMin;
	VECTOR3 vMax;
	std::vector<DWORD>  m_dwEnmID;  // �ΏۂƂȂ�G�̂h�c
	NAVIGATIONMAP()    // �R���X�g���N�^
	{
		Init();
	}
	void Init()
	{
		vMin = VECTOR3(0.0f, 0.0f, 0.0f);
		vMax = VECTOR3(0.0f, 0.0f, 0.0f);
	}
};

// --------------------------------------------------------------------  
// �C�x���g
// --------------------------------------------------------------------
// �C�x���g���
enum EVENTKIND
{
	eEvtKindNone = 0,
	eEvtChangeMap = 1,
	eEvtMoveMap = 2
};

struct EVENTMAP
{
	CBBox*    m_pBBox;       // �o�E���f�B���O�{�b�N�X
	int       m_nEvtCycle;   // �C�x���g�T�C�N�� 0:�C�x���g���s���Ă��Ȃ� 1:�C�x���g���s���i�g�p���Ă��Ȃ��j 2:�C�x���g�I���`�F�b�N�i�ڐG�I��������j
	EVENTKIND m_nEvtKind;    // �C�x���g�̎�� eEvtChangeMap:�}�b�v�ړ�, eEvtMoveMap:�ړ��}�b�v�̐���
	int       m_nEvtNo;      // �C�x���g�ԍ� eEvtChangeMap�̂Ƃ��͈ړ���̃}�b�v�ԍ��AeEvtMoveMap�̂Ƃ��͈ړ��}�b�v�̗v�f�ԍ�
	int       m_nEvtOpe1;    // eEvtMoveMap�̂݁B1:�ړ��}�b�v�̈ړ�Active�̎w��B2:�ړ��}�b�v�̈ړ��t���O�̎w��
	int       m_nEvtOpe2;    // eEvtMoveMap�̂݁B�ړ�Active�̂Ƃ���0:��\�� 1:�\��  �ړ��t���O�̂Ƃ��́@0:�ړ������@1:���s�ړ��@2:��]�@3:�g��k��
	int       m_nEvtKeyPush; // 0:�o�E���f�B���O�{�b�N�X�ɐڐG����ƃC�x���g�����A 1:�o�E���f�B���O�{�b�N�X�ɐڐG���āAENTER�L�[�������ƃC�x���g����

	EVENTMAP()    // �R���X�g���N�^
	{
		m_pBBox = NULL;
		m_nEvtCycle = 0;
		m_nEvtKind = eEvtKindNone;
		m_nEvtNo = 0;
		m_nEvtOpe1 = 0;
		m_nEvtOpe2 = 0;
		m_nEvtKeyPush = 0;
	}
	~EVENTMAP()    // �f�X�g���N�^
	{
		// ���̒��Ń��b�V����CBBox��DELETE�����Ă͂Ȃ�Ȃ�
	}
};

// --------------------------------------------------------------------  
// ���[�g�T���R���W�����}�b�v
// --------------------------------------------------------------------
struct ROUTECOLLISION
{
	float   m_Partssize;                         // ���[�g�T���P�̃m�[�h�̑傫���i���j
	VECTOR2 m_MapNumXZ;                          // ���[�g�T���R���W�����}�b�v�̑傫���i�v�f��)
	VECTOR3 m_Basepoint;                         // ���[�g�T������O�̈ʒu
	std::vector< std::vector<int> > m_Collision;  // ���[�g�T���R���W�����z��(0:�ʍs�@9:�ʍs�s��)
	std::vector<std::vector<int>>	m_RouteTable;

};

//======================================================================
// �}�b�v�@�v���V�[�W���N���X
//
// (����)�}�b�v�@�v���V�[�W���N���X�́A��{�v���V�[�W���N���X���p�����Ă��Ȃ�
//======================================================================
class CMapProc
{
protected:
	CGameMain*							m_pGMain;

	DWORD								m_dwMapNo;
	BOOL								m_bActive;
	BOOL								m_bIsMoveMap;
	BOOL								m_bIsTutorial;

	std::vector<STAGEMAP>				m_SkyMap;			// ��̃}�b�v
	std::vector<STAGEMAP>				m_StageMap;			// �X�e�[�W�̃}�b�v
	CCollision*							m_pColMesh;			// �R���W�������b�V��
	CCollision*							m_pPcColMesh;		// �ꕔ�}�b�v�ł̂ݎg�p����PC�����̓����蔻��
	std::vector<MOVEMAP>				m_MoveMap;			// �ړ��}�b�v  *���̃v���O�������ł́A�g�p���Ȃ�
	std::vector<NAVIGATIONMAP>			m_NavMap;			// �i�r�Q�[�V�����}�b�v
	std::vector<EVENTMAP>				m_EventMap;			// �C�x���g�}�b�v 
	int									m_nEvtIdx;			// �C�x���g�v�f�ԍ� 
	ROUTECOLLISION						m_RouteColMap;		// ���[�g�T���R���W�����}�b�v 
	std::vector<int>					m_MapKind;
	int									m_MapIdArray[19];
	int                                 m_nHierarchy;
	CSpPart*							m_pPcSpPart;			// PC�Q��ԕ���
	CSpPart*							m_pEnmSpPart;			// ENM��ԕ���

public:
	void Update();
	void Render();
	void Render2();
	void DestroyAll();

	void LoadMap(const char* fileName = NULL);
	void LoadMap1();
	void LoadMap2();
	void LoadMap3();
	void LoadMap4();
	void LoadMap5();
	void LoadMap6();
	void LoadMap7();
	void LoadMap8();
	void LoadMap9();
	void LoadMap10();
	void LoadMap11();
	void LoadMap12();
	void LoadMap13();
	void LoadMap14();
	void LoadMap15();
	void LoadMap16();
	void LoadMap17();
	void LoadMap18();
	void LoadMap19();
	void LoadMap20();

	void UpdateMoveMap();
	int  SetEvent(VECTOR3 vMin, VECTOR3 vMax, MATRIX4X4 mWorld, EVENTKIND nEvtKind, int nEvtNo, int nEvtOpe1 = 0, int nEvtOpe2 = 0, int nEvtKeyPush = 0);    // -- 2021.2.4
	void UpdateEvent();                                                                        
	void RunEvent(EVENTMAP& EventMap);                                                  

	BOOL Hitcheck(CBaseObj* pObj, VECTOR3* pHit, VECTOR3* pNormal);
	BOOL Hitcheck(VECTOR3 vNow, VECTOR3 vOld, VECTOR3* pHit, VECTOR3* pNormal);
	BOOL Hitcheck(VECTOR3 vNow, VECTOR3 vOld, float fRadius, VECTOR3* pHit, VECTOR3* pNormal);  

	int  isCollisionMoveGravity(MATRIX4X4* pWorld, MATRIX4X4 mWorldOld, float fRadius = 0.2f, DWORD dwTag = NONE);
	int  isCollisionMoveGravityPc(MATRIX4X4* pWorld, MATRIX4X4 mWorldOld, float fRadius = 0.2f);
	int  isCollisionMove(MATRIX4X4* pWorld, MATRIX4X4 mWorldOld, float fRadius = 0.2f);
	int  isCollisionMoveWall(MATRIX4X4 mWorld, MATRIX4X4 mWorldOld, float fRadius = 0.2f);

	void MakeRouteColMap(VECTOR2 vMapSizeXZ, float partssize, VECTOR3 vBasepoint, const char* fileName);  
	void GetMapsizeAndBasepoint(VECTOR2& vMapSizeXZ, VECTOR3& vBasepoint); 
	void MoveMap();
	void CheckCrossStageMesh(VECTOR3 vNow, VECTOR3 vOld);
	void ResetStageMeshAlfa();

	// �A�N�Z�X�֐�
	std::vector<NAVIGATIONMAP> GetNavMap();
	DWORD GetMapNo();
	void SetTutorial(BOOL flag);
	CCollision* GetCollision();
	ROUTECOLLISION* GetRouteColMap();
	int GetHierarchy();
	void SetHierarchy(int hierarchy);
	CSpPart* GetPcSpPart();
	CSpPart* GetEnmSpPart();


	// �R���X�g���N�^�i�v���V�[�W���̏������j
	CMapProc(CGameMain*	m_pGMain);
	~CMapProc();
};