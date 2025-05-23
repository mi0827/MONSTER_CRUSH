#pragma once

//! @file Camera.h
//! @brief カメラクラス
class Camera
{
public:
	//---------------
	// 関数の定義
	//---------------
	// コンストラクタ
	Camera();

	void Init();        //　初期処理

	void PlayFieldInit(); // プレイ画面の初期設定



	// プレイヤーの情報を受け取る形にします
	// 今回はプレイヤーのｎ後ろにカメラをお嫌いの出
	// プレイヤーの座標と向きを受け取れるようにします
	//! @brief 更新処理
	//! @param 移したい目標の座標
	void MouseCamera(Vector3* target_pos);

	//! @brief ターゲットカメラの更新処理
	//! @param カメラの手毎側にいるターゲットの座標
	//! @param カメラの奥側にいるターゲットの座標
	void TargetCamera(Vector3* target_pos1, Vector3* target_pos2);
	bool target_camera_on = false;


	//! @brief どちらのカメラを使用するかの変更をか用の関数
	//! @param どちらのカメラを使うか true : マウスカメラ、false : ターゲットカメラ
	//! @param カメラの手毎側にいるターゲットの座標
	//! @param カメラの奥側にいるターゲットの座標
	void UseCameraUpdate(bool change_camera, Vector3* target_pos1, Vector3* target_pos2);

	//! @brief カメラが壁に埋まらないようにする
	void HitObject(Vector3* obj_pos, Vector3* obj_size);
	//! @brief 描画処理の前にカメラ、画面のセット
	void DrawSet();
	//! @brief 描画処理
	void Draw();
	//! @brief 終了処理
	void Exit();

	//! @brief カメラ移動の際のマウスの感度を設定
	void SetCameraSensi(float mouse_sensi);

	//! @brief カメラの高さ距離などを設定する関数
	//! @param カメラの見る座標の高さの(デフォルトでプレイヤー時の値を入れておく)
	//! @param カメラから目標までの距離(デフォルトでプレイヤー時の値を入れておく)
	void SetCamera(float height = CAMERA_HEIGHT_PLAYER, float length = CAMERA_LENGTH_MAX);

	//! @brief 移したい目標をまわるようにカメラの移動処理
	//! @param 移したいターゲット
	//! @param 振り向きたい向き(1 : 右, 2 : 左)
	//! @param 移動させたい向き
	void MoveCamera(Vector3* target_pos, int direction, float speed);

	//! @brief カメラシェイクを行う関数
	//! @param カメラの振動するパワー
	//! @param どれくらいの時間揺れて欲しいか
	void CameraShakeLimited(float power, float time);

	//! @brief カメラシェイクを強制的に終了させる関数
	void CameraShakeReset();

	//! @brief カメラの向きの設定
	//! @param 設定したい向き
	void SetCameraRot(Vector3 rot);

	//! @brief カメラの向き取得する関数
	Vector3 GetCameraRot();

	//! @brief カメラとターゲットとの距離をマウスホイールで変更できるようにする
	void ChangeDistance();

	enum Direction
	{
		right, // 右
		left,   // 左
	};

	//! カメラと移す標的との距離
	static constexpr float CAMERA_LENGTH_MAX = 75.0f;      // プレイヤーとの最大値
	static constexpr float CAMERA_LENGTH_MIN = 50.0f;       // プレイヤーとの最小値
	static constexpr float CAMERA_MONSER_LENGTH = 100.0f;  // モンスター
	//! カメラが移すしたいものからy座標分変化させる値
	static constexpr float CAMERA_HEIGHT_PLAYER = 5.0f;    // プレイヤー
	static constexpr float CAMERA_HEIGHT_MONSTER = 20.0f;  // モンスター
private:

	//! カメラの回転スピード
	static constexpr float MOUSE_SENSI_CORRECT = 0.01f;  // マウス用
	static constexpr float PAD_CAMERA_ROT_SPEED = 3.0f;    // パッド用

	static constexpr float UP_ANGLE_MAX = 30.0f;           // カメラの上アングルの最大
	static constexpr float LOWER_ANGLE = 10.0f;             // カメラの下アングルの最低（地面に埋まらない程度）
	static constexpr float BOX_SIZE = 4.0f;                     // ボックスのサイズ
	static constexpr float BOX_SIZE_HALF = (BOX_SIZE / 2.0f); // 半数のサイズ

	//! ターゲットカメラの振り向き速度
	static constexpr float TARGET_ROT_SPEED = 2.3f;
	//! ターゲットカメラの振り向き可能範囲
	static constexpr float RANGE = 500;
	//! カメラの画角のデフォルト値
	static constexpr float CAMERA_ANGLE_DEFAULT = 55.0f;

	//---------------
	// 変数の定義
	//---------------
public:
	//! 移したいターゲットとの距離
	float m_length = 0;

	//! カメラが見る座標
	Vector3 m_look;
	Vector3 m_look_2{ 0.0f,0.0f,0.0f };
	float m_look_height = 0; // カメラが見る座標のY座標のプラス分用の変数


	//! カメラの座標
	Vector3 m_pos;
public:
	//! カメラの向き
	Vector3 m_rot;

	//! マウスカーソル用変数
	float m_mouse_wheel = 0;
private:


	//! マウスの移動量用の変数
	float m_mouse_move_x = 0.0f; //! X座標の移動量
	float m_mouse_move_y = 0.0f; //! Y座標の移動量
	Vector3 m_before_pos = { 0.0f,0.0f,0.0f };   //! 毎フレーム移動前の座標をいれる用の変数
	Vector3 m_hit_box_size = { 0.0f,0.0f,0.0f }; //! 壁との当たり判定用の変数

	//! マウスの感度
	float m_camera_mouse_sensi = 0;


	//！カメラシェイクでの振動の強さ
	float m_power = 0;
	//! カメラシェイクでどれだけの座標をずらすか
	Vector3 m_shake_pos;
	// どれだけ揺れていてほしいのか
	float m_shake_time = 0;
	// 変化した画角の値を保存するためのもの
	float m_change_angle_value = 0;
	// カメラシェイクの時間カウント変数
	int m_shake_tiam_count = 0;

public:
	int pad_no = 0;				//! 何番のパッドを使っているのか
	void SetPadNo(int no)
	{
		pad_no = no;
	}

	// 下の二つの変数はあとで消す予定
	// ターゲットカメラに使用する内積保存用変数
	float inner_product;
	// ターゲットカメラに使用する外積保存用の変数
	Vector3 cross_product;
};
