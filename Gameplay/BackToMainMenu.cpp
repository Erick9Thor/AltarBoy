#include "scriptingUtil/gameplaypch.h"
#include "BackToMainMenu.h"

Hachiko::Scripting::BackToMainMenu::BackToMainMenu(GameObject* game_object)
	: Script(game_object, "BackToMainMenu")
	, _button_back(nullptr)
{}

void Hachiko::Scripting::BackToMainMenu::OnAwake()
{
	bool any_null = false;

	_button_back = game_object->GetComponent<ComponentButton>();
	any_null |= (_button_back == nullptr);

	if (any_null)
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
		SceneManagement::SwitchScene("Assets/Scenes/menu.scene");
	}
}