#ifndef SVGIMPL_H_INCLUDED
#define SVGIMPL_H_INCLUDED


#include "XULWin/Component.h"
#include "XULWin/Decorator.h"
#include "XULWin/GdiplusLoader.h"


namespace Gdiplus
{
    class Graphics;
    class PointF;
}


namespace XULWin
{

    class SVGPainter
    {
    public:
        virtual void paint(Gdiplus::Graphics & g) = 0;
    };


    class SVGCanvas : public NativeControl,
        public GdiplusLoader
    {
    public:
        typedef NativeControl Super;

        SVGCanvas(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual LRESULT handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam);

    private:
        void bufferedPaint(HDC inHDC);

        virtual void paint(HDC inHDC);
    };


    class SVG : public PassiveComponent,
        public virtual SVG_FillController,
        public virtual SVG_StrokeController,
        public virtual SVG_StrokeWidthController
    {
    public:
        typedef PassiveComponent Super;

        SVG(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual void setSVGFill(const RGBColor & inColor);

        const RGBColor & getSVGFill() const;

        virtual void setSVGStroke(const RGBColor & inColor);

        virtual const RGBColor & getSVGStroke() const;

        virtual void setSVGStrokeWidth(int inStrokeWidth);

        virtual int getSVGStrokeWidth() const;

        virtual bool initAttributeControllers();

        virtual bool initStyleControllers();

    private:
        Fallible<RGBColor> mSVGFill;
        Fallible<RGBColor> mSVGStroke;
        Fallible<int> mSVGStrokeWidth;
    };


    class SVGGroup : public SVG,
        public SVGPainter
    {
    public:
        typedef SVG Super;

        SVGGroup(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initStyleControllers();

        virtual void paint(Gdiplus::Graphics & g);
    };


    class SVGPolygon : public SVG,
        public SVGPainter,
        public virtual SVG_Polygon_PointsController
    {
    public:
        typedef SVG Super;

        SVGPolygon(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initAttributeControllers();

        virtual const Points & getPoints() const;

        virtual void setPoints(const Points & inPoints);

        virtual void paint(Gdiplus::Graphics & g);

    private:
        Points mPoints;
        std::vector<Gdiplus::PointF> mPointFs;
    };


    class SVGRect : public SVG,
        public SVGPainter
    {
    public:
        typedef SVG Super;

        SVGRect(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initStyleControllers();

        virtual void paint(Gdiplus::Graphics & g);

    private:
        std::vector<Gdiplus::PointF> mPoints;
    };


    class SVGPath : public SVG,
        public virtual SVG_Path_InstructionsController,
        public SVGPainter
    {
    public:
        typedef SVG Super;

        SVGPath(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initAttributeControllers();

        virtual bool initStyleControllers();

        virtual void paint(Gdiplus::Graphics & g);

        virtual const SVGPathInstructions & getPathInstructions() const;

        virtual void setPathInstructions(const SVGPathInstructions & inPathInstructions);

    private:
        void getFloatPoints(const PathInstruction & instruction,
                            const Gdiplus::PointF & inPrevPoint,
                            std::vector<Gdiplus::PointF> & outPoints);

        static void GetAbsolutePositions(const PathInstruction & instruction,
                                         const PointF & inPrevPoint,
                                         PointFs & outPoints);

        static void GetAbsolutePositions(const PointFs & inRelativePoints,
                                         const PointF & inPrevPoint,
                                         PointFs & outPoints);

        //bool getFillColor(Gdiplus::Color & outColor);

        //bool getStrokeColor(Gdiplus::Color & outColor);

        static void GetPreparedInstructions(const SVGPathInstructions & inData, SVGPathInstructions & outPrepData);

        static void GetPointReflection(const PointF & inPoint, const PointF & inOrigin, PointF & outReflection);

        SVGPathInstructions mInstructions;
        SVGPathInstructions mPreparedInstructions;
    };

} // namespace XULWin


#endif // SVGIMPL_H_INCLUDED
