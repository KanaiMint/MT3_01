#include <Novice.h>
#include "Vector3.h"
#include "Matrix4x4.h"
#include<cmath>
#include<imgui.h>
#include<algorithm>
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
bool IsCollision(const Line& line, const Plane& plane) {
	//float tmp = plane.distance - (Vector3::Dot(line.origin, plane.normal));
	float tmp2 = Vector3::Dot(plane.normal, line.diff);
	//float T = tmp / tmp2;

	if (tmp2 == 0.0f) {
		return false;
	}
	else {
		//if (T >= 0.0f && T <= 1.0f) {
		//	return true;//線分と無限平面の当たり判定
		//}
		//if (T > 0.0f) {
		//	return true;//半直線と無限平面当たり判定
		//}
		//if (T > 0.0f) {
		//	return true;
		//}
		return true;
	}
}
bool IsCollision(const Segment& segment, const Plane& plane) {
	float tmp = plane.distance - (Vector3::Dot(segment.origin, plane.normal));
	float tmp2 = Vector3::Dot(plane.normal, segment.diff);
	float T = tmp / tmp2;

	if (tmp2 == 0.0f) {
		return false;
	}
	else {
		if (T >= 0.0f && T <= 1.0f) {
			return true;//線分と無限平面の当たり判定
		}
		//if (T > 0.0f) {
		//	return true;//半直線と無限平面当たり判定
		//}
		//if (T > 0.0f) {
		//	return true;
		//}
		
	}
	return false;
}
bool IsCollision(const Ray& ray, const Plane& plane) {
	float tmp = plane.distance - (Vector3::Dot(ray.origin, plane.normal));
	float tmp2 = Vector3::Dot(plane.normal, ray.diff);
	float T = tmp / tmp2;

	if (tmp2 == 0.0f) {
		return false;
	}
	else {
		//if (T >= 0.0f && T <= 1.0f) {
		//	return true;//線分と無限平面の当たり判定
		//}
		if (T > 0.0f) {
			return true;//半直線と無限平面当たり判定
		}
		//if (T > 0.0f) {
		//	return true;
		//}

	}
	return false;
}

Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y,vector.x,0.0f };
	}
	return { 0.0f,-vector.z,vector.y };
}

void DrawLine(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 start = Transform(Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
	Vector3 end = Transform(Transform(Vector3::Add(segment.origin, segment.diff), viewProjectionMatrix), viewportMatrix);

	Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, color);

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

//三角形
struct Triangle {
	Vector3 vertices[3];//頂点
};
bool IsCollision(const Triangle& triangle, const Segment& segment) {

	Vector3 v01 = Vector3::Subtract(triangle.vertices[0], triangle.vertices[1]);
	Vector3 v12 = Vector3::Subtract(triangle.vertices[1], triangle.vertices[2]);
	Vector3 normal =Vector3::Normalize( Vector3::Cross(v01, v12));//三角形の法線
	
	float distance= Vector3::Dot(triangle.vertices[0], normal);

	Plane plane(normal, distance);

	if (IsCollision(segment, plane)) {

		float tmp = plane.distance - (Vector3::Dot(segment.origin, plane.normal));
		float tmp2 = Vector3::Dot(plane.normal, segment.diff);
		float T = tmp / tmp2;

		Vector3 P =Vector3::Add( segment.origin ,Vector3::Multiply( T ,segment.diff));

	//	Vector3 P = o + tb;

		//各辺を結んだベクトルと、頂点と衝突店ｐを結んだベクトルのクロス石器をとる
		Vector3 cross01 = Vector3::Cross(Vector3::Subtract(triangle.vertices[0], triangle.vertices[1]), Vector3::Subtract(triangle.vertices[1], P));
		Vector3 cross12 = Vector3::Cross(Vector3::Subtract(triangle.vertices[1], triangle.vertices[2]), Vector3::Subtract(triangle.vertices[2], P));
		Vector3 cross20 = Vector3::Cross(Vector3::Subtract(triangle.vertices[2], triangle.vertices[0]), Vector3::Subtract(triangle.vertices[0], P));



		if (Vector3::Dot(cross01, normal) >= 0.0f &&
			Vector3::Dot(cross12, normal) >= 0.0f &&
			Vector3::Dot(cross20, normal) >= 0.0f) {

			return true;

		}
		return false;
	}
	return false;

}
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	Vector3 v01 = Vector3::Subtract(triangle.vertices[0], triangle.vertices[1]);
	Vector3 v12 = Vector3::Subtract(triangle.vertices[1], triangle.vertices[2]);
	Vector3 v20 = Vector3::Subtract(triangle.vertices[2], triangle.vertices[0]);


	Vector3 start1 = Transform(Transform(triangle.vertices[0], viewProjectionMatrix), viewportMatrix);
	Vector3 start2 = Transform(Transform(triangle.vertices[1], viewProjectionMatrix), viewportMatrix);
	Vector3 start3 = Transform(Transform(triangle.vertices[2], viewProjectionMatrix), viewportMatrix);

	Novice::DrawLine((int)start1.x, (int)start1.y, (int)start2.x, (int)start2.y, color);
	Novice::DrawLine((int)start2.x, (int)start2.y, (int)start3.x, (int)start3.y, color);
	Novice::DrawLine((int)start3.x, (int)start3.y, (int)start1.x, (int)start1.y, color);

}

struct AABB {
	Vector3 min;//最小店
	Vector3 max;//最大店
};

bool IsCollision(const AABB& a, const AABB& b) {
	if((a.min.x<=b.max.x&&a.max.x>=b.min.x)&&
		(a.min.y <= b.max.y && a.max.y >= b.min.y) &&
		(a.min.z <= b.max.z && a.max.z >= b.min.z) ){

		return true;
	}
	return false;
}
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	Vector3 min = aabb.min;
	Vector3 max = aabb.max;
	 
	Vector3 front[] = {
		{min.x,min.y,min.z},//LB
		{min.x,max.y,min.z},//LT
		{max.x,max.y,min.z},//RT
		{max.x,min.y,min.z},//RB
	};
	Vector3 back[] = {
		{min.x,min.y,max.z},//LB
		{min.x,max.y,max.z},//LT
		{max.x,max.y,max.z},//RT
		{max.x,min.y,max.z},//RB
	};

	for (int i = 0; i < 4; i++) {
		front[i] = Transform(Transform(front[i], viewProjectionMatrix), viewportMatrix);
		back[i] = Transform(Transform(back[i], viewProjectionMatrix), viewportMatrix);
		Novice::DrawLine((int)front[i].x, (int)front[i].y, (int)back[i].x, (int)back[i].y, color);
	}
	Novice::DrawLine((int)front[0].x, (int)front[0].y, (int)front[1].x, (int)front[1].y, color);
	Novice::DrawLine((int)front[2].x, (int)front[2].y, (int)front[1].x, (int)front[1].y, color);
	Novice::DrawLine((int)front[2].x, (int)front[2].y, (int)front[3].x, (int)front[3].y, color);
	Novice::DrawLine((int)front[0].x, (int)front[0].y, (int)front[3].x, (int)front[3].y, color);

	Novice::DrawLine((int)back[0].x, (int)back[0].y, (int)back[1].x, (int)back[1].y, color);
	Novice::DrawLine((int)back[2].x, (int)back[2].y, (int)back[1].x, (int)back[1].y, color);
	Novice::DrawLine((int)back[2].x, (int)back[2].y, (int)back[3].x, (int)back[3].y, color);
	Novice::DrawLine((int)back[0].x, (int)back[0].y, (int)back[3].x, (int)back[3].y, color);

}
bool IsCollision(const AABB& aabb, const Sphere& sphere) {

	//最近接点を求める
	Vector3 closesPoint(std::clamp(sphere.center.x, aabb.min.x, aabb.max.x),
		std::clamp(sphere.center.y, aabb.min.y, aabb.max.y),
		std::clamp(sphere.center.z, aabb.min.z, aabb.max.z));

	//最近接点と球の中心との距離を求める
	float distance =Vector3::Length(Vector3::Subtract(closesPoint,sphere.center));
	//距離が半径より小さければ衝突
	if (distance <= sphere.radius) {
		return true;
	}
	return false;
}
bool IsCollision(const AABB& aabb, const Segment& segment) {
	float Txmin = (aabb.min.x - segment.origin.x) / segment.diff.x;
	float Txmax = (aabb.max.x - segment.origin.x) / segment.diff.x;
	float Tymin = (aabb.min.y - segment.origin.y) / segment.diff.y;
	float Tymax = (aabb.max.y - segment.origin.y) / segment.diff.y;
	float Tzmin = (aabb.min.z - segment.origin.z) / segment.diff.z;
	float Tzmax = (aabb.max.z - segment.origin.z) / segment.diff.z;

	float tNearX = min(Txmin, Txmax);
	float tNearY = min(Tymin, Tymax);
	float tNearZ = min(Tzmin, Tzmax);

	float tFarX = max(Txmin, Txmax);
	float tFarY = max(Tymin, Tymax);
	float tFarZ = max(Tzmin, Tzmax);
	//AABBとの衝突点（貫通店）のｔが小さいほう
	float tmin = max(max(tNearX, tNearY), tNearZ);
	//AABBとの衝突点（貫通店）のｔが大きいほう
	float tmax = min(min(tFarX, tFarY), tFarZ);
	if (tmin <= tmax) {
		return true;
	}
	return false;
}

void DrawBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2,
	const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	const float t = 50;

	 
	for (int i = 1; i <=t; i++) {

		//制御点p0,p1を線形補間
		Vector3 p0p1 = Vector3::Lerp(controlPoint0, controlPoint1, i/t);
		//制御点p1,p2を線形補間
		Vector3 p1p2 = Vector3::Lerp(controlPoint1, controlPoint2, i/t);
		//補間点p0p1,p1p2を線形補間
		Vector3 p = Vector3::Lerp(p0p1, p1p2, i/t);

		//制御点p0,p1を線形補間
		Vector3 p0p1_ = Vector3::Lerp(controlPoint0, controlPoint1, (i-1)/t);
		//制御点p1,p2を線形補間
		Vector3 p1p2_ = Vector3::Lerp(controlPoint1, controlPoint2, (i-1)/t);
		//補間点p0p1,p1p2を線形補間
		Vector3 p_ = Vector3::Lerp(p0p1_, p1p2_, (i-1)/t);
		Vector3 p_ndc= Transform(Transform(p, viewProjectionMatrix), viewportMatrix);
		Vector3 p__ndc= Transform(Transform(p_, viewProjectionMatrix), viewportMatrix);

		Novice::DrawLine((int)p_ndc.x, (int)p_ndc.y, (int)p__ndc.x, (int)p__ndc.y, color);
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

	Segment segment
	{
		.origin{-0.7f,0.3f,0.0f},
		.diff{2.0f,-0.5f,0.0f} 
	};
	Vector3 point{ -1.5f,0.6f,0.6f };
	/*float rot = 0.0f;*/

	int color = WHITE;

	Vector3 controlPoint[3] = {
		{-0.8f,0.58f,1.0f},
		{-1.76f,1.0f,-0.3f},
		{-0.94f,-0.7f,2.3f},
	};
	
	Plane plane1{ {0.0f,1.0f,0.0f},0.1f };
	Triangle triangle{
		{
		{-1.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f},
		{1.0f,0.0f,0.0f}
		}
	};
	AABB aabb1{
		.min{-0.5f,-0.5f,-0.5f},
		.max{0.5f,0.5f,0.5f},
	};
	AABB aabb2{
		.min{0.2f,0.2f,0.2f},
		.max{1.0f,1.0f,1.0f},
	};

	Vector3 translates[3] = {
		{0.2f,1.0f,0.0f},
		{0.4f,0.0f,0.0f},
		{0.3f,0.0f,0.0f},
	};
	Vector3 rotates[3] = {
		{0.0f,1.0f,-6.8f},
		{0.0f,0.0f,-1.4f},
		{0.0f,0.0f,0.0f},
	};
	Vector3 scales[3] = {
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
	};

	Vector3 a{ 0.2f,1.0f,0.0f };
	Vector3 b{ 2.4f,3.1f,1.2f };
	Vector3 c = a + b;
	Vector3 d = a - b;
	Vector3 e = a * 2.4f;
	Vector3 rotate_(0.4f, 1.43f, -0.8f);

	Matrix4x4  rotateXMatrix = MakeRotateXMatrix(rotate_.x);
	Matrix4x4  rotateYMatrix = MakeRotateYMatrix(rotate_.y);
	Matrix4x4  rotateZMatrix = MakeRotateZMatrix(rotate_.z);
	Matrix4x4 rotateMatrix = rotateXMatrix * rotateYMatrix * rotateZMatrix;

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
		/*Vector3 project = Projection(Vector3::Subtract(point, segment.origin), segment.diff);
		Vector3 closestPoint = ClossPoint(point, segment);
		Sphere pointSphere{ point,0.01f };
		Sphere clossPointSphere{closestPoint,0.01f};	*/

		
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.3f,0.0f,0.0f }, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(1280), float(720), 0.0f, 1.0f);
#pragma region 階層構造



		////肩
		//Matrix4x4 sholderMatrix = MakeAffineMatrix(scales[0], rotates[0], translates[0]);
		////肘
		//Matrix4x4 elbowMatrix = MakeAffineMatrix(scales[1], rotates[1], translates[1]);
		////手
		//Matrix4x4 handMatrix = MakeAffineMatrix(scales[2], rotates[2], translates[2]);

		////階層構造変換
		//Matrix4x4 L_sholderMatrix = sholderMatrix;
		//Matrix4x4 L_elbowMatrix = Multiply( elbowMatrix,L_sholderMatrix );
		//Matrix4x4 L_handMatrix = Multiply(handMatrix, L_elbowMatrix);
		////変換後にビュープロジェクション作成
		//Matrix4x4 L_sholder_ViewProjectionMatrix = Multiply(L_sholderMatrix, Multiply(viewMatrix, projectionMatrix));
		//Matrix4x4 L_elbow_ViewProjectionMatrix = Multiply(L_elbowMatrix, Multiply(viewMatrix, projectionMatrix));
		//	
		////NDC
		//Vector3 sholder_tmp = Vector3(L_sholderMatrix.m[3][0], L_sholderMatrix.m[3][1], L_sholderMatrix.m[3][2]);
		//Vector3 elbow_tmp = Vector3(L_elbowMatrix.m[3][0], L_elbowMatrix.m[3][1], L_elbowMatrix.m[3][2]);
		//Vector3 hand_tmp = Vector3(L_handMatrix.m[3][0], L_handMatrix.m[3][1], L_handMatrix.m[3][2]);

		//Vector3 L_NDC_sholder = Transform(Transform(sholder_tmp, worldViewProjectionMatrix), viewportMatrix);
		//Vector3 L_NDC_elbow = Transform(Transform(elbow_tmp, worldViewProjectionMatrix), viewportMatrix);
		//Vector3 L_NDC_hand = Transform(Transform(hand_tmp, worldViewProjectionMatrix), viewportMatrix);

		//Sphere sphere1{ {sholder_tmp},0.1f };
		//Sphere sphere2{ {elbow_tmp},0.1f };
		//Sphere sphere3{ {hand_tmp},0.1f };
#pragma endregion

		ImGui::Begin("Window");

		ImGui::DragFloat3("cameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("cameraRotate", &rotate.x, 0.01f);
		//ImGui::InputFloat3("Project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		//ImGui::DragFloat3("sphere1", &sphere1.center.x, 0.01f);
		//ImGui::DragFloat3("sphere2", &sphere2.center.x, 0.01f);
		//ImGui::DragFloat("Plane", &plane1.distance, 0.01f);
		//ImGui::DragFloat3("Plane", &plane1.normal.x, 0.01f);
		//ImGui::DragFloat3("Segment", &segment.origin.x, 0.01f);
		//ImGui::DragFloat3("Segment", &segment.diff.x, 0.01f);
		//ImGui::DragFloat3("Triangle", &triangle.vertices[0].x, 0.01f);
		//ImGui::DragFloat3("AABB1", &aabb1.min.x, 0.01f);
		//ImGui::DragFloat3("AABB1", &aabb1.max.x, 0.01f);
		//ImGui::DragFloat3("AABB2", &aabb2.min.x, 0.01f);
		//ImGui::DragFloat3("AABB2", &aabb2.max.x, 0.01f);
		//ImGui::DragFloat3("Bezier_control[0]", &controlPoint[0].x, 0.01f);
		//ImGui::DragFloat3("Bezier_control[1]", &controlPoint[1].x, 0.01f);
		//ImGui::DragFloat3("Bezier_control[2]", &controlPoint[2].x, 0.01f);
		/*ImGui::DragFloat3("sholder_translate", &translates[0].x, 0.01f);
		ImGui::DragFloat3("sholder_rotate", &rotates[0].x, 0.01f);
		ImGui::DragFloat3("elbow_translate", &translates[1].x, 0.01f);
		ImGui::DragFloat3("elbow_rotate", &rotates[1].x, 0.01f);
		ImGui::DragFloat3("hand_translate", &translates[2].x, 0.01f);
		ImGui::DragFloat3("hand_rotate", &rotates[2].x, 0.01f);*/
		ImGui::Text("c:%f,%f,%f", c.x, c.y, c.z);
		ImGui::Text("d:%f,%f,%f", d.x, d.y, d.z);
		ImGui::Text("e:%f,%f,%f", e.x, e.y, e.z);
		ImGui::Text("matrix:\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n",
			rotateMatrix.m[0][0], rotateMatrix.m[0][1], rotateMatrix.m[0][2],
			rotateMatrix.m[0][3], rotateMatrix.m[1][0], rotateMatrix.m[1][1],
			rotateMatrix.m[1][2], rotateMatrix.m[1][3], rotateMatrix.m[2][0],
			rotateMatrix.m[2][1], rotateMatrix.m[2][2], rotateMatrix.m[2][3],
			rotateMatrix.m[3][0], rotateMatrix.m[3][1], rotateMatrix.m[3][2], rotateMatrix.m[3][3]
		);
		ImGui::End();

		
		//2つの球の中心点間の距離を求める	
		//float distance = sphere1.center.Length( sphere2.center );
		//半径の合計よりも短かったら衝突	
		/*if (IsCollision(aabb1,segment) == true) {
			color = RED;
		}
		else {
			color = WHITE;
		}*/
		color = WHITE;
		
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		//DrawLine(segment, worldViewProjectionMatrix, viewportMatrix, color);		
		/*DrawSphere(pointSphere, worldViewProjectionMatrix, viewportMatrix, RED);
		DrawSphere(clossPointSphere, worldViewProjectionMatrix, viewportMatrix, BLACK);		*/
		//DrawAABB(aabb1, worldViewProjectionMatrix, viewportMatrix, color);
		//DrawAABB(aabb2, worldViewProjectionMatrix, viewportMatrix, color);
		//DrawSphere(sphere1, worldViewProjectionMatrix, viewportMatrix, color);
		//DrawSphere(sphere2, worldViewProjectionMatrix, viewportMatrix, color);
		//DrawTriangle(triangle, worldViewProjectionMatrix, viewportMatrix, color);
		//DrawPlane(plane1, worldViewProjectionMatrix, viewportMatrix, color);
		
		//DrawBezier(controlPoint[0], controlPoint[1], controlPoint[2], worldViewProjectionMatrix, viewportMatrix, color);
		/*DrawSphere(sphere1, worldViewProjectionMatrix, viewportMatrix, BLUE);
		DrawSphere(sphere2, worldViewProjectionMatrix, viewportMatrix, RED);
		DrawSphere(sphere3, worldViewProjectionMatrix, viewportMatrix, GREEN);*/

		/*Novice::DrawLine((int)L_NDC_sholder.x, (int)L_NDC_sholder.y, (int)L_NDC_elbow.x, (int)L_NDC_elbow.y, color);
		Novice::DrawLine((int)L_NDC_elbow.x, (int)L_NDC_elbow.y, (int)L_NDC_hand.x, (int)L_NDC_hand.y, color);*/
		//Novice::DrawLine(L_NDC_sholder.x, L_NDC_sholder.y, L_NDC_elbow.x, L_NDC_elbow.y, color);
		
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
