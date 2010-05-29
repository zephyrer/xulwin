#include "XULWin/SVG.h"
#include "XULWin/SVGPathInstructions.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Gdiplus.h"


namespace XULWin
{

    SVGCanvas::SVGCanvas(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeControl(inParent, inAttributesMapping, TEXT("STATIC"), 0, 0)
    {
    }


    int SVGCanvas::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }


    int SVGCanvas::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }


    void SVGCanvas::paint(HDC inHDC)
    {
        Gdiplus::Graphics g(inHDC);
        g.SetInterpolationMode(Gdiplus::InterpolationModeHighQuality);
        g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            if (SVGPainter * svg = getChild(idx)->downcast<SVGPainter>())
            {
                svg->paint(g);
            }
        }
    }


    void SVGCanvas::bufferedPaint(HDC inHDC)
    {

        //
        // Get the size of the client rectangle.
        //
        RECT rc;
        GetClientRect(handle(), &rc);

        HDC compatibleDC = CreateCompatibleDC(inHDC);


        //
        // Create a bitmap big enough for our client rectangle.
        //
        HBITMAP backgroundBuffer = CreateCompatibleBitmap(inHDC, rc.right - rc.left, rc.bottom - rc.top);


        //
        // Select the bitmap into the off-screen DC.
        //
        HBITMAP backgroundBitmap = (HBITMAP)SelectObject(compatibleDC, backgroundBuffer);


        //
        // Erase the background.
        //
        HBRUSH backgroundBrush = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
        FillRect(compatibleDC, &rc, backgroundBrush);
        DeleteObject(backgroundBrush);

        //
        // Render the image into the offscreen DC.
        //
        SetBkMode(compatibleDC, TRANSPARENT);


        paint(compatibleDC);


        //
        // Blt the changes to the screen DC.
        //
        BitBlt
        (
            inHDC,
            rc.left,
            rc.top,
            rc.right - rc.left,
            rc.bottom - rc.top,
            compatibleDC, 0, 0, SRCCOPY
        );

        //
        // Done with off-screen bitmap and DC.
        //
        SelectObject(compatibleDC, backgroundBitmap);
        DeleteObject(backgroundBuffer);
        DeleteDC(compatibleDC);
    }


    LRESULT SVGCanvas::handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        if (inMessage == WM_PAINT)
        {
            HDC hDC = ::GetDC(handle());
            PAINTSTRUCT ps;
            ps.hdc = hDC;
            ::BeginPaint(handle(), &ps);
            bufferedPaint(hDC);
            ::EndPaint(handle(), &ps);
            ::ReleaseDC(handle(), hDC);
            return 0;
        }
        return Super::handleMessage(inMessage, wParam, lParam);
    }


    SVG::SVG(Component * inParent, const AttributesMapping & inAttributesMapping) :
        VirtualComponent(inParent, inAttributesMapping),
        mSVGFill(RGBColor(0, 0, 0)),
        mSVGStroke(RGBColor(0, 0, 0, 0)),
        mSVGStrokeWidth(1)
    {
        mSVGFill.setInvalid();
        mSVGStroke.setInvalid();
        mSVGStrokeWidth.setInvalid();
    }


    bool SVG::initAttributeControllers()
    {
        setAttributeController<SVG_FillController>(this);
        setAttributeController<SVG_StrokeController>(this);
        setAttributeController<SVG_StrokeWidthController>(this);
        return Super::initAttributeControllers();
    }


    bool SVG::initStyleControllers()
    {
        return Super::initStyleControllers();
    }


    int SVG::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }


    int SVG::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }


    void SVG::setSVGFill(const RGBColor & inColor)
    {
        mSVGFill = inColor;
    }


    const RGBColor & SVG::getSVGFill() const
    {
        return mSVGFill;
    }


    void SVG::setSVGStroke(const RGBColor & inColor)
    {
        mSVGStroke = inColor;
    }


    const RGBColor & SVG::getSVGStroke() const
    {
        if (mSVGStroke.isValid())
        {
            return mSVGStroke.getValue();
        }

        if (parent())
        {
            if (const SVG * svg = parent()->downcast<SVG>())
            {
                return svg->getSVGStroke();
            }
        }

        return mSVGStroke;
    }


    void SVG::setSVGStrokeWidth(int inStrokeWidth)
    {
        mStrokeWidth = inStrokeWidth;
    }


    int SVG::getSVGStrokeWidth() const
    {
        if (mStrokeWidth.isValid())
        {
            return mStrokeWidth.getValue();
        }

        if (parent())
        {
            if (const SVG * svg = parent()->downcast<SVG>())
            {
                return svg->getSVGStrokeWidth();
            }
        }

        return mStrokeWidth;
    }


    static SVG * findSVGParent(Component * inEl)
    {
        if (!inEl)
        {
            return 0;
        }

        if (SVG * g = inEl->downcast<SVG>())
        {
            return g;
        }
        else
        {
            return findSVGParent(inEl->parent());
        }
    }


    SVGGroup::SVGGroup(Component * inParent, const AttributesMapping & inAttributesMapping) :
        SVG(inParent, inAttributesMapping)
    {
    }


    bool SVGGroup::initStyleControllers()
    {
        return Super::initStyleControllers();
    }


    void SVGGroup::paint(Gdiplus::Graphics & g)
    {
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            if (SVGPainter * svg = getChild(idx)->downcast<SVGPainter>())
            {
                svg->paint(g);
            }
        }
    }


    SVGPolygon::SVGPolygon(Component * inParent, const AttributesMapping & inAttributesMapping) :
        SVG(inParent, inAttributesMapping)
    {
    }


    bool SVGPolygon::initAttributeControllers()
    {
        setAttributeController<SVG_Polygon_PointsController>(this);
        return Super::initAttributeControllers();
    }


    const Points & SVGPolygon::getPoints() const
    {
        return mPoints;
    }


    void SVGPolygon::setPoints(const Points & inPoints)
    {
        mPoints = inPoints;
        mPointFs.clear();
        for (size_t idx = 0; idx != mPoints.size(); ++idx)
        {
            const Point & point = mPoints[idx];
            mPointFs.push_back(Gdiplus::PointF((Gdiplus::REAL)point.x(), (Gdiplus::REAL)point.y()));
        }
    }


    void SVGPolygon::paint(Gdiplus::Graphics & g)
    {
        if (!mPointFs.empty())
        {
            Gdiplus::Color color(Gdiplus::Color::Black);
            SVG * svg = findSVGParent(this);
            if (svg)
            {
                RGBColor fill = svg->getCSSFill();
                color = Gdiplus::Color(fill.alpha(), fill.red(), fill.green(), fill.blue());
            }
            Gdiplus::SolidBrush solidBrush(color);
            g.FillPolygon(&solidBrush, &mPointFs[0], mPointFs.size());
        }
    }


    SVGRect::SVGRect(Component * inParent, const AttributesMapping & inAttributesMapping) :
        SVG(inParent, inAttributesMapping)
    {
    }


    bool SVGRect::initStyleControllers()
    {
        setStyleController<CSSXController>(this);
        setStyleController<CSSYController>(this);
        setStyleController<CSSWidthController>(this);
        setStyleController<CSSHeightController>(this);
        return Super::initStyleControllers();
    }


    void SVGRect::paint(Gdiplus::Graphics & g)
    {
        Gdiplus::Color color(Gdiplus::Color::Black);
        RGBColor fill = getSVGFill();
        color = Gdiplus::Color(fill.alpha(), fill.red(), fill.green(), fill.blue());
        Gdiplus::SolidBrush solidBrush(color);
        g.FillRectangle(&solidBrush, Gdiplus::RectF((Gdiplus::REAL)getCSSX(),
                                                    (Gdiplus::REAL)getCSSY(),
                                                    (Gdiplus::REAL)getWidth(),
                                                    (Gdiplus::REAL)getHeight()));
    }


    SVGPath::SVGPath(Component * inParent, const AttributesMapping & inAttributesMapping) :
        SVG(inParent, inAttributesMapping)
    {
    }


    bool SVGPath::initAttributeControllers()
    {
        setAttributeController<SVG_Path_InstructionsController>(this);
        return Super::initAttributeControllers();
    }


    bool SVGPath::initStyleControllers()
    {
        return Super::initStyleControllers();
    }


    void SVGPath::getFloatPoints(const PathInstruction & instruction,
                                 const Gdiplus::PointF & inPrevPoint,
                                 std::vector<Gdiplus::PointF> & outPoints)
    {
        for (size_t idx = 0; idx != instruction.numPoints(); ++idx)
        {
            const PointF & point = instruction.getPoint(idx);
            Gdiplus::PointF pointF;
            if (instruction.positioning() == PathInstruction::Absolute)
            {
                pointF = Gdiplus::PointF((Gdiplus::REAL)point.x(),
                                         (Gdiplus::REAL)point.y());
            }
            else
            {
                pointF = Gdiplus::PointF(inPrevPoint.X + (Gdiplus::REAL)point.x(),
                                         inPrevPoint.Y + (Gdiplus::REAL)point.y());
            }
            outPoints.push_back(pointF);
        }
    }


    void SVGPath::GetAbsolutePositions(const PointFs & inRelativePoints,
                                       const PointF & inPrevPoint,
                                       PointFs & outPoints)
    {
        for (size_t idx = 0; idx != inRelativePoints.size(); ++idx)
        {
            const PointF & point = inRelativePoints[idx];
            PointF pointF(inPrevPoint.x() + point.x(), inPrevPoint.y() + point.y());
            outPoints.push_back(pointF);
        }
    }


    void SVGPath::GetAbsolutePositions(const PathInstruction & instruction,
                                       const PointF & inPrevPoint,
                                       PointFs & outPoints)
    {
        for (size_t idx = 0; idx != instruction.numPoints(); ++idx)
        {
            const PointF & point = instruction.getPoint(idx);
            PointF pointF;
            if (instruction.positioning() == PathInstruction::Absolute)
            {
                pointF = PointF(point.x(), point.y());
            }
            else
            {
                pointF = PointF(inPrevPoint.x() + point.x(), inPrevPoint.y() + point.y());
            }
            outPoints.push_back(pointF);
        }
    }


    void SVGPath::GetPointReflection(const PointF & inPoint,
                                     const PointF & inOrigin,
                                     PointF & outReflection)
    {
        float x = inOrigin.x() - inPoint.x();
        float y = inOrigin.y() - inPoint.y();
        outReflection = PointF(x, y);
    }


    void SVGPath::GetPreparedInstructions(const SVGPathInstructions & inData, SVGPathInstructions & outPrepData)
    {
        PointFs preppedPoints;
        PointF prevPoint;
        PathInstruction prevInstruction(PathInstruction::MoveTo,
                                        PathInstruction::Relative,
                                        PointFs());
        for (size_t idx = 0; idx != inData.size(); ++idx)
        {
            const PathInstruction & instruction = inData[idx];
            switch (instruction.tagName())
            {
                case PathInstruction::MoveTo: // M
                {
                    if (!preppedPoints.empty())
                    {
                        outPrepData.push_back(PathInstruction(prevInstruction.tagName(),
                                                              PathInstruction::Absolute,
                                                              preppedPoints));
                        preppedPoints.clear();
                    }
                    if (instruction.numPoints() == 1)
                    {
                        GetAbsolutePositions(instruction, prevPoint, preppedPoints);
                        if (!preppedPoints.empty())
                        {
                            prevPoint = preppedPoints[preppedPoints.size() - 1];
                        }
                    }
                    else
                    {
                        ReportError("XMLSVG XMLSVGPath of type MoveTo has more than one points.");
                    }
                    break;
                }
                case PathInstruction::LineTo: // L
                case PathInstruction::HorizontalLineTo: // H
                case PathInstruction::VerticalLineTo: // V
                {
                    if (!preppedPoints.empty() && prevInstruction.tagName() != PathInstruction::MoveTo)
                    {
                        outPrepData.push_back(PathInstruction(prevInstruction.tagName(),
                                                              PathInstruction::Absolute,
                                                              preppedPoints));
                        if (!preppedPoints.empty())
                        {
                            prevPoint = preppedPoints[preppedPoints.size() - 1];
                        }
                        preppedPoints.clear();
                    }
                    if (instruction.numPoints() == 1)
                    {
                        if (preppedPoints.empty())
                        {
                            preppedPoints.push_back(prevPoint);
                        }
                        GetAbsolutePositions(instruction, prevPoint, preppedPoints);
                        outPrepData.push_back(PathInstruction(instruction.tagName(),
                                                              PathInstruction::Absolute,
                                                              preppedPoints));
                        if (!preppedPoints.empty())
                        {
                            prevPoint = preppedPoints[preppedPoints.size() - 1];
                        }
                        preppedPoints.clear();
                    }
                    else
                    {
                        ReportError("XMLSVG XMLSVGPath of type LineTo, HorizontalLineTo or VerticalLineTo has more than one points.");
                    }
                    break;
                }
                case PathInstruction::CurveTo: // C
                {
                    if (preppedPoints.empty())
                    {
                        // If preppedPoints is empty at this point then we need
                        // to add the begin point ourselves. We do this after
                        // the call to GetAbsolutePositions because prevPoint
                        // is already absolute positioned.
                        GetAbsolutePositions(instruction, prevPoint, preppedPoints);
                        preppedPoints.insert(preppedPoints.begin(), prevPoint);
                    }
                    else
                    {
                        GetAbsolutePositions(instruction, prevPoint, preppedPoints);
                    }
                    if (!preppedPoints.empty())
                    {
                        prevPoint = preppedPoints[preppedPoints.size() - 1];
                    }
                    break;
                }
                case PathInstruction::SmoothCurveTo: // S
                {
                    // We'll convert it to a curve instruction
                    PathInstruction curveInstruction = instruction;
                    if (prevInstruction.tagName() == PathInstruction::CurveTo)
                    {
                        PointF c2 = instruction.getPoint(0);
                        PointF endPoint = instruction.getPoint(1);

                        // S produces the same type of curve as C, but if it
                        // follows another S command or a C command, the first
                        // control point is assumed to be a reflection of the
                        // one used previously.
                        PointF c1;
                        if (instruction.positioning() == PathInstruction::Relative)
                        {
                            GetPointReflection(prevInstruction.getPoint(1),
                                               prevInstruction.getPoint(2),
                                               c1);
                        }
                        else
                        {
                            GetPointReflection(prevInstruction.getAbsolutePoint(prevPoint, 1),
                                               prevInstruction.getAbsolutePoint(prevPoint, 2),
                                               c1);
                            // now also make it absolute position
                            c1 = PointF(prevPoint.x() + c1.x(), prevPoint.y() + c1.y());
                        }
                        curveInstruction.points().insert(curveInstruction.points().begin(), c1);
                    }
                    else if (prevInstruction.tagName() == PathInstruction::SmoothCurveTo)
                    {
                        PointF c2 = instruction.getPoint(0);
                        PointF endPoint = instruction.getPoint(1);

                        // S produces the same type of curve as C, but if it
                        // follows another S command or a C command, the first
                        // control point is assumed to be a reflection of the
                        // one used previously.
                        PointF c1;
                        if (instruction.positioning() == PathInstruction::Relative)
                        {
                            GetPointReflection(prevInstruction.getPoint(0),
                                               prevInstruction.getPoint(1),
                                               c1);
                        }
                        else
                        {
                            GetPointReflection(prevInstruction.getAbsolutePoint(prevPoint, 0),
                                               prevInstruction.getAbsolutePoint(prevPoint, 1),
                                               c1);
                            // now also make it absolute position
                            c1 = PointF(prevPoint.x() + c1.x(), prevPoint.y() + c1.y());
                        }
                        curveInstruction.points().insert(curveInstruction.points().begin(), c1);
                    }
                    else
                    {
                        //"M186.2,26
                        //s-2.3, 5.9 -5.2, 5.9
                        //S186.2,29.2 186.2,26z"
                        // If the S command doesn't follow another S or C command, then it is
                        // assumed that both control points for the curve are the same.
                        if (instruction.positioning() == PathInstruction::Relative)
                        {
                            curveInstruction.points().insert(curveInstruction.points().begin(),
                                                             instruction.getPoint(0));
                        }
                        else
                        {
                            curveInstruction.points().insert(curveInstruction.points().begin(),
                                                             instruction.getAbsolutePoint(prevPoint, 0));
                        }
                    }
                    if (preppedPoints.empty())
                    {
                        // If preppedPoints is empty at this point then we need
                        // to add the begin point ourselves. We do this after
                        // the call to GetAbsolutePositions because prevPoint
                        // is already absolute positioned.
                        GetAbsolutePositions(curveInstruction, prevPoint, preppedPoints);
                        preppedPoints.insert(preppedPoints.begin(), prevPoint);
                    }
                    else
                    {
                        GetAbsolutePositions(curveInstruction, prevPoint, preppedPoints);
                    }
                    if (!preppedPoints.empty())
                    {
                        prevPoint = preppedPoints[preppedPoints.size() - 1];
                    }
                    break;
                }
                case PathInstruction::QuadraticBelzierCurve: // Q
                case PathInstruction::SmoothQuadraticBelzierCurveTo: // T
                case PathInstruction::EllipticalArc: // A
                {
                    assert(false); // not yet implemented
                    break;
                }
                case PathInstruction::ClosePath: // Z
                {
                    if (!preppedPoints.empty())
                    {
                        outPrepData.push_back(PathInstruction(prevInstruction.tagName(),
                                                              PathInstruction::Absolute,
                                                              preppedPoints));
                        prevPoint = preppedPoints[preppedPoints.size() - 1];
                        preppedPoints.clear();
                    }
                    // This command draws a straight line from your current
                    // position back to the first point that started the path.
                    if (!inData.empty())
                    {
                        if (!inData[0].points().empty())
                        {
                            preppedPoints.push_back(inData[0].points()[0]);
                            preppedPoints.push_back(prevPoint);
                            if (preppedPoints[0] != preppedPoints[1])
                            {
                                outPrepData.push_back(PathInstruction(PathInstruction::LineTo,
                                                                      PathInstruction::Absolute,
                                                                      preppedPoints));
                            }
                            preppedPoints.clear();
                        }
                    }
                    else
                    {
                        // How could inData be empty if we are inside the for loop??
                        assert(false);
                    }
                    break;
                }
            }

            prevInstruction = instruction;
        }
        if (!preppedPoints.empty())
        {
            outPrepData.push_back(PathInstruction(prevInstruction.tagName(),
                                                  PathInstruction::Absolute,
                                                  preppedPoints));
            preppedPoints.clear();
        }
    }


    void SVGPath::paint(Gdiplus::Graphics & g)
    {
        RGBColor fillColorRGB(getSVGFill());
        Gdiplus::Color fillColor(fillColorRGB.alpha(),
                                 fillColorRGB.red(),
                                 fillColorRGB.green(),
                                 fillColorRGB.blue());
        Gdiplus::SolidBrush brush(fillColor);


        RGBColor strokeColorRGB(getSVGStroke());
        Gdiplus::Color strokeColor(strokeColorRGB.alpha(),
                                   strokeColorRGB.red(),
                                   strokeColorRGB.green(),
                                   strokeColorRGB.blue());

        Gdiplus::Pen pen(strokeColor, static_cast<float>(getSVGStrokeWidth()));

        Gdiplus::GraphicsPath path;
        path.SetFillMode(Gdiplus::FillModeWinding);
        for (size_t idx = 0; idx != mPreparedInstructions.size(); ++idx)
        {
            const PathInstruction & instruction = mPreparedInstructions[idx];
            const PointFs & points = instruction.points();
            switch (instruction.tagName())
            {
                case PathInstruction::MoveTo: // should not be found in mPreparedInstructions
                {
                    assert(false);
                    break;
                }
                case PathInstruction::LineTo:
                {
                    if (instruction.numPoints() == 2)
                    {
                        path.AddLine(Gdiplus::PointF(instruction.getPoint(0).x(), instruction.getPoint(0).y()),
                                     Gdiplus::PointF(instruction.getPoint(1).x(), instruction.getPoint(1).y()));
                    }
                    else
                    {
                        ReportError("LineTo: paint failed because number of prepared points is not equal to 2.");
                    }
                    break;
                }
                case PathInstruction::HorizontalLineTo:
                case PathInstruction::VerticalLineTo:
                {
                    assert(false); // not yet supported
                    break;
                }
                case PathInstruction::CurveTo:
                case PathInstruction::SmoothCurveTo:
                {
                    std::vector<Gdiplus::PointF> gdiplusPoints;
                    for (size_t idx = 0; idx != points.size(); ++idx)
                    {
                        const PointF & point = points[idx];
                        gdiplusPoints.push_back(Gdiplus::PointF(point.x(), point.y()));
                    }
                    path.AddBeziers(&gdiplusPoints[0], gdiplusPoints.size());
                    break;
                }
                case PathInstruction::QuadraticBelzierCurve:
                case PathInstruction::SmoothQuadraticBelzierCurveTo:
                case PathInstruction::EllipticalArc:
                {
                    assert(false); // not yet implemented
                    break;
                }
                case PathInstruction::ClosePath: // should not be found in mPreparedInstructions
                {
                    assert(false);
                    break;
                }
                default:
                {
                    assert(false);
                    ReportError("Unsupported type");
                }
            }
        }
        g.FillPath(&brush, &path);
        g.DrawPath(&pen, &path);
    }


    const SVGPathInstructions & SVGPath::getPathInstructions() const
    {
        return mInstructions;
    }


    void SVGPath::setPathInstructions(const SVGPathInstructions & inPathInstructions)
    {
        mInstructions = inPathInstructions;
        mPreparedInstructions.clear();
        GetPreparedInstructions(mInstructions, mPreparedInstructions);
    }

} // namespace XULWin
