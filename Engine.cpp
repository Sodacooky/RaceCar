/*
iostream
vector
random
windows
sdl
sdl_ttf
sdl_mixer
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
	sm_pFont = TTF_OpenFont("C:/Windows/fonts/arial.ttf", 64);
	if (sm_pFont == nullptr)
	{
		//无法打开则提供第二选择，在exe文件夹下的font.ttf文件
		std::cout << "无法打开C:/Windows/fonts/arial.ttf，正在尝试打开程序目录下的font.ttf..." << std::endl;
		sm_pFont = TTF_OpenFont("font.ttf", 16);
		if (sm_pFont == nullptr)
		{
			std::cout << "无法打开程序目录下的font.ttf..." << std::endl;
			return false;
		}
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
	if (ibgu.prtDst == nullptr)
	{
		std::cout << "AddSquareToBuff() failed to allocate mem for ibgu.prtDst" << std::endl;
		return;
	}
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
	if (ibgu.prtDst == nullptr)
	{
		std::cout << "AddSquareToBuff() failed to allocate mem for ibgu.prtDst" << std::endl;
		return;
	}
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

GraphUnit* GraphEngine::LoadUnitFromFile(const char* filename, bool use_key, SDL_Color key)
{
	SDL_Surface* sur = SDL_LoadBMP(filename);
	if (sur == nullptr)
	{
		std::cout << "LoadUnitFromFile() couldn't load file: "
			<< SDL_GetError() << std::endl;
		return nullptr;
	}

	SDL_SetColorKey(sur, use_key, SDL_MapRGB(sur->format, key.r, key.g, key.b));
	SDL_Texture* tex = SDL_CreateTextureFromSurface(sm_pRen, sur);
	if (tex == nullptr)
	{
		std::cout << "LoadUnitFromFile() failed to create sdl_texture: "
			<< SDL_GetError() << std::endl;
		SDL_FreeSurface(sur);
		return nullptr;
	}

	SDL_FreeSurface(sur);

	auto pgu = new(std::nothrow) GraphUnit;//nullptr not worried
	if (pgu == nullptr)
	{
		std::cout << "LoadUnitFromMem() couldb't allocate mem for graphunit" << std::endl;
		SDL_DestroyTexture(tex);
		return nullptr;
	}
	pgu->pTex = tex;
	pgu->prtSrc = nullptr;

	return pgu;
}

GraphUnit* GraphEngine::LoadUnitFromMem(void* mem, unsigned int mem_size, bool use_key, SDL_Color key)
{
	if (mem == nullptr)
	{
		std::cout << "LoadUnitFromMem() null mem pointer" << std::endl;
		return nullptr;
	}

	SDL_RWops* rw = SDL_RWFromMem(mem, mem_size);
	if (rw == nullptr)
	{
		std::cout << "LoadUnitFromMem() failed read mem" << std::endl;
		return nullptr;
	}

	SDL_Surface* sur = SDL_LoadBMP_RW(rw, true);//stream closed?
	if (sur == nullptr)
	{
		std::cout << "LoadUnitFromMem() failed to create sdL_surface: "
			<< SDL_GetError() << std::endl;
		return nullptr;
	}

	SDL_SetColorKey(sur, use_key, SDL_MapRGB(sur->format, key.r, key.g, key.b));
	SDL_Texture* tex = SDL_CreateTextureFromSurface(sm_pRen, sur);
	if (tex == nullptr)
	{
		std::cout << "LoadUnitFromMem() failed to create sdl_texture: "
			<< SDL_GetError() << std::endl;
		SDL_FreeSurface(sur);
		SDL_RWclose(rw);
		return nullptr;
	}

	SDL_FreeSurface(sur);
	SDL_RWclose(rw);

	auto pgu = new(std::nothrow) GraphUnit;
	if (pgu == nullptr)
	{
		std::cout << "LoadUnitFromMem() couldn't allocate mem for graphunit" << std::endl;
		SDL_DestroyTexture(tex);
		return nullptr;
	}
	pgu->prtSrc = nullptr;
	pgu->pTex = tex;

	return pgu;
}

GraphUnit* GraphEngine::StringToUnit(SDL_Point* size, const char* ansi_cstr)
{
	int wcstr_size = MultiByteToWideChar(CP_ACP, 0, ansi_cstr, -1, nullptr, 0);
	WCHAR* wcstr = new(std::nothrow) WCHAR[wcstr_size];
	if (wcstr == nullptr)
	{
		std::cout << "StringToUnit() couldn't allocate mem for wcstr" << std::endl;
		return nullptr;
	}
	MultiByteToWideChar(CP_ACP, 0, ansi_cstr, -1, wcstr, wcstr_size);

	auto sur = TTF_RenderUNICODE_Solid(sm_pFont, (Uint16*)wcstr, { 255,255,255 });
	auto tex = SDL_CreateTextureFromSurface(sm_pRen, sur);
	SDL_FreeSurface(sur);

	TTF_SizeUNICODE(sm_pFont, (Uint16*)wcstr, &size->x, &size->y);
	size->x /= 4;
	size->y /= 4;

	delete[] wcstr;

	auto gu = new(std::nothrow) GraphUnit;
	if (gu == nullptr)
	{
		std::cout << "StringToUnit() couldn't allocate mem for graphunit" << std::endl;
		SDL_DestroyTexture(tex);
		return nullptr;
	}
	gu->pTex = tex;
	gu->prtSrc = nullptr;

	return gu;
}

GraphUnitPack* GraphEngine::LoadSqUnitPackFromFile(const char* filename, bool use_key, SDL_Color key)
{
	SDL_Surface* sur = SDL_LoadBMP(filename);
	if (sur == nullptr)
	{
		std::cout << "LoadSqUnitPackFromFile() couldn;t load file: "
			<< SDL_GetError() << std::endl;
		return nullptr;
	}
	SDL_SetColorKey(sur, use_key, SDL_MapRGB(sur->format, key.r, key.g, key.b));

	SDL_Texture* tex = SDL_CreateTextureFromSurface(sm_pRen, sur);
	if (tex == nullptr)
	{
		std::cout << "LoadSqUnitPackFromFile() failed to create sdl_surface"
			<< SDL_GetError() << std::endl;
		return nullptr;
	}

	auto pPack = new(std::nothrow) GraphUnitPack;
	if (pPack == nullptr)
	{
		std::cout << "LoadSqUnitPackFromFile() failed to allocate mem for graphunitpack" << std::endl;
		SDL_FreeSurface(sur);
		SDL_DestroyTexture(tex);
		return nullptr;
	}

	for (int y = 0; y != sur->h; y += 16)
	{
		for (int x = 0; x != sur->w; x += 16)
		{
			auto pgu = new(std::nothrow) GraphUnit;
			if (pgu == nullptr)
			{
				std::cout
					<< "LoadSqUnitPackFromFile() failed to allocate mem for graphunit"
					<< std::endl;
			}
			else
			{
				pgu->pTex = tex;
				pgu->prtSrc = new(std::nothrow) SDL_Rect;
				*(pgu->prtSrc) = { x,y,16,16 };
				pPack->vec_pguContent.push_back(pgu);
			}
		}
	}

	SDL_FreeSurface(sur);
	return pPack;
}

GraphUnitPack* GraphEngine::LoadSqUnitPackFromMem(void* mem, unsigned int mem_size, bool use_key, SDL_Color key)
{
	if (mem == nullptr)
	{
		std::cout << "LoadSqUnitPackFromMem() null mem pointer" << std::endl;
		return nullptr;
	}

	SDL_RWops* rw = SDL_RWFromMem(mem, mem_size);
	if (rw == nullptr)
	{
		std::cout << "LoadSqUnitPackFromMem() failed to read mem" << std::endl;
		return nullptr;
	}

	SDL_Surface* sur = SDL_LoadBMP_RW(rw, true);//stream closed?
	if (sur == nullptr)
	{
		std::cout << "LoadSqUnitPackFromFile() couldn't load file: "
			<< SDL_GetError() << std::endl;
		return nullptr;
	}
	SDL_SetColorKey(sur, use_key, SDL_MapRGB(sur->format, key.r, key.g, key.b));

	SDL_Texture* tex = SDL_CreateTextureFromSurface(sm_pRen, sur);
	if (tex == nullptr)
	{
		std::cout << "LoadSqUnitPackFromFile() failed to create sdl_surface"
			<< SDL_GetError() << std::endl;
		return nullptr;
	}

	auto pPack = new(std::nothrow) GraphUnitPack;
	if (pPack == nullptr)
	{
		std::cout << "LoadSqUnitPackFromFile() failed to allocate mem for graphunitpack" << std::endl;
		SDL_FreeSurface(sur);
		SDL_DestroyTexture(tex);
		return nullptr;
	}

	for (int y = 0; y != sur->h; y += 16)
	{
		for (int x = 0; x != sur->w; x += 16)
		{
			auto pgu = new(std::nothrow) GraphUnit;
			if (pgu == nullptr)
			{
				std::cout
					<< "LoadSqUnitPackFromFile() failed to allocate mem for graphunit"
					<< std::endl;
			}
			else
			{
				pgu->pTex = tex;
				pgu->prtSrc = new(std::nothrow) SDL_Rect;
				*(pgu->prtSrc) = { x,y,16,16 };
				pPack->vec_pguContent.push_back(pgu);
			}
		}
	}

	SDL_FreeSurface(sur);
	return pPack;
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

int RandomEngine::UniformRange(int min, int max)
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