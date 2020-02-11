#include "pch.h"
#include "Game.h"

GraphUnitPack* Game::sm_pgupTex = nullptr;
unsigned int Game::sm_unScore = 0;
bool Game::sm_bIsInRace = false;
std::map<std::string, int> Game::sm_map_strPos_nTexId;
unsigned int Game::sm_unDifficulty = 1;
std::vector<Car> Game::sm_vec_car;
SDL_Event Game::sm_msg;

int Game::Run()
{
	if (!__Init())
	{
		return -1;
	}

	bool isRetry = false;
	while (true)
	{
		if (isRetry)
		{
			__InRace();
			if (__EndPage() == 0)
			{
				isRetry = true;
			}
			else
			{
				break;
			}
		}
		else
		{
			if (__StartPage() == 0)
			{
				isRetry = true;
			}
			else
			{
				break;
			}
		}
	}

	GraphEngine::FreeUnitPack(sm_pgupTex);
	return 0;
}

bool Game::__Init()
{
	GraphEngine::Init();
	RandomEngine::Init();

	GraphEngine::ChangeWinTitle(u8"��������");
	GraphEngine::ChangeWinSize(3);
	sm_pgupTex = GraphEngine::LoadUnitPack("Res/texture.bmp", true, { 255,255,255,255 });
	return true;
}

int Game::__StartPage()
{
	auto start_pgu = GraphEngine::LoadUnit("Res/startpage_start_gb.bmp", true);
	auto quit_pgu = GraphEngine::LoadUnit("Res/startpage_quit_gb.bmp", true);
	auto cursor_pgu = GraphEngine::LoadUnit("Res/cursor_gb.bmp", true);

	int nowSelecting = 0;

	bool keyholding = false;
	while (true)
	{
		GraphEngine::ClearBuff();

		GraphEngine::AddToBuff(96, 128, 64, 16, start_pgu);
		GraphEngine::AddToBuff(96, 144, 64, 16, quit_pgu);
		GraphEngine::AddToBuff(80, 128 + nowSelecting * 16, 16, 16, cursor_pgu);

		SDL_PollEvent(&sm_msg);
		SDL_Delay(1);

		if (sm_msg.type == SDL_QUIT)
		{
			exit(0);
		}

		if (keyholding)
		{
			if (sm_msg.type == SDL_KEYUP)
			{
				switch (sm_msg.key.keysym.sym)
				{
				case SDLK_DOWN:
				case SDLK_UP:
					keyholding = false;
				default:
					break;
				}
			}
		}
		else
		{
			if (sm_msg.type == SDL_KEYDOWN)
			{
				if (sm_msg.key.keysym.sym == SDLK_DOWN)
				{
					nowSelecting++;
					keyholding = true;
				}
				else if (sm_msg.key.keysym.sym == SDLK_UP)
				{
					nowSelecting--;
					keyholding = true;
				}
				else if (sm_msg.key.keysym.sym == SDLK_RETURN)
				{
					GraphEngine::FreeUnit(start_pgu);
					GraphEngine::FreeUnit(quit_pgu);
					GraphEngine::FreeUnit(cursor_pgu);
					return nowSelecting;
				}

				if (nowSelecting >= 2)
				{
					nowSelecting = 0;
				}
				else if (nowSelecting <= -1)
				{
					nowSelecting = 1;
				}
			}
		}

		GraphEngine::PresentBuff();
	}

	return -1;
}

int Game::__InRace()
{
	//��ʼ����Ϸ
	sm_unScore = 0;
	sm_unDifficulty = 1;
	sm_vec_car.clear();
	sm_vec_car.emplace_back(sm_pgupTex);
	sm_vec_car[0].MoveTo(7, 13);

	auto timer = SDL_AddTimer(250, __CycleCB, nullptr);

	while (true)
	{
		GraphEngine::ClearBuff();

		__DrawBackground();
		__DrawAllCar();

		GraphEngine::PresentBuff();

		if (__IsThereCollision())
		{
			break;
		}
		sm_unScore += 1;

		SDL_PollEvent(&sm_msg);
		SDL_Delay(1);
		if (sm_msg.type == SDL_QUIT)
		{
			exit(0);
		}

		__ControlPlayerCar();
	}

	SDL_RemoveTimer(timer);
	sm_map_strPos_nTexId.clear();
	return 	0;
}

void Game::__DrawAllCar()
{
	for (auto& car : sm_vec_car)
	{
		car.AddToBuff();
	}
}

void Game::__DrawBackground()
{
	SDL_Point tmpPos;
	for (auto iter = sm_map_strPos_nTexId.begin(); iter != sm_map_strPos_nTexId.end(); iter++)
	{
		__StringToPos(iter->first.c_str(), tmpPos);
		GraphEngine::AddSquareToBuff(tmpPos.x, tmpPos.y,
									 sm_pgupTex->vec_pguContent[iter->second]);
	}
}

void Game::__UpdateBackground()
{
	std::string tmpStr;
	SDL_Point tmpPos;

	//����Ϸ�ĳ��θ�ֵ
	if (sm_map_strPos_nTexId.size() < GraphEngine::SQUARES_COUNT)
	{
		//�����м��·
		for (int x = 2; x <= 13; x++)
		{
			for (int y = 0; y != 14; y++)
			{
				tmpPos = { x,y };
				__PosToString(tmpStr, tmpPos);
				sm_map_strPos_nTexId[tmpStr] = RandomEngine::Uniform(10, 19);
			}
		}
		//�������ߵĲ�
		for (int x = 0; x <= 1; x++)
		{
			for (int y = 0; y != 14; y++)
			{
				tmpPos = { x,y };
				__PosToString(tmpStr, tmpPos);
				sm_map_strPos_nTexId[tmpStr] = RandomEngine::Uniform(0, 9);
			}
		}
		for (int x = 14; x <= 15; x++)
		{
			for (int y = 0; y != 14; y++)
			{
				tmpPos = { x,y };
				__PosToString(tmpStr, tmpPos);
				sm_map_strPos_nTexId[tmpStr] = RandomEngine::Uniform(0, 9);
			}
		}
	}
	else//������Ϸ����ô�ƶ�
	{
		//������һ�����õ�
		std::string tmpStr2;

		//�ɲ������ƶ�
		for (int x = 0; x <= 1; x++)
		{
			for (int y = 13; y != 0; y--)
			{
				tmpPos = { x,y - 1 };
				__PosToString(tmpStr2, tmpPos);
				tmpPos = { x,y };
				__PosToString(tmpStr, tmpPos);
				sm_map_strPos_nTexId[tmpStr] = sm_map_strPos_nTexId[tmpStr2];
			}
		}
		for (int x = 14; x <= 15; x++)
		{
			for (int y = 13; y != 0; y--)
			{
				tmpPos = { x,y - 1 };
				__PosToString(tmpStr2, tmpPos);
				tmpPos = { x,y };
				__PosToString(tmpStr, tmpPos);
				sm_map_strPos_nTexId[tmpStr] = sm_map_strPos_nTexId[tmpStr2];
			}
		}
		//��������µĲ�
		tmpPos = { 0,0 };
		__PosToString(tmpStr, tmpPos);
		sm_map_strPos_nTexId[tmpStr] = RandomEngine::Uniform(0, 9);
		tmpPos = { 1,0 };
		__PosToString(tmpStr, tmpPos);
		sm_map_strPos_nTexId[tmpStr] = RandomEngine::Uniform(0, 9);
		tmpPos = { 14,0 };
		__PosToString(tmpStr, tmpPos);
		sm_map_strPos_nTexId[tmpStr] = RandomEngine::Uniform(0, 9);
		tmpPos = { 15,0 };
		__PosToString(tmpStr, tmpPos);
		sm_map_strPos_nTexId[tmpStr] = RandomEngine::Uniform(0, 9);

		//�ɹ�·�������ƶ�
		for (int x = 2; x <= 13; x++)
		{
			for (int y = 13; y != 0; y--)
			{
				tmpPos = { x,y - 1 };
				__PosToString(tmpStr2, tmpPos);
				tmpPos = { x,y };
				__PosToString(tmpStr, tmpPos);
				sm_map_strPos_nTexId[tmpStr] = sm_map_strPos_nTexId[tmpStr2];
			}
		}
		//��������µ�·
		for (int x = 2; x <= 13; x++)
		{
			tmpPos = { x,0 };
			__PosToString(tmpStr, tmpPos);
			sm_map_strPos_nTexId[tmpStr] = RandomEngine::Uniform(10, 19);
		}
	}
}

void Game::__GenerateOnRoadCar()
{
	//��̫���˷������
	if (sm_vec_car.size() < 4 * sm_unDifficulty)
	{
		//��ʱģ��
		Car car(sm_pgupTex);
		car.IsPlayer(false);

		//�����Ŀ���
		int delta = (4 * sm_unDifficulty - sm_vec_car.size());
		if (delta > 80)
		{
			delta = 40;
		}
		while (delta > 0)
		{
			if (RandomEngine::Uniform(0, 9) > 8)
			{
				bool isSameBlock = false;
				int tmpX = RandomEngine::Uniform(2, 13);
				//�ų��ظ�λ��
				for (auto iter = sm_vec_car.end() - 1; iter != sm_vec_car.begin(); iter--)
				{
					if (iter->GetPos().x == tmpX && iter->GetPos().y == 0)
					{
						isSameBlock = true;
						break;
					}
				}
				if (!isSameBlock)
				{
					sm_vec_car.push_back(car);
					(*(sm_vec_car.end() - 1)).MoveTo(tmpX, 0);
					(*(sm_vec_car.end() - 1)).SetTexId(RandomEngine::Uniform(21, 29));
				}
			}
			if (sm_vec_car.size() >= 150)
			{
				break;
			}
			delta--;
		}
	}
}

void Game::__DeleteOutsideOnRoadCar()
{
	auto iter = sm_vec_car.begin() + 1;
	while (iter != sm_vec_car.end())
	{
		if (iter->GetPos().y >= 14)
		{
			iter = sm_vec_car.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void Game::__MoveOnRoadCar()
{
	for (auto& car : sm_vec_car)
	{
		if (!car.IsPlayer())
		{
			car.Move(0, 1);
		}
	}
}

void Game::__ControlPlayerCar()
{
	//�ǲ��ǰ��ŷ��������
	static bool isHoldingKey = false;

	//���û�ɿ�����ô�ж��ǲ����ɿ��¼�
	if (isHoldingKey)
	{
		if (sm_msg.type == SDL_KEYUP)
		{
			switch (sm_msg.key.keysym.sym)
			{
			case SDLK_LEFT:
			case SDLK_RIGHT:
				isHoldingKey = false;
				return;
				//break;
			default:
				break;
			}
		}
	}
	else//������ɿ��ģ���ô��������ƶ�����
	{
		if (sm_msg.type == SDL_KEYDOWN)
		{
			switch (sm_msg.key.keysym.sym)
			{
			case SDLK_LEFT:
				(*(sm_vec_car.begin())).Move(-1, 0);
				isHoldingKey = true;
				break;
			case SDLK_RIGHT:
				(*(sm_vec_car.begin())).Move(1, 0);
				isHoldingKey = true;
				break;
			default:
				break;
			}
		}
	}
}

bool Game::__IsThereCollision()
{
	auto playerPos = sm_vec_car[0].GetPos();

	//���ȼ����û��ײ�ϲݵ�
	if (playerPos.x <= 1 || playerPos.x >= 14)
	{
		return true;
	}

	//�����û�����ϱ�ĳ�
	for (auto& car : sm_vec_car)
	{
		if (!car.IsPlayer())
		{
			if (car.GetPos().x == playerPos.x && car.GetPos().y == playerPos.y)
			{
				return true;
			}
		}
	}

	return false;
}

Uint32 Game::__CycleCB(Uint32 interval, void* praram)
{
	__UpdateBackground();

	__GenerateOnRoadCar();

	__MoveOnRoadCar();

	__DeleteOutsideOnRoadCar();

	sm_unDifficulty = static_cast<int>(sm_unScore / 1000) + 1;

	//Ĭ������Ϊ250ms�������Ѷ��𽥼�С
	const Uint32 default_interval = 250;
	//������СΪ
	if (sm_unDifficulty > 10)
	{
		return Uint32(150);
	}
	else
	{
		return default_interval - (10 * sm_unDifficulty);
	}
}

int Game::__EndPage()
{
	auto again_pgu = GraphEngine::LoadUnit("Res/endpage_again_gb.bmp", true);
	auto quit_pgu = GraphEngine::LoadUnit("Res/endpage_quit_gb.bmp", true);
	auto cursor_pgu = GraphEngine::LoadUnit("Res/cursor_gb.bmp", true);

	SDL_Point text_size;
	std::string scoreText = "��ķ�����: ";
	std::string scoreNum;
	__IntToString(scoreNum, sm_unScore);
	scoreText += scoreNum;
	auto text_pgu = GraphEngine::FontsToUnit(&text_size, scoreText.c_str());
	std::cout << SDL_GetError() << std::endl
		<< TTF_GetError() << std::endl;

	int nowSelecting = 0;

	bool keyholding = false;
	while (true)
	{
		GraphEngine::ClearBuff();

		GraphEngine::AddToBuff(96, 128, 64, 16, again_pgu);
		GraphEngine::AddToBuff(96, 144, 64, 16, quit_pgu);
		GraphEngine::AddToBuff(80, 128 + nowSelecting * 16, 16, 16, cursor_pgu);
		GraphEngine::AddToBuff(0, 0, text_size.x, text_size.y, text_pgu);

		SDL_PollEvent(&sm_msg);
		SDL_Delay(1);
		if (sm_msg.type == SDL_QUIT)
		{
			exit(0);
		}

		if (keyholding)
		{
			if (sm_msg.type == SDL_KEYUP)
			{
				switch (sm_msg.key.keysym.sym)
				{
				case SDLK_DOWN:
				case SDLK_UP:
					keyholding = false;
				default:
					break;
				}
			}
		}
		else
		{
			if (sm_msg.type == SDL_KEYDOWN)
			{
				if (sm_msg.key.keysym.sym == SDLK_DOWN)
				{
					nowSelecting++;
					keyholding = true;
				}
				else if (sm_msg.key.keysym.sym == SDLK_UP)
				{
					nowSelecting--;
					keyholding = true;
				}
				else if (sm_msg.key.keysym.sym == SDLK_RETURN)
				{
					GraphEngine::FreeUnit(again_pgu);
					GraphEngine::FreeUnit(quit_pgu);
					GraphEngine::FreeUnit(cursor_pgu);
					return nowSelecting;
				}

				if (nowSelecting >= 2)
				{
					nowSelecting = 0;
				}
				else if (nowSelecting <= -1)
				{
					nowSelecting = 1;
				}
			}
		}

		GraphEngine::PresentBuff();
	}

	return -1;
}

void Game::__StringToPos(const char* cstr, SDL_Point& ptPos)
{
	std::stringstream ss;
	ss.str(cstr);
	ss >> ptPos.x;
	ss >> ptPos.y;
}

void Game::__PosToString(std::string& str, SDL_Point ptPos)
{
	std::stringstream ss;
	ss << ptPos.x << ' ' << ptPos.y;
	str.assign(ss.str());
}

void Game::__IntToString(std::string& str, int num)
{
	std::stringstream ss;
	ss << num;
	str.assign(ss.str());
}

Car::Car(GraphUnitPack* pgup)
{
	m_ptPos = { 2,0 };
	m_bIsPlayer = true;
	m_pgup = pgup;
	m_nTexId = 21;
}

bool Car::IsPlayer()
{
	return m_bIsPlayer;
}

void Car::IsPlayer(bool is)
{
	m_bIsPlayer = is;
}

void Car::AddToBuff()
{
	if (m_bIsPlayer)
	{
		GraphEngine::AddSquareToBuff(m_ptPos.x, m_ptPos.y, m_pgup->vec_pguContent[20], false);
	}
	else
	{
		GraphEngine::AddSquareToBuff(m_ptPos.x, m_ptPos.y,
									 m_pgup->vec_pguContent[m_nTexId], false);
	}
}

void Car::Move(int x, int y)
{
	m_ptPos.x += x;
	m_ptPos.y += y;
}

void Car::MoveTo(int x, int y)
{
	m_ptPos = { x,y };
}

SDL_Point& Car::GetPos()
{
	return m_ptPos;
}

void Car::SetTexId(int id)
{
	m_nTexId = id;
}