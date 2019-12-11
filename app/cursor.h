#pragma once

#include <functional>
#include <agg_basics.h>
#include <agg_rendering_buffer.h>
#include <agg_rasterizer_scanline_aa.h>
#include <agg_scanline_u.h>
#include <agg_renderer_scanline.h>
#include <agg_pixfmt_rgba.h>
#include <agg_path_storage.h>
#include <agg_conv_stroke.h>
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

    void setDrawCallback(std::function<void(agg::rasterizer_scanline_aa<>&, int, int)> handler)
    {
        mDrawCallback = handler;
    }

    void create()
    {
        std::vector<unsigned char> buffer;
        buffer.resize(mWidth * mHeight *4);

        if(!mDrawCallback)
            throw fm::ExceptionInfo << "FM: Empty cursor.";

        using pixfmt = agg::pixfmt_rgba32;
        using ren_base = agg::renderer_base<pixfmt>;

        agg::rendering_buffer rbuffer(&buffer[0], mWidth, mHeight, pixfmt::pix_width * mWidth);
        agg::pixfmt_rgba32 pixf(rbuffer);
        ren_base ren(pixf);
        agg::scanline_u8 sl;
        agg::rasterizer_scanline_aa<> ras;
        ren.clear(agg::rgba(0.0, 0.0, 0.0, 0.01));
        ras.gamma(agg::gamma_none());

        mDrawCallback(ras, mWidth, mHeight);

        agg::render_scanlines_aa_solid(ras, sl, ren, agg::rgba8(255, 255, 255, 255));

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
    std::function<void(agg::rasterizer_scanline_aa<>&, int, int)> mDrawCallback;
};//class Panel

}//namespace fm
