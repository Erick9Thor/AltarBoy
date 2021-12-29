#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <windows.h>
#include <stdio.h>
#include "UI/AppLog.h"

#define LOG(format, ...) Logger->AddLog(__FILE__, __LINE__, format, __VA_ARGS__);

#define M_PI 3.14159265358979323846
constexpr float to_rad = (float) M_PI / 180.0f;
constexpr float to_deg = 180.0f / (float) M_PI;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Deletes a buffer
#define RELEASE(x)        \
	{                     \
		if (x != nullptr) \
		{                 \
			delete x;     \
			x = nullptr;  \
		}                 \
	}

// Array buffer deletes
#define RELEASE_ARRAY(x)  \
	{                     \
		if (x != nullptr) \
		{                 \
			delete[] x;   \
			x = nullptr;  \
		}                 \
	}

// Configuration -----------
#define WINDOWED_RATIO 0.75f
#define FULLSCREEN false
#define RESIZABLE true
#define TITLE "ALTAR BOOY"
#define ENGINE_VERSION "0.0.1"

#define FPS_LOG_SIZE 100

#endif // __GLOBALS_H__