#include "scriptingUtil/gameplaypch.h"
#include "RoomTeleporter.h"

// NOTE FOR BERNAT: To scripts be able to work, you need to define it's 
// constructor properly. Note the name field that is same with the class name.
Hachiko::Scripting::RoomTeleporter::RoomTeleporter(GameObject* game_object)
	: Script(game_object, "RoomTeleporter")
	, _touching(false)
	, _target(nullptr)
	, _fade_image(nullptr)
	, _fade_duration(0.25f)
{
}

void Hachiko::Scripting::RoomTeleporter::OnAwake()
{

}

void Hachiko::Scripting::RoomTeleporter::OnStart()
{
	// Don't forget, if you change any field here, it will override whatever 
	// you set from the editor, as OnLoad is called before OnAwake. As opposed
	// to OnStart, OnAwake is called again when you build scripts again at run
	// time.

	if (!_fade_image) return;

	_image_transform = _fade_image->GetGameObject()->GetComponent<ComponentTransform2D>();

	if (!_image_transform) return;

	// Center image on the screen
	_image_transform->SetAnchor(float2(0.5));

	// Make image occupy entire screen (Scale multiplies final size)
	_image_transform->SetSize(float2(10.f));
	_image_transform->SetScale(float2(25000.f));

	// Set initial color to black without alpha
	_fade_image->SetColor(_clear_color);
}

void Hachiko::Scripting::RoomTeleporter::OnUpdate()
{
	
	//if (_target->GetTransform()->GetGlobalPosition().Distance(game_object->GetTransform()->GetGlobalPosition()) <= 2.0f)
	if(Input::IsKeyDown(Input::KeyCode::KEY_T))
	{
		SetActive(!_indoors);
	}

	if (_step != Step::WAITING)
	{
		Interpolate();
	}
}

void Hachiko::Scripting::RoomTeleporter::SetActive(bool v)
{
	_step = Step::FADE_IN;
	_fade_progress = 0.f;
	_indoors = v;
}

void Hachiko::Scripting::RoomTeleporter::EnterRoom()
{
	SetActive(true);
}

void Hachiko::Scripting::RoomTeleporter::ExitRoom()
{
	SetActive(false);
}

void Hachiko::Scripting::RoomTeleporter::Interpolate()
{

	// In step logic
	if (_step == Step::FADE_IN)
	{
		_fade_progress += Time::DeltaTime() / _fade_duration;
		_fade_image->SetColor(float4::Lerp(_clear_color, _opaque_color, _fade_progress));
	}
	else if (_step == Step::FADE_OUT)
	{
		_fade_progress += Time::DeltaTime() / _fade_duration;
		_fade_image->SetColor(float4::Lerp(_opaque_color, _clear_color, _fade_progress));
	}

	// Step transition logic
	if (_fade_progress >= 1.f)
	{
		if (_step == Step::FADE_IN)
		{
			// Teleport player
			// Toggle skybox based on indoors
			SceneManagement::SetSkyboxActive(!_indoors);
			_fade_progress = 0.f;
			_step = Step::FADE_OUT;
		}
		else if (_step == Step::FADE_OUT)
		{
			_step = Step::WAITING;
		}
	}

	
}
