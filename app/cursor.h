#pragma once

#include <functional>
#include "common.h"
#include "themeex.h"


namespace fm
{

class Cursor
{
public:

    Cursor(int width = ThemeEx::Default.CursorWidth, int height = ThemeEx::Default.CursorHeight) : mWidth(width), mHeight(height)
    {}

    virtual ~Cursor()
    {
        destroy();
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

    void setDrawCallback(std::function<void(unsigned char*, int, int)> handler)
    {
        mDrawCallback = handler;
    }

    void create()
    {
        std::vector<unsigned char> buffer;
        buffer.resize(mWidth * mHeight *4);

        if(!mDrawCallback)
            throw fm::ExceptionInfo << "FM: Empty cursor.";

        mDrawCallback(&buffer[0], mWidth, mHeight);

        GLFWimage image;
        image.width = mWidth;
        image.height = mHeight;
        image.pixels = &buffer[0];

        destroy();

        mCursor = ::glfwCreateCursor(&image, mWidth/2, mHeight/2);

        if(!mCursor)
            throw fm::ExceptionInfo << "FM: Can't create cursor.";
    }

    void set(GLFWwindow* window)
    {
        ::glfwSetCursor(window, mCursor);
    }

protected:

    GLFWcursor* mCursor {nullptr};
    int mWidth {0};
    int mHeight {0};
    std::function<void(unsigned char*, int, int)> mDrawCallback;
};//class Panel

}//namespace fm
