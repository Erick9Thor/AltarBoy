#include "scriptingUtil/gameplaypch.h"
#include "constants/Scenes.h"
#include "ui/BackToMainMenu.h"

Hachiko::Scripting::BackToMainMenu::BackToMainMenu(GameObject* game_object)
	: Script(game_object, "BackToMainMenu")
	, _button_back(nullptr)
{
}

void Hachiko::Scripting::BackToMainMenu::OnAwake()
{

	if (_button_back == nullptr)
	{
		HE_LOG("Owner of this script must have ComponentButton.");
	}
}

void Hachiko::Scripting::BackToMainMenu::OnStart()
{
}

void Hachiko::Scripting::BackToMainMenu::OnUpdate()
{
	if (_button_back->IsSelected())
	{
		HE_LOG("TEST_BUTTON");
		//SceneManagement::SwitchScene(Scenes::MAIN_MENU);
	}
}