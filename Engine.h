#pragma once

//供用户使用的单元，请使用LoadUnit()获取
struct GraphUnit
{
	SDL_Rect* prtSrc;
	SDL_Texture* pTex;
};

//用vector封装的一系列GraphUnit的Pack，请使用LoadUnitPack()获取
struct GraphUnitPack
{
	std::vector<GraphUnit*> vec_pguContent;
};

//支持网格和直接坐标的图像引擎
//注意：无论如何放大，有效的坐标都是256*224
class GraphEngine
{
public:
	//x轴（横向）有多少个方块
	static const int SQUARES_X_COUNT = 16;
	//y轴（竖向）有多少个方块
	static const int SQUARES_Y_COUNT = 14;
	//方块总数
	static const int SQUARES_COUNT = SQUARES_X_COUNT * SQUARES_Y_COUNT;

public:
	//初始化这个系统，请务必在加载资源前调用
	static bool Init(void);

	//改变窗口的放大倍数，最好是2的倍数
	static void ChangeWinSize(int times);

	//改变窗口标题
	static void ChangeWinTitle(const char* utf8_title);

	//将一个单元以网格的方式复制到buff，sq的范围是x0-15 y0-13
	//默认insert加到最底下以免覆盖
	static void AddSquareToBuff(int sq_x, int sq_y, GraphUnit* unit, bool bottom = true);

	//将一个单元以网格的方式复制到buff，index是网格的序列号
	//默认insert加到最前以免覆盖
	static void AddSquareToBuff(int sq_index, GraphUnit* unit, bool bottom = true);

	//直接坐标复制到buff，坐标的范围在256*224内
	static void AddToBuff(int x, int y, int w, int h, GraphUnit* unit);

	//呈现buff，但不清空buff
	static void PresentBuff(void);

	//清空buff
	static void ClearBuff(void);

	//从文件中加载graphunit，在需要被渲染时不要清理，不用时请记得清理
	static GraphUnit* LoadUnitFromFile(const char* filename, bool use_key = false,
									   SDL_Color key = { 0,255,0,255 });

	//从内存加载graphunit,在需要被渲染时不要清理，不用时请记得清理
	static GraphUnit* LoadUnitFromMem(void* mem, unsigned int mem_size,
									  bool use_key = false,
									  SDL_Color key = { 0,255,0,255 });

	//将一个字符串转换为GraphUnit
	//第一个参数返回一个等比的绘制大小，高为16
	static GraphUnit* StringToUnit(SDL_Point* size, const char* ansi_cstr);

	//从文件中加载graphunit，在需要被渲染时不要清理，不用时请记得清理
	//这个文件被切分成16长的小正方形按顺序存放
	static GraphUnitPack* LoadSqUnitPackFromFile(const char* filename, bool use_key = false,
												 SDL_Color key = { 0,255,0,255 });

	//从内存中加载graphunit，在需要被渲染时不要清理，不用时请记得清理
	//这个文件被切分成16长的小正方形按顺序存放
	static GraphUnitPack* LoadSqUnitPackFromMem(void* mem, unsigned int mem_size,
												bool use_key = false,
												SDL_Color key = { 0,255,0,255 });

	static void FreeUnit(GraphUnit* gu);

	static void FreeUnitPack(GraphUnitPack* gup);

private:
	static SDL_Window* sm_pWin;
	static SDL_Renderer* sm_pRen;

	//times of 160:90
	static int sm_nWinSizeTimes;

	static TTF_Font* sm_pFont;

	//带有最终坐标的graphunit
	struct InBuffGraphUnit
	{
		SDL_Rect* prtDst;
		GraphUnit* pgu;
	};

	//buff的队列
	static std::vector<InBuffGraphUnit> sm_vec_ibguBuff;

private:
	static SDL_Point __IndexToPoint(int index);
	static int __PointToIndex(int pt_x, int pt_y);
};

//随机数产生器，封装起来避免问题
class RandomEngine
{
public:
	static void Init();
	static int UniformRange(int min, int max);
	static bool Half();
	//使用sdl的gettick重新设置，如果sdl未初始化可能会有问题
	static void SetNewSeed();
	//设置新的种子
	static void SetNewSeed(unsigned int seed);
private:
	static std::default_random_engine sm_dre;
};