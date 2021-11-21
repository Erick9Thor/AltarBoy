#pragma once
class Program
{
	public: 
		unsigned int CreateProgram(const char* vtx_shader_file_name, const char* frg_shader_file_name) const;
		void CreateProgramID(const char* vtx_shader_file_name, const char* frg_shader_file_name);

		void use();

		inline unsigned int getProgramID() const {
			return ID;
		}

	private: 
		unsigned int ID;
};

