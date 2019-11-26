#include "framebuffer.h"
#include "exception.h"
#include <nanovg_gl.h>
#include <nanovg_gl_utils.h>


namespace fm
{
Framebuffer::Framebuffer(NVGcontext* ctx, int width, int height, int flags) : mVG(ctx)
{
    mFbo = ::nvgluCreateFramebuffer(ctx, width, height, flags);

    if(mFbo == nullptr)
        throw ExceptionInfo << "Can't create FBO.";
}

Framebuffer::~Framebuffer()
{
    ::nvgluDeleteFramebuffer(mFbo);
}

void Framebuffer::draw()
{
    if(!mFbo)
        return;

    nanogui::Vector2i s = size();

    ::nvgluBindFramebuffer(mFbo);
    ::nvgBeginFrame(mVG, s[0], s[1], 1.0f);
    ::glViewport(0, 0, s[0], s[1]);
    //    ::glClearColor(255, 0, 0, 255);
    //    ::glClear(GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

    if(mDrawCallback)
        mDrawCallback(mVG);

    ::nvgEndFrame(mVG);
    ::nvgluBindFramebuffer(nullptr);

    NVGpaint img = ::nvgImagePattern(mVG, 0.0f, 0.0f, s[0], s[1], 0.0f, mFbo->image, 1.0f);
    ::nvgFillPaint(mVG, img);
    ::nvgFill(mVG);
}

void Framebuffer::setDrawCallback(const std::function<void(NVGcontext *ctx)>& callback)
{
    mDrawCallback = callback;
}

nanogui::Vector2i Framebuffer::size()
{
    int width, height;
    ::nvgImageSize(mVG, mFbo->image, &width, &height);
    return {width, height};
}
}//namespace fm
