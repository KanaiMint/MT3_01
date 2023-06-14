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
struct Line {
	Vector3 origin;	//始点	
	Vector3 diff;	//終点への差分ベクトル
};

struct Ray {
	Vector3 origin;	//始点	
	Vector3 diff;	//終点への差分ベクトル
};

struct Segment {
	Vector3 origin;	//始点	
	Vector3 diff;	//終点への差分ベクトル
};
Vector3 Projection(const Vector3& v1,const Vector3& v2) {
	Vector3 tmp = {};
	tmp = Vector3::Multiply(Vector3::Dot(v1, Vector3::Normalize(v2)),Vector3::Normalize(v2));
	return tmp;
}
Vector3 ClossPoint(const Vector3& point, const Segment& segment) {
	Vector3 tmp;
	
	tmp = Vector3::Add(segment.origin, Projection(Vector3::Subtract( point, segment.origin),segment.diff));
	//tmp = Projection(Vector3::Subtract(point,segment.origin),segment.diff);
	return tmp;
}
//平面
struct Plane {
	Vector3 normal;//法線
	float distance;//距離
};

bool Iscollision(const Sphere& sphere, const Plane& plane) {
	
	
	float k = Vector3::Dot(plane.normal, sphere.center) - plane.distance;

	if (std::abs(k) <=sphere.radius) {
		return true;
	}
	return false;

		//k=N・spherecenter-d;
}
Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y,vector.x,0.0f };
	}
	return { 0.0f,-vector.z,vector.y };
}

//平面描画
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	Vector3 center =Vector3::Multiply(plane.distance, plane.normal);//1
	Vector3 perpendiculars[4];
	perpendiculars[0] = Vector3::Normalize(Perpendicular(plane.normal));//2
	perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y,-perpendiculars[0].z };//3
	perpendiculars[2] = Vector3::Cross(plane.normal, perpendiculars[0]);//4
	perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };
	//6
	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend =Vector3::Multiply(2.0f, perpendiculars[index]);
		Vector3 point = Vector3::Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}
	//pointsをそれぞれ結んでDrawLineで矩形を描画する。DrawTringleを使って塗りつぶしてもいいが、DepthがないのでMT3ではわかりずらい
	Novice::DrawLine((int)points[0].x, (int)points[0].y, (int)points[2].x, (int)points[2].y, color);
	Novice::DrawLine((int)points[1].x, (int)points[1].y, (int)points[2].x, (int)points[2].y, color);
	Novice::DrawLine((int)points[0].x, (int)points[0].y, (int)points[3].x, (int)points[3].y,color);
	Novice::DrawLine((int)points[3].x, (int)points[3].y, (int)points[1].x, (int)points[1].y, color);
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

	Segment segment{ {-2.0f,-1.0f,0.0f},{3.0f,2.0f,2.0f} };
	Vector3 point{ -1.5f,0.6f,0.6f };
	/*float rot = 0.0f;*/

	int color = WHITE;


	Sphere sphere1{ {0.0f,0.0f,0.0f},0.3f };
	Sphere sphere2{ {0.0f,0.0f,0.0f},0.3f };

	Plane plane1{ {0.0f,1.0f,0.0f},0.1f };
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

		Vector3 project = Projection(Vector3::Subtract(point, segment.origin), segment.diff);
		Vector3 closestPoint = ClossPoint(point, segment);

		Sphere pointSphere{ point,0.01f };
		Sphere clossPointSphere{closestPoint,0.01f};

		

		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.3f,0.0f,0.0f }, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(1280), float(720), 0.0f, 1.0f);
		
		ImGui::Begin("Window");
		ImGui::DragFloat3("cameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("cameraRotate", &rotate.x, 0.01f);
		ImGui::InputFloat3("Project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::DragFloat3("sphere1", &sphere1.center.x, 0.01f);
		//ImGui::DragFloat3("sphere2", &sphere2.center.x, 0.01f);
		ImGui::DragFloat("Plane", &plane1.distance, 0.01f);
		ImGui::DragFloat3("Plane", &plane1.normal.x, 0.01f);
		ImGui::End();

		Vector3 start = Transform(Transform(segment.origin, worldViewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(Vector3::Add(segment.origin, segment.diff), worldViewProjectionMatrix), viewportMatrix);

		//2つの球の中心点間の距離を求める	
		//float distance = sphere1.center.Length( sphere2.center );
		//半径の合計よりも短かったら衝突	
		/*if (distance <= sphere1.radius + sphere2.radius) {
			color =RED;
		}
		else {
			color = WHITE;
		}*/

		if (Iscollision(sphere1, plane1)==true) {
			color = RED;
		}
		else {
			color = WHITE;
		}
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		DrawGrid(worldViewProjectionMatrix, viewportMatrix);

		/*Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y),WHITE);

		DrawSphere(pointSphere, worldViewProjectionMatrix, viewportMatrix, RED);
		DrawSphere(clossPointSphere, worldViewProjectionMatrix, viewportMatrix, BLACK);
		*/
		DrawSphere(sphere1, worldViewProjectionMatrix, viewportMatrix, color);
		//DrawSphere(sphere2, worldViewProjectionMatrix, viewportMatrix, color);

		DrawPlane(plane1, worldViewProjectionMatrix, viewportMatrix, color);
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
