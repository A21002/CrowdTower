//=============================================================================
//		�R�c�Q�[���v���O����
//		�b�R�c�`�����P�P�Q�@�@�@�@�@�@                 ver 3.1        2022.2.16
//		�S���؋�ԕ����@�\�[�X�t�@�C��							SpPart.cpp
//=============================================================================

#include "SpPart.h"

//-----------------------------------------------------------------------------
//
// �S���؋�ԕ����R���X�g���N�^
//
// ����
//   CGameMain*	pGMain
//
//-----------------------------------------------------------------------------
CSpPart::CSpPart(CGameMain*	pGMain)
{
	m_pGMain = pGMain;
	m_Num = 0;
	m_PartLevel = 0;
}

//-----------------------------------------------------------------------------
//
// �S���؋�ԕ����f�X�g���N�^
//
//
//-----------------------------------------------------------------------------
CSpPart::~CSpPart()
{
	m_SpArray.clear();					// �z��폜
	m_SpArray.shrink_to_fit();			// �s�v���������

}

//-----------------------------------------------------------------------------
//
// �S���؋�Ԃ̍쐬
// (���`�S���؋�Ԃ̔z����쐬����)
//
// ����
//   VECTOR3 vMin  : �S�̋�Ԃ����͂ރo�E���f�B���O�{�b�N�XAABB��vMin
//   VECTOR3 vMax  : �S�̋�Ԃ����͂ރo�E���f�B���O�{�b�N�XAABB��vMax
//   int PartLevel : �ő啪�����B�ȗ��l�͂R
//
//-----------------------------------------------------------------------------
void CSpPart::CleateSpPart( VECTOR3 vMin, VECTOR3 vMax, int PartLevel)
{
	m_BasePoint = vMin;                // ��ԕ����̊�_�i�ŏ��_�j
	m_FullSize = vMax - vMin;          // ��Ԃ̑S�̃T�C�Y
	m_PartLevel = PartLevel;           // �ő啪����
	m_CellSize = m_FullSize / pow(2,m_PartLevel);    // �ő啪�����̈�̃Z���T�C�Y

	m_SpArray.clear();			// �z��폜
	m_SpArray.shrink_to_fit();	// �s�v���������

	// �v�f�����v�Z����
	int Num = 0;
	for (int i = 0; i <= m_PartLevel; i++)
	{
		Num += pow(4, i);
	}
	m_Num = Num;
	m_SpArray.resize(Num);   // �w��v�f���̔z��ɂ���

	ClearList();             // �S���؋�Ԃ̃��X�g�����ׂăN�����[����
}
//-----------------------------------------------------------------------------
//
// �S���؋�Ԃ̃��X�g�����ׂăN�����[����
//
// ����
//
//-----------------------------------------------------------------------------
void CSpPart::ClearList()
{
	// 
	for (int i = 0; i < m_SpArray.size(); i++)
	{
		m_SpArray[i].SpList.clear();
	}
}

//-----------------------------------------------------------------------------
//
// ���`�S���؋�ԂɃI�u�W�F�N�g��o�^����
//
// ����
//   VECTOR3 vMin  : �I�u�W�F�N�g�����͂ރo�E���f�B���O�{�b�N�XAABB��vMin
//   VECTOR3 vMax  : �I�u�W�F�N�g�����͂ރo�E���f�B���O�{�b�N�XAABB��vMax
//   CBaseObj* pObj: �I�u�W�F�N�g�A�h���X
//
//-----------------------------------------------------------------------------
void CSpPart::SetObj(VECTOR3 vMin, VECTOR3 vMax, CBaseObj* pObj)
{
	// ��ԃ��[�g���ԍ����Z�o
	DWORD MinMtn = Get2DMtnNumber(vMin-m_BasePoint);
	DWORD MaxMtn = Get2DMtnNumber(vMax-m_BasePoint);

	// ���x���ԍ��Ƃ��̃��x���̃��[�g���ԍ������߂�
	DWORD Level, LvlMtn;
	Get2DLevelAndMtnNumber( MinMtn, MaxMtn, Level, LvlMtn);

	// ���`�S���ؔz��̓Y�������߂�
	DWORD Idx = LvlMtn + ((pow(4, Level) - 1) / 3);

	// ���`�S���ؔz��̃��X�g�ɃI�u�W�F�N�g�̃A�h���X��o�^����
	m_SpArray[Idx].SpList.push_back(pObj);
}


void CSpPart::RemoveObj(CBaseObj* obj, int Idx)
{
	/*
	std::list<CBaseObj*>::iterator it = m_SpArray[Idx].SpList.begin();
	for (int i = 1; i < place; i++) {
		it++;
	}
	m_SpArray[Idx].SpList.erase(it);
	*/
	m_SpArray[Idx].SpList.remove(obj);

}

//-----------------------------------------------------------------------------
// ���`�S���؋�Ԃɓo�^����Ă���I�u�W�F�N�g��T�����ē����蔻����s��
// CheckPoint3
//
// ����
//   int Idx						: �T�������ԗv�f��
//   vector<CCellList>& pList		: �����蔻����s������̃��X�g
//   vector<CBaseObj*>& stacList	: ��ʋ�Ԃɂ���I�u�W�F�N�g
//	 vector<CBaseObj*>& hitList		: �q�b�g�����I�u�W�F�N�g�z��
//-----------------------------------------------------------------------------
void CSpPart::AllHitcheck(int Idx, std::vector<CCellList>& pList, std::vector<CBaseObj*>& stacList, std::vector<CBaseObj*>& hitList){
	std::list<CBaseObj*>::iterator pIt;
	std::list<CBaseObj*>::iterator it;

	// �@ ��ԓ��̃I�u�W�F�N�g���m�̏Փ˃��X�g�쐬
	if (m_SpArray[Idx].SpList.size() > 0) {
		// list�̋�ԂɃI�u�W�F�N�g�����蓖�Ă��Ă����
		
		if (pList[Idx].SpList.size() > 0) {
			// �G�̋�ԂɃI�u�W�F�N�g�������Ă����
			pIt = pList[Idx].SpList.begin();

			while (pIt != pList[Idx].SpList.end()) {
				// �G���X�g�̂��ׂẴI�u�W�F�N�g���Q��
				it = m_SpArray[Idx].SpList.begin();
				while (it != m_SpArray[Idx].SpList.end()) {
					// ������Ԃ̓G�I�u�W�F�N�g�Ƃ̓����蔻�胊�X�g�ɓ����
					hitList.push_back(*it);
					hitList.push_back(*pIt);
					it++;
				}
				pIt++;
			}
		}
		std::list<CBaseObj*>::iterator stacit = m_SpArray[Idx].SpList.begin();
		// ��ʋ�ԂƂ̓����蔻��
		for (int j = 0; j < m_SpArray[Idx].SpList.size(); j++) {
			for (int i = 0; i < stacList.size(); i++) {
				hitList.push_back(*stacit);
				hitList.push_back(stacList[i]);
			}
			stacit++;
		}
		
	}

	bool ChildFlag = false;
	// �B �q��ԂɈړ�
	DWORD ObjNum = 0;
	DWORD i, NextElem;
	for (i = 0; i < 4; i++) {
		NextElem = Idx * 4 + 1 + i;
		if (NextElem < m_Num) {
			if (pList[Idx].SpList.size() != 0) {
				if (!ChildFlag) {
					// �C �o�^�I�u�W�F�N�g���X�^�b�N�ɒǉ�
					it = pList[Idx].SpList.begin();
					for (int j = 0; j < pList[Idx].SpList.size(); j++) {
						stacList.push_back(*it);
						ObjNum++;
						it++;
					}
				}
			}
			ChildFlag = true;
			AllHitcheck(NextElem, pList, stacList, hitList);
		}
	}

	// �D �X�^�b�N����I�u�W�F�N�g���O��
	if (ChildFlag) {
		for (i = 0; i < ObjNum; i++)
			stacList.pop_back();
	}
}
 
//-----------------------------------------------------------------------------
//
// ���`�S���؋�Ԃɓo�^����Ă���I�u�W�F�N�g��T�����ē����蔻����s��
//
// ����
//   CBaseObj* pObj : �ڐG����ΏۃI�u�W�F�N�g
//   VECTOR3 vPos   : �T������ʒu
//
//-----------------------------------------------------------------------------
BOOL CSpPart::HitcheckSpPart(CBaseObj* pObj, VECTOR3 vPos)
{
	BOOL bRet = FALSE;

	// ��ԃ��[�g���ԍ����Z�o
	DWORD Mtn = Get2DMtnNumber(vPos-m_BasePoint);
	DWORD LvlMtn;

	// ��ԃ��x�����̐ڐG����
	for (int i = 0; i <= m_PartLevel; i++)
	{
		LvlMtn = Mtn >> ((m_PartLevel-i) * 2);
		if (HitcheckSpPartSub(i, LvlMtn, pObj))
		{
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

//-----------------------------------------------------------------------------
//
// ���`�S���؋�Ԃɓo�^����Ă���I�u�W�F�N�g��T�����Ď��ۂɓ����蔻����s���T�u�֐�
//
// ����
//   DWORD Level    : ��ԃ��x��
//   DWORD LvlMtn   : ���x�����[�g���ԍ�
//   CBaseObj* pObj : �ڐG����ΏۃI�u�W�F�N�g
//
//-----------------------------------------------------------------------------
BOOL CSpPart::HitcheckSpPartSub(DWORD Level, DWORD LvlMtn, CBaseObj* pObj)
{
	// ���`�S���ؔz��̓Y�������߂�
	DWORD Idx = LvlMtn + ((pow(4, Level) - 1) / 3);

	std::list<CBaseObj*>::iterator itr;    // ���X�g�̃C�e���[�^
	BOOL bRet = FALSE;
	
	// ���X�g�����ǂ��ē����蔻����s��
	for (itr = m_SpArray[Idx].SpList.begin(); itr != m_SpArray[Idx].SpList.end(); itr++)
	{
		if (*itr)     // ���X�g������Ƃ��i�I�u�W�F�N�g���o�^����Ă���Ƃ��j
		{
			// �o�E���f�B���O�{�b�N�X�ƃ��C�i�����j�̓����蔻����s��
			if (pObj->Hitcheck(*itr, (*itr)->GetBBox()))
			{
				bRet = TRUE;
			}
		}
	}
	return bRet;
}

//-----------------------------------------------------------------------------
//
// ���`�S���؋�Ԃɓo�^����Ă���I�u�W�F�N�g��T�����ė���������s��
//
// ����
//   CBaseObj* pObj : �ڐG����ΏۃI�u�W�F�N�g
//   VECTOR3 vPos   : �T������ʒu
//
//-----------------------------------------------------------------------------
BOOL CSpPart::SetKeepOffset(CBaseObj* pObj, VECTOR3 vPos)
{
	BOOL bRet = FALSE;

	// ��ԃ��[�g���ԍ����Z�o
	DWORD Mtn = Get2DMtnNumber(vPos - m_BasePoint);
	DWORD LvlMtn;

	// ��ԃ��x�����̐ڐG����
	for (int i = 0; i <= m_PartLevel; i++)
	{
		LvlMtn = Mtn >> ((m_PartLevel - i) * 2);
		if (SetKeepOffsetSub(i, LvlMtn, pObj))
		{
			bRet = TRUE;
		}
	}

	return bRet;
}

//-----------------------------------------------------------------------------
//
// ���`�S���؋�Ԃɓo�^����Ă���I�u�W�F�N�g��T�����Ď��ۂɗ���������s���T�u�֐�
//
// ����
//   DWORD Level    : ��ԃ��x��
//   DWORD LvlMtn   : ���x�����[�g���ԍ�
//   CBaseObj* pObj : �ڐG����ΏۃI�u�W�F�N�g
//
//-----------------------------------------------------------------------------
BOOL CSpPart::SetKeepOffsetSub(DWORD Level, DWORD LvlMtn, CBaseObj* pObj)
{
	// ���`�S���ؔz��̓Y�������߂�
	DWORD Idx = LvlMtn + ((pow(4, Level) - 1) / 3);

	std::list<CBaseObj*>::iterator itr;    // ���X�g�̃C�e���[�^
	BOOL bRet = FALSE;

	// ���X�g�����ǂ��ē����蔻����s��
	for (itr = m_SpArray[Idx].SpList.begin(); itr != m_SpArray[Idx].SpList.end(); itr++)
	{
		if (*itr)     // ���X�g������Ƃ��i�I�u�W�F�N�g���o�^����Ă���Ƃ��j
		{
			m_pGMain->m_nScore++;

			// �o�E���f�B���O�{�b�N�X�ƃ��C�i�����j�̓����蔻����s��
			if (pObj->SetKeepOffset(*itr))
			{
				bRet = TRUE;
			}
		}
	}
	return bRet;
}


//-----------------------------------------------------------------------------
//
// �r�b�g�����֐�
//
//-----------------------------------------------------------------------------
DWORD CSpPart::BitSeparate32(DWORD n)
{
	n = (n | (n << 8)) & 0x00ff00ff;
	n = (n | (n << 4)) & 0x0f0f0f0f;
	n = (n | (n << 2)) & 0x33333333;
	return (n | (n << 1)) & 0x55555555;
}

//-----------------------------------------------------------------------------
//
// 2D��Ԃ̃��[�g���ԍ����Z�o
//
//-----------------------------------------------------------------------------
DWORD CSpPart::Get2DMtnNumber(VECTOR3 vPos)
{
	DWORD x, z;

	x = (int)vPos.x / m_CellSize.x;
	z = (int)vPos.z / m_CellSize.z;

	return (BitSeparate32(x) | (BitSeparate32(z) << 1));
}

//-----------------------------------------------------------------------------
//
// 2D��Ԃ̋�ԃ��x���Ƃ��̃��x���̃��[�g���ԍ����Z�o
//
// ����
//   DWORD MinMtn   �ŏ����[�g���ԍ�
//   DWORD MaxMtn   �ő僂�[�g���ԍ�
//   DWORD& Level   ��ԃ��x��(Out)
//   DWORD& LvlMtn  ���̃��x���̃��[�g���ԍ�(Out)
//
//-----------------------------------------------------------------------------
void CSpPart::Get2DLevelAndMtnNumber(DWORD MinMtn, DWORD MaxMtn, DWORD& Level, DWORD& LvlMtn)
{
	DWORD XORMtn = MaxMtn ^ MinMtn;  // �r���I�_���a���Ƃ�

	Level = m_PartLevel;
	DWORD WXORMtn;

	// ��ԃ��x�������߂�
	if (XORMtn == 0)
	{
		// ��ԃ��x���ŉ��ʃ��x���ɑ�����
		Level = m_PartLevel;
	}
	else {
		// ��ԃ��x���O�`�ŉ��ʁ|�P���x���ɑ�����
		for (int i = 0; i < m_PartLevel; i++)
		{
			// �Q�r�b�g�Â��肵�Ēl�����邩�ǂ������肷��
			// �l�̂���ŏ�ʂ̃r�b�g�ʒu����ԃ��x���ł���
			WXORMtn = XORMtn >> (i*2);
			WXORMtn &= 0x00000003;
			if (WXORMtn != 0)  Level = (m_PartLevel-1) - i;
		}
	}

	// ���̃��x���̃��[�g���ԍ������߂�
	LvlMtn = MaxMtn >> ((m_PartLevel - Level) * 2);

}

//------------------------------------------------------------------------
//
// v1�`v2�����̒����ŁA���ar�̕��̃o�E���f�B���O�{�b�N�X�iAABB�j���쐬����
//
// const VECTOR3& v1 : �����̎n�_���W
// const VECTOR3& v2 : �����̏I�_���W
// const FLOAT& r    : �o�E���f�B���O�{�b�N�X�̑傫���i���a�j
// VECTOR3& vMin     : AABB��vMin(Out)
// VECTOR3& vMax     : AABB��vMax(Out)
//
//------------------------------------------------------------------------
void CSpPart::MakeAABB(const VECTOR3& v1, const VECTOR3& v2, const FLOAT& r, VECTOR3& vMin, VECTOR3& vMax)
{
	if (v1.x > v2.x) {
		vMin.x = v2.x;
	}
	else {
		vMin.x = v1.x;
	}
	vMin.x -= r;

	if (v1.y > v2.y) {
		vMin.y = v2.y;
	}
	else {
		vMin.y = v1.y;
	}
	vMin.y -= r;

	if (v1.z > v2.z) {
		vMin.z = v2.z;
	}
	else {
		vMin.z = v1.z;
	}
	vMin.z -= r;

	if (v1.x < v2.x) {
		vMax.x = v2.x;
	}
	else {
		vMax.x = v1.x;
	}
	vMax.x += r;

	if (v1.y < v2.y) {
		vMax.y = v2.y;
	}
	else {
		vMax.y = v1.y;
	}
	vMax.y += r;

	if (v1.z < v2.z) {
		vMax.z = v2.z;
	}
	else {
		vMax.z = v1.z;
	}
	vMax.z += r;
}

