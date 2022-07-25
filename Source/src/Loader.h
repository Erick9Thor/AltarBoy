#pragma once

#include "core/hepch.h"

#include "core/Scene.h"
#include "resources/ResourceScene.h"

namespace Hachiko
{
    namespace Loader
    {
        bool loading;

        void LoadScene(Scene* loaded_scene, ResourceScene* scene_resource) 
        {
            loaded_scene->Load(scene_resource->scene_data);

            loading = false;
        }
    }
}

/*

void Hachiko::ModuleSceneManager::LoadScene(ResourceScene* new_resource, bool keep_navmesh)

if (scene_resource)
    {
        Loader::loading = true;
        //std::thread worker(Loader::LoadScene, new_scene, scene_resource);

        if (!keep_navmesh)
        {
            App->navigation->SetNavmesh(scene_resource->scene_data[NAVMESH_ID].as<UID>());
        }

        new_scene->Load(scene_resource->scene_data);

        //worker.join();
    }

*/