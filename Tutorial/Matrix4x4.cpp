#include "Matrix4x4.h"
#include <memory>
#include "Vector3.h"

void Matrix4x4::SetIdentity()
{
	::memset(m_mat, 0, sizeof(float) * 16);
	m_mat[0][0] = 1;
	m_mat[1][1] = 1;
	m_mat[2][2] = 1;
	m_mat[3][3] = 1;
}

void Matrix4x4::SetTranslation(const Vector3& translation)
{
	SetIdentity();
	m_mat[3][0] = translation.GetX();
	m_mat[3][1] = translation.GetY();
	m_mat[3][2] = translation.GetZ();
}

void Matrix4x4::SetScale(const Vector3& scale)
{
	SetIdentity();
	m_mat[0][0] = scale.GetX();
	m_mat[1][1] = scale.GetY();
	m_mat[2][2] = scale.GetZ();
}

void Matrix4x4::SetOrthographicProjection(float width, float height, float near_plane, float far_plane)
{
	SetIdentity();
	m_mat[0][0] = 2.0f / width;
	m_mat[1][1] = 2.0f / height;
	m_mat[2][2] = 1.0f / (far_plane - near_plane);
	m_mat[3][2] = -(near_plane / (far_plane - near_plane));
}

void Matrix4x4::operator*=(const Matrix4x4& matrix)
{
	Matrix4x4 out;

	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			out.m_mat[i][j] =
				m_mat[i][0] * matrix.m_mat[0][j] +
				m_mat[i][1] * matrix.m_mat[1][j] +
				m_mat[i][2] * matrix.m_mat[2][j] +
				m_mat[i][3] * matrix.m_mat[3][j];
		}
	}

	::memcpy(m_mat, out.m_mat, sizeof(float) * 16);
}
