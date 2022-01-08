#pragma once

class Program
{
public:
	Program(unsigned vtx_shader, unsigned frg_shader);

	void Activate();
	void Deactivate();
	void CleanUp();

	void BindUniformFloat4x4(const char* name, const float* data, bool transpose = true);
	void BindUniformFloat3(const char* name, const float* data);
	void BindUniformFloat(const char* name, const float* data);
	void BindUniformBool(const char* name, bool value);	

	inline int GetId() const{ return id; }

private:
	unsigned id = 0;
};
