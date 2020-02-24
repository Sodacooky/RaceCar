#pragma once

/*
#include <iostream>
#include <vector>
#include <random>
#include <Windows.h>

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

#pragma comment (lib,"SDL/SDL2.lib")
#pragma comment (lib,"SDL/SDL2main.lib")
#pragma comment (lib,"SDL/SDL2_ttf.lib")
#pragma comment (lib,"SDL/SDL2_mixer.lib")
*/

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
	static bool Init();

	//�ͷſռ䲢�ر�
	static void Quit();

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
	static void PresentBuff();

	//���buff
	static void ClearBuff();

	//���ļ��м���graphunit������Ҫ����Ⱦʱ��Ҫ��������ʱ��ǵ�����
	static GraphUnit* LoadUnitFromFile(const char* filename, bool use_key = false,
									   SDL_Color key = { 0,255,0,255 });

	//���ڴ����graphunit,����Ҫ����Ⱦʱ��Ҫ��������ʱ��ǵ�����
	static GraphUnit* LoadUnitFromMem(void* mem, unsigned int mem_size,
									  bool use_key = false,
									  SDL_Color key = { 0,255,0,255 });

	//��һ���ַ���ת��ΪGraphUnit
	//��һ����������һ���ȱȵĻ��ƴ�С����Ϊ16
	static GraphUnit* StringToUnit(SDL_Point* size, const char* ansi_cstr);

	//���ļ��м���graphunit������Ҫ����Ⱦʱ��Ҫ��������ʱ��ǵ�����
	//����ļ����зֳ�16����С�����ΰ�˳����
	static GraphUnitPack*
		LoadSqUnitPackFromFile(const char* filename, bool use_key = false,
							   SDL_Color key = { 0,255,0,255 });

	//���ڴ��м���graphunit������Ҫ����Ⱦʱ��Ҫ��������ʱ��ǵ�����
	//����ļ����зֳ�16����С�����ΰ�˳����
	static GraphUnitPack*
		LoadSqUnitPackFromMem(void* mem, unsigned int mem_size,
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

//��Ч��ͨ��AudioEngine����
class SEAudio
{
public:
	SEAudio(Mix_Chunk* pChunk, int channel);
	void Play();
	void Pause();
	void Resume();
	void Stop();
	//��ǰ����ʼ��˳ʱ��360�ȣ��������ֵ��255
	//�ұ�90�ȣ����270�ȣ�ǰ��0��
	//����ͨ������ľ�������������
	void SetPosition(int angle = 0, int distance = 0);
	//
	int GetPositionAngle();
	//
	int GetPositionDistance();
	//
	int GetChannel();
	//������Ч����ʱ�Ļص�
	void SetFinishedCB(void(*cb)(int channel_useless) = nullptr);
	//�ͷ�chunk�ڴ�
	void FreeChunk();
private:
	Mix_Chunk* m_pChunk;
	int m_nChannel;
	int m_nPosAngle;
	int m_nPosDistance;
};

//��Ƶ����
class AudioEngine
{
public:
	static bool Init();
	static void Quit();

	//��ʼ���ű������֣�����Ѿ��ڲ��ţ���ʲô������
	static void PlayBGM(int fade_ms = 0);
	//��λ(����ͷ)��������
	static void RewindBGM();
	//ֹͣ���ű�������
	static void StopBGM(int fade_ms = 0);
	//��ͣ���ű�������
	static void PauseBGM();
	//����ͣλ�ü������ű�������
	static void ResumeBGM();
	//���ر������ִ��ļ���������ɵı�������
	static void LoadBGMFromFile(const char* filename);
	//���ر������ִ��ڴ棬������ɵı�������
	static void LoadBGMFromMem(void* mem, unsigned int size);
	//���ñ������ֵ�����0-128
	static void BGMVolume(int new_volume);
	//��ǰ������������
	static int BGMVolume();
	//���ñ������ֽ�����Ļص�����
	//��ͣ���ᴥ��
	static void SetFinishedBGMCB(void(*cb)() = nullptr);

	//����һ��SE������Ҫ��FreeSEAudio���ͷ����ָ����ڴ�
	//��������Խ�chunk�Լ��ͷŵ������ͷŵ�Ҳ��
	static SEAudio* LoadSEAudioFromFile(const char* filename);
	////����һ��SE������Ҫ��FreeSEAudio���ͷ����ָ����ڴ�
	//��������Խ�chunk�Լ��ͷŵ������ͷŵ�Ҳ��
	static SEAudio* LoadSEAudioFromMem(void* mem, unsigned int size);
	//�ͷ�SEAudio
	static void FreeSEAudio(SEAudio* se);
	//ֹͣ����SEAudio
	static void StopAllSEAudio();
private:
	static Mix_Music* sm_pMusBGM;
	static int sm_nBGMVolume;
	//true��ʾ�ѱ�ʹ��
	static bool sm_bChannelUsed[64];
private:
	//-1 none free channel
	static int __FindEmptyChannel();
};

//���������������װ������������
class RandomEngine
{
public:
	static void Init();
	static int UniformRange(int min, int max);
	static int NormalRange(int mu, int sigma);
	static bool Half();
	//ʹ��sdl��gettick�������ã����sdlδ��ʼ�����ܻ�������
	static void SetNewSeed();
	//�����µ�����
	static void SetNewSeed(unsigned int seed);
private:
	static std::default_random_engine sm_dre;
};