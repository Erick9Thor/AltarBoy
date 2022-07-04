#pragma once

#include <scripting/Script.h>

namespace Hachiko
{
	class GameObject;
	class ComponentTransform;
	namespace Scripting
	{
		class DoorController : public Script
		{
			SERIALIZATION_METHODS(false)

				enum State
			{
				CLOSED = 0,
				OPEN = 1
			};

		// Methods
		public:
			DoorController(GameObject* game_object);
			~DoorController() override = default;

			void OnAwake() override;
			void OnUpdate() override;

			void Open();
			void Close();

		private:
			void UpdateDoorState();

		// Variables
		public:
			SERIALIZE_FIELD(GameObject*, _door);
			ComponentObstacle* door_obstacle = nullptr;

		private:
			State _state = State::CLOSED;
		};
	}
	
}



