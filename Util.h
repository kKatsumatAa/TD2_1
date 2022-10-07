#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"


//�s��Z�b�g�n
void Vector4xMatrix4(Vector4& v, const Matrix4& m4);
void Vector3xMatrix4(Vector3& v, const Matrix4& m4 ,const bool w);
void Matrix4xMatrix4(Matrix4& m4, const Matrix4& other);
//void NormVector4(Vector4& v/*, const M4& m4*/);
void TransposeMatrix4(Matrix4& m4);

//�ϊ��s��Z�b�g�n
void SetScaleMatrix(Matrix4& m4, const Vector3& v3);
void SetRotationMatrix(Matrix4& m4, const float& angle, const char& axis);
void SetTranslationMatrix(Matrix4& m4, const Vector3& v3);

//���W�A���n
const float pi = 3.141592f;
float AngletoRadi(const float& angle);
float RaditoAngle(const float& radian);
float GetRadianVector3(const Vector3& v1, const Vector3& v2);

//���`�⊮
Vector3 LerpVector3(const Vector3& v1, const Vector3& v2, float t);
Vector3 SlerpVector3(const Vector3& v1, const Vector3& v2, float t);

//�Փ�
bool CollisionCircleCircle(const Vector3& pos1, const float& r1,
	const Vector3& pos2, const float& r2);

//�C�[�W���O
float EaseIn(float t);
float EaseOut(float t);