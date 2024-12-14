#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"

#include "src/System/Text.h"

#include "Scene_Base.h"

Scene_Base::Scene_Base()
{
}

Scene_Base::~Scene_Base()
{
}

// --------------------------------------------------------------------------
// どのキャラクターを使うかの設定
//---------------------------------------------------------------------------
void Scene_Base::SetCharacter(int player_num, int monster_num)
{
	// キャラクター番号の保存
	m_player_num = player_num;
	m_monster_num = monster_num;
}

// --------------------------------------------------------------------------
// 全シーンで使う物の初期化
//---------------------------------------------------------------------------
void Scene_Base::BaseInit()
{
	m_text.LoadText("Data/Text/Option.txt", TEXT_MAX);
}

// --------------------------------------------------------------------------
// 全シーンで使うものの描画
//---------------------------------------------------------------------------
void Scene_Base::BaseDraw(int scene_num, Vector2 draw_pos_)
{
	// 現在のフォントサイズをゲット
	int default_size = GetFontSize();
	// あたらたなフォントサイズをせてい
	SetFontSize(40);
	float h = GetFontSize();
	// 描画する座標を設定
	Vector2 draw_pos = { draw_pos_.x,draw_pos_.y };
	// テキストの描画
	m_text.TextDraw(0, draw_pos, m_text.OPTION_BACK_SIZE);
	// 指定のシーンでだけ描画内容を増やす
	if (scene_num == Battle)
	{
		draw_pos = { draw_pos_.x,draw_pos_.y + h };
		m_text.TextDraw(1, draw_pos, m_text.OPTION_BACK_SIZE);
	}
	// フォントサイズをリセット
	SetFontSize(default_size);
}

// --------------------------------------------------------------------------
// シャドーマップの初期処理
// --------------------------------------------------------------------------
void Scene_Base::ShadowMapInit()
{
	// シャドーマップの作成
	m_shadowMap_handle_1 = MakeShadowMap(4096, 4096);
	m_shadowMap_handle_2 = MakeShadowMap(4096, 4096);
	m_shadowMap_handle = MakeShadowMap(4096, 4096);
	// ライトの角度の初期化
	m_light_angle = 10.0f;

	// ライトの方向ベクトルの算出
	m_light_direction.x = sin(m_light_angle);
	m_light_direction.z = cos(m_light_angle);
	m_light_direction.y = -1.0f;
	VectorNormalize(&m_light_direction, &m_light_direction);

	// ライト方向を設定
	SetLightDirection(m_light_direction);

	// シャドーマップが想定するライトの方向のセット
	SetShadowMapLightDirection(m_shadowMap_handle_1, m_light_direction);
	SetShadowMapLightDirection(m_shadowMap_handle_2, m_light_direction);
	SetShadowMapLightDirection(m_shadowMap_handle, m_light_direction);
	// シャドーマップに描画する範囲の設定
	SetShadowMapDrawArea(m_shadowMap_handle, VGet(-8192.0f, -0.1f, -8192.0f), VGet(+8192.0f, 3000.0f, +8192.0f));

}

// --------------------------------------------------------------------------
// プレイヤーのシャドーマップの範囲設定
// --------------------------------------------------------------------------
void Scene_Base::SetShadowMapArea(int shadowMap_handle, Vector3 player_pos)
{
	// シャドーマップに描画する範囲の設定
	// 今はこの　範囲でプレイヤーのシャドウマップを設定しているが
	// 背後の影が気に入らなければ二つ目のY座標の値を上げるか全体的に描画範囲を広げろ
	SetShadowMapDrawArea(shadowMap_handle,
		VGet(player_pos.x - 50.0f, -0.1f, player_pos.z - 50.0f),
		VGet(player_pos.x + 50.0f, +100.0f, player_pos.z + 50.0f));
}

// --------------------------------------------------------------------------
// 描画に使用するシャドウマップの設定を解除
// --------------------------------------------------------------------------
void Scene_Base::UseShadowMapSet()
{
	SetUseShadowMap(1, -1);
	SetUseShadowMap(0, -1);
}

// --------------------------------------------------------------------------
// シャドーマップの削除
// --------------------------------------------------------------------------
void Scene_Base::ExitShadowMap()
{
	DeleteShadowMap(m_shadowMap_handle_1);
	DeleteShadowMap(m_shadowMap_handle_2);
	DeleteShadowMap(m_shadowMap_handle);
}

// --------------------------------------------------------------------------
// 文字列をスクリーン座標に描画するための値を返す関数
// --------------------------------------------------------------------------
VECTOR Scene_Base::DrawStringWrold(Vector3 pos, Vector3 shift_pos)
{
	// 設定された座標に文字列を描画するための準備
	// ３D空間座標を２Dの画面座標に変換
	VECTOR wpos = pos.VGet();

	// ずらしたい分ずらす
	wpos.x += shift_pos.x;
	wpos.y += shift_pos.y;


	// この座標（wpos）を関数に渡してスクリーン座標（２D画面座標 : pos2d）を取得
	VECTOR pos2d = ConvWorldPosToScreenPos(wpos);

	return pos2d;
}

// --------------------------------------------------------------------------
// 次に行いたいシーンをセットする関数
// --------------------------------------------------------------------------
void Scene_Base::SetNextScene(int next_scene)
{
	// 次に行いたいシーンの保存
	m_next_scene = next_scene;
	// 次に行いたいシーンを現在のシーンに入れ替える
	m_now_scene = next_scene;
}

// --------------------------------------------------------------------------
// フェードアウトするための処理
// --------------------------------------------------------------------------
void Scene_Base::FadeOutUpdate()
{

	// フレームカウントが指定の値以上になったら
	if (m_frame_count >= FLAME_MAX)
	{
		// カウントをリセット
		m_frame_count = 0;
		// フェードアウトの次はフェードインのターンになる
		m_turn = Main;
	}

	// フレームカウントをf増やす
	m_frame_count++;

	// 一秒でどれだけの値変化するかの割合を出す
	m_fade_ratio = 255 / FLAME_MAX;
	// 減る値の量を出す
	m_fade_value = m_fade_ratio * m_frame_count;
}


// --------------------------------------------------------------------------
// フェードアウトした後にシーンの切り替えをするための処理
// --------------------------------------------------------------------------
void Scene_Base::FadeOutSceneChange(int next_scene)
{
	// フェードアウト関数の中でカウントされたカウントか
	// 指定の値以上になったらシーンの変える
	if (m_frame_count >= FLAME_MAX)
	{
		// 次に行いたいシーンの設定
		SetNextScene(next_scene);
		// シーン変更フラグを立てる
		m_scene_change_judge = true;
	}
	// フェードアウト処理
	FadeOutUpdate();
}

// --------------------------------------------------------------------------
// フェードインするための処理
// --------------------------------------------------------------------------
void Scene_Base::FadeInUpdate()
{
	// フレームカウントが指定の値以上になったら
	if (m_frame_count >= FLAME_MAX)
	{
		// カウントをリセット
		m_frame_count = 0;
		// フェードインの後はメインのターンになる
		m_turn = Main;
	}

	// フレームカウントをf増やす
	m_frame_count++;

	// 一秒でどれだけの値変化するかの割合を出す
	m_fade_ratio = 255 / FLAME_MAX;
	// 減る値の量を出す
	m_fade_value = m_fade_ratio * m_frame_count;
}

// --------------------------------------------------------------------------
// フェードの描画処理
// --------------------------------------------------------------------------
void Scene_Base::FadeDraw()
{
	// フェードアウトはうまくいっているがフェードインがうまくいっていないのちに時間があれば再挑戦してみる
	switch (m_turn)
	{
		//case FadeIn:
		// 透明度の変更
		//SetDrawBlendMode(DX_BLENDMODE_ALPHA,  255 - m_fade_value);
		//// 黒い壁のの描画
		//DrawBox(0, 0, SCREEN_W, SCREEN_H, 0, TRUE);
		//// 暗さの変更
		////SetDrawBright( m_fade_value,  m_fade_value,  m_fade_value);
		//break;
	case Main:
		// 透明度の変更
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		// 暗さの変更
		SetDrawBright(255, 255, 255);
		break;

	case FadeOut:
		// 透明度の変更
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fade_value);
		// 黒い壁のの描画
		DrawBox(0, 0, SCREEN_W, SCREEN_H, 0, TRUE);
		// 暗さの変更
		SetDrawBright(255 - m_fade_value, 255 - m_fade_value, 255 - m_fade_value);
		break;
	}

}
