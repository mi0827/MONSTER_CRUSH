#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/Collision/BoxCollision.h"
#include "src/System/Transform.h"
#include "TargetMove.h"

TargetMove::TargetMove()
{
}

TargetMove::~TargetMove()
{
	// ポインタの解放
	//delete m_transform;
	//delete m_target;
}

//---------------------------------------------------------------------------
// 情報をセットする用の関数
//---------------------------------------------------------------------------
void TargetMove::SetInfo(Transform* transform, const float hit_r, const float MOV_SPEED, const float ROT_SPEED)
{
	// 自身の情報をセット
	m_info.m_transform = transform;
	// 移動制限の半径の設定
	m_info.m_hit_r = hit_r;
	// 移動スペードの設定
	m_info.M_MOV_SPEED = MOV_SPEED;
	// 回転スピードの設定
	m_info.M_ROT_SPEED = ROT_SPEED;
}

//---------------------------------------------------------------------------
// ターゲットの情報をセットする用の関数
//---------------------------------------------------------------------------
void TargetMove::SetTargetInfo(Transform* target_pos, const float target_hit_r)
{
	// ターゲットの座標の設定
	m_target_info.m_target = target_pos;
	// ターゲットの半径のせ設定
	m_target_info.m_target_hit_r = target_hit_r;
}

//---------------------------------------------------------------------------
// 移動に関する更新処理用関数
// Vector2 にｚ座標がないのでそこはｚ＝ｙとして使用している
//---------------------------------------------------------------------------
void TargetMove::Update(bool* run_flag)
{
	// 本体の向きの設定
	Set_Direction();
	// ターゲットと一定の距離に入ったかを受け取る
	m_hit = Target_Hit();
	// 範囲に入っていないとき
	if (m_hit)
	{
		// 向いている方向に移動
		m_info.m_transform->pos.x += m_info.M_MOV_SPEED * sinf(TO_RADIAN(m_info.m_transform->rot.y));
		m_info.m_transform->pos.z += m_info.M_MOV_SPEED * cosf(TO_RADIAN(m_info.m_transform->rot.y));
		// 移動中なので
		*run_flag = true;
	}
}

//---------------------------------------------------------------------------
// 本体の向きの設定
//---------------------------------------------------------------------------
void TargetMove::Set_Direction()
{
	// ラインのスタート位置を本体の位置に設定する
	m_line_start.set(m_info.m_transform->pos);

	// 基準のベクトル作成
	// ｚの値をいじると振り向きのがたがたがましになる
	Vector3 base(0.0f, 0.0f, 50.0f);
	// 線の本体の向きに合わせたいので回転行列を作成
	MATRIX mat = MGetRotY(TO_RADIAN(m_info.m_transform->rot.y));
	// 上で作成した基準のベクトルを行列で変換
	Vector3 change = GetVector3VTransform(base, mat);
	// ゴール座標は開始座標から変更したベクトル文先のとこ
	m_line_goal = m_line_start + change;

	// スタート座標とゴール座標が確定したので開始座標かそれぞれのベクトルを作成
	// 線の開始座標からゴール座標へのベクトル
	Vector3 line_dir = m_line_goal - m_line_start;
	line_dir.y = 0;
	// 線の開始座標からプレイヤー座標へのベクトル
	Vector3 target_dir = m_target_info.m_target->pos - m_line_start;
	target_dir.y = 0;
	// 外積を使った判断をしたいので上で作った２つのベクトルの外積を求めます
	m_cross = GetVector3Cross(line_dir, target_dir);

	// 上で作成したラインを見えるようにする
	{
		// そのままの座標だと線が地面に埋まってしまうのですこしあげています
		Vector3 start = m_line_start + Vector3(0.0f, 0.1f, 0.0f);
		Vector3 goal = m_line_goal + Vector3(0.0f, 0.1f, 0.0f);
		// 開始座標とゴール座標を結んで線の描画
		//DrawLine3D(start.VGet(), goal.VGet(), GetColor(255, 255, 0));

		// 開始座標の場所とくろいたま
		//DrawSphere3D(start.VGet(), 0.3f, 100, GetColor(0, 0, 0), GetColor(0, 0, 0), TRUE);
		// ゴール座標の黄色い玉
		//DrawSphere3D(goal.VGet(), 0.3f, 100, GetColor(255, 255, 0), GetColor(255, 255, 0), TRUE);
	}
}

//---------------------------------------------------------------------------
// ターゲットと一定の距離に入ったかを返す関数
//---------------------------------------------------------------------------
bool TargetMove::Target_Hit()
{
	Set_Direction();
	// それぞれの更新処理が終わったのでプレイヤーとNPCの位置関係から一定距離近づかないようにします
	// 本体とターゲットの距離を求める
	float distance = Get_Target_Distance();
	// 基準の距離を求める（それぞれの半径）
	float radius = m_info.m_hit_r + m_target_info.m_target_hit_r;
	// 回転していいときだけ回転する
	if (m_can_rot)
	{
		// 一定の範囲に入ったら振り向きをやめる
		if (m_cross.y > RANGE) {
			// 外積のＹの値がプラスの時はプレイヤーは線の右にいます
			m_info.m_transform->rot.y += m_info.M_ROT_SPEED;
		}
		else
			if (m_cross.y < -RANGE)
			{
				// 外積のＹの値がマイナスの時はプレイヤーは線の左にいます	
				m_info.m_transform->rot.y -= m_info.M_ROT_SPEED;
			}
			else
			{
				// 設定された値より近づいたら
				if (distance < radius) {
					// モンスターの向きも正しかったら
					// 移動に制限をかける
					return false;
				}
			}
	}
	// まだ範囲外の時は移動ができるようにしておく
	return true;
}

//---------------------------------------------------------------------------
// ターゲットとの距離を返す関数
//---------------------------------------------------------------------------
float TargetMove::Get_Target_Distance()
{
	// 本体とターゲットの距離を求める
	float distance = GetVector3Distance(m_info.m_transform->pos, m_target_info.m_target->pos);
	return distance;
}

//---------------------------------------------------------------------------
// 本体の向きを回転していいかのフラグ立てようの関数
//---------------------------------------------------------------------------
void TargetMove::Set_Can_Rotate(bool can)
{
	m_can_rot = can;
}
