#include "core/hepch.h"
#include "ComponentText.h"
#include "ComponentTransform2D.h"
#include "ComponentButton.h"
#include "ComponentCamera.h"

#include "modules/ModuleProgram.h"
#include "modules/ModuleCamera.h"

#include "Program.h"




Hachiko::ComponentText::ComponentText(GameObject* container) 
	: Component(Type::TEXT, container) {
}


void Hachiko::ComponentText::DrawGui()
{
    ImGui::PushID(this);
    if (ImGui::CollapsingHeader("Text Label", ImGuiTreeNodeFlags_DefaultOpen))
    {   
        if (ImGui::InputText("Font File", font_filename_buffer, MAX_PATH, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            std::string destination = std::string(ASSETS_FOLDER_FONTS) + "/" + font_filename_buffer;
            font = App->texture->LoadFont(destination.c_str());
            BuildLabel(game_object->GetComponent<ComponentTransform2D>());
        }
        

        if (!label)
        {
            ImGui::Text("Load a font to enable font controls");
            ImGui::PopID();
            return;
        }

        ImGui::Text("No text = size is too small, nothing can be drawn");

        if (ImGui::InputText("Text", &label_text, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            label->setText(label_text.c_str());
            Invalidate();
        }

        if (ImGui::DragFloat("Font Size", &font_size, 2.0f, 0.0f, FLT_MAX))
        {
            label->setPixelSize(static_cast<int>(font_size));
            Invalidate();
        }

        if (ImGuiUtils::CompactColorPicker("Font Color", font_color.ptr()))
        {
            label->setColor(font_color.x, font_color.y, font_color.z, font_color.w);
            Invalidate();
        }

	}
    ImGui::PopID();
}

void Hachiko::ComponentText::Draw(ComponentTransform2D* transform, Program* program)
{
    //program->BindUniformFloat4x4("model", transform->GetGlobalScaledTransform().ptr());
    // Setting diffuse flag to false makes shader default to color
    //program->BindUniformBool("diffuse_flag", false);
    //program->BindUniformFloat4("img_color", font_color.ptr());
    
    if (label)
    {
        // Make sure transform is refreshed
        transform->GetGlobalTransform();
        RefreshLabel(transform);
        // Program is activated inside hachikorender
        label->HachikoRender(program);
    }        
}

void Hachiko::ComponentText::Save(JsonFormatterValue j_component) const
{
    const JsonFormatterValue j_font_path = j_component["FontPath"];
    j_font_path = font.path.c_str();
}

void Hachiko::ComponentText::Load(JsonFormatterValue j_component)
{
    const JsonFormatterValue j_font_path = j_component["FontPath"];
    const std::string font_path = j_font_path;

    if (!font_path.empty())
    {
        font = App->texture->LoadFont(font_path.c_str());
        BuildLabel(game_object->GetComponent<ComponentTransform2D>());
    }    
}

void Hachiko::ComponentText::RefreshLabel(ComponentTransform2D* transform)
{
    if (dirty)
    {
        unsigned windowWidth, windowHeight;
        App->camera->GetMainCamera()->GetResolution(windowWidth, windowHeight);
        label->setWindowSize(windowWidth, windowHeight);
        const float2& size = transform->GetSize();
        //const float2& size = transform->GetPo();
        
        
        const float4x4& trf = transform->GetGlobalTransform();
        const float3& pos = trf.TranslatePart();
        label->scale(trf.scaleX, trf.scaleY, trf.scaleZ);
        label->setSize(size.x, size.y);
        label->setPosition(pos.x + (windowWidth / 2), pos.y + (windowHeight / 2));
        dirty = false;
    }
}

void Hachiko::ComponentText::BuildLabel(ComponentTransform2D* transform)
{
    // TODO: Get From transform
    int startX = 100;
    int startY = 100;


    unsigned windowWidth, windowHeight;
    App->camera->GetMainCamera()->GetResolution(windowWidth, windowHeight);

    const float4x4& trf = transform->GetGlobalTransform();
    
    label = std::unique_ptr<FTLabel>(new FTLabel(font.gl_font, // Font face handle
                                                 "", // Text to render
                                                 startX,
                                                 startY,
                                                 windowWidth,
                                                 windowHeight));
    
    label->setText(label_text.c_str()); // Text in constructor seems to not have effect
    label->setColor(font_color.x, font_color.y, font_color.z, font_color.w);
    label->setPixelSize(font_size);
    label->setAlignment(FTLabel::FontFlags::CenterAligned);
    //label->appendFontFlags(FTLabel::FontFlags::Indented);

    RefreshLabel(transform);
}
