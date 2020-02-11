/*
iostream
vector
random
windows
sdl
sdl_ttf
*/
#include "pch.h"
#include "Engine.h"

//GraphEngine
SDL_Window* GraphEngine::sm_pWin = nullptr;
SDL_Renderer* GraphEngine::sm_pRen = nullptr;
TTF_Font* GraphEngine::sm_pFont;
int GraphEngine::sm_nWinSizeTimes = 2;
std::vector<GraphEngine::InBuffGraphUnit> GraphEngine::sm_vec_ibguBuff;
//RandomEngine
std::default_random_engine RandomEngine::sm_dre;

bool GraphEngine::Init(void)
{
	//normal sdl init
	//in not secure way

	//init sys
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		return false;
	}

	//init win
	sm_pWin = SDL_CreateWindow(u8"KAG",
							   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							   256 * sm_nWinSizeTimes, 224 * sm_nWinSizeTimes,
							   SDL_WINDOW_SHOWN);
	if (sm_pWin == nullptr)
	{
		return false;
	}

	//init renderer
	sm_pRen = SDL_CreateRenderer(sm_pWin, -1,
								 SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (sm_pRen == nullptr)
	{
		return false;
	}

	//init font
	if (TTF_Init() != 0)
	{
		return false;
	}
	sm_pFont = TTF_OpenFont("C:/Windows/fonts/SIMHEI.TTF", 16);
	if (sm_pFont == nullptr)
	{
		return false;
	}
	//done
	return true;
}

void GraphEngine::ChangeWinSize(int times)
{
	sm_nWinSizeTimes = times;
	SDL_SetWindowSize(sm_pWin, 256 * times, 224 * times);
	SDL_SetWindowPosition(sm_pWin, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void GraphEngine::ChangeWinTitle(const char* title)
{
	SDL_SetWindowTitle(sm_pWin, title);
}

void GraphEngine::AddSquareToBuff(int sq_x, int sq_y, GraphUnit* unit, bool bottom)
{
	int size = 16 * sm_nWinSizeTimes;
	InBuffGraphUnit ibgu;
	ibgu.pgu = unit;
	ibgu.prtDst = new(std::nothrow) SDL_Rect;
	*(ibgu.prtDst) = { sq_x * size,sq_y * size,size,size };
	if (bottom)
	{
		sm_vec_ibguBuff.insert(sm_vec_ibguBuff.begin(), ibgu);
	}
	else
	{
		sm_vec_ibguBuff.push_back(ibgu);
	}
}

void GraphEngine::AddSquareToBuff(int sq_index, GraphUnit* unit, bool bottom)
{
	auto pt = __IndexToPoint(sq_index);
	AddSquareToBuff(pt.x, pt.y, unit, bottom);
}

void GraphEngine::AddToBuff(int x, int y, int w, int h, GraphUnit* unit)
{
	InBuffGraphUnit ibgu;
	ibgu.pgu = unit;
	ibgu.prtDst = new(std::nothrow) SDL_Rect;
	*(ibgu.prtDst) = { x * sm_nWinSizeTimes,y * sm_nWinSizeTimes,
		w * sm_nWinSizeTimes,h * sm_nWinSizeTimes };
	sm_vec_ibguBuff.push_back(ibgu);
}

void GraphEngine::PresentBuff(void)
{
	int square_size = 16 * sm_nWinSizeTimes;

	SDL_RenderClear(sm_pRen);

	//copy
	for (auto& ibgu : sm_vec_ibguBuff)
	{
		SDL_RenderCopy(sm_pRen, ibgu.pgu->pTex, ibgu.pgu->prtSrc, ibgu.prtDst);
	}

	//present
	SDL_RenderPresent(sm_pRen);
}

void GraphEngine::ClearBuff(void)
{
	for (auto& ibgu : sm_vec_ibguBuff)
	{
		delete ibgu.prtDst;
		ibgu.prtDst = nullptr;
	}
	sm_vec_ibguBuff.clear();
}

GraphUnit* GraphEngine::LoadUnit(const char* filename, bool use_key, SDL_Color key)
{
	SDL_Surface* sur = SDL_LoadBMP(filename);
	if (sur == nullptr)
	{
		std::cout << SDL_GetError() << std::endl;
		return nullptr;
	}
	SDL_SetColorKey(sur, use_key, SDL_MapRGB(sur->format, key.r, key.g, key.b));

	SDL_Texture* tex = SDL_CreateTextureFromSurface(sm_pRen, sur);
	SDL_FreeSurface(sur);
	if (tex == nullptr)
	{
		std::cout << SDL_GetError() << std::endl;
		return nullptr;
	}

	auto gu = new(std::nothrow) GraphUnit;//nullptr not worried
	gu->pTex = tex;
	gu->prtSrc = nullptr;

	return gu;
}

GraphUnit* GraphEngine::FontsToUnit(SDL_Point* size, const char* ansi_cstr)
{
	int wcstr_size = MultiByteToWideChar(CP_ACP, 0, ansi_cstr, -1, nullptr, 0);
	WCHAR* wcstr = new(std::nothrow) WCHAR[wcstr_size];
	MultiByteToWideChar(CP_ACP, 0, ansi_cstr, -1, wcstr, wcstr_size);

	auto sur = TTF_RenderUNICODE_Solid(sm_pFont, (Uint16*)wcstr, { 255,255,255 });
	auto tex = SDL_CreateTextureFromSurface(sm_pRen, sur);
	SDL_FreeSurface(sur);

	TTF_SizeUNICODE(sm_pFont, (Uint16*)wcstr, &size->x, &size->y);

	delete[] wcstr;

	auto gu = new(std::nothrow) GraphUnit;
	gu->pTex = tex;
	gu->prtSrc = nullptr;

	return gu;
}

GraphUnitPack* GraphEngine::LoadUnitPack(const char* filename, bool use_key, SDL_Color key)
{
	SDL_Surface* sur = SDL_LoadBMP(filename);
	if (sur == nullptr)
	{
		std::cout << SDL_GetError() << std::endl;
		return nullptr;
	}
	SDL_SetColorKey(sur, use_key, SDL_MapRGB(sur->format, key.r, key.g, key.b));

	SDL_Texture* tex = SDL_CreateTextureFromSurface(sm_pRen, sur);
	if (tex == nullptr)
	{
		std::cout << SDL_GetError() << std::endl;
		return nullptr;
	}

	auto pack = new(std::nothrow) GraphUnitPack;

	for (int y = 0; y != sur->h; y += 16)
	{
		for (int x = 0; x != sur->w; x += 16)
		{
			auto gu = new(std::nothrow) GraphUnit;
			gu->pTex = tex;
			gu->prtSrc = new(std::nothrow) SDL_Rect;
			*(gu->prtSrc) = { x,y,16,16 };
			pack->vec_pguContent.push_back(gu);
		}
	}

	SDL_FreeSurface(sur);
	return pack;
}

void GraphEngine::FreeUnit(GraphUnit* gu)
{
	delete gu->prtSrc;
	SDL_DestroyTexture(gu->pTex);
	delete gu;
	gu = nullptr;
}

void GraphEngine::FreeUnitPack(GraphUnitPack* gup)
{
	//pack's texture need to be deleted once
	SDL_DestroyTexture(gup->vec_pguContent[0]->pTex);

	for (auto& pgu : gup->vec_pguContent)
	{
		delete pgu->prtSrc;
	}

	delete gup;
	gup = nullptr;
}

SDL_Point GraphEngine::__IndexToPoint(int index)
{
	if (index >= SQUARES_COUNT || index < 0)
	{
		//illegal input
		return SDL_Point({ -1,-1 });
	}

	//convert
	int x = index % SQUARES_X_COUNT;
	int y = (index - x) / SQUARES_X_COUNT;

	return SDL_Point({ x,y });
}

int GraphEngine::__PointToIndex(int pt_x, int pt_y)
{
	return (pt_y * SQUARES_X_COUNT) + pt_x;
}

void RandomEngine::Init()
{
	sm_dre.seed((unsigned int)&sm_dre);
}

int RandomEngine::Uniform(int min, int max)
{
	std::uniform_int_distribution<int> d(min, max);
	return d(sm_dre);
}

bool RandomEngine::Half()
{
	std::uniform_int_distribution<int> d(0, 1);
	if (d(sm_dre) == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void RandomEngine::SetNewSeed()
{
	sm_dre.seed(SDL_GetTicks());
}

void RandomEngine::SetNewSeed(unsigned int seed)
{
	sm_dre.seed(seed);
}