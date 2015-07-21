#ifndef __OCTO_GRAPHICS_GRAPHICS_H__
#define __OCTO_GRAPHICS_GRAPHICS_H__

#include "engine.h"

typedef struct : engine_ctx_t
{
} graphics_engine_ctx_t;

class GraphicsEngine : public Engine
{
    public:
        GraphicsEngine(graphics_engine_ctx_t * ctx);
        ~GraphicsEngine();
};

#endif

