#pragma once

#include "common.h"
#include "framebuffer.h"
#include "themeex.h"


namespace fm
{

class Cursor
{
public:

    Cursor(NVGcontext* ctx, int width = fm::ThemeEx::Default.CursorWidth, int height = fm::ThemeEx::Default.CursorHeight)
        : mFbo(ctx, width, height, NVG_IMAGE_NEAREST)
    {}

    virtual ~Cursor()
    {
        destroy();
    }

    nanogui::Vector2i size()
    {
        return mFbo.size();
    }

    void setDrawCallback(const std::function<void(NVGcontext *ctx)>& callback)
    {
        mFbo.setDrawCallback(callback);
        mFbo.draw();

        ::glReadBuffer(GL_BACK);

        std::vector<uint32_t> buffer;
        nanogui::Vector2i size = mFbo.size();
        int width = size[0];
        int height = size[1];
        buffer.resize(width * height);

        ::glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, &buffer[0]);

        GLFWimage image;
        image.width = width;
        image.height = height;
        image.pixels = reinterpret_cast<unsigned char*>(&buffer[0]);

        destroy();

        mCursor = ::glfwCreateCursor(&image, 0, 0);

        if(!mCursor)
            throw fm::ExceptionInfo << "Can't create cursor.";        
    }

    void destroy()
    {
        if(mCursor)
        {
            ::glfwDestroyCursor(mCursor);
            mCursor = nullptr;
        }
    }

    GLFWcursor* glfwCursor()
    {
        return mCursor;
    }

protected:

    fm::Framebuffer mFbo;
    GLFWcursor* mCursor {nullptr};
};//class Panel

}//namespace fm
