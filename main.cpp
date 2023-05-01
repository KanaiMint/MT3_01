#include <Novice.h>
#include "Vector3.h"
#include "Matrix4x4.h"

const char kWindowTitle[] = "LD2A_カナイ_ミント";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 rotate{ 0.0f,0.0f,0.0f };
	Vector3 translate{ 0.0f,0.0f,0.0f };
	Vector3 cameraPosition{ 0.0f,0.0f,-10.0f };

	Vector3 v1{ 1.2f,-3.9f,2.5f };
	Vector3 v2{ 2.8f,0.4f,-1.3f };


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
		Vector3 CameraVecZ = Transform(cameraPosition,MakeRotateZMatrix(rotate.z));

		if (Novice::CheckHitKey(DIK_W)) {
			translate.z += 0.1f;
		}if (Novice::CheckHitKey(DIK_S)) {
			translate.z -= 0.1f;
		}if (Novice::CheckHitKey(DIK_D)) {
			translate.x += 0.1f;
		}if (Novice::CheckHitKey(DIK_A)) {
			translate.x -= 0.1f;
		}
		rotate.y += 0.05f;

		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, cameraPosition);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(1280), float(720), 0.0f, 1.0f);
		Vector3 screenVerties[3];
		Vector3 kLocalVertices[3];
		kLocalVertices[0] = { -0.5f,-0.5f,0.0f };
		kLocalVertices[1] = { 0.5f,-0.5f,0.0f };
		kLocalVertices[2] = { 0.0f,0.5f,0.0f };
		for (uint32_t i = 0; i < 3; i++) {

			Vector3 ndcVertex = Transform(kLocalVertices[i], worldViewProjectionMatrix);
			screenVerties[i] = Transform(ndcVertex, viewportMatrix);

		}

		Vector3 cross = Vector3::Cross(v1, v2);
		VecScreenPrintf(0, 0, cross, "Cross");
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		if (Vector3::Dot(CameraVecZ, Vector3::Cross(screenVerties[1] - screenVerties[0], screenVerties[2] - screenVerties[1]))<= 0){
			Novice::DrawTriangle(
				int(screenVerties[0].x), int(screenVerties[0].y),
				int(screenVerties[1].x), int(screenVerties[1].y),
				int(screenVerties[2].x), int(screenVerties[2].y), RED, kFillModeSolid
			);
		}

		
		

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
