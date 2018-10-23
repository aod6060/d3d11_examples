#pragma once

struct ImageData
{
	FIBITMAP* bitmap;
	void* pixels;
	uint32_t width;
	uint32_t height;
	uint32_t rowPitch;
};


void img_load(std::string path, ImageData* data);

void img_unload(ImageData* data);