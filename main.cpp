#include <Novice.h>
#include "Vector3.h"

const char kWindowTitle[] = "LD2A_カナイ_ミント";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 v1{ 1.0f,3.0f,-5.0f };
	Vector3 v2{ 4.0f,-1.0f,2.0f };
	float k = { 4.0f };
	const int kRowHeight = 20;


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		Vector3 resultAdd = Vector3::Add(v1, v2);
		Vector3 resultSubtract = Vector3::Subtract(v1, v2);
		Vector3 resultMultipry = Vector3::Multiply(k, v1);
		float resultDot = Vector3::Dot(v1, v2);
		float resultLength = Vector3::Length(v1);
		Vector3 resultNormarize = Vector3::Normalize(v2);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		VecScreenPrintf(0, 0, resultAdd, " : Add");
		VecScreenPrintf(0, kRowHeight, resultSubtract, " : Subtract");
		VecScreenPrintf(0, kRowHeight*2, resultMultipry, " : Multipry");
		Novice::ScreenPrintf(0, kRowHeight * 3, "%0.2f : Dot", resultDot);
		Novice::ScreenPrintf(0, kRowHeight * 4, "%0.2f : Length", resultLength);
		VecScreenPrintf(0, kRowHeight * 5, resultNormarize, " : Normalize");



		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
