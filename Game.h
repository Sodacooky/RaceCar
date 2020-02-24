#pragma once

class Car
{
public:
	Car(GraphUnitPack* pgup);
	bool IsPlayer();
	void IsPlayer(bool is);
	void AddToBuff();
	//相对移动
	void Move(int x, int y);
	//移动到
	void MoveTo(int x, int y);
	SDL_Point& GetPos();
	//设置我的贴图id
	void SetTexId(int id);
private:
	SDL_Point m_ptPos;
	bool m_bIsPlayer;
	GraphUnitPack* m_pgup;
	int m_nTexId;
private:
};

class Game
{
public:
	static int Run();
private:
	//存放所有用到的贴图pack
	static GraphUnitPack* sm_pgupTex;
	//当前游戏分数
	static unsigned int sm_unScore;
	//游戏是否还在游戏中
	static bool sm_bIsInRace;///////unuse now
	//游戏背景地图，游戏结束后要清空因为要用大小来判断是不是新游戏
	//坐标格式是"x y"
	static std::map<std::string, int> sm_map_strPos_nTexId;
	//游戏中所有的车
	static std::vector<Car> sm_vec_car;
	//难度因子
	static unsigned int sm_unDifficulty;
	//sdl事件
	static SDL_Event sm_msg;
private:
	//初始化
	static bool __Init();

	//开始界面
	static int __StartPage();

	//正式游戏返回分数
	static int __InRace();
	//绘制车
	static void __DrawAllCar();
	//绘制背景
	static void __DrawBackground();
	//更新背景绘图，在timer中
	static void __UpdateBackground();
	//生成路上的车子，在timer中
	static void __GenerateOnRoadCar();
	//处理离开屏幕的车子,在timer中
	static void __DeleteOutsideOnRoadCar();
	//移动路上的车子，在timer中
	static void __MoveOnRoadCar();
	//控制玩家的车子
	static void __ControlPlayerCar();
	//判断游戏结束
	static bool __IsThereCollision();
	//背后按周期运行的生成和移动路上的车子
	//sdl timer callback
	static Uint32 __CycleCB(Uint32 interval, void* praram);
	//
	static void __FinshedBGMCB();

	//结算界面
	static int __EndPage();

	//字符串"x y"转换为坐标
	static void __StringToPos(const char* cstr, SDL_Point& ptPos);
	//将坐标转换成字符串"x y"
	static void __PosToString(std::string& str, SDL_Point ptPos);
	//将数字转换为字符串
	static void __IntToString(std::string& str, int num);
};