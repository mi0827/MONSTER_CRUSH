#pragma once
//! @file SE.h
//! @brief 効果音用のクラス
class SE
{
public:
	// デストラクタ
	~SE();

	// SE分の配列を作る
	// 引数
	// １：SEの数
	void SEContainerNew(int size); 

	// SEの読み込み
	// 引数
	// １：再生するデータの情報
	// ２：配列の何番目に読み込むか
	void LoadSE(const char name[256], int No);
	
	// SEの再生
	// 引数
	// １：配列の何番目のSEを使うか
	// ２：どのタイプの再生をするか
	// 再生形式
    // DX_PLAYTYPE_NORMAL　:　ノーマル再生
    // DX_PLAYTYPE_BACK　　 : バックグラウンド再生
    // DX_PLAYTYPE_LOOP　　 : ループ再生
	// ３：再生後再生位置をデータの先頭に移動するかどうか
	void PlaySE(int No, int type, bool loop);

	// SEのボリューム調整用の関数
	// 引数
	// １：ボリュームの大きさ
	// ２：SEの数
	void SEChangeVolume(int se_volume/*,int size*/);

	// SEが再生中かを返す関数
	// 引数
	// １：配列の何番目のSEを調べるか
	// 再生中か調べる(帰ってくる物)
	// 再生中          ：  １：true
	// 再生されていない：  ０：false
	// エラー          ：−１
	bool PlayingSE(int No);

	// SEのデリート（配列の解放）
	void SEDelete();
	
public:
	// SEの入れ物用の配列
	int* m_se_container; 
	// SEの数を保存する
	int m_se_size;
};