#include "Matrix4x4.h"
#include <assert.h>
#include <cmath>
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) 
{ 
	Matrix4x4 tmp;
	tmp.m[0][0] = m1.m[0][0] + m2.m[0][0];
	tmp.m[0][1] = m1.m[0][1] + m2.m[0][1];
	tmp.m[0][2] = m1.m[0][2] + m2.m[0][2];
	tmp.m[0][3] = m1.m[0][3] + m2.m[0][3];

	tmp.m[1][0] = m1.m[1][0] + m2.m[1][0];
	tmp.m[1][1] = m1.m[0][1] + m2.m[1][1];
	tmp.m[1][2] = m1.m[1][2] + m2.m[1][2];
	tmp.m[1][3] = m1.m[1][3] + m2.m[1][3];

	tmp.m[2][0] = m1.m[2][0] + m2.m[2][0];
	tmp.m[2][1] = m1.m[0][1] + m2.m[2][1];
	tmp.m[2][2] = m1.m[2][2] + m2.m[2][2];
	tmp.m[2][3] = m1.m[2][3] + m2.m[2][3];

	tmp.m[3][0] = m1.m[3][0] + m2.m[3][0];
	tmp.m[3][1] = m1.m[0][1] + m2.m[3][1];
	tmp.m[3][2] = m1.m[3][2] + m2.m[3][2];
	tmp.m[3][3] = m1.m[3][3] + m2.m[3][3];

	return Matrix4x4(tmp); 
}

Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 tmp;
	tmp.m[0][0] = m1.m[0][0] - m2.m[0][0];
	tmp.m[0][1] = m1.m[0][1] - m2.m[0][1];
	tmp.m[0][2] = m1.m[0][2] - m2.m[0][2];
	tmp.m[0][3] = m1.m[0][3] - m2.m[0][3];

	tmp.m[1][0] = m1.m[1][0] - m2.m[1][0];
	tmp.m[1][1] = m1.m[0][1] - m2.m[1][1];
	tmp.m[1][2] = m1.m[1][2] - m2.m[1][2];
	tmp.m[1][3] = m1.m[1][3] - m2.m[1][3];

	tmp.m[2][0] = m1.m[2][0] - m2.m[2][0];
	tmp.m[2][1] = m1.m[0][1] - m2.m[2][1];
	tmp.m[2][2] = m1.m[2][2] - m2.m[2][2];
	tmp.m[2][3] = m1.m[2][3] - m2.m[2][3];

	tmp.m[3][0] = m1.m[3][0] - m2.m[3][0];
	tmp.m[3][1] = m1.m[0][1] - m2.m[3][1];
	tmp.m[3][2] = m1.m[3][2] - m2.m[3][2];
	tmp.m[3][3] = m1.m[3][3] - m2.m[3][3];

	return Matrix4x4(tmp);
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) 
{ 
	Matrix4x4 tmp;
	tmp.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0]+m1.m[0][3] * m2.m[3][0];
	tmp.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
	tmp.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
	tmp.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];
		
	
	tmp.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
	tmp.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
	tmp.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
	tmp.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];
	
	tmp.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
	tmp.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
	tmp.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
	tmp.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];
	
	tmp.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
	tmp.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
	tmp.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
	tmp.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];
	
	
	
	return Matrix4x4(tmp);
}

Matrix4x4 Inverse(const Matrix4x4& m) 
{ 
	float A = m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] +
	          m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] +
	          m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] -
	          m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
	          m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] -
	          m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] -
	          m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] -
	          m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
	          m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] +
	          m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] +
	          m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] +
	          m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +
	          m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] +
	          m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] +
	          m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] -
	          m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
	          m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] -
	          m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] -
	          m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] -
	          m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
	          m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] +
	          m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] +
	          m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] +
	          m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	Matrix4x4 tmp;
	
	tmp.m[0][0] = (1/A)*(m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] +m.m[1][3] * m.m[2][1] * m.m[3][2]-
				  m.m[1][3] * m.m[2][2] * m.m[3][1]-m.m[1][2] * m.m[2][1] * m.m[3][3]-m.m[1][1] * m.m[2][3] * m.m[3][2]);
	tmp.m[0][1] =(1/A)*( m.m[0][1] * m.m[2][3] * m.m[3][2] + m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][3] * m.m[2][2] * m.m[3][1] -
				  m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2] );
	tmp.m[0][2] =(1/A)*( m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1]+m.m[0][3]*m.m[1][1]*m.m[3][2]-
				  m.m[0][1] * m.m[1][3] * m.m[3][2] - m.m[0][2] * m.m[1][1] * m.m[3][3]-m.m[0][3] * m.m[1][2] * m.m[3][1]);
	tmp.m[0][3] = (1/A)*(m.m[0][1] * m.m[1][3] * m.m[2][2] + m.m[0][2] * m.m[1][1] * m.m[2][3]+m.m[0][3]*m.m[1][2]*m.m[2][1]-
				  m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1]-m.m[0][3] * m.m[1][1] * m.m[2][2]);
	tmp.m[1][0] = (1/A)*(m.m[1][0] * m.m[2][3] * m.m[3][2] + m.m[1][2] * m.m[2][0] * m.m[3][3]+m.m[1][3]*m.m[2][2]*m.m[3][0]-
				  m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0]-m.m[1][3] * m.m[2][0] * m.m[3][2]);
	
	tmp.m[1][1] = (1/A)*(m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0]+m.m[0][3]*m.m[2][0]*m.m[3][2]-
				  m.m[0][0] * m.m[2][3] * m.m[3][2] - m.m[0][2] * m.m[2][0] * m.m[3][3]-m.m[0][3] * m.m[2][2] * m.m[3][0]);

	tmp.m[1][2] = (1/A)*(m.m[0][0] * m.m[1][3] * m.m[3][2] + m.m[0][2] * m.m[1][0] * m.m[3][3]+m.m[0][3]*m.m[1][2]*m.m[3][0]-
				  m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0]-m.m[0][3] * m.m[1][0] * m.m[3][2]);
	tmp.m[1][3] =(1/A)*(m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0]+m.m[0][3]*m.m[1][0]*m.m[2][2]-
				  m.m[0][0] * m.m[1][3] * m.m[2][2] - m.m[0][2] * m.m[1][0] * m.m[2][3]-m.m[0][3] * m.m[1][2] * m.m[2][0]);
	
	tmp.m[2][0] =(1/A)*(m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0]+m.m[1][3]*m.m[2][0]*m.m[3][1]-
				  m.m[1][0] * m.m[2][3] * m.m[3][1] - m.m[1][1] * m.m[2][0] * m.m[3][3]-m.m[1][3] * m.m[2][1] * m.m[3][0]);
	tmp.m[2][1] =(1/A)*(m.m[0][0] * m.m[2][3] * m.m[3][1] + m.m[0][1] * m.m[2][0] * m.m[3][3]+m.m[0][3]*m.m[2][1]*m.m[3][0]-
				  m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0]-m.m[0][3] * m.m[2][0] * m.m[3][1]);
	tmp.m[2][2] =(1/A)*(m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0]+m.m[0][3]*m.m[1][0]*m.m[3][1]-
				  m.m[0][0] * m.m[1][3] * m.m[3][1] - m.m[0][1] * m.m[1][0] * m.m[3][3]-m.m[0][3] * m.m[1][1] * m.m[3][0]);
	tmp.m[2][3] =(1/A)*(m.m[0][0] * m.m[1][3] * m.m[2][1] + m.m[0][1] * m.m[1][0] * m.m[2][3]+m.m[0][3]*m.m[1][1]*m.m[2][0]-
				  m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0]-m.m[0][3] * m.m[1][0] * m.m[2][1]);
	
	tmp.m[3][0] =(1/A)*(m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[1][1] * m.m[2][0] * m.m[3][2]+m.m[1][2]*m.m[2][1]*m.m[3][0]-
				  m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0]-m.m[1][2] * m.m[2][0] * m.m[3][1]);
	tmp.m[3][1] =(1/A)*(m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0]+m.m[0][2]*m.m[2][0]*m.m[3][1]-
				  m.m[0][0] * m.m[2][2] * m.m[3][1] - m.m[0][1] * m.m[2][0] * m.m[3][2]-m.m[0][2] * m.m[2][1] * m.m[3][0]);
	tmp.m[3][2] =(1/A)*(m.m[0][0] * m.m[1][2] * m.m[3][1] + m.m[0][1] * m.m[1][0] * m.m[3][2]+m.m[0][2]*m.m[1][1]*m.m[3][0]-
				  m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0]-m.m[0][2] * m.m[1][0] * m.m[3][1]);

	tmp.m[3][3] =(1/A)*(m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0]+m.m[0][2]*m.m[1][0]*m.m[2][1]-
				  m.m[0][0] * m.m[1][2] * m.m[2][1] - m.m[0][1] * m.m[1][0] * m.m[2][2]-m.m[0][2] * m.m[1][1] * m.m[2][0]);

	return Matrix4x4(tmp); 
}

Matrix4x4 Transpose(const Matrix4x4& m)
{
	Matrix4x4 tmp;
	
	tmp.m[0][0] = m.m[0][0];
	tmp.m[0][1] = m.m[1][0];
	tmp.m[0][2] = m.m[2][0];
	tmp.m[0][3] = m.m[3][0];
	
	tmp.m[1][0] = m.m[0][1];
	tmp.m[1][1] = m.m[1][1];
	tmp.m[1][2] = m.m[2][1];
	tmp.m[1][3] = m.m[3][1];
	
	tmp.m[2][0] = m.m[0][2];
	tmp.m[2][1] = m.m[1][2]; 
	tmp.m[2][2] = m.m[2][2];
	tmp.m[2][3] = m.m[3][2];
	
	tmp.m[3][0] = m.m[0][3];
	tmp.m[3][1] = m.m[1][3];
	tmp.m[3][2] = m.m[2][3];
	tmp.m[3][3] = m.m[3][3];

	return Matrix4x4(tmp);
}

Matrix4x4 MakeIdentity4x4() 
{
	Matrix4x4 tmp;
	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 4; k++) {
			tmp.m[i][k] = 0;
		}
	}

	tmp.m[0][0] = 1;
	tmp.m[1][1] = 1;
	tmp.m[2][2] = 1;
	tmp.m[3][3] = 1;
	return Matrix4x4(tmp);
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate)
{
	Matrix4x4 tmp = {};
	tmp.m[0][0] = 1.0f;
	tmp.m[1][1] = 1.0f;
	tmp.m[2][2] = 1.0f;
	tmp.m[3][3] = 1.0f;

	tmp.m[3][0] = translate.x;
	tmp.m[3][1] = translate.y;
	tmp.m[3][2] = translate.z;

	return Matrix4x4(tmp);
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) 
{
	Matrix4x4 tmp;
	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 4; k++) {
			tmp.m[i][k] = 0;
		}
	}
	tmp.m[0][0] = scale.x;
	tmp.m[1][1] = scale.y;
	tmp.m[2][2] = scale.z;
	tmp.m[3][3] = 1.0f;

	return Matrix4x4(tmp);
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix)
{
	Vector3 result = {};
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0]+1.0f*matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1]+1.0f*matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2]+1.0f*matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3]+1.0f*matrix.m[3][3];

	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return Vector3(result); 
}

Matrix4x4 MakeRotateXMatrix(float radian)
{ 
	Matrix4x4 tmp = {};
	tmp.m[0][0] = 1.0f;
	tmp.m[1][1] = cosf(radian);
	tmp.m[1][2] = sinf(radian);
	tmp.m[2][1] = -sinf(radian);
	tmp.m[2][2] = cosf(radian);
	tmp.m[3][3] = 1.0f;

	return Matrix4x4(tmp);

}

Matrix4x4 MakeRotateYMatrix(float radian) 
{ 
	Matrix4x4 tmp = {};
	tmp.m[0][0] = cosf(radian);
	tmp.m[0][2] = -sinf(radian);
	tmp.m[1][1] = 1.0f;
	tmp.m[2][0] = sinf(radian);
	tmp.m[2][2] = cosf(radian);
	tmp.m[3][3] = 1.0f;

	return Matrix4x4(tmp);
}

Matrix4x4 MakeRotateZMatrix(float radian) 
{ 
	Matrix4x4 tmp = {};
	tmp.m[0][0] = cosf(radian);
	tmp.m[0][1] = sinf(radian);
	tmp.m[1][0] = -sinf(radian);
	tmp.m[1][1] = cosf(radian);
	tmp.m[2][2] = 1.0f;
	tmp.m[3][3] = 1.0f;

	return Matrix4x4(tmp);
}

Matrix4x4 MakeRotateXYZMatrix(const Vector3& rotate) { 
	Matrix4x4 rotateX = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateY = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZ = MakeRotateZMatrix(rotate.z);

	Matrix4x4 rotateXYZMatrix = Multiply(rotateX, Multiply(rotateY, rotateZ));
	return Matrix4x4(rotateXYZMatrix);
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	
	Matrix4x4 tmp = Multiply(
	    MakeScaleMatrix(scale),Multiply(MakeRotateXYZMatrix(rotate), MakeTranslateMatrix(translate)));
	return Matrix4x4(tmp);
}


Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
{
	Matrix4x4 tmp = {};
	tmp.m[0][0] = (1.0f / aspectRatio) * (1.0f / std::tan(fovY / 2.0f));
	tmp.m[1][1] = (1.0f / std::tan(fovY / 2.0f));
	tmp.m[2][2] = farClip / (farClip - nearClip);
	tmp.m[2][3] = 1.0f;
	tmp.m[3][2] = -nearClip*farClip / (farClip - nearClip);
	return Matrix4x4(tmp);
}

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip)
{
	Matrix4x4 tmp = {};
	tmp.m[0][0] = 2.0f / (right - left);
	tmp.m[1][1] = 2.0f / (top - bottom);
	tmp.m[2][2] = 1.0f / (farClip - nearClip);
	tmp.m[3][0] = (left + right) / (left - right);
	tmp.m[3][1] = (top+bottom) /( bottom - top);
	tmp.m[3][2] = (nearClip) / (nearClip - farClip);
	tmp.m[3][3] = 1.0f;

	return Matrix4x4(tmp);
}

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	Matrix4x4 tmp = {};
	tmp.m[0][0] = width / 2.0f;
	tmp.m[1][1] = -height / 2.0f;
	tmp.m[2][2] = maxDepth - minDepth;
	tmp.m[3][0] = left + (width / 2.0f);
	tmp.m[3][1] = top+(height/2.0f);
	tmp.m[3][2] = minDepth;
	tmp.m[3][3] = 1.0f;


	return Matrix4x4(tmp);
}



void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	
	    Novice::ScreenPrintf(x, y, "%s", label);

	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {

			Novice::ScreenPrintf(
			    x + col * kColumnWidth, y+20 + row * kRowHeight, "%6.02f", matrix.m[row][col]);
		}
	}
}
