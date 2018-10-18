#pragma once

struct Mesh
{
	// Vertices
	ID3D11Buffer* verticesBuffer = nullptr;
	uint32_t verticeCount = 0;
	// TexCoords
	ID3D11Buffer* texCoordBuffer = nullptr;
	// Normals
	ID3D11Buffer* normalsBuffer = nullptr;
	// Index
	ID3D11Buffer* indicesBuffer = nullptr;
	uint32_t indexCount = 0;

	void init(std::string path);
	void release();

	void createBuffer(std::vector<glm::vec2>& vs, ID3D11Buffer** buf);
	void createBuffer(std::vector<glm::vec3>& vs, ID3D11Buffer** buf);
	void createBuffer(std::vector<uint32_t>& is, ID3D11Buffer** buf);

	void render();

};