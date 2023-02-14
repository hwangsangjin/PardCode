#include "MeshVertex.h"
#include "Vector2.h"
#include "Vector3.h"

MeshVertex::MeshVertex(Vector3 position, Vector2 texcoord)
	: m_position(position)
	, m_texcoord(texcoord)
{
}

MeshVertex::MeshVertex(const MeshVertex& vertex_mesh)
	: m_position(vertex_mesh.GetPosition())
	, m_texcoord(vertex_mesh.GetTexcoord())
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
