#include "scriptingUtil/gameplaypch.h"
#include "CutsceneManager.h"
#include "components/ComponentVideo.h"
#include "constants/Scenes.h"

Hachiko::Scripting::CutsceneManager::CutsceneManager(GameObject* new_game_object)
    : Script(new_game_object, "CutsceneManager")
{}

void Hachiko::Scripting::CutsceneManager::OnAwake()
{
    if (_cutscene != nullptr)
    {
        _cutscene_video = _cutscene->GetComponent<ComponentVideo>();
    }
}

void Hachiko::Scripting::CutsceneManager::OnStart()
{
    if (_cutscene_video != nullptr)
    {
        _cutscene_video->Play();
    }
}

void Hachiko::Scripting::CutsceneManager::OnUpdate()
{
    if (_cutscene_video != nullptr && !_cutscene_video->IsPlaying())
    {
        SceneManagement::SwitchScene(Scenes::GAME);
    }
}
