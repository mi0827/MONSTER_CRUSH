#pragma once

//! @file Bot.h
//! @brief botクラス
class Bot
{
public:
	//! @brief コンストラクタ
	Bot();
	//! @brief デストラクタ
	~Bot();
	
	//! @brief 初期化処理
	void Init();

	//! @brief 更新処理
	void Update();

	//! @brief 描画処理
	void Draw();

	//! @brief 終了処理
	void Exit();


public:
	//! ボットの当たり判定
	CapsuleCollision m_body;

private:
	/*このシーンで別で木のモデルを読み込むみ
	このモデルにモンスターとおんなじような当たり判定をとり
	攻撃が当たったら当たったエフェクトを出す*/

	//! Transform
	Transform m_transform;
	//! ボットモデル
	Model m_bot_model;
	//! ボットのエフェクト
	Effect m_bot_effect;
	//! エフェクトの数
	static constexpr int EFFECT_MAX = 1;
	
	//! SE
	Sound m_se;
	//! SEの数
	static constexpr int SE_MAX = 1;

};

