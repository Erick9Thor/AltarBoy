#pragma once

#include <scripting/Script.h>

namespace Hachiko
{
	class GameObject;
	namespace Scripting
	{
		class SomeScript : public Script
		{
			SERIALIZATION_METHODS(false)

		public:
			SomeScript(GameObject* game_object);
			~SomeScript() override = default;

			void OnStart() override;
			void OnUpdate() override;

		private:
			SERIALIZE_FIELD(math::float3, _initial_position);
			SERIALIZE_FIELD(math::float3, _position_offset);
			SERIALIZE_FIELD(float, _lerp_position);
		};
	}
}