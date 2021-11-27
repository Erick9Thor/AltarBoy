#pragma once
#include "../Module.h"
#include "../Math.h"


class Model;

class ModuleLoadModels :
    public Module
{
	public:
		bool Init();
		update_status PreUpdate();
		update_status Update();
		update_status PostUpdate();
		bool CleanUp();

		void LoadModel(const char* _fileName);

	private:
		unsigned int progam_id;

		Model* ourModel;
};

