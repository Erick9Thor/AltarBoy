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
			void OnStart() override;
			void OnUpdate() override;

		private:
			void UpdateDoorState();
			bool CheckOpeningConditions() const;

		// Variables
		public:
			SERIALIZE_FIELD(GameObject*, _door_open);
			SERIALIZE_FIELD(GameObject*, _door_closed);
			SERIALIZE_FIELD(GameObject*, _enemies);	// Children of this object have to be defeated in order to open this door

		private:
			State _state = State::CLOSED;
		};
	}
	
}



