#include "XULWin/BoxLayouter.h"
#include "XULWin/Component.h"
#include "XULWin/Decorator.h"
#include "XULWin/Element.h"
#include "XULWin/ErrorReporter.h"


namespace XULWin
{
    

    BoxLayouter::BoxLayouter(ContentProvider * inContentProvider) :
        mContentProvider(inContentProvider)
    {
    }


    int BoxLayouter::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        if (mContentProvider->BoxLayouter_getOrient() == Horizontal)
        {
            int result = 0;
            for (size_t idx = 0; idx != mContentProvider->BoxLayouter_getChildCount(); ++idx)
            {
                result += mContentProvider->BoxLayouter_getChild(idx)->getWidth(inSizeConstraint);
            }
            return result;
        }
        else if (mContentProvider->BoxLayouter_getOrient() == Vertical)
        {
            int result = 0;
            for (size_t idx = 0; idx != mContentProvider->BoxLayouter_getChildCount(); ++idx)
            {
                int width = mContentProvider->BoxLayouter_getChild(idx)->getWidth(inSizeConstraint);
                if (width > result)
                {
                    result = width;
                }
            }
            return result;
        }
        else
        {
            ReportError("Invalid getOrient in VirtualBox"); 
            return 0;
        }
    }


    int BoxLayouter::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        if (mContentProvider->BoxLayouter_getOrient() == Horizontal)
        {
            int result = 0;
            for (size_t idx = 0; idx != mContentProvider->BoxLayouter_getChildCount(); ++idx)
            {
                int height = mContentProvider->BoxLayouter_getChild(idx)->getHeight(inSizeConstraint);
                if (height > result)
                {
                    result = height;
                }
            }
            return result;
        }
        else if (mContentProvider->BoxLayouter_getOrient() == Vertical)
        {
            int result = 0;
            for (size_t idx = 0; idx != mContentProvider->BoxLayouter_getChildCount(); ++idx)
            {
                result += mContentProvider->BoxLayouter_getChild(idx)->getHeight(inSizeConstraint);
            }
            return result;
        }
        else
        {
            ReportError("Invalid getOrient in VirtualBox");
            return 0;
        }
    }

    
    void BoxLayouter::rebuildLayout()
    {     
        Rect clientR(mContentProvider->BoxLayouter_clientRect());   
        LinearLayoutManager layout(mContentProvider->BoxLayouter_getOrient());
        bool horizontal = mContentProvider->BoxLayouter_getOrient() == Horizontal;
        std::vector<ExtendedSizeInfo> sizeInfos;

        for (size_t idx = 0; idx != mContentProvider->BoxLayouter_getChildCount(); ++idx)
        {
            Component * child = mContentProvider->BoxLayouter_getChild(idx);
            std::string flex;
            if (child->el())
            {
                flex = child->el()->getAttribute("flex");
            }
            int flexValue = String2Int(flex, 0);
            int optSize = horizontal ? child->getWidth(Preferred) : child->getHeight(Preferred);
            int minSize = horizontal ? child->getWidth(Minimum) : child->getHeight(Minimum);
            int minSizeOpposite = horizontal ? child->getHeight(Minimum) : child->getWidth(Minimum);
            sizeInfos.push_back(
                ExtendedSizeInfo(FlexWrap(flexValue),
                                 MinSizeWrap(minSize),
                                 OptSizeWrap(optSize),
                                 MinSizeOppositeWrap(minSizeOpposite),
                                 child->expansive()));
        }

        std::vector<Rect> childRects;
        layout.getRects(clientR, mContentProvider->BoxLayouter_getAlign(), sizeInfos, childRects);

        for (size_t idx = 0; idx != mContentProvider->BoxLayouter_getChildCount(); ++idx)
        {
            Component * child = mContentProvider->BoxLayouter_getChild(idx);
            const Rect & rect = childRects[idx];
            child->move(rect.x(), rect.y(), rect.width(), rect.height());
        }

        mContentProvider->BoxLayouter_rebuildChildLayouts();
    }

} // namespace XULWin
