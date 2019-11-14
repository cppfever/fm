#pragma once

#include "win32mainwindow.h"


namespace nanogui
{

using namespace ClipperLib;

void Win32MainWindow::createRegions()
{
    int width = size().x();
    int height = size().y();

    mOuter << IntPoint(0, 0)
           << IntPoint(0, height)
           << IntPoint(width, height)
           << IntPoint(width, 0);

    int xthick = themeex()->MainWindowResizeThickness;
    int ythick = themeex()->MainWindowResizeThickness;

    if(3 * xthick > width)
        xthick = width / 3;

    if(3 * ythick > height)
        ythick = height / 3;

    int x2thick = xthick * 2;
    int y2thick = ythick * 2;

    mInner << IntPoint(xthick, ythick)
           << IntPoint(xthick, height - ythick)
           << IntPoint(width - xthick , height - ythick)
           << IntPoint(width - xthick , ythick);



    ClipperLib::Clipper clipper;
    clipper.AddPath(mOuter, ClipperLib::ptSubject, true);
    clipper.AddPath(mInner, ClipperLib::ptClip, true);
    clipper.Execute(ClipperLib::ctDifference, mSizing,
                    ClipperLib::pftEvenOdd, ClipperLib::pftEvenOdd);
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

    mOuter.clear();
    mInner.clear();
    mSizing.clear();
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
    //drawPath(mOuter);
    //drawPath(mInner);
    drawPaths(mSizing);
    ::nvgPathWinding(ctx, NVG_HOLE);
    ::nvgFillColor(ctx, Color(255, 0, 0, 255));
    ::nvgFill(ctx);
}

}//namespace nanogui
