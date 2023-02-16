//=============================================================================
//		�R�c�Q�[���v���O����
//		�b�R�c�`�����P�P�Q�@�@�@�@�@�@                 ver 3.1        2022.2.16
// 
//		�o�H�T���@�w�b�_�t�@�C��								SearchRoute.h
// 
//		AStar�A���S���Y���𗘗p���Čo�H�T�����s���v���O�����ł���
// 
//=============================================================================
#pragma once
#include "GameMain.h"

// --------------------------------------------------------------------
//
// �o�H�m�[�h
//
// --------------------------------------------------------------------
// �X�e�[�^�Xenum
enum eStatus
{
	eNone = 0,
	eOpen = 1,
	eClose = 2,
};
// �o�H�m�[�h
struct ANODE
{
	eStatus status;         // �X�e�[�^�X  eNone,eOpen,eClose
	float   cost;           // ���R�X�g
	float   heuristic;      // ����R�X�g
	float   score;          // �X�R�A
	VECTOR3 parent;         // �e�m�[�h�̗v�f�ԍ�
	VECTOR3 next;           // ���m�[�h�̗v�f�ԍ�

	ANODE()    // �R���X�g���N�^
	{
		Init();
	}
	void Init()   // ����������
	{
		status = eNone;
		cost = 0;
		heuristic = 0;
		score = 0;
		parent = VECTOR3(0, 0, 0);
		next = VECTOR3(0, 999999, 0);   // ���m�[�h�̏����l�BY�l��999999�Ƃ��Ă����i�I�����f�̂��߁j
	}
};
// --------------------------------------------------------------------
//
// �o�H�T���N���X
//
// --------------------------------------------------------------------
class CSearchRoute
{
protected:
	CGameMain*							m_pGMain;		// �Q�[�����C��
	std::vector< std::vector<ANODE> >	m_ANodeArray;	// �o�H�T���z��
	VECTOR3								m_Basepoint;	// �z��̍���O�̍��W�l
	float								m_Partssize;	// �m�[�h��̑傫��
	VECTOR3								m_StIdx;		// �o�H�J�n�_
	VECTOR3								m_EndIdx;		// �o�H�ړI�_
	VECTOR3								m_NowIdx;		// ���ݒn�_
	std::vector< VECTOR3 >				m_OpenNode;		// �I�[�v���m�[�hIdx�̔z��

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

