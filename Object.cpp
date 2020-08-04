#include "Object.h"

Object::Object()
{
	rtBox = { 0, 0, 0, 0 };
	ptSpinCenter = { 0, 0 };
	dSpinAngle = 0.0;
	pSurface = nullptr;
	pTexture = nullptr;
	rtSrcCut = { 0, 0, -1, -1 };
	bIsOwnSurface = true;
}

Object::~Object()
{
	if (nullptr != pSurface && bIsOwnSurface)
	{
		SDL_FreeSurface(pSurface);
	}

	if (nullptr != pTexture)
	{
		SDL_DestroyTexture(pTexture);
	}
}

bool Object::RenderCopy(SDL_Renderer *pRen)
{
	auto prtSrc = &rtSrcCut;
	auto prtDst = &rtBox;

	if ((rtSrcCut.w + rtSrcCut.h) <= -2)
	{
		prtSrc = nullptr;
	}

	if ((rtBox.w + rtBox.h) <= -2)
	{
		prtDst = nullptr;
	}

	auto ret = SDL_RenderCopyEx(pRen, pTexture, prtSrc, prtDst, dSpinAngle, &ptSpinCenter, SDL_FLIP_NONE);
	if (0 == ret)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Object::LoadFile(const std::string &filename, SDL_Renderer *pRen)
{
	pSurface = IMG_Load(filename.c_str());
	if (pSurface == nullptr)
	{
		return false;
	}

	pTexture = SDL_CreateTextureFromSurface(pRen, pSurface);
	if (pTexture == nullptr)
	{
		SDL_FreeSurface(pSurface);
		pSurface = nullptr;
		return false;
	}

	return true;
}
