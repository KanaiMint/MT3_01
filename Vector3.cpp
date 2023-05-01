#include"Vector3.h"
#include<Novice.h>
Vector3 Vector3::Add(const Vector3& v1, const Vector3& v2)
{
	Vector3  temp;
	temp.x = v1.x + v2.x;
	temp.y = v1.y + v2.y;
	temp.z = v1.z + v2.z;

	return Vector3(temp);
}

Vector3 Vector3::Subtract(const Vector3& v1, const Vector3& v2)
{
	Vector3  temp;
	temp.x = v1.x - v2.x;
	temp.y = v1.y - v2.y;
	temp.z = v1.z - v2.z;
	return Vector3(temp);
}

Vector3 Vector3::Multiply(float scalar, const Vector3& v)
{
	Vector3  temp;
	temp.x = v.x * scalar;
	temp.y = v.y * scalar;
	temp.z = v.z * scalar;
	return Vector3(temp);
}

float Vector3::Dot(const Vector3& v1, const Vector3& v2)
{
	float temp;
	temp = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);

	return temp;
}

float Vector3::Length(const Vector3& v)
{
	float temp;
	
	temp = sqrtf( pow(v.x,2.0f)+ pow(v.y,2.0f)+ pow(v.z,2.0f));
	return temp;
}

Vector3 Vector3::Normalize(const Vector3& v)
{
	Vector3 temp;
	temp.x = v.x / v.Length(v);
	temp.y = v.y / v.Length(v);
	temp.z = v.z / v.Length(v);

	return Vector3(temp);
}

Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2)
{
	Vector3 tmp;
	tmp.x = (v1.y * v2.z) - (v1.z * v2.y);
	tmp.y = (v1.z * v2.x) - (v1.x * v2.z);
	tmp.z = (v1.x * v2.y) - (v1.y * v2.x);
	return Vector3(tmp);
}


void VecScreenPrintf(int x, int y, const Vector3& vector, const char* label)
{
	Novice::ScreenPrintf(x, y, "%0.2f", vector.x);
	Novice::ScreenPrintf(x + kColumWidth, y, "%0.2f", vector.y);
	Novice::ScreenPrintf(x + kColumWidth * 2, y, "%0.2f", vector.z);
	Novice::ScreenPrintf(x + kColumWidth * 3, y, "%s", label);
}
