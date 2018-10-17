#pragma once

#include "sys.h"

struct ConstVS
{
	glm::mat4 proj;
	glm::mat4 view;
	glm::mat4 model;
};

class AppTest : public IApp
{
private:
	// Shaders
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	// Input Layout
	ID3D11InputLayout* inputLayout = nullptr;
	// Buffers
	// Vertices
	std::vector<glm::vec3> vertices = {
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, -1.0f, 0.0f)
	};
	ID3D11Buffer* vertexBuffer = nullptr;
	// Const Buffer
	ID3D11Buffer* constVSBuffer = nullptr;
	// Rasterizer
	ID3D11RasterizerState* rastState = nullptr;
	// View Port
	D3D11_VIEWPORT viewport = {};
	// Rotation Values
	float yrot = 0.0f;

	void initShader();
	void releaseShader();

	void initBuffer();
	void releaseBuffer();

public:
	virtual void init();
	virtual void update(float delta);
	virtual void render();
	virtual void release();
};