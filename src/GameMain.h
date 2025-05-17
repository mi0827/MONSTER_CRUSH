#pragma once
//! 初期処理
void GameInit();

//! 更新処理
void GameUpdate();

//! 描画処理
void GameDraw();

//! 終了処理
void GameExit();

//! @brief BGMを変更する関数
//! @parma BGMが保存されている番号 
void ChangeBgm(int bgm_num);

