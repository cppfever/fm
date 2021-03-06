#include "agg.h"
#include "win32mainwindow.h"


using namespace nanogui;
using namespace clipper;


namespace fm
{

Win32MainWindow::Win32MainWindow(const nanogui::Vector2i &size, const std::string &caption, bool resizable, bool fullscreen, int colorBits, int alphaBits, int depthBits, int stencilBits, int nSamples, unsigned int glMajor, unsigned int glMinor)
    : nanogui::Screen(size, caption, resizable, fullscreen, colorBits, alphaBits, depthBits, stencilBits, nSamples, glMajor, glMinor),
      mArrowCursor(CursorType::Arrow),
      mLeftRightCursor(CursorType::WestEast),
      mTopBottomCursor(CursorType::NordSouth),
      mLeftTopCursor(CursorType::NordWest),
      mRightTopCursor(CursorType::NordEast)
{
    mDrawSizingPaths = false;
    mDrawHitTest = false;

    mHwnd = ::glfwGetWin32Window(glfwWindow());
    loadResources();
    createPaths();
}

ThemeEx *Win32MainWindow::themeex()
{
    return &mThemeEx;
}

Win32MainWindow::~Win32MainWindow()
{
    deletePaths();
}

bool Win32MainWindow::resizeEvent(const Vector2i &size)
{
    Screen::resizeEvent(size);
    deletePaths();
    createPaths();
    return true;
}

bool Win32MainWindow::mouseMotionEvent(const Vector2i &p, const Vector2i &rel, int button, int modifiers)
{
    resizeHitTest(IntPoint(p.x(), p.y()));
    //::UpdateWindow(mHwnd);
    return true;
}

void Win32MainWindow::createPaths()
{
    int width = size().x();
    int height = size().y();

    int xthick = themeex()->MainWindowResizeWidth;
    int ythick = themeex()->MainWindowResizeWidth;

    int xresize = themeex()->MainWindowCornerResizeWidth;
    int yresize = themeex()->MainWindowCornerResizeWidth;

    if(3 * xthick > width)
        xthick = width / 3;

    if(3 * ythick > height)
        ythick = height / 3;

    if(3 * xresize > width)
        xresize = width / 3;

    if(3 * yresize > height)
        yresize = height / 3;

    //Make Sizer path
    Path outer, inner;

    outer << IntPoint(0, 0)
          << IntPoint(0, height)
          << IntPoint(width, height)
          << IntPoint(width, 0);

    inner << IntPoint(xthick, ythick)
          << IntPoint(xthick, height - ythick)
          << IntPoint(width - xthick , height - ythick)
          << IntPoint(width - xthick , ythick);

    mClipper.Clear();
    mClipper.AddPath(outer, ptSubject, true);
    mClipper.AddPath(inner, ptClip, true);
    mClipper.Execute(ctDifference, mSizing,
                     pftEvenOdd, pftEvenOdd);

    //Make side paths
    createSidePath(mLeft, 0, yresize, width/2, height - yresize);
    createSidePath(mBottom, xresize, height/2, width - xresize, height);
    createSidePath(mRight, width/2, yresize, width, height - yresize);
    createSidePath(mTop, xresize, 0, width - xresize, height/2);

    //Make corner paths
    createCornerPath(mLeftTop, mLeft, mTop,
                     0, 0,
                     0, yresize,
                     width/2 - xresize, height/2,
                     width/2 , height/2,
                     width/2, height/2 - yresize,
                     xresize, 0);

    createCornerPath(mLeftBottom, mLeft, mBottom,
                     0, height,
                     xresize, height,
                     width/2, height/2 + yresize,
                     width/2 , height/2,
                     width/2 - xresize, height/2,
                     0, height - yresize);

    createCornerPath(mRightBottom, mRight, mBottom,
                     width, height,
                     width, height - yresize,
                     width/2 + yresize, height/2,
                     width/2 , height/2,
                     width/2, height/2 + yresize,
                     width - xresize, height);

    createCornerPath(mRightTop, mRight, mTop,
                     width, 0,
                     width - xresize, 0,
                     width/2, height/2 - yresize,
                     width/2 , height/2,
                     width/2 + xresize, height/2,
                     width, yresize);

    mClipper.Clear();
}

void Win32MainWindow::deletePaths()
{
    if(mOuterRgn)
    {
        ::DeleteObject(mOuterRgn);
        mOuterRgn = nullptr;
    }

    mClipper.Clear();

    mSizing.clear();
    mOuter.clear();
    mInner.clear();

    mLeft.clear();
    mTop.clear();
    mRight.clear();
    mBottom.clear();

    mLeftTop.clear();
    mLeftBottom.clear();
    mRightBottom.clear();
    mRightTop.clear();
}

void Win32MainWindow::combinePaths(clipper::Paths& solution, clipper::Paths& subject, clipper::Paths& clip, clipper::ClipType op)
{
    mClipper.Clear();
    mClipper.AddPaths(subject, ptSubject, true);
    mClipper.AddPaths(clip, ptClip, true);
    mClipper.Execute(op, solution, pftEvenOdd);
}

void Win32MainWindow::createCornerPath(clipper::Paths& solution, clipper::Paths& diff1, clipper::Paths& diff2,
                                       int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5)
{
    clipper::Path path;

    path << IntPoint(x0, y0)
         << IntPoint(x1, y1)
         << IntPoint(x2, y2)
         << IntPoint(x3, y3)
         << IntPoint(x4, y4)
         << IntPoint(x5, y5);

    clipper::Paths paths;
    paths << path;

    clipper::Paths diffsolutuion1;
    clipper::Paths diffsolutuion2;

    combinePaths(diffsolutuion1, paths, diff1, ctDifference);
    combinePaths(diffsolutuion2, diffsolutuion1, diff2, ctDifference);
    //solution = diffsolutuion2;//Draw full corner path
    combinePaths(solution, mSizing, diffsolutuion2, ctIntersection);
}

void Win32MainWindow::createSidePath(clipper::Paths& solution, int x0, int y0, int x1, int y1)
{
    clipper::Path path;

    path << IntPoint(x0, y0)
         << IntPoint(x0, y1)
         << IntPoint(x1, y1)
         << IntPoint(x1, y0);

    mClipper.Clear();
    mClipper.AddPaths(mSizing, ptSubject, true);
    mClipper.AddPath(path, ptClip, true);
    mClipper.Execute(ctIntersection, solution, pftEvenOdd);
}

void Win32MainWindow::drawPath(clipper::Path& path)
{
    NVGcontext* ctx = nvgContext();

    ::nvgMoveTo(ctx, path[0].X, path[0].Y);
    for(size_t i = 1; i < path.size(); i++)
        ::nvgLineTo(ctx, path[i].X, path[i].Y);
}

void Win32MainWindow::drawPaths(clipper::Paths& paths, Color color)
{
    NVGcontext* ctx = nvgContext();
    ::nvgBeginPath(ctx);

    for(auto path : paths)
        drawPath(path);

    if(paths.size() == 2)
        ::nvgPathWinding(ctx, NVG_HOLE);

    ::nvgFillColor(ctx, color);
    ::nvgFill(ctx);
}

void Win32MainWindow::drawContents()
{
}

void Win32MainWindow::draw(NVGcontext *ctx)
{
    if(mDrawSizingPaths)
    {
        drawPaths(mSizing, Color(255, 0, 0, 255));
        drawPaths(mLeft, Color(0, 255, 0, 255));
        drawPaths(mTop, Color(0, 0, 255, 255));
        drawPaths(mRight, Color(255, 0, 255, 255));
        drawPaths(mBottom, Color(0, 255, 255, 255));
        drawPaths(mLeftTop, Color(127, 255, 255, 255));
        drawPaths(mLeftBottom, Color(255, 127, 255, 255));
        drawPaths(mRightBottom, Color(127, 0, 0, 255));
        drawPaths(mRightTop, Color(0, 127, 0, 255));
    }

    if(mDrawHitTest)
    {
        double  x, y;
        ::glfwGetCursorPos(glfwWindow(), &x, &y);
        resizeHitTest(IntPoint(static_cast<int>(x), static_cast<int>(y)));
    }
}

void Win32MainWindow::loadResources()
{
}

bool Win32MainWindow::pointInPath(const clipper::IntPoint& point, const clipper::Paths& paths)
{
    for(auto path : paths)
        if(clipper::PointInPolygon(point, path))
            return true;

    return false;
}

bool Win32MainWindow::resizeHitTest(const clipper::IntPoint& point)
{
    if(pointInPath(point, mLeft))
    {
        if(mDrawHitTest)
            drawPaths(mLeft, Color(255, 0, 0, 255));

        ::glfwSetCursor(glfwWindow(), mLeftRightCursor.glfwCursor());
        return true;
    }

    if(pointInPath(point, mRight))
    {
        if(mDrawHitTest)
            drawPaths(mRight, Color(255, 0, 0, 255));

        ::glfwSetCursor(glfwWindow(), mLeftRightCursor.glfwCursor());
        return true;
    }

    if(pointInPath(point, mTop))
    {
        if(mDrawHitTest)
            drawPaths(mTop, Color(255, 0, 0, 255));

        ::glfwSetCursor(glfwWindow(), mTopBottomCursor.glfwCursor());
        return true;
    }

    if(pointInPath(point, mBottom))
    {
        if(mDrawHitTest)
            drawPaths(mBottom, Color(255, 0, 0, 255));

        ::glfwSetCursor(glfwWindow(), mTopBottomCursor.glfwCursor());
        return true;
    }

    if(pointInPath(point, mLeftTop))
    {
        if(mDrawHitTest)
            drawPaths(mLeftTop, Color(255, 0, 0, 255));

        ::glfwSetCursor(glfwWindow(), mLeftTopCursor.glfwCursor());
        return true;
    }

    if(pointInPath(point, mLeftBottom))
    {
        if(mDrawHitTest)
            drawPaths(mLeftBottom, Color(255, 0, 0, 255));

        ::glfwSetCursor(glfwWindow(), mRightTopCursor.glfwCursor());
        return true;
    }

    if(pointInPath(point, mRightBottom))
    {
        if(mDrawHitTest)
            drawPaths(mRightBottom, Color(255, 0, 0, 255));

        ::glfwSetCursor(glfwWindow(), mLeftTopCursor.glfwCursor());
        return true;
    }

    if(pointInPath(point, mRightTop))
    {
        if(mDrawHitTest)
            drawPaths(mRightTop, Color(255, 0, 0, 255));

        ::glfwSetCursor(glfwWindow(), mRightTopCursor.glfwCursor());
        return true;
    }

    ::glfwSetCursor(glfwWindow(), mArrowCursor.glfwCursor());
    return false;
}

}//namespace fm
