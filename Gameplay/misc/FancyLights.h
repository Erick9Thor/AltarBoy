#include <scripting/Script.h>

namespace Hachiko
{

class GameObject;
class ComponentPointLight;

namespace Scripting
{
class FancyLights : public Script
{
	SERIALIZATION_METHODS(false)

public:
	FancyLights(Hachiko::GameObject* game_object);
	~FancyLights() override = default;

	void OnAwake() override;
	void OnUpdate() override;

private:
	float _radius;
	float _distance;
	float _speed;
	
	SERIALIZE_FIELD(bool, _rotate_on_y);
	SERIALIZE_FIELD(float, _angle);

	Hachiko::ComponentPointLight* _point_light;
};
} // namespace Scripting
} // namespace Hachiko