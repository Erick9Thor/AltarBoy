#pragma once
#include "Module.h"

namespace Hachiko
{
    class ModuleScriptingSystem : public Module
    {
    public:
        ModuleScriptingSystem() = default;
        ~ModuleScriptingSystem() override = default;

        bool Init();

        UpdateStatus PreUpdate(const float delta) override;

        UpdateStatus Update(const float delta) override;

        UpdateStatus PostUpdate(const float delta) override;

    private:
        unsigned int _times_reloaded;
    };
} // namespace Hachiko