#include "Camera.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define DEG_TO_RAD( x ) ( x * M_PI / 180.0f ) 

inline void UpdatePitch(float& rad, float drad)
{
	rad += drad;

	(rad <= 90.0) ? rad = 90.0000000001 : rad;
	(rad >= 270.0) ? rad = 269.999999999 : rad;
}

inline void UpdateYaw(float& rad, float drad)
{
	rad += drad;
}

// z축 회전
inline void GetRotateMatZ(Eigen::Matrix4f& rotMat,float rad)
{
	rotMat.setIdentity();

	rotMat(0, 0) = cos(rad);
	rotMat(0, 1) = -sin(rad);
	rotMat(1, 0) = sin(rad);
	rotMat(1, 1) = cos(rad);
}

void Camera::UpdateVec()
{
	// https://learnopengl.com/Getting-started/Camera
	// 위 문서에서 나온 것중 dirvec의 방향을 실제 target을 바라보도록 설정
	// 직관적으로 표현하기 위함
	m_dirvec = m_target - m_pos;
	m_rightvec = m_dirvec.cross3(m_up);
	m_rightvec.normalize();
	m_upvec = m_rightvec.cross3(m_dirvec);

	UpdateMat();
}

void Camera::UpdateMat()
{
	Eigen::Matrix<float, 4, 4, Eigen::RowMajor> rotmatInv, translateMatInv;

	rotmatInv.setIdentity();
	rotmatInv.row(0) = m_rightvec;
	rotmatInv.row(1) = m_upvec;
	rotmatInv.row(2) = -m_dirvec;
	rotmatInv.col(3).setZero();
	rotmatInv(3, 3) = 1;

	translateMatInv.setIdentity();
	translateMatInv.col(3) = -m_pos;
	translateMatInv(3, 3) = 1;

	m_MVP = rotmatInv * translateMatInv ;
}

void Camera::Init()
{
	// 이 셋팅은 건드리면 안됨!!
	m_target = Eigen::Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
	m_pos = Eigen::Vector4f(0.0f, 0.0f, 1.0f, 1.0f);
	m_up = Eigen::Vector4f(0.0f, 1.0f, 0.0f, 0.0f);

	yaw = -90.0f;
	pitch = 180.0f; // dir의 방향이 target을 바라보는 방향이므로

	UpdateVec();
}

void Camera::LookAt(const Eigen::Vector4f& target, const Eigen::Vector4f& pos, const Eigen::Vector4f& up)
{
	// yaw, pitch, roll을 업데이트해준다.
	m_target = target;
	m_pos = pos;
	m_up = up;

	UpdateVec();
}

void Camera::Rotate(float dtheta, float dphi)
{
	UpdateYaw(yaw, dtheta);
	UpdatePitch(pitch, dphi);

	float yaw_rad = DEG_TO_RAD(yaw);
	float pitch_rad = -DEG_TO_RAD(pitch);// 카메라가 반대로 움직이게 함

	m_dirvec(0, 0) = cos(yaw_rad) * cos(pitch_rad);
	m_dirvec(1, 0) = sin(pitch_rad);
	m_dirvec(2, 0) = sin(yaw_rad) * cos(pitch_rad);
	m_dirvec(3, 0) = 0.0f;

	m_dirvec.normalize();

	float oldPosLen = (m_target - m_pos).norm();

	m_pos = m_target - oldPosLen * m_dirvec;
	
	UpdateVec();
}

void Camera::Zoom(float d)
{
	m_pos += (d * m_dirvec);

	UpdateVec();
}

void Camera::Translate(const Eigen::Vector4f& _right, const Eigen::Vector4f& _up)
{
	m_pos -= _right;
	m_pos -= _up;

	m_target -= _right;
	m_target -= _up;

	UpdateVec();
}

const float* Camera::GetMVPMat() const
{
	return m_MVP.data();
}

const Eigen::Vector4f Camera::GetRightVec() const
{
	return m_rightvec;
}

const Eigen::Vector4f Camera::GetUpVec() const
{
	return m_upvec;
}

const Eigen::Vector4f Camera::GetDirVec() const
{
	return m_dirvec;
}
