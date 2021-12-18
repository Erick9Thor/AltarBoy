#pragma once
#include "Module.h"
#include "SDL.h"

class ModuleHardware : public Module
{
public:

	struct hw_info {
		SDL_version sdl_version;
		int n_cpu;
		float ram_gb;
		unsigned char* gpu;
		unsigned char* gpu_brand;
		int vram_mb_budget;
		int vram_mb_free;
	};

public:

	ModuleHardware();
	~ModuleHardware() override;

	bool Init() override;

	inline hw_info GetInfo() { return info; };

private:

	mutable hw_info info;
};
