#include "Quaternion.h"
#include<cmath>

Quaternion::Quaternion()
{
}

Quaternion::Quaternion(float i, float j, float k, float w0) ://w0はデフォルトで0
	q1(i),
	q2(j),
	q3(k),
	w(w0)
{
}

Quaternion::~Quaternion()
{
}

Quaternion Quaternion::operator*(const Quaternion other) const
{
	//クォータニオンの積
	float a, b, c, d;
	a = q1 * other.w + q2 * other.q3 - q3 * other.q2 + w * other.q1;//ah+bg-cf+de//i
	b = -q1 * other.q3 + q2 * other.w + q3 * other.q1 + w * other.q2;//af-bh+ce+df//j
	c = q1 * other.q2 - q2 * other.q1 + q3 * other.w + w * other.q3;//ag+bf-ah+de+cf//k
	d = -q1 * other.q1 - q2 * other.q2 - q3 * other.q3 + w * other.w;//ae+bg+cf+dh//w

	return Quaternion(a, b, c, d);
}

VECTOR Quaternion::RotateQuaternion(VECTOR axis, VECTOR prev, float theta)
{
	//前提条件として、回転軸は単位ベクトル、thetaは度数法でないといけない
	
	//回転軸を単位ベクトルにする
	float length= sqrtf(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
	axis.x /= length;
	axis.y /= length;
	axis.z /= length;
	//thetaの範囲指定を0から360度にする必要があるかもしれない
	if(theta < 0)
	{
		theta += 360;
	}
	else if(theta >= 360)
	{
		theta -= 360;
	}

	//実際の回転処理

	//軸をもとにAのクォータニオンを作る
	Quaternion A(axis.x * sinf(theta / 2), axis.y * sinf(theta / 2), axis.z * sinf(theta / 2), cosf(theta / 2));
	//回転前のベクトル(元の座標)をクォータニオンにする
	Quaternion Vq(prev.x, prev.y, prev.z, 0);
	//Aの共役クォータニオンを作る
	Quaternion Aminus(-A.q1, -A.q2, -A.q3, A.w);
	//回転後のクォータニオンを求める
	Quaternion result = A * Vq * Aminus;
	//クォータニオンからベクトルに変換して返す
	VECTOR resultV = { result.q1, result.q2, result.q3 };
	return resultV;
}
