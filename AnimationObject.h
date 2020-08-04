#pragma once

#include <functional>
#include <vector>

#include "Object.h"

class AnimationObject :
	public Object
{
public:
	AnimationObject();

	//void callback(int& framecount, SDL_Renderer*, void* data)
	void SetCallback(const std::function<void(int&, SDL_Renderer*, void*)>& cb, void* data);

	void RemoveCallback();

	virtual bool LoadFile(const std::string& filename, SDL_Renderer* pRen, int block_size);
	virtual bool LoadFile(SDL_Surface* surface, SDL_Renderer* pRen, int block_size);

	bool RenderCopy(SDL_Renderer* pRen) override;

public:
	//将此值设置为负，标志为等待删除
	int nFrameCount;

	//一张贴图上，每帧对应的位置
	std::vector<SDL_Rect> vecTexSrcRect;

protected:
	void __SetEmptyCallback();

protected:
	std::function<void(int&, SDL_Renderer*, void*)> m_funcCallback;
	void* m_pCallbackData;
};

