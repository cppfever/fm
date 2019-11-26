#pragma once

#include "common.h"
#include <nanovg_gl_utils.h>


namespace fm
{

class Framebuffer : public nanogui::Object
{
public:

    Framebuffer(NVGcontext* ctx, int width, int height, int flags);
    virtual ~Framebuffer();

    void draw();
    void setDrawCallback(const std::function<void(NVGcontext *ctx)>& callback);
    nanogui::Vector2i size();

protected:

    NVGcontext* mVG {nullptr};
    NVGLUframebuffer* mFbo {nullptr};
    std::function<void(NVGcontext *ctx)> mDrawCallback;
};//class Panel

}//namespace fm
