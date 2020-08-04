#pragma once

#include "SDL/include/SDL.h"
#include "SDL/include/SDL_image.h"
#include <string>

class Object
{
public:
	Object();
	virtual ~Object();

	virtual bool RenderCopy(SDL_Renderer* pRen);

	// aceept what the SDL_image library supports
	virtual bool LoadFile(const std::string& filename, SDL_Renderer* pRen);

public:
	SDL_Rect rtBox;

	SDL_Point ptSpinCenter;
	double dSpinAngle;

	SDL_Surface* pSurface;
	SDL_Texture* pTexture;
	bool bIsOwnSurface;

	SDL_Rect rtSrcCut;
};
