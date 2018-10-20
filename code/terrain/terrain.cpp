#include "sys.h"

void Terrain::init(std::string path)
{
	this->getHeights(path);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<uint32_t> indices;

	// Compute Vertices, TexCoords
	for (uint32_t y = 0; y < this->height; y++)
	{
		for (uint32_t x = 0; x < this->width; x++)
		{
			float hx = this->width / 2.0f;
			float hy = this->height / 2.0f;

			// Vertices
			glm::vec3 v = glm::vec3(x - hx, heights[y * this->width + x] * scale, y - hy);
			vertices.push_back(v);
			// TexCoords
			glm::vec2 tc = glm::vec2(
				(float)x / (float)this->width, 
				(float)y / (float)this->height);
			texCoords.push_back(tc);
		}
	}

	// Indices
	for (uint32_t y = 0; y < this->height - 1; y++)
	{
		for (uint32_t x = 0; x < this->width - 1; x++)
		{
			// Triangle 1
			indices.push_back((y)* this->width + (x));
			indices.push_back((y)* this->width + (x + 1));
			indices.push_back((y + 1)* this->width + (x));
			// Triangle 2
			indices.push_back((y + 1)* this->width + (x));
			indices.push_back((y)* this->width + (x + 1));
			indices.push_back((y + 1)* this->width + (x + 1));
		}
	}

	this->verticeCount = vertices.size();
	this->indexCount = indices.size();

	// Vertices
	this->createBuffer(vertices, &this->verticeBuffer);
	// TexCoords
	this->createBuffer(texCoords, &this->texCoordBuffer);
	// Indices
	this->createBuffer(indices, &this->indicesBuffer);

}

void Terrain::release()
{
	if (indicesBuffer)
	{
		indicesBuffer->Release();
	}
	if (texCoordBuffer)
	{
		texCoordBuffer->Release();
	}
	if (verticeBuffer)
	{
		verticeBuffer->Release();
	}
}

void Terrain::render()
{
	uint32_t stride = sizeof(glm::vec3);
	uint32_t offset = 0;
	rend_getContext()->IASetVertexBuffers(0, 1, &this->verticeBuffer, &stride, &offset);
	stride = sizeof(glm::vec2);
	rend_getContext()->IASetVertexBuffers(1, 1, &this->texCoordBuffer, &stride, &offset);
	rend_getContext()->IASetIndexBuffer(this->indicesBuffer, DXGI_FORMAT_R32_UINT, 0);
	rend_getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	rend_getContext()->DrawIndexed(this->indexCount, 0, 0);
}

void Terrain::getHeights(std::string path)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str());

	if (format == -1)
	{
		std::cout << "Couldn't find image: " << path << std::endl;
		return;
	}

	if (format == FIF_UNKNOWN)
	{
		std::cout << "Couldn't determine file format attempting to get the..." << std::endl;
		format = FreeImage_GetFIFFromFilename(path.c_str());
		if (!FreeImage_FIFSupportsReading(format))
		{
			std::cout << "Detected image format cannot be read!" << std::endl;
		}
	}

	FIBITMAP* bitmap = FreeImage_Load(format, path.c_str());
	int bpp = FreeImage_GetBPP(bitmap);

	FreeImage_FlipVertical(bitmap);

	this->width = FreeImage_GetWidth(bitmap);
	this->height = FreeImage_GetHeight(bitmap);
	//uint8_t* texData = FreeImage_GetBits(bitmap);

	RGBQUAD* texData = (RGBQUAD*)FreeImage_GetBits(bitmap);

	
	this->heights.resize(width * height);

	for (uint32_t i = 0; i < (width * height); i++)
	{
		uint8_t avg = (texData[i].rgbRed + texData[i].rgbGreen + texData[i].rgbBlue) / 3;
		heights[i] = ((float)avg) / 256.0f;
	}

	FreeImage_Unload(bitmap);
}

void Terrain::createBuffer(std::vector<glm::vec2>& vs, ID3D11Buffer** buf)
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(glm::vec2) * vs.size();
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = vs.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT r = rend_getDevice()->CreateBuffer(
		&desc,
		&data,
		buf
	);

	if (FAILED(r))
	{
		std::cout << "Something wrong vec2" << std::endl;
		throw std::runtime_error("");
	}
}

void Terrain::createBuffer(std::vector<glm::vec3>& vs, ID3D11Buffer** buf)
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(glm::vec3) * vs.size();
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = vs.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT r = rend_getDevice()->CreateBuffer(
		&desc,
		&data,
		buf
	);

	if (FAILED(r))
	{
		std::cout << "Something wrong vec3" << std::endl;
		throw std::runtime_error("");
	}
}

void Terrain::createBuffer(std::vector<uint32_t>& is, ID3D11Buffer** buf)
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(uint32_t) * is.size();
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = is.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT r = rend_getDevice()->CreateBuffer(
		&desc,
		&data,
		buf
	);

	if (FAILED(r))
	{
		std::cout << "Something wrong uint32_t" << std::endl;
		throw std::runtime_error("");
	}
}
