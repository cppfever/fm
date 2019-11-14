#include "win32mainwindow.h"


namespace nanogui
{

using namespace ClipperLib;

void Win32MainWindow::createRegions()
{
    int width = size().x();
    int height = size().y();

    int xthick = themeex()->MainWindowResizeThickness;
    int ythick = themeex()->MainWindowResizeThickness;

    int xresize = themeex()->MainWindowResizeSize;
    int yresize = themeex()->MainWindowResizeSize;

    if(3 * xthick > width)
        xthick = width / 3;

    if(3 * ythick > height)
        ythick = height / 3;

    if(3 * xresize > width)
        xresize = width / 3;

    if(3 * yresize > height)
        yresize = height / 3;

//Make Sizer path
    ClipperLib::Path outer, inner;

    outer << IntPoint(0, 0)
           << IntPoint(0, height)
           << IntPoint(width, height)
           << IntPoint(width, 0);

    inner << IntPoint(xthick, ythick)
           << IntPoint(xthick, height - ythick)
           << IntPoint(width - xthick , height - ythick)
           << IntPoint(width - xthick , ythick);

    mClipper.Clear();
    mClipper.AddPath(outer, ClipperLib::ptSubject, true);
    mClipper.AddPath(inner, ClipperLib::ptClip, true);
    mClipper.Execute(ClipperLib::ctDifference, mSizing,
                    ClipperLib::pftEvenOdd, ClipperLib::pftEvenOdd);

//Make side paths

    createSidePath(mSizing, mLeft, 0, yresize, width/2, height - yresize);
    createSidePath(mSizing, mTop, xresize, 0, width - xresize, height/2);
    createSidePath(mSizing, mRight, width/2, yresize, width, height - yresize);
    createSidePath(mSizing, mBottom, xresize, height/2, width - xresize, height);

    mClipper.Clear();


    //::SetWindowRgn(mHwnd, mOuterRgn, TRUE);
    //::UpdateWindow(mHwnd);
}

void Win32MainWindow::deleteRegions()
{
    if(mOuterRgn)
    {
        ::DeleteObject(mOuterRgn);
        mOuterRgn = nullptr;
    }

    mClipper.Clear();
    mOuter.clear();
    mInner.clear();
    mSizing.clear();
    mLeft.clear();
    mTop.clear();
    mRight.clear();
    mBottom.clear();
}

void Win32MainWindow::createSidePath(ClipperLib::Paths& subject, ClipperLib::Paths& solution, int x0, int y0, int x1, int y1)
{
    ClipperLib::Path path;

    path << IntPoint(x0, y0)
             << IntPoint(x0, y1)
             << IntPoint(x1, y1)
             << IntPoint(x1, y0);

    mClipper.Clear();
    mClipper.AddPaths(subject, ClipperLib::ptSubject, true);
    mClipper.AddPath(path, ClipperLib::ptClip, true);
    mClipper.Execute(ClipperLib::ctIntersection, solution, ClipperLib::pftEvenOdd);
}

void Win32MainWindow::drawPath(ClipperLib::Path& path)
{
    NVGcontext* ctx = nvgContext();

    ::nvgMoveTo(ctx, path[0].X, path[0].Y);
    for(size_t i = 1; i < path.size(); i++)
        ::nvgLineTo(ctx, path[i].X, path[i].Y);
}

void Win32MainWindow::drawPaths(ClipperLib::Paths& paths)
{
    for(auto path : paths)
        drawPath(path);
}

void Win32MainWindow::draw(NVGcontext *ctx)
{
    ::nvgBeginPath(ctx);
    drawPaths(mSizing);
    ::nvgPathWinding(ctx, NVG_HOLE);
    ::nvgFillColor(ctx, Color(255, 0, 0, 255));
    ::nvgFill(ctx);

    ::nvgBeginPath(ctx);
    drawPaths(mLeft);
    ::nvgPathWinding(ctx, NVG_HOLE);
    ::nvgFillColor(ctx, Color(0, 255, 0, 255));
    ::nvgFill(ctx);

    ::nvgBeginPath(ctx);
    drawPaths(mTop);
    ::nvgPathWinding(ctx, NVG_HOLE);
    ::nvgFillColor(ctx, Color(0, 0, 255, 255));
    ::nvgFill(ctx);

    ::nvgBeginPath(ctx);
    drawPaths(mRight);
    ::nvgPathWinding(ctx, NVG_HOLE);
    ::nvgFillColor(ctx, Color(255, 255, 0, 255));
    ::nvgFill(ctx);

    ::nvgBeginPath(ctx);
    drawPaths(mBottom);
    ::nvgPathWinding(ctx, NVG_HOLE);
    ::nvgFillColor(ctx, Color(0, 255, 255, 255));
    ::nvgFill(ctx);

}

}//namespace nanogui
