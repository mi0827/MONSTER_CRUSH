#pragma once

// BGM関係のクラス
class BGM
{
public:
	// デストラクタ
	~BGM();

	// BGM分の配列を作る
	// 引数
	// １：BGMの数
	void BGMContainerNew(int size);

	// BGMの読み込み
	// 引数
	// １：再生するデータの情報
	void LoadBGM(const char name[256], int No);

	// BGMの再生
	// 引数
	// １：どのタイプで再生をするか
	// 再生形式
	// DX_PLAYTYPE_NORMAL　:　ノーマル再生
	// DX_PLAYTYPE_BACK　　 : バックグラウンド再生
	// DX_PLAYTYPE_LOOP　　 : ループ再生
	// ２：再生後再生位置をデータの先頭に移動するかどうか
	// ３：何番目のBGMを再生するか
	void PlayBGM( int type, bool loop, int No);

	// BGMのボリューム調整用の関数
	// 引数
	// １：ボリュームの大きさ
	// ２：BGMの数
	void BGMChangeVolume(int bgm_volume/*, int size*/);

	// BGMを止める
	// 引数
	// １：どのBGMか
	void StopBGM(int No);

	// BGNが再生中かを返す関数
	// 再生中か調べる(帰ってくる物)
	// 再生中          ：  １：true
	// 再生されていない：  ０：false
	// エラー          ：－１
	bool PlayingBGM(int No);

	// BGMのデリート（配列の解放）
	void BGMDelete();
public:
	//! BGMの入れ物用変数
	int* m_bgm_container = 0; 

	//! BGM数を保存
	int m_bgm_size;
};