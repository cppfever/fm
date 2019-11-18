#pragma once

#include "common.h"
#include "exception.h"
#include <nanovg_gl_utils.h>


namespace nanogui
{

class Framebuffer : public Object
{
public:

    Framebuffer(NVGcontext* ctx, int width, int height, int flags);
    virtual ~Framebuffer();

    void draw(NVGcontext* ctx);
    void setDrawCallback(const std::function<void(NVGcontext *ctx)>& callback);

    int image()
    {
        return mFbo->image;
    }

protected:

    NVGLUframebuffer* mFbo {nullptr};
    std::function<void(NVGcontext *ctx)> mDrawCallback;
};//class Panel

}//namespace nanogui
