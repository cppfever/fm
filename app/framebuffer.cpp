#include "framebuffer.h"
#include <nanovg_gl.h>
#include <nanovg_gl_utils.h>


namespace nanogui
{
Framebuffer::Framebuffer(NVGcontext* ctx, int width, int height, NVGimageFlags flags)
{
    mFbo = ::nvgluCreateFramebuffer(ctx, width, height, flags);

    if(mFbo == nullptr)
        throw ExceptionInfo << "Can't create FBO.";
}

Framebuffer::~Framebuffer()
{
    ::nvgluDeleteFramebuffer(mFbo);
}

void Framebuffer::draw(NVGcontext* ctx)
{
    if(!mFbo)
        return;

    //We must Save/Restore GL context when drawing in the framebuffer
    ::nvgSave(ctx);

    int width, height;
    ::nvgImageSize(ctx, mFbo->image, &width, &height);
    ::nvgluBindFramebuffer(mFbo);
    ::glViewport(0, 0, width, height);
    ::glClearColor(0, 0, 0, 0);
    ::glClear(GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    ::nvgBeginFrame(ctx, width, height, 1.0f);

    if(mDrawCallback)
        mDrawCallback(ctx);

    ::nvgEndFrame(ctx);
    ::nvgluBindFramebuffer(nullptr);

    ::nvgRestore(ctx);
}

void Framebuffer::setDrawCallback(const std::function<void(NVGcontext *ctx)>& callback)
{
    mDrawCallback = callback;
}
}//namespace nanogui
