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

enum class CursorType : uint32_t
{
    //No created
    Empty,

    //Standard
    Arrow,
    Ibeam,
    Crosshair,
    Hand,
    Hresize,
    Vresize,
    Standard,

    //Custom sizing(more variants than GLFW standards)
    WestEast,
    NordSouth,
    NordWestSouthEast,
    NordEastSouthWest,

    //Custom drawing cursor
    None
};//enum class CursorType


//Standard cursor
class Cursor
{
public:

    Cursor() = default;

    Cursor(CursorType type)
    {
        create(type);
    }

    virtual ~Cursor()
    {
        destroy();
    }

    void create(CursorType type)
    {
        destroy();

        if(type > CursorType::Standard)
            throw fm::ExceptionInfo << "FM: Invalid standard cursor type.";
        else if(type == CursorType::Arrow)
            mCursor = ::glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        else if(type == CursorType::Ibeam)
            mCursor = ::glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
        else if(type == CursorType::Crosshair)
            mCursor = ::glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
        else if(type == CursorType::Hand)
            mCursor = ::glfwCreateStandardCursor(GLFW_HAND_CURSOR);
        else if(type == CursorType::Hresize)
            mCursor = ::glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
        else if(type == CursorType::Vresize)
            mCursor = ::glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);

        mType = type;
    }

    virtual void destroy()
    {
        if(mCursor)
        {
            ::glfwDestroyCursor(mCursor);
            mCursor = nullptr;
            mType = CursorType::Empty;
        }
    }

    GLFWcursor* glfwCursor()
    {
        return mCursor;
    }

    CursorType type()
    {
        return mType;
    }

protected:

    GLFWcursor* mCursor {nullptr};
    CursorType mType {CursorType::Empty};
};//class Cursor


class CustomCursor : public Cursor
{
public:

    CustomCursor() = default;

    CustomCursor(CursorType type,
                 int width = ThemeEx::Default.CursorWidth,
                 int height = ThemeEx::Default.CursorHeight)
    {
        create(type, width, height);
    }

    CustomCursor(std::function<void(agg::rasterizer_scanline_aa<>&, int, int)> drawcallback,
                int width = ThemeEx::Default.CursorWidth,
                int height = ThemeEx::Default.CursorHeight)
    {
        create(drawcallback, width, height);
    }

    virtual ~CustomCursor()
    {
        destroy();
    }

    void destroy() override
    {
        if(mCursor)
        {
            Cursor::destroy();
            mWidth = 0;
            mHeight = 0;
        }
    }

    void create(std::function<void(agg::rasterizer_scanline_aa<>&, int, int)> drawcallback,
                int width = ThemeEx::Default.CursorWidth,
                int height = ThemeEx::Default.CursorHeight)
    {
        mDrawCallback = drawcallback;
        create(CursorType::None, width, height);
    }

    void create(CursorType type,
                int width = ThemeEx::Default.CursorWidth,
                int height = ThemeEx::Default.CursorHeight)
    {
        destroy();

        if(type <= CursorType::Standard)
            throw fm::ExceptionInfo << "FM: Invalid custom cursor type.";
        else if(type == CursorType::WestEast)
        {
            mDrawCallback = [&](agg::rasterizer_scanline_aa<>& ras, int width, int height)
            {
                agg::path_storage ps;
                agg::conv_stroke<agg::path_storage> pg(ps);
                pg.width(4.0);

                ps.remove_all();
                ps.move_to(0, height/2);
                ps.line_to(width/2, height);
                ps.line_to(width, height/2);
                ps.line_to(width/2, 0);
                ps.close_polygon();
                ras.add_path(pg);

            };
        }
        else if(type == CursorType::NordSouth)
        {

        }
        else if(type == CursorType::NordWestSouthEast)
        {

        }
        else if(type == CursorType::NordEastSouthWest)
        {

        }

        mType = type;
        mWidth = width;
        mHeight = height;

        create(width, height);

        if(!mCursor)
            throw fm::ExceptionInfo << "FM: Can't create cursor.";
    }

private:

    void create(int width,int height)
    {
        std::vector<unsigned char> buffer;
        //buffer.resize(mWidth * mHeight * 4);
        buffer.resize(20000);
        std::cout << &buffer[0] << std::endl;

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

        mCursor = ::glfwCreateCursor(&image, mWidth/2, mHeight/2);
    }

protected:

    int mWidth {0};
    int mHeight {0};
    std::function<void(agg::rasterizer_scanline_aa<>&, int, int)> mDrawCallback;


};//class CustomCursor

}//namespace fm
