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
    NordWest,
    NordEast,

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

    void create(CursorType type,
                int width = ThemeEx::Default.CursorWidth,
                int height = ThemeEx::Default.CursorHeight)
    {
        destroy();

        agg::path_storage westeast;
        westeast.move_to(0.0, 0.5);
        westeast.line_to(0.2, 0.3);
        westeast.line_to(0.2, 0.45);
        westeast.line_to(0.8, 0.45);
        westeast.line_to(0.8, 0.3);
        westeast.line_to(1.0, 0.5);
        westeast.line_to(0.8, 0.7);
        westeast.line_to(0.8, 0.55);
        westeast.line_to(0.2, 0.55);
        westeast.line_to(0.2, 0.7);
        westeast.close_polygon();

        agg::path_storage nordsouth;
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

        agg::path_storage nordwest;
        nordwest.move_to(0.1, 0.1);
        nordwest.line_to(0.4, 0.1);
        nordwest.line_to(0.3, 0.2);
        nordwest.line_to(0.8, 0.7);
        nordwest.line_to(0.9, 0.6);
        nordwest.line_to(0.9, 0.9);
        nordwest.line_to(0.6, 0.9);
        nordwest.line_to(0.7, 0.8);
        nordwest.line_to(0.2, 0.3);
        nordwest.line_to(0.1, 0.4);
        nordwest.close_polygon();

        agg::path_storage nordeast;
        nordeast.move_to(0.9, 0.1);
        nordeast.line_to(0.9, 0.4);
        nordeast.line_to(0.8, 0.3);
        nordeast.line_to(0.3, 0.8);
        nordeast.line_to(0.4, 0.9);
        nordeast.line_to(0.1, 0.9);
        nordeast.line_to(0.1, 0.6);
        nordeast.line_to(0.2, 0.8);
        nordeast.line_to(0.7, 0.2);
        nordeast.line_to(0.6, 0.1);
        nordeast.close_polygon();

        if(type <= CursorType::Standard)
            throw fm::ExceptionInfo << "FM: Invalid custom cursor type.";
        else if(type == CursorType::WestEast)
        {
            create(westeast, agg::rgba(255, 255, 255, 255), agg::rgba(0, 0, 0, 255), width, height);
        }
        else if(type == CursorType::NordSouth)
        {
            create(nordsouth, agg::rgba(255, 255, 255, 255), agg::rgba(0, 0, 0, 255), width, height);
        }
        else if(type == CursorType::NordWest)
        {
            create(nordwest, agg::rgba(255, 255, 255, 255), agg::rgba(0, 0, 0, 255), width, height);
        }
        else if(type == CursorType::NordEast)
        {
            create(nordeast, agg::rgba(255, 255, 255, 255), agg::rgba(0, 0, 0, 255), width, height);
        }

        mType = type;
    }

private:

    void create(agg::path_storage& ps,
                    agg::rgba8 path_color, agg::rgba8 stroke_color, int width, int height)
    {
        std::vector<unsigned char> buffer;
        buffer.resize(width * height * 4);
        using pixfmt = agg::pixfmt_rgba32;
        using ren_base = agg::renderer_base<pixfmt>;

        agg::rendering_buffer rbuffer(&buffer[0], width, height, pixfmt::pix_width * width);
        agg::pixfmt_rgba32 pixf(rbuffer);
        ren_base ren(pixf);

        agg::conv_stroke<agg::path_storage> pg(ps);
        pg.width(0.03);

        agg::trans_affine matrix;
        matrix.scale(width, height);

        agg::conv_transform<agg::path_storage> tpath(ps,matrix);
        agg::conv_transform<agg::conv_stroke<agg::path_storage>> tstroke(pg,matrix);

        agg::scanline_u8 sl;
        agg::rasterizer_scanline_aa<> ras;
        ren.clear(agg::rgba(0.0, 0.5, 0.0, 0.0));

        ras.add_path(tpath);
        agg::render_scanlines_aa_solid(ras, sl, ren, agg::rgba8(255, 255, 255, 255));
        ras.add_path(tstroke);
        agg::render_scanlines_aa_solid(ras, sl, ren, agg::rgba8(0, 0, 0, 255));

        GLFWimage image;
        image.width = width;
        image.height = height;
        image.pixels = &buffer[0];

        mCursor = ::glfwCreateCursor(&image, width/2, height/2);

        if(!mCursor)
            throw fm::ExceptionInfo << "FM: Can't create cursor.";

        mWidth = width;
        mHeight = height;
    }

protected:

    int mWidth {0};
    int mHeight {0};
};//class CustomCursor

}//namespace fm
