#pragma once

#include <scripting/Script.h>
#include "Stats.h"

namespace Hachiko
{
	namespace Scripting
	{
		enum class CristalState
		{
			IDLE,
			CRASHING
		};

		class CrystalExplotion : public Script
		{
			SERIALIZATION_METHODS(false)


		public:
			CrystalExplotion(GameObject* game_object);
			~CrystalExplotion() override = default;


			void OnAwake() override;
			void OnStart() override;
			void OnUpdate() override;

			void ReceiveDamage(int damage);

		private:
			void DestroyCristall();

		public:
			SERIALIZE_FIELD(Stats, _stats);
			SERIALIZE_FIELD(ComponentAnimation*, _animator);
			SERIALIZE_FIELD(CristalState, _state);
			SERIALIZE_FIELD(unsigned, _crashing_index);
		};
	}
}