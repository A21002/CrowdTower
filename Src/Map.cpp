//=============================================================================
//		  �}�b�v�̏���
//																	Map.cpp
//=============================================================================
#include  "BaseProc.h"
#include  "Map.h"
#include  "Playchar.h"
#include  "Enemy.h"
#include  "EnemyBoss.h"
#include  "Ally.h"
#include  "Weapon.h"
#include  "Camera.h"
#include  "BackFore.h"


//-----------------------------------------------------------------------------
// �}�b�v�v���V�[�W���̃R���X�g���N�^
//-----------------------------------------------------------------------------
CMapProc::CMapProc(CGameMain*	pGMain)
{
	// �N���X�S�̂̃N�����[
	// (vector�N���X�͏������s�v)
	m_pGMain = pGMain;
	m_dwMapNo = 0;
	m_bActive = FALSE;
	m_bIsMoveMap = FALSE;
	m_bIsTutorial = FALSE;
	m_pColMesh   = NULL;					// �R���W�������b�V���ւ̃|�C���^
	m_pPcColMesh = NULL;
	m_pEnmSpPart = NULL;
	m_pPcSpPart = NULL;
	m_nEvtIdx = 0;	
	m_MapIdArray[0] = { 0 };
	m_nHierarchy = 0;
}

//-----------------------------------------------------------------------------
// �}�b�v�v���V�[�W���̃f�X�g���N�^
//-----------------------------------------------------------------------------
CMapProc::~CMapProc()
{
	DestroyAll();
}

//-----------------------------------------------------------------------------
// �}�b�v�̍폜
//-----------------------------------------------------------------------------
void CMapProc::DestroyAll()
{
	m_dwMapNo = 0;
	m_bActive = FALSE;
	m_nEvtIdx = 0;			

	// ���b�V���̃f���[�g
	for(DWORD i = 0; i<m_SkyMap.size(); i++)
	{
		SAFE_DELETE(m_SkyMap[i].m_pMesh);
		SAFE_DELETE(m_SkyMap[i].m_pWave);    // �g�p���Ă��Ȃ����O�̂��ߍ폜
	}
	for(DWORD i = 0; i<m_StageMap.size(); i++)
	{
		SAFE_DELETE(m_StageMap[i].m_pMesh);
		SAFE_DELETE(m_StageMap[i].m_pWave);
	}
	for (DWORD i = 0; i<m_MoveMap.size(); i++)
	{
		SAFE_DELETE(m_MoveMap[i].m_pMesh);
		SAFE_DELETE(m_MoveMap[i].m_pColMesh);
	}
	for (DWORD i = 0; i < m_EventMap.size(); i++)  
	{
		SAFE_DELETE(m_EventMap[i].m_pBBox);
	}
	SAFE_DELETE(m_pColMesh);
	SAFE_DELETE(m_pPcColMesh);

	SAFE_DELETE(m_pPcSpPart);
	SAFE_DELETE(m_pEnmSpPart);

	// vector�̍폜
	m_SkyMap.clear();					// ��̃}�b�v  �z��폜
	m_SkyMap.shrink_to_fit();			// ��̃}�b�v  �s�v���������
	m_StageMap.clear();					// �X�e�[�W�̃}�b�v  �z��폜
	m_StageMap.shrink_to_fit();			// �X�e�[�W�̃}�b�v �s�v���������
	m_MoveMap.clear();					// �ړ��}�b�v  �z��폜
	m_MoveMap.shrink_to_fit();			// �ړ��}�b�v �s�v���������
	m_NavMap.clear();					// �i�r�Q�[�V�����}�b�v�z��폜
	m_NavMap.shrink_to_fit();			// �i�r�Q�[�V�����}�b�v�s�v���������
	m_EventMap.clear();					// �C�x���g�}�b�v�z��폜
	m_EventMap.shrink_to_fit();			// �C�x���g�}�b�v�s�v���������
	m_MapKind.clear();					// �C�x���g�}�b�v�z��폜
	m_MapKind.shrink_to_fit();			// �C�x���g�}�b�v�s�v���������
	m_RouteColMap.m_Collision.clear();			// ���[�g�T���R���W�����z��폜
	m_RouteColMap.m_Collision.shrink_to_fit();	// ���[�g�T���R���W�����z��s�v���������
	m_RouteColMap.m_RouteTable.clear();			// ���[�g�T���R���W�����z��폜
	m_RouteColMap.m_RouteTable.shrink_to_fit();	// ���[�g�T���R���W�����z��s�v���������
}

//-----------------------------------------------------------------------------
// �}�b�v�̍쐬�ƈړ��̋��ʏ���
//-----------------------------------------------------------------------------
void CMapProc::LoadMap(const char* fileName)
{
	// vector�������p�̍\����
	const STAGEMAP      sm;
	const MOVEMAP       mm;
	const NAVIGATIONMAP nv;

	// �S�}�b�v�̍폜 -----------------------------------------------
	DestroyAll();

	// �S�Ă̓G�E����E���ʂ̔�\���� 
	m_pGMain->m_pEnmProc->SetNonActive();
	m_pGMain->m_pEnmProc->ResetStartFlag();
	m_pGMain->m_pEnmBossProc->SetNonActive();
	m_pGMain->m_pAllyProc->SetNonActive();
	m_pGMain->m_pWeaponProc->SetNonActive();
	m_pGMain->m_pEffectProc->SetNonActive();
	m_pGMain->m_pCameraProc->GetCameraObjPtr()->ResetMovieFlag();
	m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetLookStairsFlag(FALSE);

	// �o�b��(0,0,-20)�̈ʒu�ɒu�� -------------------------------------
	m_pGMain->m_pPcProc->GetPcObjPtr()->SetWorld(XMMatrixTranslation(0.0f, 0.05f, -20.0f));
	m_pGMain->m_pPcProc->GetPcObjPtr()->SetWorldOld(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());
	m_pGMain->m_pPcProc->GetPcObjPtr()->SetLocalRotY(0.0f);  // ���[�J����(Y��)��0�x�ɂ���
	m_pGMain->m_pPcProc->GetPcObjPtr()->ResetStatus();
	m_pGMain->m_pPcProc->GetPcObjPtr()->SetAllyObj();

	// �X�e�[�W�}�b�v�̐ݒ�E�R���W�����}�b�v�̐ݒ� -----------------------------------------
	m_pColMesh = new CCollision(m_pGMain->m_pFbxMeshCtrl);     // �R���W�����}�b�v�̐���

		// �Εǂ̐ݒ�
	m_StageMap.push_back(sm);
	m_StageMap.back().m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Map/MapItem//Wall.mesh"));
	m_StageMap.back().m_mWorld = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	if (m_nHierarchy != 9)
	{
		// �h�A���b�V���̐ݒ�[1]
		m_StageMap.push_back(sm);
		m_StageMap.back().m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Map/MapItem/stairs/Staircase.mesh"));
		m_StageMap.back().m_mWorld = XMMatrixTranslation(0.0f, 0.0f, 26.0f);
		m_pColMesh->AddFbxLoad(_T("Data/Map/MapItem/stairs/Staircase.mesh"), VECTOR3(0.0f, 0.0f, 26.0f)); // �}�b�v���b�V���R���W�����̐ݒ�
		SetEvent(VECTOR3(0.0f, 0.0f, -4.35f), VECTOR3(4.5f, 2.8f, 4.35f), m_StageMap.back().m_mWorld, eEvtChangeMap, 1);  // �C�x���g�̐ݒ�(�}�b�v6�ֈړ�����)

		m_MoveMap.push_back(mm);
		m_MoveMap.back().m_bActive = TRUE;
		m_MoveMap.back().m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Map/MapItem/stairs/Jeil.mesh"));
		//m_MoveMap.back().m_pMesh = NULL;
		m_MoveMap.back().m_pColMesh = new CCollision(m_pGMain->m_pFbxMeshCtrl);
		m_MoveMap.back().m_pColMesh->AddFbxLoad(_T("Data/Map/MapItem/stairs/Jeil_Collision.mesh"), VECTOR3(0.0f, 0.0f, 0.0f)); // �}�b�v���b�V���R���W�����̐ݒ�
		m_MoveMap.back().m_pColMesh->SetWorldMatrix(XMMatrixTranslation(0.0f, 0.0f, 24.0f));						// �ړ��}�b�v�̈ʒu
		m_MoveMap.back().m_pColMesh->m_mWorldOld = m_MoveMap.back().m_pColMesh->m_mWorld;							// ���[���h�}�g���b�N�X��O
		m_MoveMap.back().m_nMoveFlag = -1;																			// �����͈ړ��Ȃ�
		m_MoveMap.back().m_vUp = VECTOR3(0, -0.05f, 0);																// �㉺�ړ�����
		m_MoveMap.back().m_vMin = VECTOR3(0, -6.5f, 0);																// �����l
		m_MoveMap.back().m_nChangeFlag = 0;																			// �`�F���W�t���O�̃��Z�b�g
		m_MoveMap.back().m_nLoop = 0;																				// ���[�v���Ȃ�
	}
	else {
		// �{�X�̃X�|�[��
		m_pGMain->m_pEnmBossProc->Start(VECTOR3(0.0f, 0.05f, 21.0f));
	}

	m_StageMap.push_back(sm);
	m_StageMap.back().m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Map/Sky/sky2.mesh"));
	m_StageMap.back().m_mWorld = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	
	// �X�N���v�g�ǂݎ��
	FILE* fp = fopen(fileName, "r");
	if (fp != NULL)
	{
		// �u-1�v��ǂݎ��܂Ŗ������[�v
		while (1)
		{
			int id = -1;								// �s�������̎��
			int areaID = 0;								// �_���[�W�G���A�̎��
			char address[256];							// UTF-8�`���ł̕�����
			TCHAR Taddress[256];						// TCHAR�`���ł̕�����
			VECTOR3 vPos = VECTOR3(0.0f, 0.0f, 0.0f);	// ���b�V���E�R���W�����̍��W
			VECTOR3 vMin = VECTOR3(0.0f, 0.0f, 0.0f);	// �{�b�N�X�̍����̍��W
			VECTOR3 vMax = VECTOR3(0.0f, 0.0f, 0.0f);	// �{�b�N�X�̉E��̍��W

			// �s�������̎�ނ�ǂݎ��
			fscanf(fp, "%d,", &id);
			switch (id) {
			case 0:
				// ���b�V���̕\��
				fscanf(fp, "%s", address);
				fscanf(fp, "%f, %f, %f", &vPos.x, &vPos.y, &vPos.z);
				MyImgui::ConvertU8ToU16(address, Taddress);				// UTF-8�`������TCHAR�`���ɕϊ�
				m_StageMap.push_back(sm);
				m_StageMap.back().m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, Taddress);
				m_StageMap.back().m_mWorld = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
				break;

			case 1:
				// �R���W�������b�V���̐ݒ�
				fscanf(fp, "%s", address);
				fscanf(fp, "%f, %f, %f", &vPos.x, &vPos.y, &vPos.z);
				MyImgui::ConvertU8ToU16(address, Taddress);				// UTF-8�`������TCHAR�`���ɕϊ�
				m_pColMesh->AddFbxLoad(Taddress, vPos);
				break;

			case 2:
				// �J������PC�̒�����ɂ���Ɠ��߂�����I�u�W�F�N�g�̃{�b�N�X
				fscanf(fp, "%f, %f, %f, %f, %f, %f", &vMin.x, &vMin.y, &vMin.z, &vMax.x, &vMax.y, &vMax.z);
				m_StageMap.back().m_pBox = new CBBox(m_pGMain->m_pShader, vMin, vMax);
				m_StageMap.back().m_pBox->m_mWorld = m_StageMap.back().m_mWorld;
				break;

			case 3:
				if (m_nHierarchy != 9)
				{
					// �G�̃X�|�[���͈�
					fscanf(fp, "%f, %f, %f, %f, %f, %f", &vMin.x, &vMin.y, &vMin.z, &vMax.x, &vMax.y, &vMax.z);
					m_NavMap.push_back(nv);
					m_NavMap.back().vMin = vMin;
					m_NavMap.back().vMax = vMax;
				}
				break;

			case 4:
				// �_���[�W�G���A�͈̔�
				fscanf(fp, "%f, %f, %f, %f, %f, %f, %d", &vMin.x, &vMin.y, &vMin.z, &vMax.x, &vMax.y, &vMax.z, &areaID);
				m_pGMain->m_pWeaponProc->m_pWeaponDamageAreaProc->Start(vMin, vMax, areaID);
				break;

			case -1:
				break;
			}

			if (id == -1)
			{
				break;
			}
		}
		fclose(fp);
	}

	m_pGMain->m_pEnmProc->ResetEnmNum();      // �G�̔������̃��Z�b�g

	// ��ԕ����̐���             
	VECTOR3 vMin, vMax;
	m_pColMesh->GetChkAABB(vMin, vMax);
	m_pPcSpPart = new CSpPart(m_pGMain);
	m_pPcSpPart->CleateSpPart(vMin, vMax, 3);

	m_pEnmSpPart = new CSpPart(m_pGMain);
	m_pEnmSpPart->CleateSpPart(vMin, vMax, 3);

	m_bActive = TRUE;
}

//-----------------------------------------------------------------------------
// �}�b�v�P�̍쐬�ƈړ��̏���
//-----------------------------------------------------------------------------
void CMapProc::LoadMap1()
{
	// vector�������p�̍\����
	const MOVEMAP       mm;

	LoadMap("Data/Map/Tower/Map1/Script.txt");

	// ��̐ݒu
	m_MoveMap.push_back(mm);
	m_MoveMap.back().m_bActive = TRUE;
	m_MoveMap.back().m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Map/Tower/Map1/LeftWall.mesh"));
	m_MoveMap.back().m_pColMesh = new CCollision(m_pGMain->m_pFbxMeshCtrl);
	m_MoveMap.back().m_pColMesh->AddFbxLoad(_T("Data/Map/Tower/Map1/Wall_Collision.mesh"), VECTOR3(0, 0, 0)); // �}�b�v���b�V���R���W�����̐ݒ�
	m_MoveMap.back().m_pColMesh->SetWorldMatrix(XMMatrixTranslation(-7.5f, 0.0f, -15.5f));						// �ړ��}�b�v�̈ʒu
	m_MoveMap.back().m_pColMesh->m_mWorldOld = m_MoveMap.back().m_pColMesh->m_mWorld;							// ���[���h�}�g���b�N�X��O
	m_MoveMap.back().m_nMoveFlag = 0;																			// �����͈ړ��Ȃ�
	m_MoveMap.back().m_vUp = VECTOR3(-0.05f, 0, 0);																// �㉺�ړ�����
	m_MoveMap.back().m_vMin = VECTOR3(-22.5f, 0, 0);																// �����l
	m_MoveMap.back().m_nChangeFlag = 0;																			// �`�F���W�t���O�̃��Z�b�g
	m_MoveMap.back().m_nLoop = 0;

	m_MoveMap.push_back(mm);
	m_MoveMap.back().m_bActive = TRUE;
	m_MoveMap.back().m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Map/Tower/Map1/RightWall.mesh"));
	m_MoveMap.back().m_pColMesh = new CCollision(m_pGMain->m_pFbxMeshCtrl);
	m_MoveMap.back().m_pColMesh->AddFbxLoad(_T("Data/Map/Tower/Map1/Wall_Collision.mesh"), VECTOR3(0, 0, 0)); // �}�b�v���b�V���R���W�����̐ݒ�
	m_MoveMap.back().m_pColMesh->SetWorldMatrix(XMMatrixTranslation(7.5f, 0.0f, -15.5f));						// �ړ��}�b�v�̈ʒu
	m_MoveMap.back().m_pColMesh->m_mWorldOld = m_MoveMap.back().m_pColMesh->m_mWorld;							// ���[���h�}�g���b�N�X��O
	m_MoveMap.back().m_nMoveFlag = 0;																			// �����͈ړ��Ȃ�
	m_MoveMap.back().m_vUp = VECTOR3(0.05f, 0, 0);																// �㉺�ړ�����
	m_MoveMap.back().m_vMax = VECTOR3(22.5f, 0, 0);																// �����l
	m_MoveMap.back().m_nChangeFlag = 0;																			// �`�F���W�t���O�̃��Z�b�g
	m_MoveMap.back().m_nLoop = 0;

	SetEvent(VECTOR3(-7.5f, 0.0f, -1.5f), VECTOR3(7.5f, 0.0f, 1.5f), XMMatrixTranslation(0.0f, 0.0f, -15.5f), eEvtMoveMap, 1, 2, 1);  // �C�x���g�̐ݒ�(�}�b�v6�ֈړ�����)

	// ���[�g�T���R���W�����}�b�v�̍쐬  --------------------------------
	MakeRouteColMap(VECTOR2(50, 40), 2.0f, VECTOR3(-25, 0, -15), "Data/Map/Tower/Map1/RouteTable.csv");

	const int MapNum = 10;
	for (int i = 1; i < MapNum; i++) {
		m_MapKind.push_back(i + 1);
	}
	
	// �}�b�v�̏��Ԃ������_���Őݒ�
	for (int i = 0; i < MapNum - 1; i++) {
		int tmp = Random(0, m_MapKind.size() - 1);
		m_MapIdArray[i] = m_MapKind[tmp];
		m_MapKind.erase(m_MapKind.begin() + tmp);
	}

	// �`���[�g���A���̕\��
	if (m_bIsTutorial) {
		m_pGMain->m_pBackForeProc->m_nDialogKind = DIALOG_TUTORIAL1;
		m_pGMain->m_isMove = FALSE;
	}	

}

//-----------------------------------------------------------------------------
// �}�b�v�Q�̍쐬�ƈړ��̏���
//-----------------------------------------------------------------------------
void CMapProc::LoadMap2()
{
	LoadMap("Data/Map/Tower/Map2/Script.txt");

	// ���[�g�T���R���W�����}�b�v�̍쐬  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map2/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// �}�b�v�R�̍쐬�ƈړ��̏���
//-----------------------------------------------------------------------------
void CMapProc::LoadMap3()
{
	LoadMap("Data/Map/Tower/Map3/Script.txt");

	// ���[�g�T���R���W�����}�b�v�̍쐬  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map3/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// �}�b�v�S�̍쐬�ƈړ��̏���
//-----------------------------------------------------------------------------
void CMapProc::LoadMap4()
{
	LoadMap("Data/Map/Tower/Map4/Script.txt");

	// ���[�g�T���R���W�����}�b�v�̍쐬  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map4/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// �}�b�v�T�̍쐬�ƈړ��̏���
//-----------------------------------------------------------------------------
void CMapProc::LoadMap5()
{
	LoadMap("Data/Map/Tower/Map5/Script.txt");

	// ���[�g�T���R���W�����}�b�v�̍쐬  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map5/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// �}�b�v�U�̍쐬�ƈړ��̏���
//-----------------------------------------------------------------------------
void CMapProc::LoadMap6()
{
	LoadMap("Data/Map/Tower/Map6/Script.txt");

	// ���[�g�T���R���W�����}�b�v�̍쐬  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map6/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// �}�b�v�V�̍쐬�ƈړ��̏���
//-----------------------------------------------------------------------------
void CMapProc::LoadMap7()
{
	LoadMap("Data/Map/Tower/Map7/Script.txt");

	// PC��p�R���W����
	m_pPcColMesh = new CCollision(m_pGMain->m_pFbxMeshCtrl);
	m_pPcColMesh->AddFbxLoad(_T("Data/Map/Tower/Map7/PC_Collision.mesh"), VECTOR3(0.0f, 0.0f, 0.0f));
	
	// ���[�g�T���R���W�����}�b�v�̍쐬  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map7/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// �}�b�v�W�̍쐬�ƈړ��̏���
//-----------------------------------------------------------------------------
void CMapProc::LoadMap8()
{
	LoadMap("Data/Map/Tower/Map8/Script.txt");

	// PC��p�R���W����
	m_pPcColMesh = new CCollision(m_pGMain->m_pFbxMeshCtrl);
	m_pPcColMesh->AddFbxLoad(_T("Data/Map/Tower/Map8/PC_Collision.mesh"), VECTOR3(0.0f, 0.0f, 0.0f));

	// ���[�g�T���R���W�����}�b�v�̍쐬  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map8/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// �}�b�v�X�̍쐬�ƈړ��̏���
//-----------------------------------------------------------------------------
void CMapProc::LoadMap9()
{
	// vector�������p�̍\����
	LoadMap("Data/Map/Tower/Map9/Script.txt");

	// ���[�g�T���R���W�����}�b�v�̍쐬  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map9/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// �}�b�v�P�O�̍쐬�ƈړ��̏���
//-----------------------------------------------------------------------------
void CMapProc::LoadMap10()
{
	LoadMap("Data/Map/Tower/Map10/Script.txt");

	// ���[�g�T���R���W�����}�b�v�̍쐬  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map10/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// �}�b�v�P�P�̍쐬�ƈړ��̏���
//-----------------------------------------------------------------------------
void CMapProc::LoadMap11()
{
	LoadMap("Data/Map/Tower/Map11/Script.txt");

	// PC��p�R���W����
	m_pPcColMesh = new CCollision(m_pGMain->m_pFbxMeshCtrl);
	m_pPcColMesh->AddFbxLoad(_T("Data/Map/Tower/Map11/PC_Collision.mesh"), VECTOR3(0.0f, 0.0f, 0.0f));

	// ���[�g�T���R���W�����}�b�v�̍쐬  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map11/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// �}�b�v�P�Q�̍쐬�ƈړ��̏���
//-----------------------------------------------------------------------------
void CMapProc::LoadMap12()
{
	LoadMap("Data/Map/Tower/Map12/Script.txt");

	// PC��p�R���W����
	m_pPcColMesh = new CCollision(m_pGMain->m_pFbxMeshCtrl);
	m_pPcColMesh->AddFbxLoad(_T("Data/Map/Tower/Map12/PC_Collision.mesh"), VECTOR3(0.0f, 0.0f, 0.0f));

	// ���[�g�T���R���W�����}�b�v�̍쐬  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map12/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// �}�b�v�P�R�̍쐬�ƈړ��̏���
//-----------------------------------------------------------------------------
void CMapProc::LoadMap13()
{
	LoadMap("Data/Map/Tower/Map13/Script.txt");

	// ���[�g�T���R���W�����}�b�v�̍쐬  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map13/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// �}�b�v�P�S�̍쐬�ƈړ��̏���
//-----------------------------------------------------------------------------
void CMapProc::LoadMap14()
{
	LoadMap("Data/Map/Tower/Map14/Script.txt");

	// ���[�g�T���R���W�����}�b�v�̍쐬  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map14/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// �}�b�v�P�T�̍쐬�ƈړ��̏���
//-----------------------------------------------------------------------------
void CMapProc::LoadMap15()
{
	LoadMap("Data/Map/Tower/Map15/Script.txt");

	// ���[�g�T���R���W�����}�b�v�̍쐬  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map15/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// �}�b�v�P�U�̍쐬�ƈړ��̏���
//-----------------------------------------------------------------------------
void CMapProc::LoadMap16()
{
	LoadMap("Data/Map/Tower/Map16/Script.txt");

	// ���[�g�T���R���W�����}�b�v�̍쐬  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map16/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// �}�b�v�P�V�̍쐬�ƈړ��̏���
//-----------------------------------------------------------------------------
void CMapProc::LoadMap17()
{
	LoadMap("Data/Map/Tower/Map17/Script.txt");

	// ���[�g�T���R���W�����}�b�v�̍쐬  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map17/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// �}�b�v�P�W�̍쐬�ƈړ��̏���
//-----------------------------------------------------------------------------
void CMapProc::LoadMap18()
{
	LoadMap("Data/Map/Tower/Map18/Script.txt");

	// ���[�g�T���R���W�����}�b�v�̍쐬  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map18/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// �}�b�v�P�X�̍쐬�ƈړ��̏���
//-----------------------------------------------------------------------------
void CMapProc::LoadMap19()
{
	LoadMap("Data/Map/Tower/Map19/Script.txt");

	// ���[�g�T���R���W�����}�b�v�̍쐬  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map19/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// �}�b�v�Q�O�̍쐬�ƈړ��̏���
//-----------------------------------------------------------------------------
void CMapProc::LoadMap20()
{
	LoadMap("Data/Map/Tower/Map20/Script.txt");

	// ���[�g�T���R���W�����}�b�v�̍쐬  --------------------------------
	MakeRouteColMap(VECTOR2(50, 50), 2.0f, VECTOR3(-25, 0, -25), "Data/Map/Tower/Map20/RouteTable.csv");
}

//-----------------------------------------------------------------------------
// �}�b�v�v���V�[�W���̂����蔻��
//
// �E����i��ɂo�b�̕���j�ƃR���W�����}�b�v�Ƃ̂����蔻��
//
//   CBaseObj*    pObj      ����̃I�u�W�F�N�g
//   VECTOR3* pHit		�ڐG�_�̍��W�i�o�́j
//   VECTOR3* pNormal	�ڐG�_�̖@���x�N�g���i�o�́j

//
//   �߂�l�@BOOL
//           TRUE:�q�b�g�����Ƃ�       FALSE�F�q�b�g���Ă��Ȃ��Ƃ�
//-----------------------------------------------------------------------------
BOOL   CMapProc::Hitcheck(CBaseObj* pObj, VECTOR3* pHit, VECTOR3* pNormal)
{
	return  Hitcheck(GetPositionVector(pObj->GetWorld()), GetPositionVector(pObj->GetWorldOld()), pHit, pNormal);
}

//-----------------------------------------------------------------------------
// �}�b�v�v���V�[�W���̂����蔻��
//
// �E����i��ɂo�b�̕���j�ƃR���W�����}�b�v�Ƃ̂����蔻��
//
//   VECTOR3 vNow       ����̌��݈ʒu
//   VECTOR3 vOld       ����̈�O�̈ʒu
//   VECTOR3* vHit		�ڐG�_�̍��W�i�o�́j
//   VECTOR3* vNormal	�ڐG�_�̖@���x�N�g���i�o�́j
//
//   �߂�l�@BOOL
//           TRUE:�q�b�g�����Ƃ�       FALSE�F�q�b�g���Ă��Ȃ��Ƃ�
//-----------------------------------------------------------------------------
BOOL   CMapProc::Hitcheck(VECTOR3 vNow, VECTOR3 vOld, VECTOR3* pHit, VECTOR3* pNormal)
{
	BOOL  bRet;
	for (int i = 0; i < m_MoveMap.size(); i++)
	{
		if (m_MoveMap[i].m_pColMesh && m_MoveMap[i].m_bActive)
		{
			bRet = m_MoveMap[i].m_pColMesh->isCollisionLay(vNow, vOld, *pHit, *pNormal);
			if (bRet) return bRet;
		}
	}

	if (m_pColMesh && m_pColMesh->isCollisionLay(vNow, vOld, *pHit, *pNormal))
	{
		return TRUE;
	}
	else {
		return FALSE;
	}
}

//----------------------------------------------------------------------------- 
// �}�b�v�v���V�[�W���̂����蔻��
//
// �E����i��ɂo�b�̕���j�̋��ƃR���W�����}�b�v�Ƃ̂����蔻��
//
//   VECTOR3 vNow       ����̌��݂̃I�u�W�F�N�g���S�ʒu
//   VECTOR3 vOld       ����̈�O�̃I�u�W�F�N�g���S�ʒu
//   float& fRadius     ���̔��a
//   VECTOR3 &vHit      �ڐG���̃I�u�W�F�N�g���S�ʒu�̍��W�i�o�́j
//   VECTOR3 &vNormal   �ڐG�_�̖@���x�N�g���i�o�́j
//
//   �߂�l�@BOOL
//           TRUE:�q�b�g�����Ƃ�       FALSE�F�q�b�g���Ă��Ȃ��Ƃ�
//-----------------------------------------------------------------------------
BOOL   CMapProc::Hitcheck(VECTOR3 vNow, VECTOR3 vOld, float fRadius, VECTOR3* pHit, VECTOR3* pNormal)
{
	BOOL  bRet;
	for (int i = 0; i < m_MoveMap.size(); i++)
	{
		if (m_MoveMap[i].m_pColMesh && m_MoveMap[i].m_bActive)
		{
			bRet = m_MoveMap[i].m_pColMesh->isCollisionSphere(vNow, vOld, fRadius, *pHit, *pNormal);
			if (bRet) return bRet;
		}
	}

	if (m_pColMesh && m_pColMesh->isCollisionSphere(vNow, vOld, fRadius, *pHit, *pNormal))
	{
		return TRUE;
	}
	else {
		return FALSE;
	}

}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̃��C�ƃ��b�V���ڐG����p�z��Ƃ̐ڐG����ƃX���X������������
//	���፷�Əd�͂��l�������A�ڐG������s��						
//  
//  MATRIX4X4* pWorld     �I�u�W�F�N�g�̌��݂̃}�g���b�N�X(in,out)
//  MATRIX4X4  mWorldOld  �I�u�W�F�N�g�̈�O�̃}�g���b�N�X		
//  float fRadius          �I�u�W�F�N�g�̔��a�i�ȗ��l��0.2�j
//  
//  �߂�l�@int
//		�G���[		= -1
//		�ʏ���ړ�	= 1
//		���n		= 2
//		������		= 3
//-----------------------------------------------------------------------------
int  CMapProc::isCollisionMoveGravity(MATRIX4X4* pWorld, MATRIX4X4 mWorldOld, float fRadius, DWORD dwTag)
{
	int  nRet = 0, nRetMove = 0; 

	if (dwTag != ENM) {
		// �ړ��}�b�v�Ƃ̐ڐG����ƈړ�
		for (int i = 0; i < m_MoveMap.size(); i++)
		{
			if (m_MoveMap[i].m_pColMesh && m_MoveMap[i].m_bActive)
			{
				nRetMove = m_MoveMap[i].m_pColMesh->isCollisionMoveGravity(*pWorld, mWorldOld, fRadius);
				if (nRetMove == 1 || nRetMove == 2) break;   // �ړ��}�b�v�ƐڐG�����Ƃ�
			}
		}
	}

	// �ʏ�̃}�b�v�Ƃ̐ڐG����ƈړ�
	if (m_pColMesh)
	{
		nRet = m_pColMesh->isCollisionMoveGravity(*pWorld, mWorldOld, fRadius);  
	}

	if (nRetMove == 1 || nRetMove == 2)  // �ړ��}�b�v�ƐڐG���Ă����Ƃ�  
	{
		return nRetMove;
	}
	else {
		return nRet;
	}
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̃��C�ƃ��b�V���ڐG����p�z��Ƃ̐ڐG����ƃX���X��������PC��p�̐���
//	���፷�Əd�͂��l�������A�ڐG������s��
//  
//  MATRIX4X4* pWorld     �I�u�W�F�N�g�̌��݂̃}�g���b�N�X(in,out)
//  MATRIX4X4  mWorldOld  �I�u�W�F�N�g�̈�O�̃}�g���b�N�X		
//  float fRadius          �I�u�W�F�N�g�̔��a�i�ȗ��l��0.2�j
//  
//  �߂�l�@int
//		�G���[		= -1
//		�ʏ���ړ�	= 1
//		���n		= 2
//		������		= 3
//-----------------------------------------------------------------------------
int CMapProc::isCollisionMoveGravityPc(MATRIX4X4* pWorld, MATRIX4X4 mWorldOld, float fRadius)
{
	int nRet = 0, nRetMove = 0;
	if (m_pPcColMesh) {
		// �ړ��}�b�v�Ƃ̐ڐG����ƈړ�
			for (int i = 0; i < m_MoveMap.size(); i++)
			{
				if (m_MoveMap[i].m_pColMesh && m_MoveMap[i].m_bActive)
				{
					nRetMove = m_MoveMap[i].m_pColMesh->isCollisionMoveGravity(*pWorld, mWorldOld, fRadius);
					if (nRetMove == 1 || nRetMove == 2) break;   // �ړ��}�b�v�ƐڐG�����Ƃ�
				}
			}

		// �ʏ�̃}�b�v�Ƃ̐ڐG����ƈړ�
		if (m_pPcColMesh)
		{
			nRet = m_pPcColMesh->isCollisionMoveGravity(*pWorld, mWorldOld, fRadius);
		}
	}
	else {
		// �ݒ肳��Ă��Ȃ��ꍇ
		nRet = isCollisionMoveGravity(pWorld, mWorldOld, fRadius);
	}

	if (nRetMove == 1 || nRetMove == 2)  // �ړ��}�b�v�ƐڐG���Ă����Ƃ�
	{
		return nRetMove;
	}
	else {
		return nRet;
	}
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̃��C�ƃ��b�V���ڐG����p�z��Ƃ̐ڐG����ƃX���X������������
//	�d�͂��l�����A�ڐG������s��									
//  
//  MATRIX4X4* pWorld     �I�u�W�F�N�g�̌��݂̃}�g���b�N�X(in,out)
//  MATRIX4X4  mWorldOld  �I�u�W�F�N�g�̈�O�̃}�g���b�N�X		
//  float fRadius          �I�u�W�F�N�g�̔��a�i�ȗ��l��0.2�j
//  
//  �߂�l�@int
//		�ڐG�����Ƃ��@		�P
//		�ڐG���Ă��Ȃ��Ƃ�	�O
//-----------------------------------------------------------------------------
int CMapProc::isCollisionMove(MATRIX4X4* pWorld, MATRIX4X4 mWorldOld, float fRadius)
{
	int  nRet = 0, nRetMove = 0;  // -- 2019.9.3

	// �ړ��}�b�v�Ƃ̐ڐG����ƈړ�
	for (int i = 0; i < m_MoveMap.size(); i++)
	{
		if (m_MoveMap[i].m_pColMesh && m_MoveMap[i].m_bActive)
		{
			nRetMove = m_MoveMap[i].m_pColMesh->isCollisionMove(*pWorld, mWorldOld, fRadius);
			if (nRetMove == 1) break;
		}
	}

	// �ʏ�̃}�b�v�Ƃ̐ڐG����ƈړ�
	if (m_pColMesh)
	{
		nRet = m_pColMesh->isCollisionMove(*pWorld, mWorldOld, fRadius); 
	}

	if (nRetMove == 1)  // �ړ��}�b�v�ƐڐG���Ă����Ƃ�     // -- 2019.9.3
	{
		return nRetMove;
	}
	else {
		return nRet;
	}
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̃��C�ƃ��b�V���ڐG����p�z��Ƃ̐ڐG����ƃX���X������������
//	�d�͂��l�����A�ڐG������s��									
//  
//  MATRIX4X4* pWorld     �I�u�W�F�N�g�̌��݂̃}�g���b�N�X(in,out)
//  MATRIX4X4  mWorldOld  �I�u�W�F�N�g�̈�O�̃}�g���b�N�X		
//  float fRadius          �I�u�W�F�N�g�̔��a�i�ȗ��l��0.2�j
//  
//  �߂�l�@int
//		�ڐG�����Ƃ��@		�P
//		�ڐG���Ă��Ȃ��Ƃ�	�O
//-----------------------------------------------------------------------------
int CMapProc::isCollisionMoveWall(MATRIX4X4 mWorld, MATRIX4X4 mWorldOld, float fRadius)
{
	int ret = 0;
	ret = m_pColMesh->CheckWallMove(mWorld, mWorldOld, fRadius);
	if (ret == 1) {
		m_pColMesh->CheckWallMove(mWorld, mWorldOld, fRadius);
	}

	return ret;
}

//-----------------------------------------------------------------------------
// �o�H�T���p�̃R���W�����z��̐���
//
// ����
//   VECTOR2 vMapSizeXZ : �o�H�T���z��̕��ʏ�̑傫���i���j
//   float   partssize  : �z��v�f�i�m�[�h�j��̑傫���i���j
//   VECTOR3 vBasepoint : �o�H�T���z�񍶎�O�̊�_�i���j
//-----------------------------------------------------------------------------
void CMapProc::MakeRouteColMap(VECTOR2 vMapSizeXZ, float partssize, VECTOR3 vBasepoint, const char* fileName)
{
	
	const int LimitHeight = 0.7f;    // �ǂƌ��Ȃ�����

	m_RouteColMap.m_Partssize = partssize;		// �m�[�h��̑傫��
	m_RouteColMap.m_Basepoint = vBasepoint;		// �z��̍���O�̍��W�l

	int znum = (int)(vMapSizeXZ.y / partssize);   // 1�����ڗv�f��
	int xnum = (int)(vMapSizeXZ.x / partssize);   // 2�����ڗv�f��
	m_RouteColMap.m_MapNumXZ = VECTOR2((float)xnum, (float)znum);   // �m�[�h�z��̑傫��(�v�f��)

	m_RouteColMap.m_Collision.clear();			// ���[�g�T���R���W�����z��폜
	m_RouteColMap.m_Collision.shrink_to_fit();	// ���[�g�T���R���W�����z��s�v���������
	m_RouteColMap.m_RouteTable.clear();
	m_RouteColMap.m_RouteTable.shrink_to_fit();

	//	�o�H�R���W�����z��̃T�C�Y�̏�����
	m_RouteColMap.m_Collision.resize(znum, std::vector<int>(xnum));
	m_RouteColMap.m_RouteTable.resize(znum * xnum, std::vector<int>(znum * xnum));
		
	FILE* fp = fopen(fileName, "r");
	
	for (int i = 0; i < xnum * znum; i++)
	{
		for (int j = 0; j < xnum * znum; j++)
		{
			fscanf(fp, "%d,", &m_RouteColMap.m_RouteTable[i][j]);
		}
	}
	fclose(fp);
}

//-----------------------------------------------------------------------------
// �o�H�T���z��̕��ʏ�̑傫���i���j�ƍ���O�̊�_�i���j�����߂�	
//
// (�R���W�����}�b�vAABB�̍ŏ��lvMin,�ő�lvMax����Z�o����)
//
// ����
//   VECTOR2 vMapSizeXZ : �o�H�T���z��̕��ʏ�̑傫���i���j(Out)
//   VECTOR3 vBasepoint : �o�H�T���z�񍶎�O�̊�_�i���j(Out)
//-----------------------------------------------------------------------------
void CMapProc::GetMapsizeAndBasepoint(VECTOR2& vMapSizeXZ, VECTOR3& vBasepoint)
{

	VECTOR3 vMin;
	VECTOR3 vMax;

	// �R���W�����}�b�v�̑傫���𓾂�
	// (�傫���́A�}�b�vAABB�̍ŏ��l�ƍő�l�œ�����)
	m_pColMesh->GetChkAABB(vMin, vMax);

	vMapSizeXZ = VECTOR2(vMax.x - vMin.x, vMax.z - vMin.z);
	vBasepoint.x = vMin.x;
	vBasepoint.y = 0;
	vBasepoint.z = vMin.z;
}

//-----------------------------------------------------------------------------
// �}�b�v�v���V�[�W���̍X�V
//-----------------------------------------------------------------------------
void CMapProc::Update()
{
	CDirectInput* pDI = m_pGMain->m_pDI;

	// �C�x���g�̍X�V����                   
	UpdateEvent();

	// ��̍X�V�@�@�J�����𒆐S�Ƃ����ʒu�ɒu���A�x����]������
	if (m_SkyMap.size() > 0)
	{
		MATRIX4X4 mRot;
		mRot = XMMatrixRotationY(XMConvertToRadians(0.005f));
		mRot = mRot * GetRotateMatrix(m_SkyMap[0].m_mWorld);
		m_SkyMap[0].m_mWorld = XMMatrixTranslation( m_pGMain->m_vEyePt.x, m_pGMain->m_vEyePt.y, m_pGMain->m_vEyePt.z);
		m_SkyMap[0].m_mWorld = mRot * m_SkyMap[0].m_mWorld;
	}

	// �ړ��}�b�v�X�V����
	UpdateMoveMap();

	// �}�b�v�ړ�
	if (m_bIsMoveMap) {
		// �K�i�ɐG��Ă���
		if (m_nHierarchy == 10) {
			// �N���A��ʂւ̑J��
			if (m_pGMain->m_pBackForeProc->GetWhiteOut()) {
				m_pGMain->m_dwGameStatus = GAMECLEAR;
				m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetCtrl(CAMERA_GAMECLEAR);
			}
		}
		else
		if (m_pGMain->m_pBackForeProc->GetFadeInFinish())
		{
			// �t�F�[�h�A�E�g���I����Ă���
			MoveMap();
			m_bIsMoveMap = FALSE;
			m_pGMain->m_pBackForeProc->SetFade(FALSE);
		}
	}

	//�����_�����O
	Render();

	// ���߂����Z�b�g
	ResetStageMeshAlfa();
}

//----------------------------------------------------------------------------- 
// �C�x���g�̃Z�b�g
//
// ����
//   VECTOR3 vMin         : �o�E���f�B���O�{�b�N�X�ŏ��l
//   VECTOR3 vMax         : �o�E���f�B���O�{�b�N�X�ő�l
//   MATRIX4X4 mWorld     : �o�E���f�B���O�{�b�N�X�̃��[���h�}�g���b�N�X
//   EVENTKIND nEvtKind   : �C�x���g�̎�� eEvtChangeMap:�}�b�v�ړ�, eEvtMoveMap:�ړ��}�b�v�̐���
//   int nEvtNo           : �C�x���g�ԍ� eEvtChangeMap�̂Ƃ��͈ړ���̃}�b�v�ԍ��AeEvtMoveMap�̂Ƃ��͈ړ��}�b�v�̗v�f�ԍ�
//   int nEvtOpe1         : eEvtMoveMap�̂݁B1:�ړ��}�b�v�̈ړ�Active�̎w��B2:�ړ��}�b�v�̈ړ��t���O�̎w��(�ȗ��l0)
//   int nEvtOpe2         : eEvtMoveMap�̂݁B�ړ�Active�̂Ƃ���0:��\�� 1:�\��  �ړ��t���O�̂Ƃ��́@0:�ړ������@1:���s�ړ��@2:��]�@3:�g��k��(�ȗ��l0)
//   int nEvtKeyPush      : 0:�o�E���f�B���O�{�b�N�X�ɐڐG����ƃC�x���g�����A 1:�o�E���f�B���O�{�b�N�X�ɐڐG���āAENTER�L�[�������ƃC�x���g����(�ȗ��l0)
//
// �߂�l
//   int �Z�b�g�����C�x���g�z��̗v�f�ԍ�
//-----------------------------------------------------------------------------
int  CMapProc::SetEvent(VECTOR3 vMin, VECTOR3 vMax, MATRIX4X4 mWorld, EVENTKIND nEvtKind, int nEvtNo, int nEvtOpe1, int nEvtOpe2, int nEvtKeyPush)
{
	const EVENTMAP      em;

	m_EventMap.push_back(em);

	// �o�E���f�B���O�{�b�N�X�̐ݒ�
	m_EventMap[m_nEvtIdx].m_pBBox = new CBBox(m_pGMain->m_pShader, vMin, vMax);
	m_EventMap[m_nEvtIdx].m_pBBox->m_mWorld = mWorld;

	// �e���ڂ̐ݒ�
	m_EventMap[m_nEvtIdx].m_nEvtCycle = 0;
	m_EventMap[m_nEvtIdx].m_nEvtKind = nEvtKind;
	m_EventMap[m_nEvtIdx].m_nEvtNo = nEvtNo;
	m_EventMap[m_nEvtIdx].m_nEvtOpe1 = nEvtOpe1;
	m_EventMap[m_nEvtIdx].m_nEvtOpe2 = nEvtOpe2;
	m_EventMap[m_nEvtIdx].m_nEvtKeyPush = nEvtKeyPush;

	m_nEvtIdx++;
	return  m_nEvtIdx - 1;
}

//-----------------------------------------------------------------------------  
// �C�x���g�̍X�V�E���s
//-----------------------------------------------------------------------------
void  CMapProc::UpdateEvent()
{
	VECTOR3 vHit, vNrm;

	for (int i = 0; i < m_EventMap.size(); i++)
	{
		// PC�̃o�E���f�B���O�{�b�N�X�ɐڐG���Ă��邩�̔���
		if (m_EventMap[i].m_pBBox && m_EventMap[i].m_pBBox->OBBCollisionDetection(m_pGMain->m_pPcProc->GetPcObjPtr()->GetBBox(), &vHit, &vNrm))
		{
			// PC�̃o�E���f�B���O�{�b�N�X�ɐڐG���Ă���Ƃ�
			if (m_EventMap[i].m_nEvtCycle == 0)  // �C�x���g�T�C�N�����O�̂Ƃ�(�܂��ڐG���Ă��Ȃ������Ƃ�)
			{
				if (m_EventMap[i].m_nEvtKeyPush == 1 && !(m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_RETURN) || m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A)))  // Enter�L�[�v�b�V�����K�v�ȂƂ�
				{
					VECTOR3 vPc = GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());
					vPc.y += 2.0f;
					// PC�̓���ɁAEnter�L�[�v�b�V�����s���Ă��Ȃ����̕\��������
					m_pGMain->m_pFont->Draw3D(vPc, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vEyePt, _T("Push Enter Key!"), VECTOR2(0.8f, 0.2f), RGB(255, 0, 0), 1.0f, _T("HGP�n�p�p�޼��UB"));
				}
				else {
					// �C�x���g���s
					RunEvent(m_EventMap[i]);
				}
			}
		}
		else {
			// PC�̃o�E���f�B���O�{�b�N�X�ɐڐG���Ă��Ȃ��Ƃ�
			if (m_EventMap[i].m_nEvtCycle == 2)  // �C�x���g�T�C�N�����Q�̂Ƃ�(���ɐڐG���Ă��ďI���҂��̂Ƃ�)
			{
				m_EventMap[i].m_nEvtCycle = 0;  // �C�x���g�T�C�N�����O�ɖ߂�
			}
		}
	}
}

//----------------------------------------------------------------------------- 
// �C�x���g�̎��s
//
// ����  EVENTMAP& EventMap:�C�x���g�}�b�v
//-----------------------------------------------------------------------------
void  CMapProc::RunEvent(EVENTMAP& EventMap)
{
	int i;
	if (EventMap.m_nEvtKind == eEvtChangeMap)   // �}�b�v�ړ��̂Ƃ�
	{
		if (!m_bIsMoveMap) {
			if (m_nHierarchy == 10) {
				m_pGMain->m_pBackForeProc->SetBlackOut(TRUE);
			}
			else {
				m_pGMain->m_pBackForeProc->SetFade(TRUE);
			}
			m_bIsMoveMap = TRUE;
		}
	}
	else if (EventMap.m_nEvtKind == eEvtMoveMap)  // �ړ��}�b�v�̐���̂Ƃ�
	{
		i = EventMap.m_nEvtNo;
		if (EventMap.m_nEvtOpe1 == 1)   // �\���L���Ɋւ��鏈��
		{
			m_MoveMap[i].m_bActive = EventMap.m_nEvtOpe2;
		}
		else if (EventMap.m_nEvtOpe1 == 2)   // �ړ��Ɋւ��鏈��
		{
			m_MoveMap[i].m_nMoveFlag = EventMap.m_nEvtOpe2;
			m_MoveMap[i + 1].m_nMoveFlag = EventMap.m_nEvtOpe2;
		}
		EventMap.m_nEvtCycle = 1;  // �C�x���g�T�C�N�����Q�ɂ���
	}
}

//-----------------------------------------------------------------------------
// �ړ��}�b�v�̍X�V
//
// �ړ��}�b�v�̕��s�ړ����]�̍X�V���s��
// ���E�l�i��[���[���j�ɒB�����Ƃ��́A���]�ichange�j���s��
// ���]�ichange�j�������Ƃ��́Am_nChangeFlag�ɂP���Ԃ�
// ���[�v�����Ȃ��Ƃ��͔��]�̃^�C�~���O�Œ�~����   
//-----------------------------------------------------------------------------
void CMapProc::UpdateMoveMap()
{
	for (DWORD i = 0; i < m_MoveMap.size(); i++)
	{
		if (m_MoveMap[i].m_pColMesh && m_MoveMap[i].m_bActive)   // �ړ��}�b�v�̍X�V����
		{
			m_MoveMap[i].m_pColMesh->m_mWorldOld = m_MoveMap[i].m_pColMesh->m_mWorld;   // ���[���h�}�g���b�N�X��O
			if (m_pGMain->m_pEnmProc->CountActiveEnm() == 0 && m_MoveMap[0].m_nMoveFlag == -1) {
				// �G���S�ł�����
				m_MoveMap[0].m_nMoveFlag = 1;
				m_pGMain->m_pDownCage->Play(AUDIO_LOOP);
			}
			
			m_MoveMap[i].m_nChangeFlag = 0;      // �`�F���W�t���O�̃��Z�b�g
			if (m_MoveMap[i].m_nMoveFlag == 1)   // ���s�ړ��̏���
			{
				MATRIX4X4 mPosUp;
				mPosUp = XMMatrixTranslation(m_MoveMap[i].m_vUp.x, m_MoveMap[i].m_vUp.y, m_MoveMap[i].m_vUp.z);
				m_MoveMap[i].m_pColMesh->SetWorldMatrix(mPosUp * m_MoveMap[i].m_pColMesh->m_mWorld);  // �ړ������̏���
				if (m_MoveMap[i].m_vUp.x != 0)  // ���E�ɓ�����
				{
					if (GetPositionVector(m_MoveMap[i].m_pColMesh->m_mWorld).x < m_MoveMap[i].m_vMin.x ||  // ���E�ɓ��������߉E�[���[�Ŕ��]
						GetPositionVector(m_MoveMap[i].m_pColMesh->m_mWorld).x > m_MoveMap[i].m_vMax.x )
					{
						m_MoveMap[i].m_vUp.x *= -1;
						m_MoveMap[i].m_nChangeFlag = 1;
					}
				}
				if (m_MoveMap[i].m_vUp.y != 0)  // �㉺�ɓ�����
				{
					if (GetPositionVector(m_MoveMap[i].m_pColMesh->m_mWorld).y < m_MoveMap[i].m_vMin.y ||  // �㉺�ɓ��������ߏ�[���[�Ŕ��]
						GetPositionVector(m_MoveMap[i].m_pColMesh->m_mWorld).y > m_MoveMap[i].m_vMax.y)
					{
						m_MoveMap[i].m_vUp = VECTOR3(0.0f, 0.0f, 0.0f);
						m_pGMain->m_pCameraProc->GetCameraObjPtr()->SetLookStairsFlag(false);
						m_pGMain->m_pDownCage->Stop();
						m_pGMain->m_pWeaponProc->m_pWeaponItemBoxProc->Start(VECTOR3(0.0f, 0.0f, 20.0f));
						if (m_nHierarchy == 1 && !m_pGMain->m_bIsBoxTutorial)
						{
							m_pGMain->m_bIsBoxTutorial = TRUE;
							m_pGMain->m_pBackForeProc->m_nDialogKind = DIALOG_TUTORIAL3;
							m_pGMain->m_isMove = FALSE;
						}
					}
				}
				if (m_MoveMap[i].m_vUp.z != 0)  // �O��ɓ�����
				{
					if (GetPositionVector(m_MoveMap[i].m_pColMesh->m_mWorld).z < m_MoveMap[i].m_vMin.z ||  // �O��ɓ��������ߑO�[��[�Ŕ��]
						GetPositionVector(m_MoveMap[i].m_pColMesh->m_mWorld).z > m_MoveMap[i].m_vMax.z)
					{
						m_MoveMap[i].m_vUp.z *= -1;
						m_MoveMap[i].m_nChangeFlag = 1;
					}
				}
			}
			else if (m_MoveMap[i].m_nMoveFlag == 2){   // ��]�̏���
				MATRIX4X4 mRotUp;
				if (m_MoveMap[i].m_vUp.x != 0)  // �w��]
				{
					mRotUp = XMMatrixRotationX(XMConvertToRadians(m_MoveMap[i].m_vUp.x));
					m_MoveMap[i].m_pColMesh->SetWorldMatrix(mRotUp * m_MoveMap[i].m_pColMesh->m_mWorld);  // �w��]�����̏���
					if (GetRotateVector(m_MoveMap[i].m_pColMesh->m_mWorld).x < m_MoveMap[i].m_vMin.x ||  // �w��]���邽�ߒ[�Ŕ��]
						GetRotateVector(m_MoveMap[i].m_pColMesh->m_mWorld).x > m_MoveMap[i].m_vMax.x)
					{
						m_MoveMap[i].m_vUp.x *= -1;  // �w��]���邽�ߒ[�Ŕ��]
						m_MoveMap[i].m_nChangeFlag = 1;
					}
				}else if (m_MoveMap[i].m_vUp.y != 0)  // �x��]
				{
					mRotUp = XMMatrixRotationY(XMConvertToRadians(m_MoveMap[i].m_vUp.y));
					m_MoveMap[i].m_pColMesh->SetWorldMatrix(mRotUp * m_MoveMap[i].m_pColMesh->m_mWorld);  // �w��]�����̏���
					if (GetRotateVector(m_MoveMap[i].m_pColMesh->m_mWorld).y < m_MoveMap[i].m_vMin.y ||  // �x��]���邽�ߒ[�Ŕ��]
						GetRotateVector(m_MoveMap[i].m_pColMesh->m_mWorld).y > m_MoveMap[i].m_vMax.y)
					{
						m_MoveMap[i].m_vUp.y *= -1;  // �x��]���邽�ߒ[�Ŕ��]
						m_MoveMap[i].m_nChangeFlag = 1;
					}
				}
				else if (m_MoveMap[i].m_vUp.z != 0)  // �y��]
				{
					mRotUp = XMMatrixRotationZ(XMConvertToRadians(m_MoveMap[i].m_vUp.z));
					m_MoveMap[i].m_pColMesh->SetWorldMatrix(mRotUp * m_MoveMap[i].m_pColMesh->m_mWorld);  // �w��]�����̏���
					if (GetRotateVector(m_MoveMap[i].m_pColMesh->m_mWorld).z < m_MoveMap[i].m_vMin.z ||  // �y��]���邽�ߒ[�Ŕ��]
						GetRotateVector(m_MoveMap[i].m_pColMesh->m_mWorld).z > m_MoveMap[i].m_vMax.z)
					{
						m_MoveMap[i].m_vUp.z *= -1;  // �y��]���邽�ߒ[�Ŕ��]
						m_MoveMap[i].m_nChangeFlag = 1;
					}
				}
			}
			else if (m_MoveMap[i].m_nMoveFlag == 3) {   // �g��k���̏���      // -- 2019.12.30
				MATRIX4X4 mScaleUp;
				mScaleUp = XMMatrixScaling(m_MoveMap[i].m_vUp.x, m_MoveMap[i].m_vUp.y, m_MoveMap[i].m_vUp.z);
				m_MoveMap[i].m_pColMesh->SetWorldMatrix(mScaleUp * m_MoveMap[i].m_pColMesh->m_mWorld);  // �g��k�������̏���
				VECTOR3 vScaleNow = GetScaleVector(m_MoveMap[i].m_pColMesh->m_mWorld);     // ���݂̊g�嗦�𓾂�
				if (vScaleNow.x < m_MoveMap[i].m_vMin.x ||      // �K��̊g��k���ɒB������
					vScaleNow.x > m_MoveMap[i].m_vMax.x ||
					vScaleNow.y < m_MoveMap[i].m_vMin.y ||
					vScaleNow.y > m_MoveMap[i].m_vMax.y ||
					vScaleNow.z < m_MoveMap[i].m_vMin.z ||
					vScaleNow.z > m_MoveMap[i].m_vMax.z)
				{
					m_MoveMap[i].m_vUp.x = 1 / avoidZero(m_MoveMap[i].m_vUp.x);  // �g��k���̔��]
					m_MoveMap[i].m_vUp.y = 1 / avoidZero(m_MoveMap[i].m_vUp.y);  // �g��k���̔��]
					m_MoveMap[i].m_vUp.z = 1 / avoidZero(m_MoveMap[i].m_vUp.z);  // �g��k���̔��]
					m_MoveMap[i].m_nChangeFlag = 1;
				}
			}
			
			// ���[�v�����Ȃ��Ƃ��̒�~���f�B���]�̃^�C�~���O�Œ�~����        
			if (m_MoveMap[i].m_nLoop == 0 && m_MoveMap[i].m_nChangeFlag == 1)
			{
				m_MoveMap[i].m_nMoveFlag = 0;    // �ړ����Ȃ�
			}
		}
	}
}

//============================================================================
//  �}�b�v�v���V�[�W���̃����_�����O
// 
//    �����@�@�Ȃ�
// 
//    �߂�l�@�Ȃ�
//============================================================================
void  CMapProc::Render(void)
{
	if (m_bActive) {

		// ��̃����_�����O
		if (m_SkyMap.size() > 0)
		{
			// ��̃����_�����O   �A�e�����Ȃ������_�����O 
			// Z�o�b�t�@�𖳌���
			m_pGMain->m_pD3D->SetZBuffer(FALSE);
			m_SkyMap[0].m_pMesh->Render(m_SkyMap[0].m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, VECTOR3(0, 0, 0), m_pGMain->m_vEyePt);  // �A�e�����Ȃ������_�����O 
			// Z�o�b�t�@��L����
			m_pGMain->m_pD3D->SetZBuffer(TRUE);
		}

		// �}�b�v�����_�����O
		for (DWORD i = 0; i<m_StageMap.size(); i++)
		{
			if (m_StageMap[i].m_nMaterialFlag == 0)  // �ʏ�̃u�����h�X�e�[�g
			{
				if (m_StageMap[i].m_pMesh) m_StageMap[i].m_pMesh->RenderToon(m_StageMap[i].m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, normalize(VECTOR3(0.8f, 1, -1)), m_pGMain->m_vEyePt);
			}
			else if (m_StageMap[i].m_nMaterialFlag == 2)  // �f�B�X�v���C�X�����g�}�b�s���O
			{
				// �f�B�X�v���C�X�����g�}�b�v�����_�����O
				if (m_StageMap[i].m_pMesh) 	m_StageMap[i].m_pMesh->RenderDisplace(m_StageMap[i].m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
			}
		}

		// �ړ��}�b�v�����_�����O     * ���̃v���O�����ł͎g�p���Ă��Ȃ�
		for (DWORD i = 0; i<m_MoveMap.size(); i++)
		{
			if( m_MoveMap[i].m_nMaterialFlag == 0 )  // �ʏ�̃u�����h�X�e�[�g
			{
				if (m_MoveMap[i].m_pMesh && m_MoveMap[i].m_bActive)	m_MoveMap[i].m_pMesh->RenderToon(m_MoveMap[i].m_pColMesh->m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, normalize(VECTOR3(0.8f, 1, -1)), m_pGMain->m_vEyePt);
			}
			else if( m_MoveMap[i].m_nMaterialFlag == 2 )  // �f�B�X�v���C�X�����g�}�b�s���O
			{
				if (m_MoveMap[i].m_pMesh && m_MoveMap[i].m_bActive)	m_MoveMap[i].m_pMesh->RenderDisplace(m_MoveMap[i].m_pColMesh->m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
			}
		}
	}
}

//============================================================================
//  �}�b�v�v���V�[�W���̃����_�����O ���̂Q
// 
//  �������̃}�b�v�I�u�W�F�N�g��`�悷�邽�߁A�僋�[�v�̍Ō�Ɏ��s�����
// 
//    �����@�@�Ȃ�
// 
//    �߂�l�@�Ȃ�
//============================================================================
void  CMapProc::Render2(void)
{
	if (m_bActive) {

		// �}�b�v�����_�����O
		for (DWORD i = 0; i<m_StageMap.size(); i++)
		{
			if (m_StageMap[i].m_nMaterialFlag == 1)  // �����F�̃u�����h�X�e�[�g
			{
				// �����F�̃����_�����O
				if (m_StageMap[i].m_pMesh) m_StageMap[i].m_pMesh->RenderToon(m_StageMap[i].m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, normalize(VECTOR3(0.8f, 1, -1)), m_pGMain->m_vEyePt);
			}
		}

		// �ړ��}�b�v�����_�����O     * ���̃v���O�����ł͎g�p���Ă��Ȃ�
		for (DWORD i = 0; i<m_MoveMap.size(); i++)
		{
			if (m_MoveMap[i].m_nMaterialFlag == 1)  // �����F�̃u�����h�X�e�[�g
			{
				if (m_MoveMap[i].m_pMesh && m_MoveMap[i].m_bActive)	m_MoveMap[i].m_pMesh->RenderToon(m_MoveMap[i].m_pColMesh->m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, normalize(VECTOR3(0.8f, 1, -1)), m_pGMain->m_vEyePt);
			}
		}

		// �g�̃����_�����O
		for (DWORD i = 0; i<m_StageMap.size(); i++)
		{
			if (m_StageMap[i].m_nMaterialFlag == 3)  
			{
				// �g�����_�����O
				if (m_StageMap[i].m_pWave)
					m_StageMap[i].m_pWave->Render(m_StageMap[i].m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
			}
		}
	}
}

//-----------------------------------------------------------------------------
//  �����_�����񂾐��l����Ƀ}�b�v�J��
//-----------------------------------------------------------------------------
void CMapProc::MoveMap()
{
	switch (m_MapIdArray[m_nHierarchy - 1])
	{
	case 2:
		LoadMap2();
		break;
	case 3:
		LoadMap3();
		break;
	case 4:
		LoadMap4();
		break;
	case 5:
		LoadMap5();
		break;
	case 6:
		LoadMap6();
		break;
	case 7:
		LoadMap7();
		break;
	case 8:
		LoadMap8();
		break;
	case 9:
		LoadMap9();
		break;
	case 10:
		LoadMap10();
		break;
	case 11:
		LoadMap11();
		break;
	case 12:
		LoadMap12();
		break;
	case 13:
		LoadMap13();
		break;
	case 14:
		LoadMap14();
		break;
	case 15:
		LoadMap15();
		break;
	case 16:
		LoadMap16();
		break;
	case 17:
		LoadMap17();
		break;
	case 18:
		LoadMap18();
		break;
	case 19:
		LoadMap19();
		break;
	case 20:
		LoadMap20();
		break;
	}
	m_nHierarchy++;
}

//-----------------------------------------------------------------------------
//  �J������Player���f���̊ԂɃ}�b�v�I�u�W�F�N�g������ꍇ�A���߂�����
//-----------------------------------------------------------------------------
void CMapProc::CheckCrossStageMesh(VECTOR3 vNow, VECTOR3 vOld)
{
	VECTOR3 vHit = VECTOR3(0.0f, 0.0f, 0.0f);
	VECTOR3 vNrm = VECTOR3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < m_StageMap.size(); i++) {
		// ���ߔ�����s��Ȃ��I�u�W�F�N�g���p�X����
		if (m_StageMap[i].m_pBox != NULL)
		{
			if (m_StageMap[i].m_pBox->OBBCollisionLay(vNow, vOld, &vHit, &vNrm))
			{
				m_StageMap[i].m_pMesh->m_vDiffuse.w = 0.3f;
				m_StageMap[i].m_nMaterialFlag = 1;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//  �}�b�v�̓��ߏ������Z�b�g����
//-----------------------------------------------------------------------------
void CMapProc::ResetStageMeshAlfa()
{
	for (int i = 0; i < m_StageMap.size(); i++)
	{
		m_StageMap[i].m_pMesh->m_vDiffuse.w = 1.0f;
		m_StageMap[i].m_nMaterialFlag = 0;
	}
}

//============================================================================
//  �A�N�Z�X�֐�
//============================================================================
std::vector<NAVIGATIONMAP> CMapProc::GetNavMap() 
{ 
	return m_NavMap; 
}

DWORD CMapProc::GetMapNo() 
{ 
	return m_dwMapNo; 
}

void CMapProc::SetTutorial(BOOL flag)
{
	m_bIsTutorial = flag;
}

CCollision* CMapProc::GetCollision() 
{ 
	return m_pColMesh; 
}

ROUTECOLLISION* CMapProc::GetRouteColMap() 
{ 
	return  &m_RouteColMap;
}

int CMapProc::GetHierarchy() 
{ 
	return m_nHierarchy; 
}

void CMapProc::SetHierarchy(int hierarchy)
{ 
	m_nHierarchy = hierarchy; 
}

CSpPart* CMapProc::GetPcSpPart() 
{ 
	return m_pPcSpPart; 
}

CSpPart* CMapProc::GetEnmSpPart() 
{ 
	return m_pEnmSpPart; 
}
