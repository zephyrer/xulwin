#ifndef LISTBOXIMPL_H_INCLUDED
#define LISTBOXIMPL_H_INCLUDED


#include "XULWin/NativeControl.h"


namespace XULWin
{


    class ListBox : public NativeControl,
                    public RowsController
    {
    public:
        typedef NativeControl Super;

        ListBox(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initAttributeControllers();

        virtual int getRows() const;

        virtual void setRows(int inRows);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual void onChildAdded(Component * inChild);

    private:
        Fallible<int> mRows;
    };

} // namespace XULWin


#endif // LISTBOXIMPL_H_INCLUDED
