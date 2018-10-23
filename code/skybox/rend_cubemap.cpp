#include "sys.h"


// Takes in a vector with size of 6
void CubeMap::init(std::vector<std::string>& paths)
{

}

void CubeMap::release()
{

}

void CubeMap::bind(int inputState)
{

}

void CubeMap::loadImageInfo(
	ImageInfo* imageInfo, 
	std::string path)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str());

	if (format == -1)
	{
		std::cout << "Failed to find image type: " << path << std::endl;
		throw std::runtime_error("");
	}

	if (format == FIF_UNKNOWN)
	{
		std::cout << "Couldn't determine file format attempt..." << std::endl;
		format = FreeImage_GetFIFFromFilename(path.c_str());
		if (!FreeImage_FIFSupportsReading(format))
		{

		}
	}
}

