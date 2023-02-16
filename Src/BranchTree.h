#pragma once
#include "BaseProc.h"
#include <list>


#define MAX_BRANCH_LEVEL 5
#define COLLISIONLIST_REALLOCSIZE 100

class CCellToObj;
class CBaseObj;
// ��ԃN���X
class CCell
{
private:
	CCellToObj* m_pLate;
	
public:
	// �R���X�g���N�^
	CCell();
	// �f�X�g���N�^
	~CCell();

	// �����N��S�ă��Z�b�g����
	void ResetLink(CCellToObj* spOFT);

	//��ԂɃI�u�W�F�N�g��o�^
	bool Push(CCellToObj* spOFT);

	CCellToObj* GetFirstObj();

	// �폜�����I�u�W�F�N�g���`�F�b�N
	bool OnRemove(CCellToObj* pRemoveObj);
};

// ��ԂƃI�u�W�F�N�g�����т���N���X
class CCellToObj
{
public:
	CBaseObj*	m_pObj;
	CCell*		m_pCell;

	// ���X�g�̑O
	CCellToObj*	m_pPre;
	// ���X�g�̌��
	CCellToObj* m_pNext;
	int m_id;

	// �R���X�g���N�^�A�f�X�g���N�^
	CCellToObj(int id = 0);
	virtual ~CCellToObj();

	// ��Ԃ���O���
	bool Remove();
};

// �Փ˃��X�g�N���X
class CollisionList
{
private:
	CBaseObj** root_;		// ���X�g���[�g�|�C���^
	size_t	pos_;
	size_t	mallocSize_;

public:
	size_t GetSize();
	CBaseObj** GetRootPtr();
	void ResetPos();
	void wright(CBaseObj* obj1, CBaseObj* obj2);
	void reflesh();

	// �R���X�g���N�^�A�f�X�g���N�^
	CollisionList();
	~CollisionList();
};

// �ՓˑΉ����X�g�N���X
class CLinerCollisionManager
{
protected:
	CCell** ppCellAry;		// ���^�|�C���^�z��
	unsigned int m_nPow[MAX_BRANCH_LEVEL + 1];
	float m_fW;
	float m_fH;
	float m_fLeft;
	float m_fTop;
	float m_fuW;
	float m_fuH;
	int m_nCellNum;
	unsigned int m_nuLevel;
	CollisionList m_CollList;

public:
	// �R���X�g���N�^
	CLinerCollisionManager();
	// �f�X�g���N�^
	~CLinerCollisionManager();

	// ���胊�X�g�̍쐬
	bool Init(int level, float left, float top, float right, float bottom);
	// ��Ԃ��Z�o���A�o�^
	bool Set(float left, float top, float right, float bottom, CCellToObj* spOFT);
	// �Փ˔��胊�X�g���쐬
	DWORD GetAllCollisionList(CollisionList** colList);
	// ��ԓ��ŏՓ˔��胊�X�g���쐬
	bool GetCollisionList(DWORD Elem, std::list<CBaseObj*>& ColStac);

	// ���W�����Ԕԍ����쐬
	DWORD GetMortonNumber(float left, float top, float right, float bottom);
	// ���W�����`4���ؗv�f�ԍ��ϊ��֐�
	DWORD GetPointElem(float pos_x, float pos_y);
	// 2D���[�g����Ԕԍ��Z�o�֐�
	WORD Get2DMortonNumber(WORD x, WORD y);
	// �r�b�g�����֐�
	DWORD BitSeparate32(DWORD n);

	// ��Ԃ𐶐�
	bool CreateNewCell(DWORD Elem);
};