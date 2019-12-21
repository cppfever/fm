#pragma once

#include "common.h"
#include "themeex.h"
#include "framebuffer.h"
#include "cursor.h"

namespace fm
{

class Win32MainWindow : public nanogui::Screen
{
public:

    Win32MainWindow(const nanogui::Vector2i &size, const std::string &caption,
                    bool resizable = true, bool fullscreen = false, int colorBits = 8,
                    int alphaBits = 8, int depthBits = 24, int stencilBits = 8,
                    int nSamples = 4,
                    unsigned int glMajor = 3, unsigned int glMinor = 3);

    ~Win32MainWindow();

    ThemeEx* themeex();
    bool resizeEvent(const nanogui::Vector2i &size) override;
    bool mouseMotionEvent(const nanogui::Vector2i &p, const nanogui::Vector2i &rel, int button, int modifiers) override;

protected:    

    virtual void createPaths();
    virtual void deletePaths();
    void createSidePath(clipper::Paths& solution, int x0, int y0, int x1, int y1);
    void createCornerPath(clipper::Paths& solution, clipper::Paths& diff1, clipper::Paths& diff2,
                          int x0, int y0, int x1, int y1, int x2, int y2,
                          int x3, int y3, int x4, int y4, int x5, int y5);

    void combinePaths(clipper::Paths& solution, clipper::Paths& subject, clipper::Paths& clip, clipper::ClipType op);
    void drawPath(clipper::Path& path);
    void drawPaths(clipper::Paths& paths, nanogui::Color color);
    void drawContents() override;
    void draw(NVGcontext* ctx) override;

    void loadResources();
    bool pointInPath(const clipper::IntPoint& p, const clipper::Paths& paths);
    virtual bool resizeHitTest(const clipper::IntPoint& p);

    HWND mHwnd {nullptr};
    HRGN mOuterRgn {nullptr};

    fm::ThemeEx mThemeEx;
    clipper::Clipper mClipper;
    clipper::Paths mSizing,
    mOuter, mInner,
    mLeft, mTop, mRight, mBottom,
    mLeftTop, mRightTop, mLeftBottom, mRightBottom;
    bool mDrawSizingPaths {false};
    bool mDrawHitTest {false};

    GLFWcursor* mArrowCursor {nullptr};
    GLFWcursor* mHorCursor {nullptr};
    GLFWcursor* mVertCursor {nullptr};
    fm::CustomCursor mLeftTopCursor;
    GLFWcursor* mRightTopCursor {nullptr};

};//class Win32MainWindow

}//namespace fm
