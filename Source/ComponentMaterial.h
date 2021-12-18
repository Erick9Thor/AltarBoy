#pragma once
#include "Component.h"
#include "ModuleTexture.h"
class ComponentMaterial :
    public Component
{
public:
    ComponentMaterial(GameObject* conatiner);
    ~ComponentMaterial() override;
    static inline Type GetType() { return Type::Material; };
    void SetTexture(Texture new_texture) { texture = new_texture; }
    inline unsigned GetTextureId() const { return texture.id; }
private:
    Texture texture;
    // Shader id
};

