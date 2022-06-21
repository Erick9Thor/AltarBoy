#pragma once
#include <scripting/Script.h>


namespace Hachiko
{
	class ComponentTransform;

	namespace Scripting
	{
		class Shake : public Script
		{
			SERIALIZATION_METHODS(false)

		public:
			Shake(GameObject* game_object);
			~Shake() override = default;

			void OnAwake() override;
			void OnStart() override;
			void OnUpdate() override;

		private:
			ComponentTransform* transform;

			SERIALIZE_FIELD(float, _shake_time);
			SERIALIZE_FIELD(float, _shake_intensity);
			SERIALIZE_FIELD(float, _shake_magnitude);

			float shake_elapsed = 0.0f;
		};
	}
}


