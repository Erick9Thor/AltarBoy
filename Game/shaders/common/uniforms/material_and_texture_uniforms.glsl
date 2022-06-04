#ifndef _MATERIAL_AND_TEXTURE_UNIFORMS_INCLUDE_
#define _MATERIAL_AND_TEXTURE_UNIFORMS_INCLUDE_

// Texture Batching
uniform sampler2DArray allMyTextures[gl_MaxTextureImageUnits-8];

readonly layout(std430, binding = 1) buffer Materials {
    Material materials[];
} materialsBuffer;

#endif
