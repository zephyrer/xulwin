#ifndef LISTVIEWIMPL_H_INCLUDED
#define LISTVIEWIMPL_H_INCLUDED


#include "XULWin/Component.h"


namespace XULWin
{
    class ListHeaderImpl;

    class ListViewImpl : public NativeControl
    {
    public:
        typedef NativeControl Super;

        ListViewImpl(Component * inParent,
                     const AttributesMapping & inAttributesMapping);

        virtual ~ListViewImpl();

        virtual bool initComponent();

        bool initAttributeControllers();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;
        
        virtual void onChildAdded(Component * inChild);

        void addListHeader(ListHeaderImpl * inListHeader);

    private:
        LRESULT handleGetDispInfo(WPARAM wParam, LPARAM lParam);

        ScopedEventListener mEventListener;
    };

} // namespace XULWin


#endif // LISTVIEWIMPL_H_INCLUDED
