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

    CustomCursor(std::function<void(agg::renderer_base<agg::pixfmt_rgba32>&, int, int)> drawcallback,
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

    void create(std::function<void(agg::renderer_base<agg::pixfmt_rgba32>&, int, int)> drawcallback,
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

        static agg::path_storage nordsouth;
        nordsouth.move_to(0.5, 0.0);
        nordsouth.line_to(0.7, 0.2);
        nordsouth.line_to(0.55, 0.2);
        nordsouth.line_to(0.55, 0.8);
        nordsouth.line_to(0.7, 0.8);
        nordsouth.line_to(0.5, 1.0);
        nordsouth.line_to(0.3, 0.8);
        nordsouth.line_to(0.45, 0.8);
        nordsouth.line_to(0.45, 0.2);
        nordsouth.line_to(0.3, 0.2);
        nordsouth.close_polygon();


        if(type <= CursorType::Standard)
            throw fm::ExceptionInfo << "FM: Invalid custom cursor type.";
        else if(type == CursorType::WestEast)
        {
            mDrawCallback = [&](agg::renderer_base<agg::pixfmt_rgba32>& ren, int width, int height)
            {
//                agg::path_storage ps;
//                ps.remove_all();
//                ps.move_to(0.5, 0.0);
//                ps.line_to(0.7, 0.2);
//                ps.line_to(0.55, 0.2);
//                ps.line_to(0.55, 0.8);
//                ps.line_to(0.7, 0.8);
//                ps.line_to(0.5, 1.0);
//                ps.line_to(0.3, 0.8);
//                ps.line_to(0.45, 0.8);
//                ps.line_to(0.45, 0.2);
//                ps.line_to(0.3, 0.2);
//                ps.close_polygon();

                agg::conv_stroke<agg::path_storage> pg(nordsouth);
                pg.width(0.03);

                agg::trans_affine matrix;
                //matrix.rotate(45.0 * M_PI / 180.0);
                //matrix.translate(0.5, -0.25);
                matrix.scale(width, height);


                agg::conv_transform<agg::path_storage> tpath(nordsouth,matrix);
                agg::conv_transform<agg::conv_stroke<agg::path_storage>> tstroke(pg,matrix);

                agg::scanline_u8 sl;
                agg::rasterizer_scanline_aa<> ras;
                ren.clear(agg::rgba(0.0, 0.5, 0.0, 1.00));

                ras.add_path(tpath);
                agg::render_scanlines_aa_solid(ras, sl, ren, agg::rgba8(255, 255, 255, 255));
                ras.add_path(tstroke);
                agg::render_scanlines_aa_solid(ras, sl, ren, agg::rgba8(0, 0, 0, 255));
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
        buffer.resize(mWidth * mHeight * 4);

        if(!mDrawCallback)
            throw fm::ExceptionInfo << "FM: Empty cursor.";

        using pixfmt = agg::pixfmt_rgba32;
        using ren_base = agg::renderer_base<pixfmt>;

        agg::rendering_buffer rbuffer(&buffer[0], mWidth, mHeight, pixfmt::pix_width * mWidth);
        agg::pixfmt_rgba32 pixf(rbuffer);
        ren_base ren(pixf);
        mDrawCallback(ren, mWidth, mHeight);

        GLFWimage image;
        image.width = mWidth;
        image.height = mHeight;
        image.pixels = &buffer[0];

        mCursor = ::glfwCreateCursor(&image, mWidth/2, mHeight/2);
    }

protected:

    int mWidth {0};
    int mHeight {0};
    std::function<void(agg::renderer_base<agg::pixfmt_rgba32>&, int, int)> mDrawCallback;
};//class CustomCursor

}//namespace fm
