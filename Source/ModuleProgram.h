#pragma once
#include "Module.h"

#include "MathGeoLib.h"

class ModuleProgram :
    public Module
{
public:
    ModuleProgram();

    bool Init();
    // TODO: Add cleanup (Probably)
    char* LoadShaderSource(const char* shader_file_name);
    unsigned int CompileShader(unsigned type, const char* source);
    unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);
    unsigned int vertex_shader_id;
    unsigned int fragment_shader_id;
    unsigned int program_id;
private:
};

