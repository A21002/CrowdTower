//=============================================================================
//		�R�c�Q�[���v���O����
//		�b�R�c�`�����P�P�Q�@�@�@�@�@�@                 ver 3.1        2022.2.16
// 
//		�o�H�T���@�\�[�X�t�@�C��								SearchRoute.cpp
// 
//		AStar�A���S���Y���𗘗p���Čo�H�T�����s���v���O�����ł���
// 
//=============================================================================
#include  "SearchRoute.h"
#include  "Map.h"

//-----------------------------------------------------------------------------
//
// �o�H�T���R���X�g���N�^
//
// ����
//   CGameMain*	pGMain
//
//-----------------------------------------------------------------------------
CSearchRoute::CSearchRoute(CGameMain*	pGMain)
{
	m_pGMain = pGMain;
	m_Basepoint = VECTOR3(0, 0, 0);	// �z��̍���O�̍��W�l
	m_Partssize = 0;				// �m�[�h��̑傫��
	m_StIdx = VECTOR3(0, 0, 0);		// �o�H�J�n�_
	m_EndIdx = VECTOR3(0, 0, 0);	// �o�H�ړI�_
	m_NowIdx = VECTOR3(0, 0, 0);	// ���ݒn�_�_
}

//-----------------------------------------------------------------------------
//
// �o�H�T���f�X�g���N�^
//
//
//-----------------------------------------------------------------------------
CSearchRoute::~CSearchRoute()
{
	m_ANodeArray.clear();			// �o�H�T���z��폜
	m_ANodeArray.shrink_to_fit();	// �o�H�T���s�v���������
	m_OpenNode.clear();				// �o�H�T��Open�z��폜
	m_OpenNode.shrink_to_fit();		// �o�H�T��Open�z��s�v���������
}

//-----------------------------------------------------------------------------
//
// �o�H�T���z��̐���
// �i�}�b�v�̃��[�g�T���R���W�������g�p���āA�o�H�T���z��̐����A�������j
//
// �E �o�H�T���z��́A���x�ł������B�f�[�^�͐����̓s�x�N�����[�����
//
// ����
//
//-----------------------------------------------------------------------------
void CSearchRoute::MakeANode()
{
	m_Partssize = m_pGMain->m_pMapProc->GetRouteColMap()->m_Partssize;		// �m�[�h��̑傫��
	m_Basepoint = m_pGMain->m_pMapProc->GetRouteColMap()->m_Basepoint;		// �z��̍���O�̍��W�l
	m_StIdx = VECTOR3(0, 0, 0);		// �o�H�J�n�_
	m_EndIdx = VECTOR3(0, 0, 0);	// �o�H�ړI�_
	m_NowIdx = VECTOR3(0, 0, 0);	// ���ݒn�_�_

	int znum = (int)(m_pGMain->m_pMapProc->GetRouteColMap()->m_MapNumXZ.y);   // 1�����ڗv�f��
	int xnum = (int)(m_pGMain->m_pMapProc->GetRouteColMap()->m_MapNumXZ.x);   // 2�����ڗv�f��

	// �o�H�T���z��E�o�H�T��Open�z��̏�����
	m_ANodeArray.clear();			// �o�H�T���z��폜
	m_ANodeArray.shrink_to_fit();	// �o�H�T���s�v���������
	m_OpenNode.clear();				// �o�H�T��Open�z��폜
	m_OpenNode.shrink_to_fit();		// �o�H�T��Open�z��s�v���������

	//	�o�H�T���z��̃T�C�Y�̏�����
	m_ANodeArray.resize(znum, std::vector<ANODE>(xnum));
}
//-----------------------------------------------------------------------------
//
// �o�H�T���z��̌o�H�}�̍쐬
//
// �E�o�H�}�́A�J�n�ʒu�E�ړI�n��ς��āA���x�ł��쐬�B
// �E�o�H�T���z��̓N�����[����㏑�������B�Ȃ��A�R���W�������͍Ďg�p�����B
//
// ����
//   VECTOR3 vStart : �o�H�T���J�n�ʒu
//   VECTOR3 vEnd   : �o�H�T���ړI�n
//
// �߂�l
//   TRUE:�o�H�m��   FALSE:�o�H�Ȃ�
//-----------------------------------------------------------------------------
BOOL CSearchRoute::MakeANodeRoute(VECTOR3 vStart, VECTOR3 vEnd)
{
	// �J�n�_�E�ړI�n����o�H�T���z��Y���̐ݒ�
	m_StIdx.x = (int)((vStart.x - m_Basepoint.x) / m_Partssize);
	m_StIdx.z = (int)((vStart.z - m_Basepoint.z) / m_Partssize);
	m_EndIdx.x = (int)((vEnd.x - m_Basepoint.x) / m_Partssize);
	m_EndIdx.z = (int)((vEnd.z - m_Basepoint.z) / m_Partssize);

	// �m�[�h�̏����N�����[
	for (int z = 0; z < m_ANodeArray.size(); z++)
	{
		for (int x = 0; x < m_ANodeArray[z].size(); x++)
		{
			m_ANodeArray[z][x].Init();
		}
	}
	// �o�H�T��Open�z��̏�����
	m_OpenNode.clear();				// �o�H�T��Open�z��폜
	m_OpenNode.shrink_to_fit();		// �o�H�T��Open�z��s�v���������

	// �J�n�_�̐ݒ�
	OpenNode(m_StIdx, VECTOR3(0, 0, 0), TRUE);  // �J�n�_�̃m�[�h��Open
	m_NowIdx = m_StIdx;

	// �ړI�n�ɓ��B����܂ŌJ��Ԃ�
	while (!(m_NowIdx.x == m_EndIdx.x && m_NowIdx.z == m_EndIdx.z))
	{
		// �W�����̃m�[�h���n����������
		if (OpenOctPos(m_NowIdx) <= 0)
		{
			; // ����̃m�[�h���ЂƂ�Open�ł��Ȃ�����
		}

		// ����m_NowIdx�̃m�[�h��Close����
		CloseNode(m_NowIdx);

		// ��m�[�h�̔���
		// Open�m�[�h�̒��ň�ԃX�R�A���������m�[�h��I������
		VECTOR3 Idx;
		if (!SearchOpenNodeScore(Idx))
		{
			//  Open�m�[�h����������Ƃ��͏I��
			//MessageBox(NULL, _T("Open�m�[�h���������"), _T("�o�H�T������"), IDOK);
			return FALSE;
		}

		// �I�������m�[�h����m�[�h�Ƃ���B
		m_NowIdx = Idx;

	}

	// �ړI�n����e�m�[�h�������̂ڂ��Čo�H���m�肷��
	DecisionRoute();

	return TRUE;
}
//-----------------------------------------------------------------------------
//
// �w��ʒu���W�����̃m�[�h���n����������
//
// ����
//   VECTOR3 Idx    : �I�[�v�����悤�Ƃ���e�̈ʒu
//
// �߂�l
//   Open�ł����m�[�h����Ԃ�
//-----------------------------------------------------------------------------
int CSearchRoute::OpenOctPos(VECTOR3 Idx)
{
	int n = 0;

	if (OpenNode(VECTOR3(Idx.x - 1, Idx.y, Idx.z - 1), Idx)) n++;
	if (OpenNode(VECTOR3(Idx.x, Idx.y, Idx.z - 1), Idx)) n++;
	if (OpenNode(VECTOR3(Idx.x + 1, Idx.y, Idx.z - 1), Idx)) n++;
	if (OpenNode(VECTOR3(Idx.x - 1, Idx.y, Idx.z), Idx))     n++;
	if (OpenNode(VECTOR3(Idx.x + 1, Idx.y, Idx.z), Idx))     n++;
	if (OpenNode(VECTOR3(Idx.x - 1, Idx.y, Idx.z + 1), Idx)) n++;
	if (OpenNode(VECTOR3(Idx.x, Idx.y, Idx.z + 1), Idx)) n++;
	if (OpenNode(VECTOR3(Idx.x + 1, Idx.y, Idx.z + 1), Idx)) n++;

	return n;
}
//-----------------------------------------------------------------------------
//
// �m�[�h���n����������
//
// ����
//   VECTOR3 Idx    : �I�[�v���ʒu
//   VECTOR3 parent : �e�̈ʒu
//   BOOL bBegin    : �ŏ��̃m�[�h�̂Ƃ�TRUE�i�ȗ��lFALSE�j
//
// �߂�l
//   BOOL TRUE:�I�[�v���ł����Ƃ�   FALSE: �I�[�v���ł��Ȃ��Ƃ�
//-----------------------------------------------------------------------------
BOOL CSearchRoute::OpenNode(VECTOR3 Idx, VECTOR3 parent, BOOL bBegin)
{
	// Open���悤�Ƃ���ʒu
	int x = (int)Idx.x;
	int z = (int)Idx.z;

	// �e�m�[�h�̈ʒu
	int px = (int)parent.x;  // 
	int pz = (int)parent.z;

	// Open�ł��邩�ǂ����̃`�F�b�N
	if (!CheckOpenNode(x, z))
	{
		return FALSE;  // �I�[�v���ł��Ȃ�
	}

	// Open����
	m_ANodeArray[z][x].status = eOpen;
	if (bBegin)   // �ŏ��̃m�[�h��
	{
		m_ANodeArray[z][x].cost = 0;  // �R�X�g��0
	}
	else {
		m_ANodeArray[z][x].cost = m_ANodeArray[pz][px].cost + 1;  // �e�̃R�X�g�{�P
	}
	m_ANodeArray[z][x].heuristic = Calcheuristic(x, z);
	m_ANodeArray[z][x].score = m_ANodeArray[z][x].cost
							+ m_ANodeArray[z][x].heuristic;
	m_ANodeArray[z][x].parent = parent;

	// Open�m�[�h�z��ɃI�[�v�������m�[�h��Idx�ԍ���o�^����
	m_OpenNode.push_back(VECTOR3(x, 0, z));

	return TRUE;
}
//-----------------------------------------------------------------------------
//
// �m�[�h���b������������
//
// ����
//   VECTOR3 Idx    : �N���[�X�ʒu
//
//-----------------------------------------------------------------------------
void CSearchRoute::CloseNode(VECTOR3 Idx)
{
	int x = (int)Idx.x;
	int z = (int)Idx.z;

	m_ANodeArray[z][x].status = eClose;

	// Open�m�[�h�z�񂩂�폜����
	for (int i = 0; i < m_OpenNode.size(); i++)
	{
		if (m_OpenNode[i].x == x &&
			m_OpenNode[i].z == z)
		{
			m_OpenNode.erase(m_OpenNode.begin() + i);  // �z�񂩂�폜
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//
// �ړI�n����e�m�[�h�������̂ڂ��Čo�H���m�肷��
//
// �E�ړI�n����e�m�[�hparent�������̂ڂ��ďo���n�܂Ŗ߂�B
// �E���̂Ƃ��A���m�[�hnext�Ɏ��̃m�[�h�̓Y����ݒ肵�Ă���
// �E���̏������I���ƁA�o���n����next�����ǂ��ĖړI�n�܂ōs����悤�ɂȂ�
//
//-----------------------------------------------------------------------------
void CSearchRoute::DecisionRoute()
{
	VECTOR3 Idx = m_EndIdx;   // �ŏ����ړI�n�ƂȂ�
	VECTOR3 IdxOld = Idx;
	int     x, z, ox, oz;

	while (!(Idx.x == m_StIdx.x && Idx.z == m_StIdx.z))  // �I���͏o���n�ƂȂ�
	{
		IdxOld = Idx;
		ox = (int)IdxOld.x;
		oz = (int)IdxOld.z;
		Idx = m_ANodeArray[oz][ox].parent;
		x = (int)Idx.x;
		z = (int)Idx.z;
		m_ANodeArray[z][x].next = IdxOld;  // ��O�̃m�[�h�̓Y����ݒ肷��
	}
}

//-----------------------------------------------------------------------------
//
// �n�������m�[�h�̒��ň�ԃX�R�A�̏��Ȃ��m�[�h��T������
//
// ����
//   VECTOR3 Idx    : �T�����ʂ̃m�[�h�ʒu
//
// �߂�l
//   BOOL TRUE:�T���ł����Ƃ�   FALSE: �m�[�h����������Ƃ�
//
//-----------------------------------------------------------------------------
BOOL CSearchRoute::SearchOpenNodeScore(VECTOR3& Idx)
{
	if (m_OpenNode.size() <= 0)  return FALSE;

	float scoreMin = 999999;
	float costMin  = 999999;

	// Open�m�[�h�z���T��
	for (int i = 0; i < m_OpenNode.size(); i++)
	{
		int x = (int)m_OpenNode[i].x;
		int z = (int)m_OpenNode[i].z;

		// �X�R�A�����Ȃ����A����X�R�A�̂Ƃ��͎��R�X�g�����Ȃ��Ƃ�
		if (m_ANodeArray[z][x].score < scoreMin ||
			(m_ANodeArray[z][x].score == scoreMin && m_ANodeArray[z][x].cost < costMin ))  // �Œ�X�R�A�E�R�X�g���H
		{
			scoreMin = m_ANodeArray[z][x].score;
			costMin  = m_ANodeArray[z][x].cost;
			Idx = VECTOR3(x, 0, z);
		}
	}

	return TRUE;
}
//-----------------------------------------------------------------------------
//
// �m�[�h���n�������ł��邩�ǂ����`�F�b�N����
//
// ����
//   int x : �Y����
//   int z : �Y����
//
// �߂�l
//   BOOL TRUE:Open�ł���Ƃ�   FALSE:Open�ł��Ȃ��Ƃ�
//
//-----------------------------------------------------------------------------
BOOL CSearchRoute::CheckOpenNode(int x, int z)
{
	// Open�ł��邩�ǂ����̃`�F�b�N
	if (z < 0 || z >= m_ANodeArray.size())    return FALSE;
	if (x < 0 || x >= m_ANodeArray[z].size()) return FALSE;
	if (m_ANodeArray[z][x].status != eNone) return FALSE;
	if (m_pGMain->m_pMapProc->GetRouteColMap()->m_Collision[z][x] != 0) return FALSE;
	return TRUE;
}
//-----------------------------------------------------------------------------
//
// ����R�X�g�̌v�Z
//
//   �΂߈ړ��̂Ƃ��̐���R�X�g�̌v�Z
//
// ����
//   int x : �Y����
//   int z : �Y����
//
// �߂�l
//   float ����R�X�g
//
//-----------------------------------------------------------------------------
float CSearchRoute::Calcheuristic(int x, int z)
{
	// �c���̋��������߂�
	int dx = abs((int)m_EndIdx.x - x);
	int dy = abs((int)m_EndIdx.z - z);

	float heuristic;

	heuristic = dx + dy;   // �@ ���p����(�}���n�b�^������)�𐄒�R�X�g�Ƃ���

	//heuristic = magnitude(VECTOR2(dx, dy));  // �A ��������(���[�N���b�h����)�𐄒�R�X�g�Ƃ���

	//heuristic = (dx > dy) ? dx : dy;    // �B �c�E���̑������𐄒�R�X�g�Ƃ���

	return heuristic;
}

//-----------------------------------------------------------------------------
//
// ���������m�[�h��T�����A�w�肵���ʒu�Y������o�H�̎��̈ʒu�Y����Ԃ�
//
// ����
//   VECTOR3 NowIdx  : ���̈ʒu�Y��
//   VECTOR3 NextIdx : ���̈ړ��ʒu�Y��(Out)
//
// �߂�l
//   BOOL TRUE:��������   FALSE:�����Ȃ��i�I���j
//
//-----------------------------------------------------------------------------
BOOL CSearchRoute::GetNextRoute(VECTOR3 NowIdx, VECTOR3& NextIdx)
{
	int x, z;

	// �o�H�T���z��Y��
	x = (int)NowIdx.x;
	z = (int)NowIdx.z;

	if (m_ANodeArray[z][x].next.y == 999999)  // ���̈ʒu���Ȃ��Ƃ�
	{
		NextIdx = NowIdx;  // ���̈ʒu��Ԃ�
		return FALSE;
	}
	NextIdx = m_ANodeArray[z][x].next;  // ���̈ʒu��Ԃ�

	return TRUE;
}

//-----------------------------------------------------------------------------
//
// �ʒu�Y�������[���h�ʒu�ɕϊ�����
//
// ����
//   VECTOR3 Idx : �ʒu�Y��
//
// �߂�l
//   VECTOR3 �ʒu
//
//-----------------------------------------------------------------------------
VECTOR3  CSearchRoute::ConvIdxToPos(VECTOR3 Idx)
{
	VECTOR3 vPos = VECTOR3(0, 0, 0);

	// �ʒu�Y������ʒu�����߂�
	vPos.x = Idx.x * m_Partssize + m_Basepoint.x - m_Partssize / 2;
	vPos.z = Idx.z * m_Partssize + m_Basepoint.z + m_Partssize / 2;

	return vPos;
}
//-----------------------------------------------------------------------------
//
// ���[���h�ʒu���ʒu�Y���ɕϊ�����
//
// ����
//   VECTOR3 vPos : �ʒu
//
// �߂�l
//   VECTOR3  �ʒu�Y��
//
//-----------------------------------------------------------------------------
VECTOR3  CSearchRoute::ConvPosToIdx(VECTOR3 vPos)
{
	VECTOR3 Idx = VECTOR3(0, 0, 0);

	// ���[���h�ʒu���ʒu�Y���ɕϊ�����
	Idx.x = (int)((vPos.x - m_Basepoint.x) / m_Partssize);
	Idx.z = (int)((vPos.z - m_Basepoint.z) / m_Partssize);

	return Idx;
}

// ---------------------------------------------------------------------------- -
//
// �o�H�e�[�u���Y�����ʒu�Y���ɕϊ�����
//
// ����
//   int MapId : �o�H�e�[�u���Y��
//
// �߂�l
//   VECTOR3  �ʒu�Y��
//
//-----------------------------------------------------------------------------
VECTOR3 CSearchRoute::ConvMapIdToArrId(int MapId)
{
	int xNum = (int)(m_pGMain->m_pMapProc->GetRouteColMap()->m_MapNumXZ.x);

	//MapId -= 1;
	VECTOR3 ret = VECTOR3(0, 0, 0);
	ret.x = MapId % xNum;
	ret.z = MapId / xNum;

	return ret;
}

// ---------------------------------------------------------------------------- -
//
// �ʒu�Y�����o�H�e�[�u���Y���ɕϊ�����
//
// ����
//   VECTOR3 : �ʒu�Y��
//
// �߂�l
//   int�@�o�H�e�[�u���Y��
//
//-----------------------------------------------------------------------------
int CSearchRoute::ConvArrIdToMapId(VECTOR3 ArrId)
{
	int xNum = (int)(m_pGMain->m_pMapProc->GetRouteColMap()->m_MapNumXZ.x);
	int ret = 1;

	ret += ArrId.z * xNum;
	ret += ArrId.x;

	return ret;
}

//-----------------------------------------------------------------------------
// �o�H�e�[�u���̍쐬
// CheckPoint1
//-----------------------------------------------------------------------------
BOOL CSearchRoute::MakeRouteTable()
{
	int xNum = (int)(m_pGMain->m_pMapProc->GetRouteColMap()->m_MapNumXZ.x);
	int zNum = (int)(m_pGMain->m_pMapProc->GetRouteColMap()->m_MapNumXZ.y);
	int num = xNum * zNum;
	std::vector<std::vector<int>> Map(num, std::vector<int>(num, 0));
	CSearchRoute* pSearch = new CSearchRoute(m_pGMain);
	pSearch->MakeANode();

	// ���ݒn�_
	for (int i = 0; i < num; i++) {
		// i���ʒu�Y��iId�ɕϊ�
		VECTOR3 iId = ConvMapIdToArrId(i);
		// iId�����[���h���WiPos�ɕϊ�
		VECTOR3 iPos = ConvIdxToPos(iId);

		// �ړI�n
		for (int j = 0; j < num; j++) {
			// i��j�������Ȃ�Map[j][i]��-1�������break
			if (i == j) {
				Map[j][i] = -1;
			}
			else {
				// j���ʒu�Y��jId�ɕϊ�
				VECTOR3 jId = ConvMapIdToArrId(j);
				// jId�����[���h���WjPos�ɕϊ�
				VECTOR3 jPos = ConvIdxToPos(jId);

				// Start:iPos�AEnd:jPos�Ōo�H�T��
				pSearch->MakeANodeRoute(iPos, jPos);

				VECTOR3 NextId = VECTOR3(0, 0, 0);
				// iId�̎���NextId�ɓ����
				pSearch->GetNextRoute(iId, NextId);
				// NextId���e�[�u���Y��n�ɕϊ�
				int n = ConvArrIdToMapId(NextId);
				// Map[j][i]��n������
				Map[j][i] = n;
			}
		}
	}

	// �t�@�C���ɏ����o��
	FILE* fp = fopen("RouteTable.csv", "w");
	for (int i = 0; i < num; i++) {
		for (int j = 0; j < num; j++) {
			fprintf(fp, "%d,", Map[i][j]);
		}
		fprintf(fp, "\n");
	}

	fclose(fp);

	return TRUE;
}

// ---------------------------------------------------------------------------- -
// ���̃O���b�h�̃��[���h���W��Ԃ�
// CheckPoint2
//-----------------------------------------------------------------------------
VECTOR3 CSearchRoute::GetNextPos(VECTOR3 currentPos, VECTOR3 goalPos, BOOL* isAreaOut)
{
	if (currentPos.x == -25.0f) {
		currentPos.x += 1.0f;
	}
	if (currentPos.x == 25.0f) {
		currentPos.x -= 1.0f;
	}

	if (currentPos.z == -25.0f) {
		currentPos.z += 1.0f;
	}
	if (currentPos.z == 25.0f) {
		currentPos.z -= 1.0f;
	}

	VECTOR3 currentId = ConvPosToIdx(currentPos);
	VECTOR3 goalId = ConvPosToIdx(goalPos);

	// �o�H�T���͈͊O�ɖړI�n���ݒ肳��Ă���ꍇ
	if (goalId.x < 0 || m_pGMain->m_pMapProc->GetRouteColMap()->m_MapNumXZ.x < goalId.x)
	{
		*isAreaOut = TRUE;
		return currentPos;
	}
	if (goalId.z < 0 || m_pGMain->m_pMapProc->GetRouteColMap()->m_MapNumXZ.y < goalId.z)
	{
		*isAreaOut = TRUE;
		return currentPos;
	}
	int c = ConvArrIdToMapId(currentId);
	int g = ConvArrIdToMapId(goalId);

	int r = m_pGMain->m_pMapProc->GetRouteColMap()->m_RouteTable[g - 1][c - 1];

	VECTOR3 retId;
	if (r == -1) {
		retId = currentId;
	}
	else {
		retId = ConvMapIdToArrId(r);
	}
	VECTOR3 retPos = ConvIdxToPos(retId);

	return retPos;
}