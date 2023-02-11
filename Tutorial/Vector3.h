#pragma once

class Vector3
{
public:
	Vector3() = default;
	Vector3(float x, float y, float z);
	Vector3(const Vector3& vector);

	static Vector3 LinearInterpolation(const Vector3& start, const Vector3& end, float delta);

	float GetX() const;
	float GetY() const;
	float GetZ() const;

private:
	float m_x = 0;
	float m_y = 0;
	float m_z = 0;
};

