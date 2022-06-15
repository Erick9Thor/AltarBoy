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

		public:
			DoorController(GameObject* game_object);
			~DoorController() override = default;

			void OnAwake() override;
			void OnStart() override;
			void OnUpdate() override;

		private:
			State _state = State::CLOSED;
		};
	}
	
}



