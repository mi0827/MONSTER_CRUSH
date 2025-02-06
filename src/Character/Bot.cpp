#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/Transform.h"

#include "src/Model/Model.h"

#include "src/Collision/CapsuleCollision.h"
#include "src/Collision/BoxCollision.h"


#include "src/Effect/Effect.h"
#include "src/Sound/Sound.h"

#include "src/Model/Model.h"

#include "Bot.h"


//------------------------------------------
// コンストラクタ
//------------------------------------------
Bot::Bot()
{
}

//------------------------------------------
// デストラクタ
//------------------------------------------
Bot::~Bot()
{
}

//------------------------------------------
// 初期化処理
//------------------------------------------
void Bot::Init()
{
	// 座標の設定
	m_transform.pos.set(0.0f, 80.0f, 500.0f);
	// 向きの設定
	m_transform.rot.set(0.0f, 0.0f, 0.0f);
	// 大きさの設定
	m_transform.scale.set(0.3f, 0.2f, 0.3f);
	// モデルの読み込み
	m_bot_model.LoadModel("Data/Model/Bot/bot.mv1");
	// 座標の設定
	m_bot_model.SetModelPos(m_transform.pos);
	// エフェクトの初期化
	m_bot_effect.NewArraySecure(EFFECT_MAX);
	// エフェクトの読み込み
	m_bot_effect.LoadEffect("Data/Model/Bot/Effect/HitDamage.efkefc", 0, 1.0f);
	

	// 当たり判定の初期化
	Vector3 pos2 = { m_transform.pos.x,m_transform.pos.y + 10,m_transform.pos.z };
	m_body.CreateCapsuleCoordinatePos(m_transform.pos, pos2, 15.0f);

	// SEの初期化
	m_se.NewArraySecureSound(SE_MAX);
	m_se.LoadSound("Data/Model/Bot/SE/Damage.mp3",0);
}

//------------------------------------------
// 更新処理
//------------------------------------------
void Bot::Update()
{
	// 攻撃を受けた時
	// エフェクトが再生中なら
	if (m_bot_effect.IsPlayingEffect())
	{
		// 再生中のエフェクトを止める
		m_bot_effect.StopEffect();
	}
	// エフェクトの再生
	Vector3 effect_pos = { m_transform.pos.x,m_transform.pos.y + 10.0f ,m_transform.pos.z };
	m_bot_effect.PlayEffect(0, effect_pos);
	// エフェクトサイズの変更
	m_bot_effect.SetEffectSize({ 4.0f,4.0f,4.0f });
	// SEが再生中なら止める
    m_se.StopSound();
	// SEの再生
	if (m_se.PlayingSound() == false )
	{
		// SEの再生
		m_se.PlaySound_(0, DX_PLAYTYPE_BACK, true);
		// SEが再生されたので再生可能状態を変更する
		m_se.m_playing_flag = false;
	}
}

//------------------------------------------
// 描画処理
//------------------------------------------
void Bot::Draw()
{
	// ボットモデルの描画
	m_bot_model.DrawModel(&m_transform);
	// 当たり判定の描画
	// m_body.Draw();
}

//------------------------------------------
// 終了処理
//------------------------------------------
void Bot::Exit()
{
}
