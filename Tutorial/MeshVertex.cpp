#include "MeshVertex.h"
#include "Vector2.h"
#include "Vector3.h"

MeshVertex::MeshVertex(Vector3 position, Vector2 texcoord, Vector3 normal)
	: m_position(position)
	, m_texcoord(texcoord)
	, m_normal(normal)
{
}

MeshVertex::MeshVertex(const MeshVertex& vertex_mesh)
	: m_position(vertex_mesh.GetPosition())
	, m_texcoord(vertex_mesh.GetTexcoord())
	, m_normal(vertex_mesh.GetNormal())
{
}

Vector3 MeshVertex::GetPosition() const
{
	return m_position;
}

Vector2 MeshVertex::GetTexcoord() const
{
	return m_texcoord;
}

Vector3 MeshVertex::GetNormal() const
{
	return m_normal;
}