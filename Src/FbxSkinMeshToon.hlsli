// -----------------------------------------------------------------------
// 
// Ｆｂｘスキンメッシュのシェーダー
// (トゥーンシェーディング方式)
// 
//                                                              2022.2.16
//                                                   FbxSkinMeshToon.hlsli
// -----------------------------------------------------------------------

#define MAX_BONE 255

// グローバル変数	
Texture2D g_Texture: register(t0);   //テクスチャーは レジスターt(n)
Texture2D g_NormalTexture : register(t1);   // 法線マップ
Texture2D g_ToonTexture: register(t4);   // Toonテクスチャ

SamplerState g_samLinear : register(s0);//サンプラーはレジスターs(n)

// コンスタントバッファ
// ワールドから射影までの変換行列・他(b0)
cbuffer global : register(b0)
{
	matrix g_mW;          // ワールド行列
	matrix g_mWVP;        // ワールドから射影までの変換行列
	float4 g_LightDir;    // ライトの方向ベクトル
	float4 g_EyePos;      // 視点位置
	float4 g_Diffuse;     // ディフューズ色	
	float4 g_DrawInfo;    // 各種情報　x:テクスチャ有り無し。(使っていない)
};

// ボーン行列(b1)
cbuffer cbBones : register(b1)
{
	matrix BoneFramePose[MAX_BONE];		// 指定フレームでのメッシュに影響を与える全ての骨のポーズ行列
};

// スキニング後の頂点・法線が入る
struct Skin
{
	float4 Pos4;
	float3 Normal;
	float3 Tangent; // 接線      // -- 2021.6.11
	float3 Binormal; // 従法線   // -- 2021.6.11
};

// マテリアルカラーのコンスタントバッファ
cbuffer global : register(b3)               // -- 2020.12.15
{
	float4 g_MatDiffuse = float4(0.5, 0.5, 0.5, 1);   // ディフューズ色	
	float4 g_MatSpecular = float4(0, 0, 0, 0);        // スペキュラ色
};

// バーテックスシェーダーの入力パラメータ(頂点フォーマットと同一)
struct VS_IN
{
	float3 Pos     : POSITION;		// 頂点座標
	float3 Normal  : NORMAL;		// 法線
	float2 Tex     : TEXCOORD;		// テクセル
	uint4  Bones   : BONE_INDEX;	// ボーンのインデックス
	float4 Weights : BONE_WEIGHT;	// ウェイト
	float3 Tangent : TANGENT; // 接線
	float3 Binormal : BINORMAL; // 従法線

};

// バーテックスシェーダーの出力構造体
// (ピクセルシェーダーの入力となる)
struct VS_OUTPUT
{
	float4 Pos    : SV_POSITION;
	float3 Normal : NORMAL;
	float2 Tex    : TEXCOORD;
	float4 lightTangentSpace : TEXCOORD1; // 接空間に変換されたライトベクトル
	float2 ToonUV : TEXCOORD2;

};

// 接空間行列の逆行列を算出
float4x4 InverseTangentMatrix(float3 tangent, float3 binormal, float3 normal)
{
	tangent = normalize(mul(tangent, (float3x3)g_mW));     // 接線をワールド変換する      // -- 2021.6.11
	binormal = normalize(mul(binormal, (float3x3)g_mW));   // 従法線をワールド変換する
	normal = normalize(mul(normal, (float3x3)g_mW));       // 法線をワールド変換する

	float4x4 mat = { float4(tangent, 0.0f),
		float4(binormal, 0.0f),
		float4(normal, 0.0f),
		{ 0.0f, 0.0f, 0.0f, 1.0f } };

	return transpose(mat); // 転置行列にする
}


// 頂点をスキニング（ボーンによる変形）するサブ関数          // -- 2021.6.11
// （バーテックスシェーダーで使用）
Skin SkinVert(VS_IN In)
{
	Skin Out;
	Out.Pos4 = float4(0.0f, 0.0f, 0.0f, 0.0f);
	Out.Normal = float3(0.0f, 0.0f, 0.0f);
	Out.Tangent = float3(0.0f, 0.0f, 0.0f);
	Out.Binormal = float3(0.0f, 0.0f, 0.0f);

	float4 pos4 = float4(In.Pos, 1);
	float3 normal = In.Normal;
	float3 tangent = In.Tangent;
	float3 binormal = In.Binormal;


	// ボーン0
	uint iBone = In.Bones.x;
	float fWeight = In.Weights.x;
	matrix m = BoneFramePose[iBone];
	Out.Pos4 += fWeight * mul(pos4, m);
	Out.Normal += fWeight * mul(normal, (float3x3)m);
	Out.Tangent += fWeight * mul(tangent, (float3x3)m);
	Out.Binormal += fWeight * mul(binormal, (float3x3)m);
	// ボーン1
	iBone = In.Bones.y;
	fWeight = In.Weights.y;
	m = BoneFramePose[iBone];
	Out.Pos4 += fWeight * mul(pos4, m);
	Out.Normal += fWeight * mul(normal, (float3x3)m);
	Out.Tangent += fWeight * mul(tangent, (float3x3)m);
	Out.Binormal += fWeight * mul(binormal, (float3x3)m);
	// ボーン2
	iBone = In.Bones.z;
	fWeight = In.Weights.z;
	m = BoneFramePose[iBone];
	Out.Pos4 += fWeight * mul(pos4, m);
	Out.Normal += fWeight * mul(normal, (float3x3)m);
	Out.Tangent += fWeight * mul(tangent, (float3x3)m);
	Out.Binormal += fWeight * mul(binormal, (float3x3)m);
	// ボーン3
	iBone = In.Bones.w;
	fWeight = In.Weights.w;
	m = BoneFramePose[iBone];
	Out.Pos4 += fWeight * mul(pos4, m);
	Out.Normal += fWeight * mul(normal, (float3x3)m);
	Out.Tangent += fWeight * mul(tangent, (float3x3)m);
	Out.Binormal += fWeight * mul(binormal, (float3x3)m);

	return Out;
}


//
//バーテックスシェーダー
// (トゥーンシェーディング)
//
VS_OUTPUT VS(VS_IN In)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	float4x4 inverseTangentMat;

	Skin vSkinned = SkinVert(In);		// スキニング

	output.Pos = mul(vSkinned.Pos4, g_mWVP);  // 頂点をワールド・ビュー・プロジェクション変換する
	output.Normal = mul(vSkinned.Normal, (float3x3)g_mW);     // 法線をワールド変換する
	output.Normal = normalize(output.Normal);     // 法線を正規化する

	// 接空間行列の逆行列を算出
	inverseTangentMat = InverseTangentMatrix(normalize(vSkinned.Tangent), normalize(vSkinned.Binormal), normalize(vSkinned.Normal));   // -- 2021.6.11

	// ライトベクトルを接空間上に変換
	output.lightTangentSpace = mul(float4(g_LightDir.xyz, 1.0), inverseTangentMat);

	output.Tex = In.Tex;     // テクスチャ座標はそのまま出力

	// トゥーンシェーディングの処理
	// トゥーンペイント（陰影）のテクスチャ座標ToonUVを作成する。
	// 物体表面の明るさ0.01～0.99をテクスチャのＵ座標にする。
	// （ボーダーラインの位置（0や1）を避けるようにする）
	output.ToonUV.x = min(0.99f, max(0.01f, dot(output.Normal, normalize(g_LightDir.xyz))));
	output.ToonUV.y = 0.01f;

	return output;
}

//
//ピクセルシェーダー
// (トゥーンシェーディング)
//
float4 PS(VS_OUTPUT In) : SV_Target
{
	float4 Color;      // 最終出力値
	uint width, height;
	g_Texture.GetDimensions(width, height);  // ディフューズテクスチャのサイズを得る  // -- 2020.12.15

	// テクスチャカラー
	float4 texColor;
	if (width == 0)  // ディフューズテクスチャがないとき
	{
		texColor = g_MatDiffuse;
	}
	else {
		texColor = g_Texture.Sample(g_samLinear, In.Tex);
	}
	float4 texNormal = g_NormalTexture.Sample(g_samLinear, In.Tex);
	float4 texToon = g_ToonTexture.Sample(g_samLinear, In.ToonUV);

	if (g_LightDir.x == 0 && g_LightDir.y == 0 && g_LightDir.z == 0)
	{
		// 光の方向が0,0,0の場合は光の計算をしないで描画
		Color.rgb = texColor.rgb * g_Diffuse.rgb;
		Color.a = texColor.a - (1 - g_Diffuse.a);
	}
	else {
		if (texNormal.w == 1.0f) {
			// 法線マップがある場合は法線マッピング付きのライティング

			// 法線マッピングの明るさ計算
			float4 normalVector = 2.0 * texNormal - 1.0f;  // ベクトルへ変換(法線マップの色は0～1.0。これを-1.0～1.0のベクトル範囲にする)
			normalVector = normalize(normalVector);  // 法線ベクトルの正規化
			//float NMbright = dot(normalize(In.lightTangentSpace), normalVector);// ライトの向きと法線マップの法線とで明度算出
			float NMbright = dot(normalize(In.lightTangentSpace), normalVector) * 0.9f + 0.5f;// ライトの向きと法線マップの法線とで明度算出
			NMbright = max(0.0f, NMbright);   // マイナスは0に補正

			// トゥーンレンダリング
			// 通常のテクスチャ色（texColor）にトゥーンペイント（ToonUVのサンプラ）を掛け合わせる
			Color.rgb = texColor.rgb * 1.1f * texToon.rgb * g_Diffuse.rgb;         // バンプマッピングなし(少し明るく)
			//Color.rgb = texColor.rgb * 1.2f * NMbright * texToon.rgb * g_Diffuse.rgb;  // バンプマッピングあり(少し明るく)
			Color.a = texColor.a - (1 - g_Diffuse.a);

		}
		else {
			// 法線マップがセットされていない場合は、トゥーンレンダリング
			// 通常のテクスチャ色（texColor）にトゥーンペイント（ToonUVのサンプラ）を掛け合わせる
			Color.rgb = texColor.rgb * 1.1f * texToon.rgb * g_Diffuse.rgb;    // (少し明るく)
			Color.a = texColor.a - (1 - g_Diffuse.a);
		}
	}
	return saturate(Color);  // 最終出力値を0～１の範囲に制限する
}

