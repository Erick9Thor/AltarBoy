#ifndef _MODULE_DEBUGDRAW_H_
#define _MODULE_DEBUGDRAW_H_

#include "Module.h"

#include "Math/float4x4.h"

class DDRenderInterfaceCoreGL;
class Camera;

class ModuleDebugDraw : public Module
{
public:
	ModuleDebugDraw();
	~ModuleDebugDraw() override;

	bool Init() override;
	update_status Update(const float delta) override;
	bool CleanUp() override;

	void Draw(const float4x4& view, const float4x4& proj, unsigned width, unsigned height);
	void SetDebugDraw(bool v) { debug_draw = v; }

private:
	static DDRenderInterfaceCoreGL* implementation;
	bool debug_draw = false;
};

#endif /* _MODULE_DEBUGDRAW_H_ */
