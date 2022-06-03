#pragma once

#include "Importer.h"

namespace Hachiko
{
    class ResourceStateMachine;

    class StateMachineImporter : public Importer
    {
        friend class ModelImporter;

    public:
        StateMachineImporter() = default;
        ~StateMachineImporter() override = default;

        // Copies the state machine file to lib
        void Import(const char* path, YAML::Node& meta) override;
        // Updates current state machine resource state to assets and lib
        void Save(UID id, const Resource* resource) override;
        Resource* Load(UID id) override;

    private:
    };
} // namespace Hachiko