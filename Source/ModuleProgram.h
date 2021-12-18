#pragma once
#include "Module.h"

#include "MathGeoLib.h"

class ModuleProgram :
    public Module
{
public:
    ModuleProgram();
    ~ModuleProgram() override;

    bool Init() override;
    bool CleanUp() override;

    void Activate();
    void Deactivate();
    void BindUniformFloat4x4(const char* name, const float* data, bool transpose = true);
    void BindUniformFloat3(const char* name, const float* data);
    void BindUniformFloat(const char* name, const float* data);
    void BindUniformBool(const char* name, bool value);

    inline const int GetProgramId() const { return program_id; }

    void OptionsMenu();

private:
    char* LoadShaderSource(const char* shader_file_name);
    unsigned int CompileShader(unsigned type, const char* source);
    unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);

    unsigned int vertex_shader_id;
    unsigned int fragment_shader_id;
    unsigned int program_id;

    struct Light {
        float3 position;
        float3 direction;
        float3 color;
        float ambient_strength;
        bool directional;
    };

    Light light;
};

