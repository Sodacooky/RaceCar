#pragma once

class Car
{
public:
	Car(GraphUnitPack* pgup);
	bool IsPlayer();
	void IsPlayer(bool is);
	void AddToBuff();
	//����ƶ�
	void Move(int x, int y);
	//�ƶ���
	void MoveTo(int x, int y);
	SDL_Point& GetPos();
	//�����ҵ���ͼid
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
	//��������õ�����ͼpack
	static GraphUnitPack* sm_pgupTex;
	//��ǰ��Ϸ����
	static unsigned int sm_unScore;
	//��Ϸ�Ƿ�����Ϸ��
	static bool sm_bIsInRace;///////unuse now
	//��Ϸ������ͼ����Ϸ������Ҫ�����ΪҪ�ô�С���ж��ǲ�������Ϸ
	//�����ʽ��"x y"
	static std::map<std::string, int> sm_map_strPos_nTexId;
	//��Ϸ�����еĳ�
	static std::vector<Car> sm_vec_car;
	//�Ѷ�����
	static unsigned int sm_unDifficulty;
	//sdl�¼�
	static SDL_Event sm_msg;
private:
	//��ʼ��
	static bool __Init();

	//��ʼ����
	static int __StartPage();

	//��ʽ��Ϸ���ط���
	static int __InRace();
	//���Ƴ�
	static void __DrawAllCar();
	//���Ʊ���
	static void __DrawBackground();
	//���±�����ͼ����timer��
	static void __UpdateBackground();
	//����·�ϵĳ��ӣ���timer��
	static void __GenerateOnRoadCar();
	//�����뿪��Ļ�ĳ���,��timer��
	static void __DeleteOutsideOnRoadCar();
	//�ƶ�·�ϵĳ��ӣ���timer��
	static void __MoveOnRoadCar();
	//������ҵĳ���
	static void __ControlPlayerCar();
	//�ж���Ϸ����
	static bool __IsThereCollision();
	//�����������е����ɺ��ƶ�·�ϵĳ���
	//sdl timer callback
	static Uint32 __CycleCB(Uint32 interval, void* praram);
	//
	static void __FinshedBGMCB();

	//�������
	static int __EndPage();

	//�ַ���"x y"ת��Ϊ����
	static void __StringToPos(const char* cstr, SDL_Point& ptPos);
	//������ת�����ַ���"x y"
	static void __PosToString(std::string& str, SDL_Point ptPos);
	//������ת��Ϊ�ַ���
	static void __IntToString(std::string& str, int num);
};