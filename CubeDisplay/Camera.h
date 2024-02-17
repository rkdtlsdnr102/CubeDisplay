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
	Eigen::Vector4f m_rightvec;// ī�޶� ���� x�� ����
	Eigen::Vector4f m_upvec; // ī�޶� ���� y�� ����
	Eigen::Vector4f m_dirvec; // ī�޶� ���� z�� ����( ���⺤��)

	Eigen::Matrix4f m_MVP;

private:
	void UpdateVec();
	void UpdateMat();

public:
	void Init();
	void LookAt(const Eigen::Vector4f& target, const Eigen::Vector4f& pos, const Eigen::Vector4f& up);
	void Rotate(float theta, float phi);
	void Zoom(float d);
	void Translate(const Eigen::Vector4f &_right, const Eigen::Vector4f &_up);

	const float* GetMVPMat() const;
	const Eigen::Vector4f GetRightVec() const;
	const Eigen::Vector4f GetUpVec() const;
	const Eigen::Vector4f GetDirVec() const;

};
