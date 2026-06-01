#include "Quaternion.h"

#include "Vector3.h"

Quaternion Quaternion::Conjugate(const Quaternion& quaternion){
    //q*=w-xi-yj-zk=(qw,-qv)
    //虚数部分があるところはマイナスになるよ
    Quaternion result = {
        .x = -quaternion.x,
        .y = -quaternion.y,
        .z = -quaternion.z,
        .w = quaternion.w,
    };

    return result;
}

inline float_t Quaternion::Norm(const Quaternion& quaternion){
    //||q||=sqrtf(qq*)
    float_t result = 0.0f;
    result = sqrtf(
        quaternion.w * quaternion.w +
        quaternion.x * quaternion.x +
        quaternion.y * quaternion.y +
        quaternion.z * quaternion.z);

    return result;
}

inline Quaternion Quaternion::Normalize(const Quaternion& quaternion){

    //長さを求める
    float_t length = sqrtf(
        quaternion.x * quaternion.x +
        quaternion.y * quaternion.y +
        quaternion.z * quaternion.z +
        quaternion.w * quaternion.w
    );

    //修正前
    Quaternion preResult = {
        preResult.x = quaternion.x,
        preResult.y = quaternion.y,
        preResult.z = quaternion.z,
        preResult.w = quaternion.w,
    };


    //長さが0の時以外割る
    if (length != 0.0f) {
        preResult.x = quaternion.x / length;
        preResult.y = quaternion.y / length;
        preResult.z = quaternion.z / length;
        preResult.w = quaternion.w / length;

    }

    //結果
    Quaternion result = {
        .x = preResult.x,
        .y = preResult.y,
        .z = preResult.z,
        .w = preResult.w,
    };

    return result;
}

inline Quaternion Quaternion::Inverse(const Quaternion& quaternion){
    //q^-1=q*
    //   ||q||^2

    //ノルムを求める
    float_t norm = Norm(quaternion);
    Quaternion conjugate = Conjugate(quaternion);

    float_t t = norm * norm;

    //結果
    Quaternion result = {
        .x = conjugate.x / t,
        .y = conjugate.y / t,
        .z = conjugate.z / t,
        .w = conjugate.w / t,
    };

    return result;
}

Quaternion Quaternion::MakeRotateAxisAngleQuaternion(const Vector3& axis, float_t angle){
    //q=(cos/2,n*sin/2)
    Vector3 n = Vector3::Normalize(axis);

    //結果
    Quaternion result = {
        .x = n.x * std::sinf(angle / 2.0f),
        .y = n.y * std::sinf(angle / 2.0f),
        .z = n.z * std::sinf(angle / 2.0f),
        .w = std::cosf(angle / 2.0f),
    };

    return result;
}

Quaternion Quaternion::Slerp(const Quaternion& q0, const Quaternion& q1, const float_t& t){
    //q0・q1=||q0||・||q1||*cos
    //今は単位Quaternionを求めたいのでノルムは1なので
    //q0・q1 = cosでOK!!
    float_t dot =
        q0.x * q1.x +
        q0.y * q1.y +
        q0.z * q1.z +
        q0.w * q1.w;

    //修正用
    Quaternion fixQuaternion0 = q0;

    //2通りあることから
    //詳しくは資料の14ページで
    if (dot < 0.0f) {
        //もう片方の回転を利用する
        fixQuaternion0.x = -q0.x;
        fixQuaternion0.y = -q0.y;
        fixQuaternion0.z = -q0.z;
        fixQuaternion0.w = -q0.w;

        //内積も反転
        dot = -dot;
    }

    //dotが限りなく1に近い場合
    const float_t EPSILON = 0.0005f;
    Quaternion result = {};
    if (dot > 1.0f - EPSILON) {
        // 直線補間を行う
        result = {
            .x = (1.0f - t) * fixQuaternion0.x + t * q1.x,
            .y = (1.0f - t) * fixQuaternion0.y + t * q1.y,
            .z = (1.0f - t) * fixQuaternion0.z + t * q1.z,
            .w = (1.0f - t) * fixQuaternion0.w + t * q1.w,
        };
        return result;
    }

    //角度を求める
    float_t theta = std::acosf(dot);
    //Quaternionの前にある係数
    //scale...係数
    float_t scale0 = std::sin((1 - t) * theta) / std::sin(theta);
    float_t scale1 = std::sin(t * theta) / std::sin(theta);

    //結果
    result = {
        .x = scale0 * fixQuaternion0.x + scale1 * q1.x,
        .y = scale0 * fixQuaternion0.y + scale1 * q1.y,
        .z = scale0 * fixQuaternion0.z + scale1 * q1.z,
        .w = scale0 * fixQuaternion0.w + scale1 * q1.w,
    };


    return result;
}
