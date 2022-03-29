#include "core/hepch.h"
#include "ComponentImage.h"
#include "ComponentTransform2D.h"

#include "Program.h"

Hachiko::ComponentImage::ComponentImage(GameObject* container) 
	: Component(Type::IMAGE, container) {
}


void Hachiko::ComponentImage::DrawGui()
{
    ImGui::PushID(this);
    if (ImGui::CollapsingHeader("Image", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Image placeholder insp");
	}
    ImGui::PopID();
}

void Hachiko::ComponentImage::Draw(ComponentTransform2D* transform, Program* program) const
{
	// Bind matrix
    program->BindUniformFloat4x4("model", transform->GetGlobalScaledTransform().ptr());
    // TODO    
	// Bind Texture	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Hachiko::ComponentImage::Save(YAML::Node& node) const {}

void Hachiko::ComponentImage::Load(const YAML::Node& node) {}
