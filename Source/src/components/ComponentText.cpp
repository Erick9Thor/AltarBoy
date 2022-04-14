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

        if (ImGui::InputText("Text", &label_text, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            label->setText(label_text.c_str());
        }

        if (ImGui::DragFloat("Font Size", &font_size, 2.0f, 0.0f, FLT_MAX))
        {
            label->setPixelSize(static_cast<int>(font_size));
        }

        if (ImGuiUtils::CompactColorPicker("Font Color", font_color.ptr()))
        {
            label->setColor(font_color.x, font_color.y, font_color.z, font_color.w);
        }

        static float2 sc(1.0f);
        if (ImGui::DragFloat2("Sc", sc.ptr(), 2.0f, 0.0f, FLT_MAX))
        {
            label->scale(sc.x, sc.y, 1.0f);
        }

        /* static float2 sz(1.0f);
        if (ImGui::DragFloat2("Sc", sc.ptr(), 2.0f, 0.0f, FLT_MAX))
        {
            label->setSize(sc.x, sc.y);
        }*/
        /* static float2 ps(100.0f);
        if (ImGui::DragFloat2("Ps", ps.ptr(), 2.0f, 0.0f, FLT_MAX))
        {
            label->setPosition(ps.x, ps.y);
        } */
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
        const float3& scale = transform->GetScale();
        const float2& size = transform->GetSize();
        //const float2& size = transform->GetPo();
        //label->scale(scale.x, scale.y, scale.z);
        //label->setSize(size.x, size.y);
        const float3& pos = transform->GetGlobalScaledTransform().TranslatePart();
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
    
    label = std::unique_ptr<FTLabel>(new FTLabel(font.gl_font, // Font face handle
                                                "Hello world!", // Text to render
                                                startX,
                                                startY,
                                                windowWidth,
                                                windowHeight));
    label->setColor(font_color.x, font_color.y, font_color.z, font_color.w);
    label->setPixelSize(64);
    label->setAlignment(FTLabel::FontFlags::CenterAligned);
    label->appendFontFlags(FTLabel::FontFlags::Indented);

    RefreshLabel(transform);
}
