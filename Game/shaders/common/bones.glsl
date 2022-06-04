#define MAX_BONES 64

struct PalettePerInstance {
    uint  numBones;
    uint  paletteOffset;
    uint  padding0, padding1;
};