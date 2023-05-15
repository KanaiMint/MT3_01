#include <Novice.h>
#include "Vector3.h"
#include "Matrix4x4.h"
#include<cmath>
#include<imgui.h>

const char kWindowTitle[] = "LD2A_カナイ_ミント";

static float M_PI=(float)3.14159265358979323846;

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;//Gridの半分の幅
	const uint32_t kSubdivision = 10;//分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);//一つ分の長さ
	Vector3 tmp1 = {};
	Vector3 tmp2 = {};
	Vector3 scrMatrix1 = {};
	Vector3 scrMatrix2 = {};
	//奥から手前へ線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		//ワールド座標の始点と終点	
		float x = kGridEvery * xIndex - kGridHalfWidth;
		tmp1 = { x,0.0f,kGridHalfWidth };//始点
		tmp2 = { x,0.0f,-kGridHalfWidth };//終点
		Vector3 ndcMatrix1 = Transform(tmp1, viewProjectionMatrix);
		scrMatrix1 = Transform(ndcMatrix1, viewportMatrix);
		Vector3 ndcMatrix2 = Transform(tmp2, viewProjectionMatrix);
		scrMatrix2 = Transform(ndcMatrix2, viewportMatrix);

		Novice::DrawLine((int)scrMatrix1.x, (int)scrMatrix1.y, (int)scrMatrix2.x, (int)scrMatrix2.y, 0xAAAAAAFF);	
	}
	//左から右へ線を順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		//ワールド座標の始点と終点
		float z = kGridEvery * zIndex - kGridHalfWidth;
		tmp1 = { kGridHalfWidth,0.0f,z };//始点
		tmp2 = { - kGridHalfWidth ,0.0f,z };//終点
		Vector3 ndcMatrix1 = Transform( tmp1, viewProjectionMatrix);
		scrMatrix1 = Transform(ndcMatrix1, viewportMatrix);
		Vector3 ndcMatrix2 = Transform(tmp2, viewProjectionMatrix);
		scrMatrix2 = Transform(ndcMatrix2, viewportMatrix);

		Novice::DrawLine((int)scrMatrix1.x, (int)scrMatrix1.y, (int)scrMatrix2.x, (int)scrMatrix2.y, 0x000000FF);

	}

}
struct Sphere {
	Vector3 center;//中心点
	float radius;//半径
};
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = 20;//分割数
	const float kLonEvery = (360.0f/kSubdivision)* (M_PI / 180.0f);//経度分割一つ分の角度
	const float kLatEvery = (360.0f / kSubdivision) *(M_PI/180.0f);//緯度分割一つ分の角度

	Vector3 ndcMatrix1 ={};
	Vector3	scrMatrix1 ={};
	Vector3 ndcMatrix2 ={};
	Vector3 scrMatrix2 ={};
	Vector3 ndcMatrix3 ={};
	Vector3 scrMatrix3 ={};
	//緯度の方向に分割-pai/2~pai/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex){
		float lat = -M_PI / 2.0f + kLatEvery * latIndex;//現在の緯度
		float lat_b = (M_PI / kSubdivision);
		//経度の方向に分割θ～2pai
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision;++lonIndex) {
			float lon = lonIndex * kLonEvery;//現在の経度
			float lon_b = (2 * M_PI) / kSubdivision;

			//world座標系でのa,b,cを求める
			Vector3 a, b, c;
			a = Vector3::Add(sphere.center,Vector3::Multiply(sphere.radius,{ cosf(lat) * cosf(lon),sinf(lat),cosf(lat) * sinf(lon) }));
			b = Vector3::Add(sphere.center, Vector3::Multiply(sphere.radius, { cosf(lat + lat_b) * cosf(lon),sinf(lat + lat_b),cosf(lat + lat_b) * sinf(lon) }));
			c = Vector3::Add(sphere.center, Vector3::Multiply(sphere.radius, { cosf(lat) * cosf(lon + lon_b),sinf(lat),cosf(lat) * sinf(lon + lon_b) }));

			 ndcMatrix1 = Transform(a, viewProjectionMatrix);
			scrMatrix1 = Transform(ndcMatrix1, viewportMatrix);

			 ndcMatrix2 = Transform(b, viewProjectionMatrix);
			 scrMatrix2 = Transform(ndcMatrix2, viewportMatrix);

			 ndcMatrix3 = Transform(c, viewProjectionMatrix);
			 scrMatrix3 = Transform(ndcMatrix3, viewportMatrix);

			Novice::DrawLine((int)scrMatrix1.x, (int)scrMatrix1.y, (int)scrMatrix2.x, (int)scrMatrix2.y, color);
			Novice::DrawLine((int)scrMatrix1.x, (int)scrMatrix1.y, (int)scrMatrix3.x, (int)scrMatrix3.y, color);


		}
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 rotate{ 0.26f,0.0f,0.0f };
	Vector3 translate{ 0.0f,0.0f,0.0f };
	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };

	Vector3 v1{ 1.2f,-3.9f,2.5f };
	Vector3 v2{ 2.8f,0.4f,-1.3f };

	Sphere sphere{
		{0.0f,0.0f,1.0f},
		0.5f
	};
	/*float rot = 0.0f;*/

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
		//	Vector3 CameraVecZ = Transform(cameraTranslate,MakeRotateZMatrix(rotate.z));

		if (Novice::CheckHitKey(DIK_W)) {
			translate.z += 0.1f;
		}if (Novice::CheckHitKey(DIK_S)) {
			translate.z -= 0.1f;
		}if (Novice::CheckHitKey(DIK_D)) {
			translate.x += 0.1f;
		}if (Novice::CheckHitKey(DIK_A)) {
			translate.x -= 0.1f;
		}
		//rotate.y += 0.05f;

		

		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.3f,0.0f,0.0f }, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(1280), float(720), 0.0f, 1.0f);
		
		ImGui::Begin("Window");
		ImGui::DragFloat3("cameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("cameraRotate", &rotate.x, 0.01f);
		ImGui::DragFloat3("SphereCenter", &sphere.center.x, 0.01f);
		ImGui::DragFloat("SphereRadius", &sphere.radius, 0.01f);
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		DrawSphere(sphere, worldViewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);
		
		

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
