
//---------------------------------------------------------------------------------
//	頂点シェーダー
//---------------------------------------------------------------------------------

//	頂点シェーダーで処理するために受け取るもの
struct VS_INPUT
{
    float4 LPos : POSITION; //	ローカル座標
    float3 LNormal : NORMAL; //	ローカル法線
    float4 Color : COLOR; //	色
};

//	頂点シェーダーの関数で計算した結果を入れるもの
struct VS_OUTPUT
{
    float4 SPos : POSITION; //	画面座標
    float4 Color : COLOR; //	色
      
    // 今回はピクセルシェーダーでワールド座標を使いたい
    // なので、渡す物の中にワールド座標を入れて用にします
    float WPos : TEXCOORD1;  // ワールド座標
};

//	ワールド変換行列
float4x4 World : register(c94);
//	ビュー変換行列
float4x4 View : register(c6);
//	プロジェクション変換行列
float4x4 Projection : register(c2);

//---------------------------------------------------------------------------------
//	各頂点の計算
//---------------------------------------------------------------------------------
VS_OUTPUT main(VS_INPUT In)
{
	//	ライトの方向
    float3 LightDir = { 0.5f, -2.0f, 1.0f };
	//	環境光
    float4 Ambient = { 0.3f, 0.3f, 0.3f, 1.0f };

    VS_OUTPUT Out = (VS_OUTPUT) 0;

	//	ローカル座標をワールド座標に変換（ワールド変換）
    float4 WPos = mul(In.LPos, World);
	//	ワールド座標をカメラ座標に変換（ビュー変換）
    float4 CPos = mul(WPos, View);
	//	カメラ座標を画面座標に変換（プロジェクション変換）
    Out.SPos = mul(CPos, Projection);

	//	ライトの向きの逆方向のベクトル（正規化）
    float3 L = normalize(-LightDir);
	//	ローカル法線をワールド法線に
    float3 N = normalize(mul(In.LNormal, (float3x3) World));
	//	２つのベクトル（ N, L ）から明るさ計算
    Out.Color = In.Color * max(Ambient, dot(N, L));

	// 色をくっきり分けるために頂点シェーダーではなくピクセルシェーダーで色を変える

// ワールド座標のXの値を見てマイナスの部分だけ色を変えてみます
//if(WPos.x < 0.0f)
//{
   // 色を変えてみます
//   Out.Color.g = 1.0f;
//}
    
	// ピクセルシェーダーにわたる用のワールド座標
    Out.WPos = (float3)WPos;
    
	//	計算した結果の座標と色を入れたものを返します
    return Out;
}
