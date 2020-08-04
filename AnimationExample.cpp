// AnimationExample.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <random>
#include <cmath>
#include "SDL/include/SDL.h"
#include "AnimationObject.h"

//C式函数型
//画一个圆渐变圆
void ZoomCircle(int& frameCount, SDL_Renderer* pRen, void* _this)
{
	auto self = (AnimationObject*)_this;

	if (frameCount >= 120)
	{
		frameCount = -1;
		return;
	}

	SDL_Rect tmpRect;
	tmpRect.w = frameCount;
	tmpRect.h = tmpRect.w;
	tmpRect.x = self->rtBox.x - tmpRect.w / 2;
	tmpRect.y = self->rtBox.y - tmpRect.h / 2;


	int brightness = 255;
	if (frameCount > 60)
	{
		brightness = 255 - 2 * (frameCount - 61);
	}
	SDL_SetRenderDrawColor(pRen, brightness, brightness, brightness, 255);
	SDL_RenderDrawRect(pRen, &tmpRect);
	SDL_SetRenderDrawColor(pRen, 0, 0, 0, 255);

	std::cout << "ZoomCircle frame: " << frameCount << std::endl;
}

int main(int argc, char* argv[])
{
	auto pWin = SDL_CreateWindow(u8"AnimationExample",
								 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
								 800, 600,
								 SDL_WINDOW_SHOWN);

	auto pRen = SDL_CreateRenderer(pWin, -1,
								   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//闭包形式
	//动画帧
	auto PlayList = [](int& frameCount, SDL_Renderer* pRen, void* _this)
	{
		auto self = (AnimationObject*)_this;

		if (frameCount / 10 >= self->vecTexSrcRect.size())
		{
			frameCount = -1;
			return;
		}
		std::cout << "PlayList frame: " << frameCount << std::endl;
		self->rtSrcCut = self->vecTexSrcRect[frameCount / 10];
	};

	std::vector<AnimationObject*> animeObjPool;

	SDL_Surface* list_sur = IMG_Load("list.png");

	SDL_Event msg;
	while (true)
	{
		if (SDL_PollEvent(&msg) == 0)
		{
			SDL_zero(msg);
		}

		if (msg.type == SDL_QUIT)
		{
			for (auto pAnime : animeObjPool)
			{
				delete pAnime;
			}
			break;
		}

		if (msg.type == SDL_MOUSEMOTION)
		{
			SDL_FlushEvent(SDL_MOUSEMOTION);
		}

		if (msg.type == SDL_MOUSEBUTTONDOWN)
		{
			if (msg.button.button == SDL_BUTTON_LEFT)
			{
				auto circleAnime = new AnimationObject;
				circleAnime->rtBox = { msg.button.x,msg.button.y,0,0 };
				circleAnime->SetCallback(ZoomCircle, circleAnime);
				animeObjPool.push_back(circleAnime);
			}
			else if (msg.button.button == SDL_BUTTON_RIGHT)
			{
				auto listAnime = new AnimationObject;
				listAnime->LoadFile(list_sur, pRen, 32);
				listAnime->rtBox = { msg.button.x,msg.button.y,64,64 };
				listAnime->SetCallback(PlayList, listAnime);
				animeObjPool.push_back(listAnime);
			}
		}

		SDL_RenderClear(pRen);
		for (auto iter = animeObjPool.begin(); iter != animeObjPool.end();/* iter++ */)
		{
			if ((**iter).nFrameCount < 0)
			{
				delete *iter;
				iter = animeObjPool.erase(iter);
			}
			else
			{
				if (!(**iter).RenderCopy(pRen))
				{
					std::cout << "有为false\n";
				}
				++iter;
			}
		}
		SDL_RenderPresent(pRen);

	}
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧:
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5.
//   转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
