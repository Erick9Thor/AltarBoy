#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <windows.h>
#include <stdio.h>
#include "ImGuiComponents/AppLog.h"

#define LOG(format, ...) Logger->AddLog(__FILE__, __LINE__, format, __VA_ARGS__);

#define DEGTORAD 0.0174532925199432957f

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Deletes a buffer
#define RELEASE( x )\
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Array buffer deletes
#define RELEASE_ARRAY( x )\
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }

// Configuration -----------
#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720
#define FULLSCREEN false
#define RELIZABLE true
#define VSYNC true
#define TITLE "ALTAR BOOY"
#define ENGINE_VERSION "0.0.1"

#define FPS_LOG_SIZE 100

#endif // __GLOBALS_H__