#include "core/hepch.h"
#include "StandaloneGLTexture.h"

Hachiko::StandaloneGLTexture::StandaloneGLTexture(unsigned int width, 
    unsigned int height, int internal_format, int border, unsigned int format, 
    unsigned int type, int min_filter, int mag_filter) 
    : _width(width) 
    , _height(height) 
    , _internal_format(internal_format)
    , _border(border)
    , _format(format)
    , _type(type)
    , _min_filter(min_filter)
    , _mag_filter(mag_filter)
{
    // Generate and bind frame buffer:
    glGenFramebuffers(1, &_framebuffer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer_id);
    // Generate texture:
    glGenTextures(1, &_texture_id);
    glBindTexture(GL_TEXTURE_2D, _texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, _internal_format, _width, _height, _border, _format, _type, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _mag_filter);
}

Hachiko::StandaloneGLTexture::~StandaloneGLTexture()
{
    // Free texture:
    glDeleteTextures(1, &_texture_id);
    // Free framebuffer:
    glDeleteBuffers(1, &_framebuffer_id);
}

void Hachiko::StandaloneGLTexture::Bind() const 
{
    glBindTexture(GL_TEXTURE_2D, _texture_id);
}

void Hachiko::StandaloneGLTexture::BindForDrawing(bool resize_viewport) const 
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer_id);
    glViewport(0, 0, _width, _height);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Hachiko::StandaloneGLTexture::BindForReading(unsigned int location) const 
{
    glActiveTexture(GL_TEXTURE0 + location);
    Bind();
}

void Hachiko::StandaloneGLTexture::Unbind() const 
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Hachiko::StandaloneGLTexture::Resize(unsigned int width, unsigned int height) 
{
    _width = width;
    _height = height;

    glBindTexture(GL_TEXTURE_2D, _texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, _internal_format, 
        _width, _height, _border, _format, _type, NULL);
}

unsigned int Hachiko::StandaloneGLTexture::GetTextureId() const
{
    return _texture_id;
}

unsigned int Hachiko::StandaloneGLTexture::GetFramebufferId() const
{
    return _framebuffer_id;
}

void Hachiko::StandaloneGLTexture::GetSize(unsigned int& width, unsigned int& height) const 
{
    width = _width;
    height = _height;
}