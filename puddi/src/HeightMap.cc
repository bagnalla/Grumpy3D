//#include "HeightMap.h"
//#include <SDL\SDL_image.h>
//#include <iostream>
//
//namespace puddi
//{
//	// PUBLIC
//
//	HeightMap* HeightMap::CreateHeightMapFromFile(std::string fileName)
//	{
//		SDL_Surface *surface = IMG_Load(fileName.c_str());
//
//		if (surface == NULL)
//		{
//			std::cerr << "unable to load file " << fileName << std::endl;
//			return nullptr;
//		}
//
//		unsigned char *pixels = (unsigned char*)surface->pixels;
//		int bytesPerPixel = surface->format->BytesPerPixel;
//		int maxHeight = INT_MIN;
//
//		for (int i = 0; i < surface->h; ++i)
//		{
//			for (int j = 0; j < surface->w; ++j)
//			{
//				unsigned char height = pixels[(i * surface->w + j) * bytesPerPixel];
//				if (height > maxHeight)
//					maxHeight = height;
//				vertices.push_back(vec4(i, j, height * scaleZ, 1.0f));
//			}
//		}
//	}
//
//	HeightMap::HeightMap(size_t w, size_t h)
//	{
//		heights = new unsigned char*[height];
//		for (int i = 0; i < height; ++i)
//			heights[i] = new unsigned char[width];
//	}
//
//	HeightMap::~HeightMap()
//	{
//		for (int i = 0; i < height; ++i)
//			delete[] heights[i];
//		delete[] heights;
//	}
//
//	// PRIVATE
//
//}
