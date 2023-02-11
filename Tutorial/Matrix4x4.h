#pragma once

class Vector3;

class Matrix4x4
{
public:
	void SetIdentity();
	void SetTranslation(const Vector3& translation);
	void SetScale(const Vector3& scale);
	void SetRotationX(float x);
	void SetRotationY(float y);
	void SetRotationZ(float z);
	void SetOrthographicProjection(float width, float height, float near_plane, float far_plane);

	void operator*=(const Matrix4x4& matrix);
	void SetMatrix(const Matrix4x4& matrix);

private:
	float m_mat[4][4] = {};
};

