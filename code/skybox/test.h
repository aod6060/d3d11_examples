#pragma once

#include "sys.h"

struct ConstVS
{
	glm::mat4 proj;
	glm::mat4 view;
	glm::mat4 model;
};

struct ConstPS
{
	float tcScale;
	glm::vec3 unused1;
};

class AppTest : public IApp
{
private:
	// Camera
	Camera camera;
	// Shaders
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	// Input Layout
	ID3D11InputLayout* inputLayout = nullptr;

	// Terrain Shader
	TerrainShader terrainShader;

	// Terrain
	Terrain terrain;

	// Terrain Textures
	Texture2D blendMap;
	Texture2D channelBlack;
	Texture2D channelRed;
	Texture2D channelGreen;
	Texture2D channelBlue;

	// Mesh Shader
	MeshShader meshShader;

	// Mesh
	Mesh cube;
	Mesh sphere;
	Mesh cylender;
	Mesh torus;
	Mesh monkey;

	// Texture
	Texture2D example;

	std::vector<Mesh*> meshes;
	uint32_t amount = 0;

	// Const Buffer
	// ConstVS
	ConstVS constVS = {};
	ConstBuffer<ConstVS> constVSBuf;

	// ConstPS
	ConstPS constPS = {};
	ConstBuffer<ConstPS> constPSBuf;

	// Rasterizer
	ID3D11RasterizerState* rastState = nullptr;
	// View Port
	D3D11_VIEWPORT viewport = {};
	// Rotation Values
	float yrot = 0.0f;
	
	//Texture2D sand;

	void initMesh();
	void releaseMesh();

public:
	virtual void init();
	virtual void update(float delta);
	virtual void render();
	virtual void release();
};