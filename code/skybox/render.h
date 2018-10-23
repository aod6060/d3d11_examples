#pragma once



void rend_init();
void rend_release();

void rend_clear(const glm::vec4& color, float depth);
void rend_present();

ID3D11Device* rend_getDevice();
ID3D11DeviceContext* rend_getContext();

template<typename T>
struct ConstBuffer
{
	T value;
	ID3D11Buffer* buf = nullptr;

	void set(T& value)
	{
		this->value = value;
	}

	void init()
	{
		D3D11_BUFFER_DESC bufDesc = {};
		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.ByteWidth = sizeof(T);
		bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufDesc.MiscFlags = 0;
		bufDesc.StructureByteStride = 0;

		HRESULT r = rend_getDevice()->CreateBuffer(
			&bufDesc,
			nullptr,
			&this->buf
		);

		if (FAILED(r))
		{
			std::cout << "Failed to create Const PS Buffer" << std::endl;
			throw std::runtime_error("");
		}
	}

	void release()
	{
		if (buf)
		{
			buf->Release();
		}
	}

	void update()
	{
		D3D11_MAPPED_SUBRESOURCE mapped = {};
		rend_getContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
		memcpy(mapped.pData, &this->value, sizeof(T));
		rend_getContext()->Unmap(buf, 0);
	}

};

struct Texture2D
{
	ID3D11Texture2D* tex = nullptr;
	ID3D11ShaderResourceView* resView = nullptr;
	ID3D11SamplerState* sampState = nullptr;

	void init(std::string path);
	void release();

	void bind(int inputState);
};

struct ImageInfo
{
	void* pixels;
	uint32_t rowPitch;
	uint32_t width;
	uint32_t height;
};

struct CubeMap
{
	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* shaderRes = nullptr;
	ID3D11SamplerState* sampState = nullptr;

	// Takes in a vector with size of 6
	void init(std::vector<std::string>& paths);
	void release();

	void bind(int inputState);

	void loadImageInfo(ImageInfo* imageInfo, std::string path);


};