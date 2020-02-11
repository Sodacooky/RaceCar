#pragma once

//���û�ʹ�õĵ�Ԫ����ʹ��LoadUnit()��ȡ
struct GraphUnit
{
	SDL_Rect* prtSrc;
	SDL_Texture* pTex;
};

//��vector��װ��һϵ��GraphUnit��Pack����ʹ��LoadUnitPack()��ȡ
struct GraphUnitPack
{
	std::vector<GraphUnit*> vec_pguContent;
};

//֧�������ֱ�������ͼ������
//ע�⣺������ηŴ���Ч�����궼��256*224
class GraphEngine
{
public:
	//x�ᣨ�����ж��ٸ�����
	static const int SQUARES_X_COUNT = 16;
	//y�ᣨ�����ж��ٸ�����
	static const int SQUARES_Y_COUNT = 14;
	//��������
	static const int SQUARES_COUNT = SQUARES_X_COUNT * SQUARES_Y_COUNT;

public:
	//��ʼ�����ϵͳ��������ڼ�����Դǰ����
	static bool Init(void);

	//�ı䴰�ڵķŴ����������2�ı���
	static void ChangeWinSize(int times);

	//�ı䴰�ڱ���
	static void ChangeWinTitle(const char* utf8_title);

	//��һ����Ԫ������ķ�ʽ���Ƶ�buff��sq�ķ�Χ��x0-15 y0-13
	//Ĭ��insert�ӵ���������⸲��
	static void AddSquareToBuff(int sq_x, int sq_y, GraphUnit* unit, bool bottom = true);

	//��һ����Ԫ������ķ�ʽ���Ƶ�buff��index����������к�
	//Ĭ��insert�ӵ���ǰ���⸲��
	static void AddSquareToBuff(int sq_index, GraphUnit* unit, bool bottom = true);

	//ֱ�����긴�Ƶ�buff������ķ�Χ��256*224��
	static void AddToBuff(int x, int y, int w, int h, GraphUnit* unit);

	//����buff���������buff
	static void PresentBuff(void);

	//���buff
	static void ClearBuff(void);

	//���ļ��м���graphunit������Ҫ����Ⱦʱ��Ҫ��������ʱ��ǵ�����
	static GraphUnit* LoadUnit(const char* filename, bool use_key = false,
							   SDL_Color key = { 0,255,0,255 });

	//��һ���ַ���ת��ΪGraphUnit
	//��һ����������һ���ȱȵĻ��ƴ�С����Ϊ16
	static GraphUnit* FontsToUnit(SDL_Point* size, const char* ansi_cstr);

	//���ļ��м���graphunit������Ҫ����Ⱦʱ��Ҫ��������ʱ��ǵ�����
	//����ļ����зֳ�16����С�����ΰ�˳����
	static GraphUnitPack* LoadUnitPack(const char* filename, bool use_key = false,
									   SDL_Color key = { 0,255,0,255 });

	static void FreeUnit(GraphUnit* gu);

	static void FreeUnitPack(GraphUnitPack* gup);

private:
	static SDL_Window* sm_pWin;
	static SDL_Renderer* sm_pRen;

	//times of 160:90
	static int sm_nWinSizeTimes;

	static TTF_Font* sm_pFont;

	//�������������graphunit
	struct InBuffGraphUnit
	{
		SDL_Rect* prtDst;
		GraphUnit* pgu;
	};

	//buff�Ķ���
	static std::vector<InBuffGraphUnit> sm_vec_ibguBuff;

private:
	static SDL_Point __IndexToPoint(int index);
	static int __PointToIndex(int pt_x, int pt_y);
};

//���������������װ������������
class RandomEngine
{
public:
	static void Init();
	static int Uniform(int min, int max);
	static bool Half();
	//ʹ��sdl��gettick�������ã����sdlδ��ʼ�����ܻ�������
	static void SetNewSeed();
	//�����µ�����
	static void SetNewSeed(unsigned int seed);
private:
	static std::default_random_engine sm_dre;
};