#pragma once
namespace Hachiko
{
    class Window
    {
    public:
        Window(const char* name, bool active);
        virtual ~Window();

        // --- Life cycle --- //
        virtual void Init() {}

        virtual void Update() {}

        virtual void CleanUp() {}

    public:
        const char* name;
        bool active = true;
    };
}
