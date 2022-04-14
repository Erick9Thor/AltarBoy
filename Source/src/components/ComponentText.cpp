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
            BuildLabel();
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
	}
    ImGui::PopID();
}

void Hachiko::ComponentText::Draw(ComponentTransform2D* transform, Program* program) const
{
    //program->BindUniformFloat4x4("model", transform->GetGlobalScaledTransform().ptr());
    // Setting diffuse flag to false makes shader default to color
    //program->BindUniformBool("diffuse_flag", false);
    //program->BindUniformFloat4("img_color", font_color.ptr());
    
    if (label)
    {
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
        BuildLabel();
    }    
}

void Hachiko::ComponentText::RefreshWindowSize()
{
    if (label)
    {
        unsigned windowWidth, windowHeight;
        App->camera->GetMainCamera()->GetResolution(windowWidth, windowHeight);
        label->setWindowSize(windowWidth, windowHeight);
    }
}

void Hachiko::ComponentText::BuildLabel()
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
    //label->setText(label_text);
    /* label->setPosition(500, 250);
    label->setSize(100, 100);
    label->setPixelSize(24);
    label->setIndentation(50);
    label->setAlignment(FTLabel::FontFlags::LeftAligned);
    label->setColor(0.89, 0.26, 0.3, 0.9);
    label->rotate(90, 0, 1, 0); */
}
