//=============================================================================
//		�R�c�Q�[���v���O����
//		�b�R�c�`�����P�P�Q�@�@�@�@�@�@                 ver 3.1        2022.2.16
//		�S���؋�ԕ����@�w�b�_�t�@�C��							SpPart.h
//=============================================================================
#pragma once
#include <list>
#include "GameMain.h"

struct CCellList
{
	std::list<CBaseObj*> SpList;		// ��ԕ����z��̒��ɐݒ肷��I�u�W�F�N�g�̃��X�g
};

class CSpPart
{
protected:
	CGameMain*				m_pGMain;		// �Q�[�����C��
	VECTOR3					m_BasePoint;	// ��ԕ����̊�_�i�ŏ��_�j
	VECTOR3					m_FullSize;		// ��Ԃ̑S�̃T�C�Y
	int						m_PartLevel;	// �ő啪����
	VECTOR3					m_CellSize;		// �ő啪�����̈�̃Z���T�C�Y
	std::vector<CCellList>	m_SpArray;		// ��ԕ����z��
	int						m_Num;			// ��Ԑ�

public:
	void CleateSpPart(VECTOR3 vMin, VECTOR3 vMax, int PartLevel=3);
	void ClearList();
	void SetObj(VECTOR3 vMin, VECTOR3 vMax, CBaseObj* pObj);
	void  RemoveObj(CBaseObj* obj, int Idx);
	// �ߐڃI�u�W�F�N�g���X�g
	void AllHitcheck(int, std::vector<CCellList>&, std::vector<CBaseObj*>&, std::vector<CBaseObj*>&);

	// �����蔻��
	BOOL HitcheckSpPart(CBaseObj* pObj, VECTOR3 vPos);
	BOOL HitcheckSpPartSub(DWORD Level, DWORD LvlMtn, CBaseObj* pObj);
	// �ߐڔ���
	BOOL SetKeepOffset(CBaseObj* pObj, VECTOR3 vPos);
	BOOL SetKeepOffsetSub(DWORD Level, DWORD LvlMtn, CBaseObj* pObj);

	DWORD BitSeparate32(DWORD n);
	DWORD Get2DMtnNumber(VECTOR3 vPos);
	void Get2DLevelAndMtnNumber(DWORD MinMtn, DWORD MaxMtn, DWORD& Level, DWORD& XOR);

	void MakeAABB(const VECTOR3& v1, const VECTOR3& v2, const FLOAT& r, VECTOR3& vMin, VECTOR3& vMax);

	// �A�N�Z�X�֐�
	std::vector<CCellList> GetSpArray() { return m_SpArray; }

	CSpPart(CGameMain*	pGMain);
	~CSpPart();

};