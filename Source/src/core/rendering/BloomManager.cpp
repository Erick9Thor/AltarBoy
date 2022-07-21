#include "core/hepch.h"
#include "BloomManager.h"
#include "StandaloneGLTexture.h"

#include "Application.h"
#include "modules/ModuleProgram.h"
#include "modules/ModuleRender.h"

Hachiko::BloomManager::BloomManager() 
{

}

Hachiko::BloomManager::~BloomManager() 
{
    Uninitialize();
}

void Hachiko::BloomManager::Initialize() 
{
    if (_initialized)
    {
        return;
    }

    _main_texture = CreateBloomTexture();
    _temp_gaussian_texture = CreateBloomTexture();

    _initialized = true;
}

void Hachiko::BloomManager::Resize(unsigned int width, unsigned int height)
{
    _main_texture->Resize(width, height);
    _temp_gaussian_texture->Resize(width, height);
}

void Hachiko::BloomManager::ApplyBloom(unsigned int texture_to_use) 
{
    //App->renderer->EnableBlending();
    
    // Copy texture_to_use to _main_texture:
    // Bind frame buffer attached to main texture:
    _main_texture->BindBuffer(true);
    App->program->GetTextureCopyProgram()->Activate();
    // Bind the texture to copy:
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_to_use);
    // Render the texture_to_used to a quad in full NDC:
    App->renderer->RenderNDCQuad();
    glBindVertexArray(0);
    // Deactivate copy texture shader:
    Program::Deactivate();
    // Unbind buffers and textures:
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    //App->renderer->DisableBlending();

    // Blur the texture, so we have a bloom effect:
    const int blur_pixel_size_integer = static_cast<int>(bloom_blur_pixel_size);
    unsigned int width, height;
    _main_texture->GetSize(width, height);
    App->renderer->ApplyGaussianFilter(_main_texture->GetFramebufferId(),
                        _main_texture->GetTextureId(),
                        _temp_gaussian_texture->GetFramebufferId(),
                        _temp_gaussian_texture->GetTextureId(),
                        _gaussian_intensity,
                        _gaussian_sigma,
                        blur_pixel_size_integer,
                        width,
                        height,
                        App->program->GetGaussianFilteringProgram());
}

void Hachiko::BloomManager::BindForReading() 
{
    _main_texture->BindForReading(6);
}

void Hachiko::BloomManager::Uninitialize() 
{
    if (!_initialized)
    {
        return;
    }

    _initialized = false;
    delete _main_texture;
    delete _temp_gaussian_texture;
}

Hachiko::StandaloneGLTexture* Hachiko::CreateBloomTexture()
{
    return new StandaloneGLTexture(BloomDefaultValues::TEXTURE_WIDTH,
                                   BloomDefaultValues::TEXTURE_HEIGHT,
                                   GBuffer::GetEmissiveTextureInternalFormat(),
                                   0,
                                   GBuffer::GetEmissiveTextureFormat(),
                                   GBuffer::GetEmissiveTextureType(),
                                   GL_LINEAR,
                                   GL_LINEAR);
}
