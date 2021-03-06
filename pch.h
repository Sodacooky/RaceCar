﻿// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <random>
#include <sstream>
#include <functional>

#include <iostream>
#include <vector>
#include <random>
#include <Windows.h>

#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"

#include "Engine.h"

#pragma comment (lib,"SDL/SDL2.lib")
#pragma comment (lib,"SDL/SDL2main.lib")
#pragma comment (lib,"SDL/SDL2_ttf.lib")
#pragma comment (lib,"SDL/SDL2_mixer.lib")
//#pragma comment (lib,"SDL/SDL2test.lib")
//#undef main
//#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )
// 添加要在此处预编译的标头

#endif //PCH_H
