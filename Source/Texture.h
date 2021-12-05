#pragma once

#include <string>
using namespace std;

class Texture
{
	public:
		Texture(const char* file_data, const char* path);
		~Texture();

		inline unsigned int getID() { return texture_id; }

		void DrawImGui();

	private:
		unsigned int texture_id;
		string texture_path;
};

