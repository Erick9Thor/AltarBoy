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
			SERIALIZE_FIELD(std::vector<int>, _int_vector);
			SERIALIZE_FIELD(int, _counter);
		};
	}
}