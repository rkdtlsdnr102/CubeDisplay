#pragma once
#include "Dense"

class Camera
{

	float yaw;
	float pitch;

	Eigen::Vector4f m_target;
	Eigen::Vector4f m_up;
	Eigen::Vector4f m_pos;

	// dx, dy, dz
	Eigen::Vector4f m_rightvec;// 카메라 기준 x축 벡터
	Eigen::Vector4f m_upvec; // 카메라 기준 y축 벡터
	Eigen::Vector4f m_dirvec; // 카메라 기준 z축 벡터( 방향벡터)

	Eigen::Matrix4f m_MVP;

	// Set Perspective
	float m_left;
	float m_right;
	float m_bottom;
	float m_top;
	float m_near;
	float m_far;

private:
	void UpdateVec();
	void UpdateMat();

public:
	void Init();
	void LookAt(const Eigen::Vector4f& target, const Eigen::Vector4f& pos, const Eigen::Vector4f& up);
	void Rotate(float theta, float phi);
	void Zoom(float d);
	void Translate(const Eigen::Vector4f &_right, const Eigen::Vector4f &_up);
	void SetPerspective(float left, float bottom, float right, float top, float near, float far);

	const float* GetMVPMat() const;
	const Eigen::Vector4f GetRightVec() const;
	const Eigen::Vector4f GetUpVec() const;
	const Eigen::Vector4f GetDirVec() const;

};

