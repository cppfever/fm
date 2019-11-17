#pragma once

#include <windows.h>
#include "std.h"
#include <nanogui/nanogui.h>
//#include <nanovg.h>
//#include <nanovg_gl.h>
//#include <nanovg_gl_utils.h>
#include <clipper.hpp>
#include "themeex.h"


namespace nanogui
{

using namespace ClipperLib;


class Win32MainWindow : public Screen
{
public:

    Win32MainWindow(const nanogui::Vector2i &size, const std::string &caption,
                    bool resizable = true, bool fullscreen = false, int colorBits = 8,
                    int alphaBits = 8, int depthBits = 24, int stencilBits = 8,
                    int nSamples = 4,
                    unsigned int glMajor = 3, unsigned int glMinor = 3);

    ~Win32MainWindow();

    ThemeEx* themeex();
    bool resizeEvent(const Vector2i &size) override;
    bool mouseMotionEvent(const Vector2i &p, const Vector2i &rel, int button, int modifiers) override;

protected:    

    virtual void createPaths();
    virtual void deletePaths();
    void createSidePath(Paths& solution, int x0, int y0, int x1, int y1);
    void createCornerPath(Paths& solution, Paths& diff1, Paths& diff2, int x0, int y0, int x1, int y1, int x2, int y2,
                           int x3, int y3, int x4, int y4, int x5, int y5);

    void combinePaths(Paths& solution, Paths& subject, Paths& clip, ClipType op);
    void drawPath(Path& path);
    void drawPaths(Paths& paths, Color color);
    void draw(NVGcontext* ctx) override;

    void loadResources();
    bool pointInPath(const IntPoint& p, const Paths& paths);
    virtual bool resizeHitTest(const IntPoint& p);

    HWND mHwnd {nullptr};
    HRGN mOuterRgn {nullptr};
    ThemeEx mThemeEx;
    Clipper mClipper;
    Paths mSizing,
    mOuter, mInner,
    mLeft, mTop, mRight, mBottom,
    mLeftTop, mRightTop, mLeftBottom, mRightBottom;
    bool mDrawSizingPaths {false};
    bool mDrawHitTest {false};
    GLFWcursor* mArrowCursor {nullptr};
    GLFWcursor* mHorCursor {nullptr};
    GLFWcursor* mVertCursor {nullptr};
    GLFWcursor* mLeftTopCursor {nullptr};
    GLFWcursor* mRightBottomCursor {nullptr};

};//class Win32MainWindow

}//namespace nanogui
