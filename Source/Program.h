#pragma once
class Program
{
	public: 
		Program();
		unsigned int CreateProgram(const char* vtx_shader_file_name, const char* frg_shader_file_name) const;

		inline unsigned int getDefaultProgram() const {
			return default_program;
		}

	private: 
		unsigned int default_program;
};

