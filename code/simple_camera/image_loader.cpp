#include "sys.h"


void img_load(std::string path, ImageData* data)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str());

	if (format == -1)
	{
		throw std::runtime_error("Couldn't load image file");
	}

	if (format == FIF_UNKNOWN)
	{
		format = FreeImage_GetFIFFromFilename(path.c_str());
		if (!FreeImage_FIFSupportsReading(format))
		{
			throw std::runtime_error("Can't read image file due to corrution");
		}
	}

	FIBITMAP* temp = FreeImage_Load(format, path.c_str());
	int bpp = FreeImage_GetBPP(temp);

	FIBITMAP* temp32;

	if (bpp == 32)
	{
		temp32 = temp;
	}
	else
	{
		temp32 = FreeImage_ConvertTo32Bits(temp);
	}

	FreeImage_FlipVertical(temp32);

	data->bitmap = temp32;
	data->pixels = (void*)FreeImage_GetBits(data->bitmap);
	data->width = FreeImage_GetWidth(data->bitmap);
	data->height = FreeImage_GetHeight(data->bitmap);
	data->rowPitch = data->width * 4;

	if (bpp != 32)
	{
		FreeImage_Unload(temp);
	}

	temp32 = nullptr;
	temp = nullptr;
}

void img_unload(ImageData* data)
{
	FreeImage_Unload(data->bitmap);
	data->pixels = nullptr;
}