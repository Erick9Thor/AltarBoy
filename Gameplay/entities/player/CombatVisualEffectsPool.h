#include <scripting/Script.h>
#include "entities/player/PlayerController.h"
#include <entities/enemies/EnemyController.h>

namespace Hachiko
{

	class GameObject;
	class ComponentBillboard;

	namespace Scripting
	{
		class CombatVisualEffectsPool : public Script
		{
			SERIALIZATION_METHODS(false);
		public:
			CombatVisualEffectsPool(GameObject* game_object);
			~CombatVisualEffectsPool() = default;

			void OnAwake() override;
			void OnUpdate() override;

			void PlayPlayerAttackEffect(PlayerController::WeaponUsed weapon_type, int attack_index, float3 position);
			void PlayEnemyAttackEffect(EnemyType _enemy_type, float3 position);

			ComponentBillboard* GetCurrentBillboard() const;

		private:
			std::vector<ComponentBillboard*> _billboards;
			SERIALIZE_FIELD(int, _current_billboard_index);

			SERIALIZE_FIELD(GameObject*, _melee_vfx);
			SERIALIZE_FIELD(GameObject*, _claw_vfx);
			SERIALIZE_FIELD(GameObject*, _sword_vfx);
			SERIALIZE_FIELD(GameObject*, _hammer_vfx);
		};
	} // namespace Scripting
} // namespace Hachiko