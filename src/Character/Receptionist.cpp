#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/Transform.h"

#include "src/Model/Model.h"
#include "src/Animation/Animation.h"

#include "src/Collision/CapsuleCollision.h"
#include "src/Collision/BoxCollision.h"

#include "src/Character/Receptionist.h"


//---------------------------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------------------------
Receptionist::Receptionist()
{
}

//---------------------------------------------------------------------------
// デストラクタ
//---------------------------------------------------------------------------
Receptionist::~Receptionist()
{
}

//---------------------------------------------------------------------------
// 初期処理
//---------------------------------------------------------------------------
void Receptionist::Init()
{
	// 座標の初期座標
	m_transform.pos.set(-20, 0,120);
	// スケールの設定
	m_transform.scale.set(0.1f, 0.1f, 0.1f);
	// 向きの設定
	m_transform.rot.set(0.0f, 0.0f, 0.0f);



	// モデルの読み込み
	m_model.LoadModel("Data/Model/Receptionist/receptionist.mv1");
	// キャラクターのマテリアル調整
    // モデルのマテリアルを確保
    // マテリアルのアンビエントをあげるとプレイヤーにかかっている影が明るくなる
    m_model.SetMaterialAmbient({ 1.0f,1.0f,1.0f,0.0f });

	// アニメーションの初期設定
	// アニメーションの数を設定しておく
	m_animation.InitAnimation(anim_max, idle);

	// アニメーションの読み込み
	// とりあえずの状態を設定しておく
	// 後で変更予定
	m_animation.LoadAnimation("Data/Model/Receptionist/Animation/idle.mv1", idle,0,1.0f);
	// 最初はアイドルアニメーションをつけておく
	m_animation.InitAttachAnimation(&m_model, 0, true);

}

//---------------------------------------------------------------------------
// 更新処理
//---------------------------------------------------------------------------
void Receptionist::Update()
{
	// アニメーションの再生
	m_animation.PlayAnimation(&m_model, true);


	// Bodyの当たり判定の設定
	m_hit_body.CreateNodoCapsule(&m_model, 69, 5, BDOY_R);
}

//---------------------------------------------------------------------------
// 描画処理
//---------------------------------------------------------------------------
void Receptionist::Draw()
{
	// モデルの描画
	m_model.DrawModel(&m_transform);

	// ボディーの当たり判定の描画
	//m_hit_body.Draw();
}

//---------------------------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------------------------
void Receptionist::Exit()
{
}
