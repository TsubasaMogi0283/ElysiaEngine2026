#include "Vector3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"

inline Vector3 Vector3::TransformCalculation(const Vector3& v, const Matrix4x4& m){
	
	Vector3 result = {
		.x = (v.x * m.m[0][0]) + (v.y * m.m[1][0]) + (v.z * m.m[2][0]) + (1.0f * m.m[3][0]),
		.y = (v.x * m.m[0][1]) + (v.y * m.m[1][1]) + (v.z * m.m[2][1]) + (1.0f * m.m[3][1]),
		.z = (v.x * m.m[0][2]) + (v.y * m.m[1][2]) + (v.z * m.m[2][2]) + (1.0f * m.m[3][2])
	};
	float_t w = (v.x * m.m[0][3]) + (v.y * m.m[1][3]) + (v.z * m.m[2][3]) + (1.0f * m.m[3][3]);

	//0除算を避ける
	if (w != 0.0f) {
		result.x /= w;
		result.y /= w;
		result.z /= w;

	}
	return result;
}

Vector3 Vector3::RotateVector(const Vector3& vector, const Quaternion& quaternion){
    //wはいらない
    Quaternion convertQuaternion = {
        .x = vector.x,
        .y = vector.y,
        .z = vector.z,
        .w = 0.0f,
    };

    Quaternion conjugate = Quaternion::Conjugate(quaternion);

    //乗算
    Quaternion preResult = quaternion * (convertQuaternion * conjugate);

    //結果
    //wはいらない
    Vector3 result = {
        .x = preResult.x,
        .y = preResult.y,
        .z = preResult.z
    };
    return result;
}
