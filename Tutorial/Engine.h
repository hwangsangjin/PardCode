#pragma once

#include "Prerequisites.h"
#include "Graphics.h"
#include "TextureManager.h"
#include "MeshManager.h"

class Engine
{
public:
	static void Create();
	static void Release();
	static Engine* GetInstance();

	Graphics* GetGraphics();
	TextureManager* GetTextureManager();
	MeshManager* GetMeshManager();
	void GetMeshVertexLayoutShaderByteCodeAndSize(void** byte_code, size_t* size);

private:
	Engine();
	~Engine();

	static Engine* m_engine;
	Graphics* m_graphics = nullptr;
	TextureManager* m_texture_manager = nullptr;
	MeshManager* m_mesh_manager = nullptr;

	unsigned char m_mesh_vertex_layout_byte_code[1024];
	size_t m_mesh_vertex_layout_size = 0;
};

