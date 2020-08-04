#include "AnimationObject.h"

AnimationObject::AnimationObject()
{
	__SetEmptyCallback();
	nFrameCount = 0;
}

void AnimationObject::SetCallback(const std::function<void(int&, SDL_Renderer*, void*)>& cb, void* data)
{
	m_pCallbackData = data;
	m_funcCallback = cb;
}

void AnimationObject::RemoveCallback()
{
	__SetEmptyCallback();
}

bool AnimationObject::LoadFile(const std::string& filename, SDL_Renderer* pRen, int block_size)
{
	if (!Object::LoadFile(filename, pRen))
	{
		throw "Object::LoadFile() failed";
		return false;
	}

	if (pSurface->w % block_size != 0 || pSurface->h % block_size != 0)
	{
		SDL_FreeSurface(pSurface);
		return false;
	}

	for (int y = 0; y < pSurface->h; y += block_size)
	{
		for (int x = 0; x < pSurface->w; x += block_size)
		{
			vecTexSrcRect.push_back({ x,y,block_size,block_size });
		}
	}

	return true;
}

bool AnimationObject::LoadFile(SDL_Surface* surface, SDL_Renderer* pRen, int block_size)
{
	if (nullptr == surface)
	{
		return false;
	}
	pSurface = surface;

	if (pSurface->w % block_size != 0 || pSurface->h % block_size != 0)
	{
		SDL_FreeSurface(pSurface);
		return false;
	}

	for (int y = 0; y < pSurface->h; y += block_size)
	{
		for (int x = 0; x < pSurface->w; x += block_size)
		{
			vecTexSrcRect.push_back({ x,y,block_size,block_size });
		}
	}


	pTexture = SDL_CreateTextureFromSurface(pRen, pSurface);
	bIsOwnSurface = false;

	return true;
}

bool AnimationObject::RenderCopy(SDL_Renderer* pRen)
{
	if (nFrameCount < 0) return false;

	m_funcCallback(nFrameCount, pRen, m_pCallbackData);
	if (pTexture != nullptr && !Object::RenderCopy(pRen))
	{
		//只有当贴图不为null时，渲染失败才是失败
		//如果贴图为null，copy也不会执行
		//throw "渲染失败";
		return false;
	}

	if (nFrameCount >= 0) nFrameCount++;
	return true;
}

void AnimationObject::__SetEmptyCallback()
{
	m_pCallbackData = nullptr;
	m_funcCallback = [](int&, SDL_Renderer*, void*) {};//空闭包，什么都不做
}
