#include "core/hepch.h"

#include "Particle.h"

#include "modules/ModuleRender.h"
#include "modules/ModuleProgram.h"

#include "components/ComponentCamera.h"
#include "components/ComponentTransform.h"
#include "components/ComponentParticleSystem.h"

#include "resources/ResourceTexture.h"

using namespace Hachiko;

void Hachiko::Particle::Update()
{
    current_life -= EngineTimer::delta_time;

    if (current_life <= 0)
    {
        Reset();
    }
}

void Hachiko::Particle::Reset()
{
    Deactivate();
    SetCurrentLife(GetInitialLife());
    SetCurrentPosition(GetInitialPosition());
    SetCurrentSize(GetInitialSize());
    SetCurrentSpeed(GetInitialSpeed());
    SetCurrentDirection(GetInitialDirection());
}

void Particle::Draw(ComponentCamera* camera, Program* program)
{
    glActiveTexture(GL_TEXTURE0);
    int glTexture = 0;
    has_texture = 0;
    if (texture != nullptr)
    {
        glTexture = texture->GetImageId();
        Hachiko::ModuleTexture::Bind(glTexture, static_cast<int>(Hachiko::ModuleProgram::TextureSlots::DIFFUSE));
        has_texture = 1;
    }

    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    if (render_mode == ParticleRenderMode::PARTICLE_ADDITIVE)
    {
        glBlendFunc(GL_ONE, GL_ONE);
    }
    else
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    float4x4 model_matrix = float4x4::identity;
    GetModelMatrix(camera, model_matrix);
    program->BindUniformFloat4x4("model", model_matrix.ptr());
    program->BindUniformFloat4x4("view", &camera->GetViewMatrix()[0][0]);
    program->BindUniformFloat4x4("proj", &camera->GetProjectionMatrix()[0][0]);

    program->BindUniformFloat("x_factor", &x_factor);
    program->BindUniformFloat("y_factor", &y_factor);
    //program->BindUniformFloat("current_frame", &current_frame);
    //program->BindUniformFloat2("animation_index", animation_index.ptr());

    program->BindUniformFloat4("input_color", current_color.ptr());
    program->BindUniformInts("has_texture", 1, &has_texture);

    //int flip_x = has_flip_x ? 1 : 0;
    //int flip_y = has_flip_y ? 1 : 0;
    //program->BindUniformInts("flip_x", 1, &flip_x);
    //program->BindUniformInts("flip_y", 1, &flip_y);

    glBindVertexArray(App->renderer->GetParticleVao());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // Clear
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    return;
}

void Hachiko::Particle::GetModelMatrix(ComponentCamera* camera, float4x4& out_matrix) const
{
    // Vertical (cilindrical) orientation
    float3 cameraPos = camera->GetFrustum()->Pos();
    float3 cameraDir = (float3(cameraPos.x, current_position.y, cameraPos.z) - current_position).Normalized();
    out_matrix = float4x4::LookAt(float3::unitZ, cameraDir, float3::unitY, float3::unitY);
    out_matrix = float4x4::FromTRS(current_position, out_matrix.RotatePart(), float3(current_size, 0.0f));
}

bool Hachiko::Particle::IsActive() const
{
    return active;
}

void Hachiko::Particle::Activate()
{
    active = true;
}

void Hachiko::Particle::Deactivate()
{
    active = false;
}

float Particle::GetInitialLife() const
{
    return emitter->GetParticlesLife().values.x;
}

float Particle::GetInitialSpeed() const
{
    return emitter->GetParticlesSpeed().values.x;
}

const float2& Particle::GetInitialSize() const
{
    return emitter->GetParticlesSize().values;
}

const float3& Particle::GetInitialPosition() const
{
    return emitter->GetParticlesEmissionPosition();
}

const float3& Hachiko::Particle::GetInitialDirection() const
{
    return emitter->GetParticlesDirection();
}

float Particle::GetCurrentLife() const
{
    return current_life;
}

void Particle::SetCurrentLife(const float current_life)
{
    this->current_life = current_life;
}

float Particle::GetCurrentSpeed() const
{
    return current_speed;
}

void Particle::SetCurrentSpeed(const float current_speed)
{
    this->current_speed = current_speed;
}

const float4& Particle::GetCurrentColor() const
{
    return current_color;
}

void Particle::SetCurrentColor(const float4& current_color)
{
    this->current_color = current_color;
}

const float2& Particle::GetCurrentSize() const
{
    return current_size;
}

void Particle::SetCurrentSize(const float2& current_size)
{
    // Apply same size for both coordinates. Particles are always squares!
    this->current_size.x = current_size.x;
    this->current_size.y = current_size.x;
}

const float3& Particle::GetCurrentPosition() const
{
    return current_position;
}

void Particle::SetCurrentPosition(const float3& current_position)
{
    this->current_position = current_position;
}

ParticleRenderMode Particle::GetRenderMode() const
{
    return render_mode;
}

void Particle::SetRenderMode(const ParticleRenderMode render_mode)
{
    this->render_mode = render_mode;
}

const float3& Hachiko::Particle::GetCurrentDirection() const
{
    return current_direction;
}

void Hachiko::Particle::SetCurrentDirection(const float3& current_direction)
{
    this->current_direction = current_direction;
}

void Hachiko::Particle::SetEmitter(ComponentParticleSystem* emitter) 
{
    this->emitter = emitter;
}


