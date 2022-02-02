#include "SceneImporter.h"

#include "../GameObject.h"
#include "../Application.h"
#include "../Scene.h"

#include "../Modules/ModuleFileSystem.h"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/error/en.h"

#include "../Utils/Logger.h"

#include <string>

Scene* SceneImporter::LoadScene(const char* file_path)
{
	const char* buffer = App->file_sys->Load(file_path);

	if (sizeof(buffer) == 0) { 
		return nullptr; 
	}

	// Parse document from file
	rapidjson::Document document;
	document.Parse<rapidjson::kParseStopWhenDoneFlag>(buffer);
	if (document.HasParseError())
	{
		LOG("Error parsing JSON: %s (offset: %u)", rapidjson::GetParseError_En(document.GetParseError()), document.GetErrorOffset());
		return nullptr;
	}
	
	JsonFormaterValue j_scene(document, document);

	Scene* scene = new Scene();
	scene->Load(j_scene);
	return scene;
}

bool SceneImporter::SaveScene(Scene* scene, const char* file_path)
{
	rapidjson::Document document;
	document.SetObject();
	JsonFormaterValue j_Scene(document, document);

	scene->Save(j_Scene);

	// Write document to buffer
	rapidjson::StringBuffer string_buffer;
	
	rapidjson::PrettyWriter<
		/*typename OutputStream  */ rapidjson::StringBuffer,
		/*typename SourceEncoding*/ rapidjson::UTF8<>,
		/*typename TargetEncoding*/ rapidjson::UTF8<>,
		/*typename StackAllocator*/ rapidjson::CrtAllocator,
		0
	> writer(string_buffer);

	document.Accept(writer);

	// Save to file
	return App->file_sys->Save(file_path, string_buffer.GetString(), string_buffer.GetSize());
}
