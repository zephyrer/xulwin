#ifndef BOXLAYOUTER_H_INCLUDED
#define BOXLAYOUTER_H_INCLUDED


#include "XULWin/Enums.h"
#include "XULWin/Rect.h"


namespace XULWin
{
    class Component;

    class BoxLayouter
    {
    public:
        class ContentProvider
        {
        public:
            virtual Orient BoxLayouter_getOrient() const = 0;

            virtual Align BoxLayouter_getAlign() const = 0;

            virtual size_t BoxLayouter_getChildCount() const = 0;

            virtual const Component * BoxLayouter_getChild(size_t idx) const = 0;

            virtual Component * BoxLayouter_getChild(size_t idx) = 0;

            virtual Rect BoxLayouter_clientRect() const = 0;

            virtual void BoxLayouter_rebuildChildLayouts() = 0;
        };
        BoxLayouter(ContentProvider * inContentProvider);

        void rebuildLayout();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;

    private:
        ContentProvider * mContentProvider;
    };

} // namespace XULWin


#endif // BOXLAYOUTER_H_INCLUDED
