//=============================================================================
//		�G�t�F�N�g�̏���
//=============================================================================

#pragma once
#include "GameMain.h"

#define  PARTICLE_MAX   200
#define  BILLBOARD_MAX  200
#define  BILLFIRE_MAX   200

// 
class CDirect3D;
// ========================================================================================
//	�p�[�e�B�N��
//�i�|�C���g�X�v���C�g�Ńp�[�e�B�N���������j
// ========================================================================================
// �p�[�e�B�N���\����
struct PART
{
	VECTOR3 Pos;
	VECTOR3 Dir;
	float       Speed;
	int         BirthFrame;
	PART()
	{
		ZeroMemory(this, sizeof(PART));
	}
};

// ���_�̍\����
struct PARTICLE_VERTEX
{
	VECTOR3 Pos; //�ʒu
};
struct BILLBOARD_VERTEX
{
	VECTOR3 Pos; //�ʒu
	VECTOR2 UV; //�e�N�X�`���[���W
};

// -----------------------------------------------------------------------------
//	�v���X�p�[�e�B�N���̃I�u�W�F�N�g�N���X
// ----------------------------------------------------------------------------
class CPlusParticleObj : public CBaseObj
{
protected:
	// 
	CDirect3D*	   m_pD3D;
	CShader*       m_pShader;
	FLOAT          m_fAlpha;
	float          m_fSize;  // �p�[�e�B�N���̃T�C�Y // -- 2018.8.23
	int            m_MaxParticle;
	VECTOR3        m_vEmitPos;
	PART*          m_pPartArray;
	int            m_Frame;
	int            m_FrameEnd;
	float          m_fSpeed;  // �p�[�e�B�N���̈ړ��X�s�[�h
	int            m_iBarthFrame;     // �p�[�e�B�N���̈ړ��J�n�܂ł̍ő�҂����ԁi�����_���j

	ID3D11Buffer*  m_pVertexBuffer;
	ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`���[�i�p�[�e�B�N���v���V�[�W���̃e�N�X�`���A�h���X���Z�b�g���邾���j

public:	
	// 
	CPlusParticleObj(CBaseProc*  pProc, int MaxParticle );      // -- 2022.2.16
	virtual	~CPlusParticleObj();

	void Init(int MaxParticle);
	HRESULT SetSrc();

	BOOL Start(VECTOR3 vEmitPos, VECTOR3 vNormal = VECTOR3(0.0f,0.0f,0.0f)) override;
	void Update() override;
	void Render();
	void RenderParticle(MATRIX4X4 W, MATRIX4X4 V, MATRIX4X4 P);  // -- 2017.8.25

	void SetTexture(ID3D11ShaderResourceView* pTexture) { m_pTexture = pTexture; }

};
// ========================================================================================
//	�v���X�p�[�e�B�N���̃v���V�[�W���N���X
// ========================================================================================
class CPlusParticleProc : public CBaseProc
{
protected:
	ID3D11ShaderResourceView* m_pTexture;  //�e�N�X�`���[�i�p�ӂ���̂̓��\�[�X�r���[�����ł����j
public:
	BOOL Start(VECTOR3 vEmitPos, VECTOR3 vNormal = VECTOR3(0.0f, 0.0f, 0.0f));
	CPlusParticleProc(CGameMain* pGMain);	// �R���X�g���N�^
	~CPlusParticleProc();	// �f�X�g���N�^
};

// -----------------------------------------------------------------------------
//	�}�C�i�X�p�[�e�B�N���̃I�u�W�F�N�g�N���X
// ----------------------------------------------------------------------------
class CMinusParticleObj : public CBaseObj
{
protected:
	// 
	CDirect3D* m_pD3D;
	CShader* m_pShader;
	FLOAT          m_fAlpha;
	float          m_fSize;  // �p�[�e�B�N���̃T�C�Y // -- 2018.8.23
	int            m_MaxParticle;
	VECTOR3        m_vEmitPos;
	PART* m_pPartArray;
	int            m_Frame;
	int            m_FrameEnd;
	float          m_fSpeed;  // �p�[�e�B�N���̈ړ��X�s�[�h
	int            m_iBarthFrame;     // �p�[�e�B�N���̈ړ��J�n�܂ł̍ő�҂����ԁi�����_���j

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`���[�i�p�[�e�B�N���v���V�[�W���̃e�N�X�`���A�h���X���Z�b�g���邾���j

public:
	// 
	CMinusParticleObj(CBaseProc* pProc, int MaxParticle);      // -- 2022.2.16
	virtual	~CMinusParticleObj();

	void Init(int MaxParticle);
	HRESULT SetSrc();

	BOOL Start(VECTOR3 vEmitPos, VECTOR3 vNormal = VECTOR3(0.0f, 0.0f, 0.0f)) override;
	void Update() override;
	void Render();
	void RenderParticle(MATRIX4X4 W, MATRIX4X4 V, MATRIX4X4 P);  // -- 2017.8.25

	void SetTexture(ID3D11ShaderResourceView* pTexture) { m_pTexture = pTexture; }

};
// ========================================================================================
//	�}�C�i�X�p�[�e�B�N���̃v���V�[�W���N���X
// ========================================================================================
class CMinusParticleProc : public CBaseProc
{
protected:
	ID3D11ShaderResourceView* m_pTexture;  //�e�N�X�`���[�i�p�ӂ���̂̓��\�[�X�r���[�����ł����j
public:
	BOOL Start(VECTOR3 vEmitPos, VECTOR3 vNormal = VECTOR3(0.0f, 0.0f, 0.0f));
	CMinusParticleProc(CGameMain* pGMain);	// �R���X�g���N�^
	~CMinusParticleProc();	// �f�X�g���N�^
};


// ========================================================================================
//	�r���{�[�h
// ========================================================================================
// ------------------------------------------------------------------------
//	�����r���{�[�h�̃I�u�W�F�N�g�N���X
// ------------------------------------------------------------------------
class CBillboardObj : public CBaseObj
{
protected:
	float			m_fDestWidth;
	float			m_fDestHeight;
	DWORD			m_dwSrcX;
	DWORD			m_dwSrcY;
	DWORD			m_dwSrcWidth;
	DWORD			m_dwSrcHeight;
public:

	// 
	CDirect3D*	   m_pD3D;
	CShader*       m_pShader;
	VECTOR3        m_vPos;
	VECTOR2        m_vUVOffset;
	VECTOR2        m_vUVScale;
	DWORD          m_dwNumX;
	DWORD          m_dwNumY;
	DWORD          m_dwFrame;
	FLOAT          m_fAlpha;
	DWORD          m_dwImageWidth;
	DWORD          m_dwImageHeight;

	ID3D11Buffer*  m_pVertexBuffer;
	ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`���[�i�r���{�[�h�v���V�[�W���̃e�N�X�`���A�h���X���Z�b�g���邾���j

public:
	// 
	CBillboardObj(CBaseProc* pProc);     // -- 2022.2.16
	virtual	~CBillboardObj();

	void Init();
	HRESULT SetSrc( float fDestWidth, float fDestHeight, DWORD dwSrcX, DWORD dwSrcY, DWORD dwSrcWidth, DWORD dwSrcHeight, DWORD dwNumX, DWORD dwNumY);
	DWORD   GetSrcWidth()  {return m_dwSrcWidth;}
	DWORD   GetSrcHeight() {return m_dwSrcHeight;}
	float   GetDestWidth()  {return m_fDestWidth;}
	float   GetDestHeight() {return m_fDestHeight;}

	BOOL Start(VECTOR3 vPos) override;
	void Update() override;
	BOOL Render();
	void RenderMesh();

};
// ========================================================================================
//	�����r���{�[�h�̃v���V�[�W���N���X
// ========================================================================================
class CBillboardProc : public CBaseProc
{
protected:
	ID3D11ShaderResourceView* m_pTexture;  //�e�N�X�`���[�i�p�ӂ���̂̓��\�[�X�r���[�����ł����j
	DWORD m_dwImageWidth;
	DWORD m_dwImageHeight;
public:
	BOOL Start(VECTOR3 vPos);
	HRESULT Load(TCHAR* szFName);
	CBillboardProc(CGameMain* pGMain);	// �R���X�g���N�^
	virtual	~CBillboardProc();	// �f�X�g���N�^
};

// ========================================================================================
//	�r���{�[�h�@�ߐڍU���G�t�F�N�g
// ========================================================================================
// ------------------------------------------------------------------------
//	�r���{�[�h�@�ߐڍU���G�t�F�N�g�̃I�u�W�F�N�g�N���X
// ------------------------------------------------------------------------
class CBillSlashObj : public CBaseObj
{
protected:
	float			m_fDestWidth;
	float			m_fDestHeight;
	DWORD			m_dwSrcX;
	DWORD			m_dwSrcY;
	DWORD			m_dwSrcWidth;
	DWORD			m_dwSrcHeight;
public:
	CDirect3D* m_pD3D;
	CShader* m_pShader;
	VECTOR3        m_vPos;
	VECTOR2        m_vUVOffset;
	VECTOR2        m_vUVScale;  // -- 2019.7.17
	DWORD          m_dwNumX;
	DWORD          m_dwNumY;
	DWORD          m_dwFrame;
	FLOAT          m_fAlpha;
	DWORD          m_dwImageWidth;
	DWORD          m_dwImageHeight;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`���[�i�r���{�[�h�v���V�[�W���̃e�N�X�`���A�h���X���Z�b�g���邾���j

public:
	// 
	CBillSlashObj(CBaseProc* pProc);     // -- 2022.2.16
	virtual	~CBillSlashObj();

	void Init();
	HRESULT SetSrc(float fDestWidth, float fDestHeight, DWORD dwSrcX, DWORD dwSrcY, DWORD dwSrcWidth, DWORD dwSrcHeight, DWORD dwNumX, DWORD dwNumY);
	DWORD   GetSrcWidth() { return m_dwSrcWidth; }
	DWORD   GetSrcHeight() { return m_dwSrcHeight; }
	float   GetDestWidth() { return m_fDestWidth; }
	float   GetDestHeight() { return m_fDestHeight; }

	BOOL Start(MATRIX4X4 mWorld, DWORD Owner) override;
	void Update() override;
	BOOL Render();
	void RenderMesh();

};
// ========================================================================================
//	�r���{�[�h�@�ߐڍU���G�t�F�N�g�̃v���V�[�W���N���X
// ========================================================================================
class CBillSlashProc : public CBaseProc
{
protected:
	ID3D11ShaderResourceView* m_pTexture;  //�e�N�X�`���[�i�p�ӂ���̂̓��\�[�X�r���[�����ł����j
	DWORD m_dwImageWidth;
	DWORD m_dwImageHeight;
public:
	BOOL Start(MATRIX4X4 mWorld);
	HRESULT Load(TCHAR* szFName);
	CBillSlashProc(CGameMain* pGMain);	// �R���X�g���N�^
	virtual	~CBillSlashProc();	// �f�X�g���N�^
};

// ========================================================================================
//	�r���{�[�h�@�a���G�t�F�N�g
// ========================================================================================
// ------------------------------------------------------------------------
//	�r���{�[�h�@�a���̃I�u�W�F�N�g�N���X
// ------------------------------------------------------------------------
class CBillSlashShotObj : public CBaseObj
{
protected:
	float			m_fDestWidth;
	float			m_fDestHeight;
	DWORD			m_dwSrcX;
	DWORD			m_dwSrcY;
	DWORD			m_dwSrcWidth;
	DWORD			m_dwSrcHeight;
public:

	CDirect3D* m_pD3D;
	CShader* m_pShader;
	VECTOR3        m_vPos;
	VECTOR2        m_vUVOffset;
	VECTOR2        m_vUVScale; 
	DWORD          m_dwNumX;
	DWORD          m_dwNumY;
	DWORD          m_dwFrame;
	FLOAT          m_fAlpha;
	DWORD          m_dwImageWidth;
	DWORD          m_dwImageHeight;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`���[�i�r���{�[�h�v���V�[�W���̃e�N�X�`���A�h���X���Z�b�g���邾���j

public:
	CBillSlashShotObj(CBaseProc* pProc);     
	virtual	~CBillSlashShotObj();

	void Init();
	HRESULT SetSrc(float fDestWidth, float fDestHeight, DWORD dwSrcX, DWORD dwSrcY, DWORD dwSrcWidth, DWORD dwSrcHeight, DWORD dwNumX, DWORD dwNumY);
	DWORD   GetSrcWidth() { return m_dwSrcWidth; }
	DWORD   GetSrcHeight() { return m_dwSrcHeight; }
	float   GetDestWidth() { return m_fDestWidth; }
	float   GetDestHeight() { return m_fDestHeight; }

	BOOL Start(MATRIX4X4 mWorld, DWORD Owner) override;
	void Update() override;
	BOOL Render();
	void RenderMesh();

};
// ========================================================================================
//	�r���{�[�h�@�a���̃v���V�[�W���N���X
// ========================================================================================
class CBillSlashShotProc : public CBaseProc
{
protected:
	ID3D11ShaderResourceView* m_pTexture;  //�e�N�X�`���[�i�p�ӂ���̂̓��\�[�X�r���[�����ł����j
	DWORD m_dwImageWidth;
	DWORD m_dwImageHeight;
public:
	CBaseObj* Start(MATRIX4X4 mWorld);
	HRESULT Load(TCHAR* szFName);
	CBillSlashShotProc(CGameMain* pGMain);	// �R���X�g���N�^
	virtual	~CBillSlashShotProc();	// �f�X�g���N�^
};

// ========================================================================================
//	�r���{�[�h�@�����G�t�F�N�g
// ========================================================================================
// ------------------------------------------------------------------------
//	�r���{�[�h�@�����̃I�u�W�F�N�g�N���X
// ------------------------------------------------------------------------
class CBillAxeShotObj :public CBaseObj
{
protected:
	float			m_fDestWidth;
	float			m_fDestHeight;
	DWORD			m_dwSrcX;
	DWORD			m_dwSrcY;
	DWORD			m_dwSrcWidth;
	DWORD			m_dwSrcHeight;
public:
	CDirect3D* m_pD3D;
	CShader* m_pShader;
	VECTOR3        m_vPos;
	VECTOR2        m_vUVOffset;
	VECTOR2        m_vUVScale;  
	DWORD          m_dwNumX;
	DWORD          m_dwNumY;
	DWORD          m_dwFrame;
	FLOAT          m_fAlpha;
	DWORD          m_dwImageWidth;
	DWORD          m_dwImageHeight;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`���[�i�r���{�[�h�v���V�[�W���̃e�N�X�`���A�h���X���Z�b�g���邾���j

public:
	CBillAxeShotObj(CBaseProc* pProc);     
	virtual	~CBillAxeShotObj();

	void Init();
	HRESULT SetSrc(float fDestWidth, float fDestHeight, DWORD dwSrcX, DWORD dwSrcY, DWORD dwSrcWidth, DWORD dwSrcHeight, DWORD dwNumX, DWORD dwNumY);
	DWORD   GetSrcWidth() { return m_dwSrcWidth; }
	DWORD   GetSrcHeight() { return m_dwSrcHeight; }
	float   GetDestWidth() { return m_fDestWidth; }
	float   GetDestHeight() { return m_fDestHeight; }

	BOOL Start(MATRIX4X4 mWorld, DWORD Owner) override;
	void Update() override;
	BOOL Render();
	void RenderMesh();
};

// ========================================================================================
//	�r���{�[�h�@�����̃v���V�[�W���N���X
// ========================================================================================
class CBillAxeShotProc : public CBaseProc
{
protected:
	ID3D11ShaderResourceView* m_pTexture;  //�e�N�X�`���[�i�p�ӂ���̂̓��\�[�X�r���[�����ł����j
	DWORD m_dwImageWidth;
	DWORD m_dwImageHeight;
public:
	CBaseObj* Start(MATRIX4X4 mWorld);
	HRESULT Load(TCHAR* szFName);
	CBillAxeShotProc(CGameMain* pGMain);	// �R���X�g���N�^
	virtual	~CBillAxeShotProc();	// �f�X�g���N�^
};

// ========================================================================================
//	�G�t�F�N�g�S�̂̃��C���v���V�[�W���N���X
// ========================================================================================
class CEffectProc : public CBaseProc
{
public:
	CPlusParticleProc*		m_pParticleProc;
	CMinusParticleProc*		m_pMinusParticleProc;
	CBillboardProc*			m_pBillProc;
	CBillSlashProc*			m_pBillSlashProc;
	CBillSlashShotProc*		m_pBillSlashShotProc;
	CBillAxeShotProc*		m_pBillAxeShotProc;

public:
	CEffectProc(CGameMain* pGMain);	// �R���X�g���N�^
	virtual	~CEffectProc() { ; }
};

