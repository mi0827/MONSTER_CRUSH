#pragma once




//! 初期処理
void GameInit();

//! 更新処理
void GameUpdate();

//! 描画処理
void GameDraw();

//! 終了処理
void GameExit();




//
////! ステータスバー描画用の関数
//void StatusDraw();
//
////! 攻撃に関する更新処理
//void AttackUpdate();

////! 頂点シェーダー用の変数
//int vertex_shader;
//
////! ピクセルシェーダー用の変数
//int pixel_shader;



// シーン切り替え用の関数
// 引数
// １：今のシーン
// ２：次のシーン
void Scene_Change_Judge(int& now_scene, const int& next_scene);
