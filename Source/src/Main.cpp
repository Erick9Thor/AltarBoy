#include "core/hepch.h"

#ifdef _DEBUG
#define DEBUG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif

void DumpLeaks(void)
{
    _CrtDumpMemoryLeaks(); // Show leaks with file and line where allocation was made
}

enum class MainStates
{
    MAIN_CREATION,
    MAIN_START,
    MAIN_UPDATE,
    MAIN_FINISH,
    MAIN_EXIT
};

Hachiko::Application* App = nullptr;
Hachiko::Logger* Logging = nullptr;

int main(int argc, char** argv)
{
    atexit(DumpLeaks);
    Logging = new Hachiko::Logger();

    int main_return = EXIT_FAILURE;
    auto state = MainStates::MAIN_CREATION;

    while (state != MainStates::MAIN_EXIT)
    {
        switch (state)
        {
        case MainStates::MAIN_CREATION: HE_LOG("Application Creation --------------");
            App = new Hachiko::Application();
            state = MainStates::MAIN_START;
            break;

        case MainStates::MAIN_START: HE_LOG("Application Init --------------");
            if (App->Init() == false)
            {
                HE_LOG("Application Init exits with error -----");
                state = MainStates::MAIN_EXIT;
            }
            else
            {
                state = MainStates::MAIN_UPDATE;
                HE_LOG("Application Update --------------");
            }

            break;

        case MainStates::MAIN_UPDATE:
        {
            const UpdateStatus update_return = App->Update();
            OPTICK_FRAME("MainThread");

            if (update_return == UpdateStatus::UPDATE_ERROR)
            {
                HE_LOG("Application Update exits with error -----");
                state = MainStates::MAIN_EXIT;
            }

            if (update_return == UpdateStatus::UPDATE_STOP)
                state = MainStates::MAIN_FINISH;
        }
        break;

        case MainStates::MAIN_FINISH: HE_LOG("Application CleanUp --------------");
            if (App->CleanUp() == false)
            {
                HE_LOG("Application CleanUp exits with error -----");
            }
            else
                main_return = EXIT_SUCCESS;

            state = MainStates::MAIN_EXIT;

            break;
        }
    }

    delete App;
    HE_LOG("Bye :)\n");
    delete Logging;
    return main_return;
}
