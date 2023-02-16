//=============================================================================
//		エフェクトの処理
//=============================================================================

#pragma once
#include "GameMain.h"

#define  PARTICLE_MAX   200
#define  BILLBOARD_MAX  200
#define  BILLFIRE_MAX   200

// 
class CDirect3D;
// ========================================================================================
//	パーティクル
//（ポイントスプライトでパーティクルを実現）
// ========================================================================================
// パーティクル構造体
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

// 頂点の構造体
struct PARTICLE_VERTEX
{
	VECTOR3 Pos; //位置
};
struct BILLBOARD_VERTEX
{
	VECTOR3 Pos; //位置
	VECTOR2 UV; //テクスチャー座標
};

// -----------------------------------------------------------------------------
//	プラスパーティクルのオブジェクトクラス
// ----------------------------------------------------------------------------
class CPlusParticleObj : public CBaseObj
{
protected:
	// 
	CDirect3D*	   m_pD3D;
	CShader*       m_pShader;
	FLOAT          m_fAlpha;
	float          m_fSize;  // パーティクルのサイズ // -- 2018.8.23
	int            m_MaxParticle;
	VECTOR3        m_vEmitPos;
	PART*          m_pPartArray;
	int            m_Frame;
	int            m_FrameEnd;
	float          m_fSpeed;  // パーティクルの移動スピード
	int            m_iBarthFrame;     // パーティクルの移動開始までの最大待ち時間（ランダム）

	ID3D11Buffer*  m_pVertexBuffer;
	ID3D11ShaderResourceView* m_pTexture;//テクスチャー（パーティクルプロシージャのテクスチャアドレスをセットするだけ）

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
//	プラスパーティクルのプロシージャクラス
// ========================================================================================
class CPlusParticleProc : public CBaseProc
{
protected:
	ID3D11ShaderResourceView* m_pTexture;  //テクスチャー（用意するのはリソースビューだけでいい）
public:
	BOOL Start(VECTOR3 vEmitPos, VECTOR3 vNormal = VECTOR3(0.0f, 0.0f, 0.0f));
	CPlusParticleProc(CGameMain* pGMain);	// コンストラクタ
	~CPlusParticleProc();	// デストラクタ
};

// -----------------------------------------------------------------------------
//	マイナスパーティクルのオブジェクトクラス
// ----------------------------------------------------------------------------
class CMinusParticleObj : public CBaseObj
{
protected:
	// 
	CDirect3D* m_pD3D;
	CShader* m_pShader;
	FLOAT          m_fAlpha;
	float          m_fSize;  // パーティクルのサイズ // -- 2018.8.23
	int            m_MaxParticle;
	VECTOR3        m_vEmitPos;
	PART* m_pPartArray;
	int            m_Frame;
	int            m_FrameEnd;
	float          m_fSpeed;  // パーティクルの移動スピード
	int            m_iBarthFrame;     // パーティクルの移動開始までの最大待ち時間（ランダム）

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11ShaderResourceView* m_pTexture;//テクスチャー（パーティクルプロシージャのテクスチャアドレスをセットするだけ）

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
//	マイナスパーティクルのプロシージャクラス
// ========================================================================================
class CMinusParticleProc : public CBaseProc
{
protected:
	ID3D11ShaderResourceView* m_pTexture;  //テクスチャー（用意するのはリソースビューだけでいい）
public:
	BOOL Start(VECTOR3 vEmitPos, VECTOR3 vNormal = VECTOR3(0.0f, 0.0f, 0.0f));
	CMinusParticleProc(CGameMain* pGMain);	// コンストラクタ
	~CMinusParticleProc();	// デストラクタ
};


// ========================================================================================
//	ビルボード
// ========================================================================================
// ------------------------------------------------------------------------
//	爆発ビルボードのオブジェクトクラス
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
	ID3D11ShaderResourceView* m_pTexture;//テクスチャー（ビルボードプロシージャのテクスチャアドレスをセットするだけ）

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
//	爆発ビルボードのプロシージャクラス
// ========================================================================================
class CBillboardProc : public CBaseProc
{
protected:
	ID3D11ShaderResourceView* m_pTexture;  //テクスチャー（用意するのはリソースビューだけでいい）
	DWORD m_dwImageWidth;
	DWORD m_dwImageHeight;
public:
	BOOL Start(VECTOR3 vPos);
	HRESULT Load(TCHAR* szFName);
	CBillboardProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CBillboardProc();	// デストラクタ
};

// ========================================================================================
//	ビルボード　近接攻撃エフェクト
// ========================================================================================
// ------------------------------------------------------------------------
//	ビルボード　近接攻撃エフェクトのオブジェクトクラス
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
	ID3D11ShaderResourceView* m_pTexture;//テクスチャー（ビルボードプロシージャのテクスチャアドレスをセットするだけ）

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
//	ビルボード　近接攻撃エフェクトのプロシージャクラス
// ========================================================================================
class CBillSlashProc : public CBaseProc
{
protected:
	ID3D11ShaderResourceView* m_pTexture;  //テクスチャー（用意するのはリソースビューだけでいい）
	DWORD m_dwImageWidth;
	DWORD m_dwImageHeight;
public:
	BOOL Start(MATRIX4X4 mWorld);
	HRESULT Load(TCHAR* szFName);
	CBillSlashProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CBillSlashProc();	// デストラクタ
};

// ========================================================================================
//	ビルボード　斬撃エフェクト
// ========================================================================================
// ------------------------------------------------------------------------
//	ビルボード　斬撃のオブジェクトクラス
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
	ID3D11ShaderResourceView* m_pTexture;//テクスチャー（ビルボードプロシージャのテクスチャアドレスをセットするだけ）

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
//	ビルボード　斬撃のプロシージャクラス
// ========================================================================================
class CBillSlashShotProc : public CBaseProc
{
protected:
	ID3D11ShaderResourceView* m_pTexture;  //テクスチャー（用意するのはリソースビューだけでいい）
	DWORD m_dwImageWidth;
	DWORD m_dwImageHeight;
public:
	CBaseObj* Start(MATRIX4X4 mWorld);
	HRESULT Load(TCHAR* szFName);
	CBillSlashShotProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CBillSlashShotProc();	// デストラクタ
};

// ========================================================================================
//	ビルボード　遠投エフェクト
// ========================================================================================
// ------------------------------------------------------------------------
//	ビルボード　遠投のオブジェクトクラス
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
	ID3D11ShaderResourceView* m_pTexture;//テクスチャー（ビルボードプロシージャのテクスチャアドレスをセットするだけ）

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
//	ビルボード　遠投のプロシージャクラス
// ========================================================================================
class CBillAxeShotProc : public CBaseProc
{
protected:
	ID3D11ShaderResourceView* m_pTexture;  //テクスチャー（用意するのはリソースビューだけでいい）
	DWORD m_dwImageWidth;
	DWORD m_dwImageHeight;
public:
	CBaseObj* Start(MATRIX4X4 mWorld);
	HRESULT Load(TCHAR* szFName);
	CBillAxeShotProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CBillAxeShotProc();	// デストラクタ
};

// ========================================================================================
//	エフェクト全体のメインプロシージャクラス
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
	CEffectProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CEffectProc() { ; }
};

