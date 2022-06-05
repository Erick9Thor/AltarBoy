#include "scriptingUtil/gameplaypch.h"
#include "RoomTeleporter.h"

// NOTE FOR BERNAT: To scripts be able to work, you need to define it's 
// constructor properly. Note the name field that is same with the class name.
Hachiko::Scripting::RoomTeleporter::RoomTeleporter(GameObject* game_object)
	: Script(game_object, "RoomTeleporter")
	, _touching(false)
	, _target(nullptr)
	, _fade_image(nullptr)

{
}

void Hachiko::Scripting::RoomTeleporter::OnAwake()
{
	// Don't forget, if you change any field here, it will override whatever 
	// you set from the editor, as OnLoad is called before OnAwake. As opposed
	// to OnStart, OnAwake is called again when you build scripts again at run
	// time.
}

void Hachiko::Scripting::RoomTeleporter::OnStart()
{
}

void Hachiko::Scripting::RoomTeleporter::OnUpdate()
{
	if (_target->GetTransform()->GetGlobalPosition().Distance(game_object->GetTransform()->GetGlobalPosition()) <= 2.0f)
	{
		// Check the ComponentImage to see the changes I've made to expose it
		// to Gameplay module.
		// I also added Set and Get methods for you, have a quality development
		// time, hmu for any further questions.
		_fade_image->SetColor(float4(1.0f, 0.0f, 0.0f, 0.5f));
	}
}
