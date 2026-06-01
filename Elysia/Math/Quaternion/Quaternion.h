#pragma once

/**
 * @file Quaternion.h
 * @brief クォータニオン
 * @author 茂木翼
 */

#include <cmath>

/// <summary>
/// ベクトル(3D)
/// </summary>
struct Vector3;

/// <summary>
/// クォータニオン
/// </summary>
struct Quaternion {
public:
	// 要素
	float_t x;
	float_t y;
	float_t z;
	float_t w;

public:
	//計算とそれぞれの要素について
	// 
	//q=w+xi+yj+zk
	//i^2=j^2=k^2=ijk=-1
	//ij=k,jk=i,ki=j,
	//ji=-k,kj=-i,ik=-j

	//q=(qw,qv)
	//qw=w
	//qv=(x,y,z)

	/// <summary>
	/// クォータニオンの積
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	inline Quaternion operator*(const Quaternion& other) const {
		//wはそのまま
		//xはi部分
		//yはj部分
		//zはk部分
		Quaternion result = {
			.x = this->w * other.x + this->x * other.w + this->y * other.z - this->z * other.y,
			.y = this->w * other.y - this->x * other.z + this->y * other.w + this->z * other.x,
			.z = this->w * other.z + this->x * other.y - this->y * other.x + this->z * other.w,
			.w = this->w * other.w - this->x * other.x - this->y * other.y - this->z * other.z,
		};

		return result;
	}

public:

	/// <summary>
	/// 単位のQuaternionを返す
	/// </summary>
	/// <returns></returns>
	static inline Quaternion IdentityQuaternion() {
		//e=1+0i+0j+0k
		//qe=eq=q
		Quaternion result = {
			.x = 0.0f,
			.y = 0.0f,
			.z = 0.0f,
			.w = 1.0f,
		};
		return result;
	};

	/// <summary>
	/// 共役Quaternionを返す
	/// </summary>
	/// <param name="quaternion"></param>
	/// <returns></returns>
	static Quaternion Conjugate(const Quaternion& quaternion);

	/// <summary>
	/// Quaternionのnormを返す
	/// </summary>
	/// <param name="quaternion"></param>
	/// <returns></returns>
	static inline float_t Norm(const Quaternion& quaternion);

	/// <summary>
	/// 正規化したQuaternionを返す
	/// </summary>
	/// <param name="quaternion"></param>
	/// <returns></returns>
	static Quaternion Normalize(const Quaternion& quaternion);

	/// <summary>
	/// 逆Quaternionを返す
	/// </summary>
	/// <param name="quaternion"></param>
	/// <returns></returns>
	static Quaternion Inverse(const Quaternion& quaternion);

	/// <summary>
	/// 任意軸回転を表すQuaternionの生成
	/// </summary>
	/// <param name="axis"></param>
	/// <param name="angle"></param>
	/// <returns></returns>
	static Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float_t angle);
	
	/// <summary>
	/// 回転の補間
	/// </summary>
	/// <param name="q0"></param>
	/// <param name="q1"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	static Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, const float_t& t);



};


