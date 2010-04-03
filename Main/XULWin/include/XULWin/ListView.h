#ifndef LISTVIEWIMPL_H_INCLUDED
#define LISTVIEWIMPL_H_INCLUDED


#include "XULWin/Component.h"


namespace XULWin
{
    class ListHeader;

    class ListView : public NativeControl
    {
    public:
        typedef NativeControl Super;

        ListView(Component * inParent,
                 const AttributesMapping & inAttributesMapping);

        virtual ~ListView();

        virtual bool init();

        bool initAttributeControllers();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual void onChildAdded(Component * inChild);

        void addListHeader(ListHeader * inListHeader);

    private:
        LRESULT handleGetDispInfo(WPARAM wParam, LPARAM lParam);

        ScopedEventListener mEventListener;
    };

} // namespace XULWin


#endif // LISTVIEWIMPL_H_INCLUDED
