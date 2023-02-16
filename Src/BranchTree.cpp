#include "BranchTree.h"

//-----------------------------------------------------------------------------------------------------
//
// CLinerCollisionManager�N���X
//
//-----------------------------------------------------------------------------------------------------
// �R���X�g���N�^
CLinerCollisionManager::CLinerCollisionManager()
{
	m_nuLevel = 0;
	m_fW = 0.0f;
	m_fH = 0.0f;
	m_fLeft = 0.0f;
	m_fTop = 0.0f;
	m_fuW = 0.0f;
	m_fuH = 0.0f;
	m_nCellNum = 0;
	ppCellAry = NULL;
	
	m_nPow[0] = 1;
	for (int i = 1; i < MAX_BRANCH_LEVEL + 1; i++)
	{
		m_nPow[i] = m_nPow[i - 1] * 4;
	}
}

// �f�X�g���N�^
CLinerCollisionManager::~CLinerCollisionManager()
{
	for (int i = 0; i < m_nCellNum; i++)
	{
		if (ppCellAry[i] != NULL)
		{
			delete ppCellAry[i];
		}
	}
	delete[] ppCellAry;
}

// ����������
bool CLinerCollisionManager::Init(int level, float left, float top, float right, float bottom)
{
	// ���x�����ō����x���ȉ���
	if (level > MAX_BRANCH_LEVEL) {
		return false;
	}

	// �z��쐬
	m_nCellNum = (m_nPow[level + 1] - 1) / 3;
	ppCellAry = new CCell * [m_nCellNum];
	ZeroMemory(ppCellAry, sizeof(CCell*) * m_nCellNum);

	// �̈��o�^
	m_fLeft = left;
	m_fTop = top;
	m_fW = right - left;
	m_fH = bottom - top;
	m_fuW = m_fW / (1 << level);
	m_fuH = m_fH / (1 << level);

	m_nuLevel = level;

	return true;
}

// ���W�����Ԕԍ����Z�o
DWORD CLinerCollisionManager::GetMortonNumber(float left, float top, float right, float bottom)
{
	// �ŏ����x���ɂ�����e���ʒu���Z�o
	DWORD LT = GetPointElem(left, top);
	DWORD RB = GetPointElem(right, bottom);

	// ��Ԕԍ��̔r���I�_���a����
	// �������x�����Z�o
	DWORD Def = RB ^ LT;
	unsigned int HiLevel = 0;
	unsigned int i;
	for (i = 0; i < m_nuLevel; i++)
	{
		DWORD Check = (Def >> (i * 2)) & 0x3;
		if (Check != 0)
			HiLevel = i + 1;
	}
	DWORD SpaceNum = RB >> (HiLevel * 2);
	DWORD AddNum = (m_nPow[m_nuLevel - HiLevel] - 1) / 3;
	SpaceNum += AddNum;

	if (SpaceNum > m_nCellNum)
		return 0xffffffff;

	return SpaceNum;
}

DWORD CLinerCollisionManager::GetPointElem(float pos_x, float pos_y)
{
	return Get2DMortonNumber((WORD)((pos_x - m_fLeft) / m_fuW), (WORD)((pos_y - m_fTop) / m_fuH));
}

WORD CLinerCollisionManager::Get2DMortonNumber(WORD x, WORD y)
{
	return (WORD)(BitSeparate32(x) | (BitSeparate32(y) << 1));
}

DWORD CLinerCollisionManager::BitSeparate32(DWORD n)
{
	n = (n | (n << 8)) & 0x00ff00ff;
	n = (n | (n << 4)) & 0x0f0f0f0f;
	n = (n | (n << 2)) & 0x33333333;
	return (n | (n << 1)) & 0x55555555;
}

bool CLinerCollisionManager::Set(float left, float top, float right, float bottom, CCellToObj* spOFT)
{
	// �I�u�W�F�N�g�̋��E�͈͂���o�^���[�g���ԍ����Z�o
	DWORD Elem = GetMortonNumber(left, top, right, bottom);
	if (Elem < m_nCellNum) {
		// ��Ԃ������ꍇ�͐V�K�쐬
		if (!ppCellAry[Elem])
			CreateNewCell(Elem);
		return ppCellAry[Elem]->Push(spOFT);
	}
	return false;	// �o�^���s
}

// ��Ԃ𐶐�
bool CLinerCollisionManager::CreateNewCell(DWORD Elem)
{
	// �����̗v�f�ԍ�
	while (!ppCellAry[Elem])
	{
		// �w��̗v�f�ԍ��ɋ�Ԃ�V�K�쐬
		ppCellAry[Elem] = new CCell();

		// �e��ԂɃW�����v
		Elem = (Elem - 1) >> 2;
		if (Elem >= m_nCellNum) break;
	}
	return true;
}

// �Փ˔��胊�X�g���쐬����
DWORD CLinerCollisionManager::GetAllCollisionList(CollisionList** colList)
{
	// ���X�g�i�z��j�͕K�����������܂�
	m_CollList.ResetPos();

	// ���[�g��Ԃ̑��݂��`�F�b�N
	if (ppCellAry[0] == NULL)
		return 0;	// ��Ԃ����݂��Ă��Ȃ�

	// ���[�g��Ԃ�����
	std:: list<CBaseObj*> ColStac;
	GetCollisionList(0, ColStac);

	*colList = &m_CollList;

	return (DWORD)m_CollList.GetSize();
}

// ��ԓ��Փ˔��胊�X�g���쐬
bool CLinerCollisionManager::GetCollisionList(DWORD Elem, std::list<CBaseObj*>& ColStac)
{
	std::list<CBaseObj*>::iterator it;
	// �@ ��ԓ��̃I�u�W�F�N�g���m�̏Փ˃��X�g�쐬
	CCellToObj* spOFT1 = ppCellAry[Elem]->GetFirstObj();
	while (spOFT1 != 0)
	{
		CCellToObj* spOFT2 = spOFT1->m_pNext;
		while (spOFT2 != 0) {
			m_CollList.wright(spOFT1->m_pObj, spOFT2->m_pObj);
			spOFT2 = spOFT2->m_pNext;
		}
		// �A �Փ˃X�^�b�N�Ƃ̏Փ˃��X�g�쐬
		for (it = ColStac.begin(); it != ColStac.end(); it++) {
			m_CollList.wright(spOFT1->m_pObj, *it);
		}
		spOFT1 = spOFT1->m_pNext;
	}

	bool ChildFlag = false;
	// �B �q��ԂɈړ�
	DWORD ObjNum = 0;
	DWORD i, NextElem;
	for (i = 0; i < 4; i++) {
		NextElem = Elem * 4 + 1 + i;
		if (NextElem < m_nCellNum && ppCellAry[Elem * 4 + 1 + i]) {
			if (!ChildFlag) {
				// �C �o�^�I�u�W�F�N�g���X�^�b�N�ɒǉ�
				spOFT1 = ppCellAry[Elem]->GetFirstObj();
				while (spOFT1 != 0) {
					ColStac.push_back(spOFT1->m_pObj);
					ObjNum++;
					spOFT1 = spOFT1->m_pNext;
				}
			}
			ChildFlag = true;
			GetCollisionList(Elem * 4 + 1 + i, ColStac);	// �q��Ԃ�
		}
	}

	// �D �X�^�b�N����I�u�W�F�N�g���O��
	if (ChildFlag) {
		for (i = 0; i < ObjNum; i++)
			ColStac.pop_back();
	}

	return true;
}
//-----------------------------------------------------------------------------------------------------
//
// CCell�N���X
//
//-----------------------------------------------------------------------------------------------------
// �R���X�g���N�^
CCell::CCell()
{
	m_pLate = 0;
}

// �f�X�g���N�^
CCell::~CCell()
{
	//if (m_pLate != NULL)
	//{
	//	ResetLink(m_pLate);
	//}
}

// �����N��S�ă��Z�b�g����
void CCell::ResetLink(CCellToObj* spOFT)
{
	if (spOFT->m_pPre != 0) {
		ResetLink(spOFT->m_pPre);
	}
	spOFT = 0;
}

// OFT���v�b�V��
bool CCell::Push(CCellToObj* spOFT)
{
	if (spOFT == 0) return false;	// �����I�u�W�F�N�g�͓o�^���Ȃ�
	if (spOFT->m_pCell == this) return false;	// 2�d�o�^�`�F�b�N
	if (m_pLate == 0) {
		m_pLate = spOFT;	// ��ԂɐV�K�o�^
	}
	else
	{
		// �ŐVOFT�I�u�W�F�N�g���X�V
		spOFT->m_pNext = m_pLate;
		m_pLate->m_pPre = spOFT;
		m_pLate = spOFT;
	}
	spOFT->m_pCell = this;	// ��Ԃ�o�^
	return true;
}

CCellToObj* CCell::GetFirstObj()
{
	if (m_pLate == NULL) {
		return NULL;
	}
	if (m_pLate->m_pObj->GetActive())
	{
		return m_pLate;
	}
	else {
		return NULL;
	}
}

// �폜�����I�u�W�F�N�g���`�F�b�N
bool CCell::OnRemove(CCellToObj* pRemoveObj)
{
	if (m_pLate == pRemoveObj) {
		// ���̃I�u�W�F�N�g�ɑ}���ւ�
		if (m_pLate != NULL)
			m_pLate = m_pLate->m_pNext;
	}
	return true;
}

//-----------------------------------------------------------------------------------------------------
//
// CCellToObj�N���X
//
//-----------------------------------------------------------------------------------------------------
// �R���X�g���N�^
CCellToObj::CCellToObj(int id)
{
	m_id = 0;
	m_pCell = NULL;
	m_pObj = NULL;
	m_pPre = NULL;
	m_pNext = NULL;
}

// �f�X�g���N�^
CCellToObj::~CCellToObj()
{

}

// ���烊�X�g����O���
bool CCellToObj::Remove()
{
	// ���ɊO��Ă���ꍇ�͏I��
	if (!m_pCell)
	{
		return false;
	}

	// ���g�̓o�^���Ă����Ԃɒʒm
	if (!m_pCell->OnRemove(this))
	{
		return false;
	}

	// ��E����
	if (m_pPre != 0) 
	{
		m_pPre->m_pNext = m_pNext;
	}
	if (m_pNext != 0) 
	{
		m_pNext->m_pPre = m_pPre;
	}
	m_pPre = 0;
	m_pNext = 0;
	m_pCell = NULL;
	return true;
}

//-----------------------------------------------------------------------------------------------------
//
// CollisionList�N���X
//
//-----------------------------------------------------------------------------------------------------
// �R���X�g���N�^
CollisionList::CollisionList()
{
	root_ = 0;
	pos_ = 0;
	mallocSize_ = 0;
	root_ = (CBaseObj**)malloc(0);
}

// �f�X�g���N�^
CollisionList::~CollisionList()
{
	reflesh();
}

size_t CollisionList::GetSize()
{
	return pos_;
}

CBaseObj** CollisionList::GetRootPtr()
{
	return root_;
}

void CollisionList::ResetPos()
{
	pos_ = 0;
}

void CollisionList::wright(CBaseObj* obj1, CBaseObj* obj2)
{
	if (pos_ >= mallocSize_) {
		root_ = (CBaseObj**)realloc(root_, sizeof(CBaseObj*) * (mallocSize_ + COLLISIONLIST_REALLOCSIZE));
		mallocSize_ += COLLISIONLIST_REALLOCSIZE;
	}
	root_[pos_++] = obj1;
	root_[pos_++] = obj2;
}

void CollisionList::reflesh()
{
	if (root_)
	{
		free(root_);
	}
}