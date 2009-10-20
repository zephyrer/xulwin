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


    class SVGComponent : public PassiveComponent
    {
    public:
        typedef PassiveComponent Super;

        SVGComponent(Component * inParent, const AttributesMapping & inAttributesMapping);

    private:
    };


    class SVGGroup : public SVGComponent,
                     public SVGPainter
    {
    public:
        typedef SVGComponent Super;

        SVGGroup(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initStyleControllers();

        virtual void paint(Gdiplus::Graphics & g);
    };


    class SVGPolygon : public SVGComponent,
                       public SVGPainter,
                       public virtual PointsController
    {
    public:
        typedef SVGComponent Super;

        SVGPolygon(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initAttributeControllers();

        virtual const Points & getPoints() const;

        virtual void setPoints(const Points & inPoints);

        virtual void paint(Gdiplus::Graphics & g);

    private:
        Points mPoints;
        std::vector<Gdiplus::PointF> mNativePoints;
    };


    class SVGRect : public SVGComponent,
                    public SVGPainter
    {
    public:
        typedef SVGComponent Super;

        SVGRect(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initStyleControllers();

        virtual void paint(Gdiplus::Graphics & g);

    private:
        std::vector<Gdiplus::PointF> mNativePoints;
    };


    class SVGPath : public SVGComponent,
                    public virtual PathInstructionsController,
                    public SVGPainter
    {
    public:
        typedef SVGComponent Super;

        SVGPath(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initAttributeControllers();

        virtual bool initStyleControllers();

        virtual void paint(Gdiplus::Graphics & g);

        virtual const PathInstructions & getPathInstructions() const;

        virtual void setPathInstructions(const PathInstructions & inPathInstructions);

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

        static void GetPreparedInstructions(const PathInstructions & inData, PathInstructions & outPrepData);

        static void GetPointReflection(const PointF & inPoint, const PointF & inOrigin, PointF & outReflection);

        PathInstructions mInstructions;
        PathInstructions mPreparedInstructions;
    };

} // namespace XULWin


#endif // SVGIMPL_H_INCLUDED
