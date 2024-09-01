#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/Model/Model.h"
#include "HitField.h"


void SetHitGround(Model* ground_model)
{
	// あたり情報の作成
	MV1SetupCollInfo(ground_model->m_model);
}

void HitGroundCharacter(Vector3* character_pos, Model* ground_model)
{
	// 当たり情報の更新
	MV1RefreshCollInfo(ground_model->m_model);

	// 線の開始座標(キャラクターの上のほうの座標)
	Vector3 line_start = *character_pos + Vector3(0.0f, 100.0f, 0.0f);
	// 線のゴール座標(プレイヤーの下のほうの座標)
	Vector3 line_goal = *character_pos + Vector3(0.0f, -100.0f, 0.0f);
	// 地面モデルと線の当たり判定
	MV1_COLL_RESULT_POLY character_hit = MV1CollCheck_Line(ground_model->m_model, -1, line_start.VGet(), line_goal.VGet());

	// 当たっていたらの判定
	if (character_hit.HitFlag)
	{
		// キャラクタ―座標をその高さに合わせる
		character_pos->y = character_hit.HitPosition.y;
	}
}
