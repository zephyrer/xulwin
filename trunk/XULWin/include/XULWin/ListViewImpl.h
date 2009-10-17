#ifndef LISTVIEWIMPL_H_INCLUDED
#define LISTVIEWIMPL_H_INCLUDED


#include "XULWin/ElementImpl.h"


namespace XULWin
{
    class ListHeaderImpl;

    class ListViewImpl : public NativeControl
    {
    public:
        typedef NativeControl Super;

        ListViewImpl(ElementImpl * inParent,
                     const AttributesMapping & inAttributesMapping);

        virtual ~ListViewImpl();

        virtual bool initImpl();

        bool initAttributeControllers();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;
        
        virtual void onChildAdded(ElementImpl * inChild);

        void addListHeader(ListHeaderImpl * inListHeader);

    private:
        LRESULT handleGetDispInfo(WPARAM wParam, LPARAM lParam);

        ScopedEventListener mEventListener;
    };

} // namespace XULWin


#endif // LISTVIEWIMPL_H_INCLUDED
