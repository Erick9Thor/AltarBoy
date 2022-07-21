#pragma once

namespace Hachiko
{
    class StandaloneGLTexture
    {
    public:
        StandaloneGLTexture(unsigned int width, unsigned int height, 
            int internal_format, int border, unsigned int format, 
            unsigned int type, int min_filter, int mag_filter, bool generate_depth = false);
        ~StandaloneGLTexture();

        void Bind() const;
        void BindBuffer(bool resize_viewport = false) const;
        void BindForReading(unsigned int location = 0) const;
        void Unbind() const;
        void Resize(unsigned int width, unsigned int height);
        
        [[nodiscard]] unsigned int GetTextureId() const;
        [[nodiscard]] unsigned int GetFramebufferId() const;
        void GetSize(unsigned int& width, unsigned int& height) const;

    private:
        unsigned int _texture_id;
        unsigned int _depth_id;
        unsigned int _framebuffer_id;
        unsigned int _width;
        unsigned int _height;
        int _internal_format;
        unsigned int _format;
        unsigned int _type;
        int _border;
        int _min_filter;
        int _mag_filter;
        bool _has_depth;
    };
}

