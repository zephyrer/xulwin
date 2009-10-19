#ifndef LAYOUT_H_INCLUDED
#define LAYOUT_H_INCLUDED


#include "XULWin/Enums.h"
#include "XULWin/GenericGrid.h"
#include "XULWin/Point.h"
#include "XULWin/Rect.h"
#include <vector>


namespace XULWin
{    
    
    class Margins
    {
    public:        
        Margins() : mLeft(0), mTop(0), mRight(0), mBottom(0) {}

        Margins (int inLeft, int inTop, int inRight, int inBottom) : mLeft(inLeft), mTop(inTop), mRight(inRight), mBottom(inBottom) {}

        int left() const { return mLeft; }

        int top() const { return mTop; }

        int right() const { return mRight; }

        int bottom() const { return mBottom; }
    
        bool operator==(const Margins & inOtherMargins)
        {
            return mLeft == inOtherMargins.mLeft
                && mTop == inOtherMargins.mTop
                && mRight == inOtherMargins.mRight
                && mBottom == inOtherMargins.mBottom;
        }
        
        bool operator!=(const Margins & inOtherMargins)
        {
            return mLeft != inOtherMargins.mLeft
                || mTop != inOtherMargins.mTop
                || mRight != inOtherMargins.mRight
                || mBottom != inOtherMargins.mBottom;
        }

    private:
        int mLeft;
        int mTop;
        int mRight;
        int mBottom;
    };


    // Introduced wrapper class so that the compiler will
    // report an error if we accidentally mix up the order
    // of parameters in the SizeInfo class.
    struct FlexWrap
    {
        explicit FlexWrap(int inValue) :
            Value(inValue)
        {
        }
        int Value;
    };

    struct MinSizeWrap
    {
        explicit MinSizeWrap(int inValue) :
            Value(inValue)
        {
        }
        int Value;
    };

    struct OptSizeWrap
    {
        explicit OptSizeWrap(int inValue) :
            Value(inValue)
        {
        }
        int Value;
    };

    struct MinSizeOppositeWrap
    {
        explicit MinSizeOppositeWrap(int inValue) :
            Value(inValue)
        {
        }
        int Value;
    };

    struct SizeInfo
    {
        SizeInfo(FlexWrap inFlex, MinSizeWrap inMinSize, OptSizeWrap inOptSize) :
            Flex(inFlex.Value),
            MinSize(inMinSize.Value),
            OptSize(inOptSize.Value)
        {
        }
        int Flex;
        int MinSize;
        int OptSize;
    };


    struct ExtendedSizeInfo : public SizeInfo
    {
        ExtendedSizeInfo(FlexWrap inFlex, MinSizeWrap inMinSize, OptSizeWrap inOptSize, MinSizeOppositeWrap inMinSizeOpposite, bool inExpansive) :
            SizeInfo(inFlex, inMinSize, inOptSize),
            MinSizeOpposite(inMinSizeOpposite.Value),
            Expansive(inExpansive)
        {
        }
        int MinSizeOpposite;
        bool Expansive;
    };


    class LinearLayoutManager
    {
    public:
        LinearLayoutManager(Orient inOrient);

        void getRects(const Rect & inRect, Align inAlign, const std::vector<ExtendedSizeInfo> & inSizeInfos, std::vector<Rect> & outRects);
        static void GetSizes(int inLength, const std::vector<SizeInfo> & inSizeInfos, std::vector<int> & outSizes);
        static void GetSizes(int inLength, const std::vector<int> & inSizeInfos, std::vector<int> & outSizes);        

        Orient orientation() const;

    private:
        Orient mOrient;
    };


    struct CellInfo
    {
        CellInfo(int inMinWidgetWidth,
                 int inMinWidgetHeight,
                 Align inRowAlign,
                 Align inColAlign) :
            MinWidgetWidth(inMinWidgetWidth),
            MinWidgetHeight(inMinWidgetHeight),
            RowAlign(inRowAlign),
            ColAlign(inColAlign)
        {
        }

        int MinWidgetWidth;
        int MinWidgetHeight;
        Align RowAlign;
        Align ColAlign;
    };
    

    class GridLayoutManager
    {
    public:
        static void GetOuterRects(const Rect & inRect,
                                  const std::vector<SizeInfo> & inColWidths,
                                  const std::vector<SizeInfo> & inRowHeights,
                                  GenericGrid<Rect> & outRects);

        static void GetInnerRects(const GenericGrid<Rect> & inOuterRects,
                                  const GenericGrid<CellInfo> & inWidgetInfos,
                                  GenericGrid<Rect> & outInnerRects);
    };

} // namespace XULWin


#endif // LAYOUT_H_INCLUDED
