#pragma once

//---------------------------------------------------------------------------------
//	カメラクラス
// Baseクラスを継承
class Camera
{
public:
	//---------------
	// 関数の定義
	//---------------
	// コンストラクタ
	Camera();

	void Init();        //　初期処理

	void PlayField_Init(); // プレイ画面の初期設定

	// プレイヤーの情報を受け取る形にします
	// 今回はプレイヤーのｎ後ろにカメラをお嫌いの出
	// プレイヤーの座標と向きを受け取れるようにします
	void Update(Vector3* player_pos); // 更新処理
	void Hit_Object(Vector3* obj_pos, Vector3* obj_size); // カメラが壁に埋まらないようにする
	void Draw_Set();                  // 描画処理の前にカメラ、画面のセット
	void Draw();		              // 画処理
	void Exit();		              // 終了処理

	//---------------
	// 変数の定義
	//---------------
public:
	// カメラが見る座標
	Vector3 m_look;
	// カメラの座標
	Vector3 m_pos;
	// カメラの向き
	Vector3 m_rot;
public:
	//--------------------
	// 画面分割用の変数
	int m_screen_field = 0;   // 描画する画面を入れる画像用変数
	Vector2 m_field_pos = { 0.0f, 0.0f };  // 描画する座標
	Vector2 m_field_size = { 0.0f, 0.0f, }; // 描画するサイズ

private:
	// マウスの移動量用の変数
	float m_mouse_move_x = 0.0f; // X座標の移動量
	float m_mouse_move_y = 0.0f; // Y座標の移動量
	Vector3 m_before_pos = { 0.0f,0.0f,0.0f };   // 毎フレーム移動前の座標をいれる用の変数
	Vector3 m_hit_box_size = { 0.0f,0.0f,0.0f }; // 壁との当たり判定用の変数
public:
	int pad_no = 0;				// 何番のパッドを使っているのか
	void SetPadNo(int no)
	{
		pad_no = no;
	}


	
};
