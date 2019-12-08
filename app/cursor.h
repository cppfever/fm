#pragma once

#include "common.h"
#include "framebuffer.h"
#include "themeex.h"


namespace fm
{

class Cursor
{
public:

    Cursor(int width = fm::ThemeEx::Default.CursorWidth, int height = fm::ThemeEx::Default.CursorHeight)
    {}

    virtual ~Cursor()
    {
        destroy();
    }

    void draw(NVGcontext* ctx)
    {
        if(!mFbo)
            return;

        nanogui::Vector2i s = size();
        int width = s[0];
        int height = s[1];

        std::vector<unsigned char> buffer;
        buffer.resize(width * height *4);


        GLFWimage image;
        image.width = width;
        image.height = height;
        image.pixels = &buffer[0];

        destroy();

        mCursor = ::glfwCreateCursor(&image, 0, 0);

        if(!mCursor)
            throw fm::ExceptionInfo << "Can't create cursor.";


        ::nvgluBindFramebuffer(nullptr);
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

    GLFWcursor* mCursor {nullptr};
};//class Panel

}//namespace fm
