#pragma once

#include <windows.h>
#include <stdio.h>

#include "Utils/Logger.h"

#define LOG(format, ...) Logging->log(__FILENAME__, __LINE__, format, __VA_ARGS__);

#define M_PI 3.14159265358979323846
constexpr float to_rad = (float) M_PI / 180.0f;
constexpr float to_deg = 180.0f / (float) M_PI;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR,
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

// File System config -----------
#define VERSION "0.1"
#define ASSETS_FOLDER "Assets"
#define ASSETS_FOLDER_SCENES "Assets/Scenes"

#define SETTINGS_FOLDER "Settings"
#define LIBRARY_FOLDER "Library"
#define LIBRARY_TEXTURES_FOLDER "Library/Textures"
#define LIBRARY_MESH_FOLDER "Library/Meshes"
#define LIBRARY_SCENE_FOLDER "Library/Scenes"
#define LIBRARY_MATERIAL_FOLDER "Library/Materials"
#define LIBRARY_SHADERS_FOLDER "Library/Shaders"
#define LIBRARY_FONTS_FOLDER "Library/Fonts"

#define JPG_TEXTURE_EXTENSION ".jpg"
#define PNG_TEXTURE_EXTENSION ".png"

#define MATERIAL_EXTENSION ".mat"
#define SCENE_EXTENSION ".scene"
#define PREFAB_EXTENSION ".prefab"
#define META_EXTENSION ".meta"

#define FRAGMENT_SHADER_EXTENSION ".frag"
#define VERTEX_SHADER_EXTENSION ".vert"
#define DEFAULT_SHADER_EXTENSION ".glsl"

#define MODEL_EXTENSION ".fbx"

#define SKYBOX_EXTENSION ".hdr"

// Defering for file system https://stackoverflow.com/questions/32432450/what-is-standard-defer-finalizer-implementation-in-c
#ifndef defer
struct defer_dummy
{};
template<class F> struct deferrer
{
	F f;
	~deferrer() { f(); }
};
template<class F> deferrer<F> operator*(defer_dummy, F f) { return {f}; }
#define DEFER_(LINE) zz_defer##LINE
#define DEFER(LINE) DEFER_(LINE)
#define defer auto DEFER(__LINE__) = defer_dummy {}* [&]()
#endif // defer